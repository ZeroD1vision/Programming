/**
 * @file lib.cpp
 * @brief Реализация параллельного алгоритма Борувки для поиска MST
 */

#include "lib.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <limits>
#include <iomanip>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace chrono;

// ==================== ParallelBoruvka ====================


vector<pair<int, int>> ParallelBoruvka::findMST(double& duration) {
    auto start = high_resolution_clock::now();
    vector<pair<int, int>> mst;
    int n_components = n_;
    mutex mtx; // Мьютекс для синхронизации доступа к общему массиву

    
    // Создаем разные потоки в них запихиваем по одному такому массиву (n; n + chunk)
    // Основной цикл алгоритма Борувки
    while (n_components > 1) {
        // Вектор для хранения минимальных рёбер для каждой компоненты
        vector<tuple<int, int, int>> min_edge_per_comp(n_, make_tuple(INF, -1, -1));
        vector<thread> threads;

        // Делим на потоки с помощью 
        int chunk = max(1, n_ / n_threads_);

        // Шаг 1: Найти мин. ребро для каждой компоненты
        for (int t = 0; t < n_threads_; t++) {
            int start_idx = t * chunk;
            int end_idx = (t == n_threads_ - 1) ? n_ : (t + 1) * chunk;

            // Создаем поток для обработки своего блока вершин
            threads.emplace_back([&, start_idx, end_idx]() -> void {
                for (int u = start_idx; u < end_idx; u++) {
                    int best_v = -1;
                    int min_weight = INF;
                    int u_component = find_component(u);

                    // Поиск минимального ребра, соединяющего компоненту u с другой компонентой
                    for (size_t i = 0; i < adj[u].size(); ++i) {
                        const pair<int, int>& neighbor = adj[u][i];
                        int v = neighbor.first;
                        int weight = neighbor.second;
                        int v_component = find_component(v);

                        // Ребро должно соединять разные компоненты и иметь минимальный вес
                        if ((u_component != v_component) && weight < min_weight) {
                            min_weight = weight;
                            best_v = v;
                        }
                    }

                    // Если нашли подходящее ребро, обновляем минимум для компоненты
                    if (best_v != -1) {
                        auto [curr_w, curr_u, curr_v] = min_edge_per_comp[u_component]; // Читаем текущее значение ВНЕ блокировки
                        if (min_weight < curr_w) {  // Проверяем ВНЕ блокировки (чтобы избежать лишних lock'ов)
                            lock_guard<mutex> lock(mtx);  // Блокируем
                            // ДВОЙНАЯ ПРОВЕРКА ВНУТРИ блокировки:
                            auto [curr_w2, curr_u2, curr_v2] = min_edge_per_comp[u_component];
                            if (min_weight < curr_w2) {
                                min_edge_per_comp[u_component] =  make_tuple(min_weight, u, best_v);  // Обновляем
                            }
                        }
                    }
                }
            });
        }

        // Ожидаем завершения всех потоков
        for (auto& t : threads) {
            t.join();
        }
        
        // Шаг 2: Собираем кандидаты (одно на компоненту)
        vector<tuple<int, int, int>> candidates;
        for (int i = 0; i < n_; i++) {
            tuple<int, int, int>& edge = min_edge_per_comp[i];
            if (std::get<0>(edge) != INF) {     // Проверяем вес
                candidates.push_back(edge);
            }
        }

        // Сортировка кандидатов по весу для более быстроого выбора
        sort(candidates.begin(), candidates.end(), 
            [](const auto& a, const auto& b) -> bool {
                return get<0>(a) < get<0>(b); // Не звоните, я - гений
            });

        // Шаг 3: Добавление рёбер в MST и объединение компонент
        int edges_added = 0;
        for (const auto& candidate : candidates) {
            int weight = get<0>(candidate);
            int u = get<1>(candidate);
            int v = get<2>(candidate);
            
            int comp_u = find_component(u);
            int comp_v = find_component(v);
            
            // Проверяем, что компоненты ещё не объединены
            if (comp_u != comp_v) {
                mst.push_back({u, v});
                union_components(u, v);
                edges_added++;
                n_components--;
                
                // Прерываемся если осталась одна компонента
                if (n_components <= 1) break;
            }
        }

        // Защита от бесконечного цикла при несвязном
        if (edges_added == 0) {
            break;
        }
    }

    auto end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start).count();
    return mst;
}

int ParallelBoruvka::calculateTotalWeight(const vector<pair<int, int>>& mst) {
    int total_weight = 0;
    for (const auto& edge : mst) {
        int u = edge.first;
        int v = edge.second;

        for (const auto& neighbor : adj[u]) {
            if (neighbor.first == v) {
                total_weight += neighbor.second;
                break;
            }
        }
    }
    return total_weight;
}

ParallelBoruvka::ParallelBoruvka(const vector<vector<pair<int, int>>>& graph, int n_threads) 
        : adj(graph), n_(graph.size()), n_threads_(n_threads) {
    parent.resize(n_);
    rank.resize(n_, 0);
    for (int i = 0; i < n_; i++) {
        parent[i] = i; // Каждая вершина является корнем своей собственной компоненты связности
    }
}
    
int ParallelBoruvka::find_component(int x) {
    if (parent[x] != x) {
        parent[x] = find_component(parent[x]);
    }
    return parent[x];
}

void ParallelBoruvka::union_components(int x, int y) {
    int x_comp = find_component(x);
    int y_comp = find_component(y);

    // Объединяем только если вершины в разных компонентах
    if (x_comp != y_comp) {
        // Меньшее дерево присоединяется к большему
        if (rank[x_comp] < rank[y_comp]) {
            parent[x_comp] = y_comp;
        } else if (rank[y_comp] < rank[x_comp]) {
            parent[y_comp] = x_comp;
        } else {
            // При равных рангах увеличиваем ранг деерва
            parent[y_comp] = x_comp;
            rank[x_comp]++;
        }
    }
}



// ==================== Вспомогательные функции ====================

vector<vector<pair<int, int>>> readGraph(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        throw runtime_error("Cannot open input file: " + filename);
    }

    vector<vector<pair<int, int>>> graph;
    string line;
    
    // Читаем количество вершин
    getline(fin, line);
    int n_vertices = stoi(line);
    graph.resize(n_vertices);

    // Читаем списки смежности для каждой вершины
    for (int i = 0; i < n_vertices && getline(fin, line); i++) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string token;

        // Разбор строк формата "сосед:вес сосед:вес ..."
        while (iss >> token) {
            size_t colon_pos = token.find(':');
            if (colon_pos != string::npos) {
                try {
                    int vertex = stoi(token.substr(0, colon_pos)) - 1; // 1-based -> 0-based
                    int weight = stoi(token.substr(colon_pos + 1));
                    
                    // Проверка корректности номера вершины
                    if (vertex >= 0 && vertex < n_vertices) {
                        graph[i].push_back({vertex, weight});
                    } else {
                        cerr << "Warning: vertex " << vertex + 1 << " out of range [1, " << n_vertices << "]" << endl;
                    }
                } catch (const exception& e) {
                    cerr << "Error parsing token '" << token << "': " << e.what();
                }
            }
        }
    }

    fin.close();
    return graph;
}

string getTestFileName(int test_num) {
    stringstream ss;
    // Генерация имени файла в формате ./tests/gpg_t1_001.txt
    ss << "../tests/epg_t1_" << setw(3) << setfill('0') << test_num << ".txt";
    return ss.str();
}
