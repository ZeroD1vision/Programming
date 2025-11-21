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

// ==================== Gamma-Algorithm ====================

LayingGraph::LayingGraph(const vector<vector<int>>& adj_matrix) 
    : graph(adj_matrix), n(adj_matrix.size()) {}

bool LayingGraph::isLaying() {
    if (n <= 2) return true;

    // Шаг 1: Поиск начального цикла
    vector<int> first_cycle = findFirstCycle();
    if (first_cycle.empty()) {
        return true;
    }

    // Укладываем вершины и еджи цикла
    for (int v : first_cycle) {
        laying_vertices.insert(v);
    }
    for (int i = 0; i < first_cycle.size(); i++) {
        int u = first_cycle[i];
        int v = first_cycle[(i + 1) % first_cycle.size()];
        laying_edges.insert({min(u, v), max(u, v)});
    }

    // Повторяем шаги 2-7 пока не установим непланарность или не уложим все
    while (true) {
        // Шаг 2: Поиск доступных сегментов обоих типов, А) и Б)
        // Шаг 3: Определение контактных (внутри findSegments())
        vector<Segment> segments = findSegments();
        if (segments.empty()) break; // Если нечего укладывать - заканчиваем
        
        Segment selected_segment; // Выбранный сегмент для укладки
        int selected_face = -1; // Выбранная грань для укладки сегмента 
        bool found = false; // Флаг, что пока ничего не выбрано.
        
        int min_suitable_count = INF; // Рамзер текущего минимального по включению массива подходящих граней

        // Для всех доступных сегментов
        for (const auto& seg : segments) {
            // Шаг 4: Находим грани вмещающие каждый из сегментов
            vector<int> suitable_faces = findFacesForSegment(seg);
            
            // Шаг 5: Проверка плананрности (никто не вмещает => не уложим)
            if (suitable_faces.empty()) {
                return false; // Не планарен
            }

            // Шаг 6: Выбор минимального сегмента (его вмещают мин кол-во граней)
            size_t suitable_faces_count = suitable_faces.size();
            if (!found || suitable_faces_count < min_suitable_count) {
                selected_segment = seg;
                min_suitable_count = suitable_faces_count;
                selected_face = suitable_faces[0]; // Так как можем брать любой подходящий
                found = true;
            }
        }

        // Шаг 7: Укладка выбранного сегмента
        vector<int> path = findPathInSegment(selected_segment, selected_face);
        if (!path.empty()) {
            placeSegment(selected_segment, selected_face, path); // Укалдываем по пути
        } else {
            return false; // Не нашли путь
        }
    }

    return true;
}

vector<int> LayingGraph::findFirstCycle() {
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);

    // Из всех вершин
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            stack<int> st; // Закидываем каждую в стек
            st.push(i);
            parent[i] = -1;
            visited[i] = true;

            // Пока стек не пуст
            while(!st.empty()) {
                // Забираем вершину из него как u
                int u = st.top();
                st.pop();

                // Для всех соседей u
                for (int v = 0; v < n; v++) {
                    if (graph[u][v] && u != v) { // Соседи
                        st.push(v); // Суем в стек
                        parent[v] = u; // Устанавливаем родителя текущей
                        visited[v] = true; // Посещена
                    } else if (visited[v] && v != parent[u]) { // Пришли к знакомому, но не родитель
                        vector<int> cycle; // Массив для хранения вершин цикла
                        int curr = u; // Текущая врешина как curr
                        while (curr != v) { // Пока не дошли до той с кого начали 
                            cycle.push_back(curr); // Суем в цикл
                            curr = parent[curr]; // Устанавливаем родителя
                        }
                        cycle.push_back(v); // Суем в цикл последнюю v
                        // Итог: [a, b, c] в дальнейшем коде будет интерпретировано как [a, b, c, a] 
                        return cycle;
                    }
                }
            }
        }
    }
    return {}; // Цикла нет
}