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

vector<Segment> LayingGraph::findSegments() {
    vector<Segment> segments;
    // Найти все неуложенные (спать)
    set<int> not_laying_vertices;
    for (int i = 0; i < n; i++) {
        // Если не найден среди уложенных
        if (laying_vertices.find(i) == laying_vertices.end()) {
            not_laying_vertices.insert(i);
        }
    }
    // Найти тип А) сегменты
    // Проходим по всем парам возможным
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            // Если есть ребро НЕ уложено но оба конца УЛОЖЕНЫ
            if (graph[u][v] && 
                laying_edges.find({u, v} && 
                laying_vertices.find(u) && 
                laying_vertices.find(v))) {
                
                // Создаем сегмент
                Segment seg;
                seg.type = 0; // Типа А)
                seg.u = u;
                seg.v = v;
                seg.contact_vertices = {u, v}; // Устанавливаем контактные вершины
                segments.push_back(seg); // Суем в выходной массив
            }
        }
    }

    // Найти тип Б) сегменты
    // Делим на компоненты связности (для размещения, удобной работы)
    vector<set<int>> components = findConnectedComponents(not_laying_vertices);

    // Делаем каждую такую компоненту сегментом типа А) 
    for (const auto& comp : components) {
        Segment seg;
        seg.type = 1; // Типа Б)
        seg.vertices = comp;

        // Находим все контактные вершины сегмента
        // Для каждой точки из компоненты среди всех соседей ищем контакных уложенных
        for (int u : comp) {
            for (int v = 0; v < n; v++) {
                if (graph[u][v] && laying_vertices.find(v) != laying_vertices.end()) {
                    seg.contact_vertices.insert(v);
                    seg.edges.insert({min(u, v), max(u, v)});
                }
            }
        }

        // Добавляем внутренние ребра из компоненты
        for (int u : comp) {
            for (int v : comp) {
                if (graph[u][v] && u < v) {
                    seg.edges.insert({u, v});
                }
            }
        }

        // Если есть хоть один контактный вертис
        if (!seg.contact_vertices.empty()) {
            segments.push_back(seg);
        }
    }
    return segments;
}

vector<int> LayingGraph::findFacesForSegment(const Segment& seg) {
    vector<int> suitable_faces; // Индексы подходящих граней

    // Цикл по уложенным граням
    for (size_t i = 0; i < faces.size(); i++) {
        // Создаем для каждой грани список точек ее границы (отсортированный)
        const auto& face = faces[i];
        set<int> face_vertices(face.begin(), face.end()); // Заносим все точки границы в список

        bool suitable = true;
        // Проверка, что все контактные вершины сегмента лежат на границе грани
        for (int v : seg.contact_vertices) {
            if (face_vertices.find(v) == face_vertices.end()) {
                suitable = false;
                break;
            }
        }

        if (suitable) {
            suitable_faces.push_back(i);
        }
    }
    
    return suitable_faces;
}

vector<int> LayingGraph::findPathInSegment(const Segment& seg, int face_id) {
    // Для сегмента А) путь - просто ребро 
    if (seg.type == 0) {
        return {seg.u, seg.v};
    }

    // Извлекаем список контактных вершин сегмента
    const auto& contact_vertices = seg.contact_vertices;
    if (contact_vertices < 2) {
        return {};
    }

    // Перебираем пары контактных вершин, пока не найдем путь
    vector<int> contacts(contact_vertices.begin(), contact_vertices.end());
    set<int> allowed_vertices = seg.vertices;
    allowed_vertices.insert(contacts.begin(), contacts.end());

    // Находим первый попавшийся путь между контактными вершинами сегмента
    // Цикл по всем точкам (начало)
    for (size_t i = 0; i < contacts.size(); i++) {
        for (size_t j = i + 1; j < contacts.size(); j++) { // Цикл по всем точкам после начала (конец)
            // Произвольные начало и конец
            int start = contacts[i];
            int end = contacts[j];

            // Есть ли путь между ними
            vector<int> path = bfs(start, end, allowed_vertices);
            if (!path.empty()) {
                return path;
            }
        }
    }

    return {};
}

void LayingGraph::placeSegment(const Segment& seg, int face_id, vector<int>& path) {
    for (int v : path) {
        laying_vertices.insert();
    }
    for (size_t i = 0; i < path.size() - 1; i++) {
        int u = path[i], v = path[i + 1];
        laying_edges.insert({min(u, v), max(u, v)});
    }

    splitFace(face_id, path);
}

void LayingGraph::splitFace(int face_id, vector<int>& path) {
    if (face_id >= faces.size() || path.size() < 2) return;

    vector<int> face = faces[face_id];
    vector<int> new_face_1, new_face_2;

    int start = path[0], end = path[path.size() - 1];
    
    // Шаг 1: Поиск итераторов start и end в face
    auto start_it = find(face.begin(), face.end(), start);
    auto end_it = find(face.begin(), face.end(), end);

    // Если start и end не найдены — выходим (предполагаем, что они должны быть)
    if (start_it == face.end() || end_it == face.end()) return;

    // Определим направление
    bool start_before_end = (start_it < end_it);
    
    if (start_before_end) {
        // new_face_1: часть face от start до end + внутренние вершины пути (в обратном порядке)
        new_face_1.insert(new_face_1.end(), start_it, end_it + 1);
        for (auto i = path.rbegin() + 1; i != path.rend() - 1; ++i) {
            if (find(face.begin(), face.end(), *i) == face.end()) {
                new_face_1.push_back(*i);
            }
        }

        // new_face_2: от end до конца face + от начала face до start + внутренние вершины пути (прямой порядок)
        new_face_2.insert(new_face_2.end(), end_it, face.end());
        new_face_2.insert(new_face_2.end(), face.begin(), start_it + 1);
        for (auto i = path.begin() + 1; i != path.end() - 1; ++i) {
            if (find(face.begin(), face.end(), *i) == face.end()) {
                new_face_2.push_back(*i);
            }
        }
    } else {
        // Симметричный случай
        new_face_1.insert(new_face_1.end(), end_it, start_it + 1);
        for (auto i = path.begin() + 1; i != path.end() - 1; ++i) {
            if (find(face.begin(), face.end(), *i) == face.end()) {
                new_face_1.push_back(*i);
            }
        }

        new_face_2.insert(new_face_2.end(), start_it, face.end());
        new_face_2.insert(new_face_2.end(), face.begin(), end_it + 1);
        for (auto i = path.rbegin() + 1; i != path.rend() - 1; ++i) {
            if (find(face.begin(), face.end(), *i) == face.end()) {
                new_face_2.push_back(*i);
            }
        }
    }

    // Замыкание граней: если не замкнуты, добавляем первый элемент в конец
    if (!new_face_1.empty() && new_face_1.front() != new_face_1.back()) {
        new_face_1.push_back(new_face_1[0]);
    }
    if (!new_face_2.empty() && new_face_2.front() != new_face_2.back()) {
        new_face_2.push_back(new_face_2[0]);
    }

    // Убираем старую грань и добавляем новые (если не пусты)
    if (!new_face_1.empty() && !new_face_2.empty()) {
        faces.erase(faces.begin() + face_id);
        faces.push_back(new_face_1);
        faces.push_back(new_face_2);
    }
}

