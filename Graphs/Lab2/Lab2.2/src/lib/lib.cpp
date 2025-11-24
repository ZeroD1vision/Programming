/**
 * @file lib.cpp
 * @brief Реализация параллельного алгоритма Борувки для поиска MST
 */

#include "lib.h"
#include <fstream>
#include <queue>
#include <stack>
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
    cout << "Starting planarity check for graph with " << n << " vertices." << endl;
    if (n <= 2) {
        cout << "Graph has <= 2 vertices, considered planar." << endl;
        return true;
    }

    // Шаг 1: Поиск начального цикла
    cout << "Step 1: Finding first cycle..." << endl;
    vector<int> first_cycle = findFirstCycle();
    if (first_cycle.empty()) {
        cout << "No cycle found, graph is a tree, planar." << endl;
        return true;
    }
    cout << "First cycle found: ";
    for (int v : first_cycle) cout << v << " ";
    cout << endl;

    // Замыкаем цикл
    faces.push_back(first_cycle);
    // Создать вторую грань (внешнюю) - обратный порядок
    vector<int> outer_face = first_cycle;
    reverse(outer_face.begin(), outer_face.end());
    faces.push_back(outer_face);
    cout << "Faces initialized: face 0 (inner): ";
    for (int v : faces[0]) cout << v << " ";
    cout << ", face 1 (outer): ";
    for (int v : faces[1]) cout << v << " ";
    cout << endl;

    // Укладываем вершины и еджи цикла
    for (int v : first_cycle) {
        laying_vertices.insert(v);
    }
    for (int i = 0; i < first_cycle.size(); i++) {
        int u = first_cycle[i];
        int v = first_cycle[(i + 1) % first_cycle.size()];
        laying_edges.insert({min(u, v), max(u, v)});
    }
    cout << "Initial layout: vertices: ";
    for (int v : laying_vertices) cout << v << " ";
    cout << ", edges: ";
    for (auto& e : laying_edges) cout << "(" << e.first << "," << e.second << ") ";
    cout << endl;

    // Повторяем шаги 2-7 пока не установим непланарность или не уложим все
    int iteration = 0;
    while (true) {
        iteration++;
        cout << "Iteration " << iteration << ": Starting steps 2-7." << endl;
        // Шаг 2: Поиск доступных сегментов обоих типов, А) и Б)
        // Шаг 3: Определение контактных (внутри findSegments())
        vector<Segment> segments = findSegments();
        if (segments.empty()) {
            cout << "No more segments to lay, graph is planar." << endl;
            break; // Если нечего укладывать - заканчиваем
        }
        cout << "Found " << segments.size() << " segments." << endl;
        
        Segment selected_segment; // Выбранный сегмент для укладки
        int selected_face = -1; // Выбранная грань для укладки сегмента 
        bool found = false; // Флаг, что пока ничего не выбрано.
        
        int min_suitable_count = INF; // Рамзер текущего минимального по включению массива подходящих граней

        // Для всех доступных сегментов
        for (const auto& seg : segments) {
            cout << "Checking segment: type=" << seg.type << ", vertices=";
            if (seg.type == 0) {
                cout << "{" << seg.u << "," << seg.v << "}";
            } else {
                for (int v : seg.vertices) cout << v << " ";
            }
            cout << ", contacts=";
            for (int c : seg.contact_vertices) cout << c << " ";
            cout << endl;
            // Шаг 4: Находим грани вмещающие каждый из сегментов
            vector<int> suitable_faces = findFacesForSegment(seg);
            
            // Шаг 5: Проверка плананрности (никто не вмещает => не уложим)
            if (suitable_faces.empty()) {
                cout << "No suitable faces for this segment, graph is not planar." << endl;
                return false; // Не планарен
            }
            cout << "Suitable faces: ";
            for (int f : suitable_faces) cout << f << " ";
            cout << endl;

            // Шаг 6: Выбор минимального сегмента (его вмещают мин кол-во граней)
            size_t suitable_faces_count = suitable_faces.size();
            if (!found || suitable_faces_count < min_suitable_count) {
                selected_segment = seg;
                min_suitable_count = suitable_faces_count;
                selected_face = suitable_faces[0]; // Так как можем брать любой подходящий
                found = true;
            }
        }

        cout << "Selected segment: type=" << selected_segment.type << endl;
        // Try ALL suitable faces for this segment
        bool placed = false;
        for (int face_candidate : findFacesForSegment(selected_segment)) {
            cout << "Trying face " << face_candidate << " for segment" << endl;
            vector<int> path = findPathInSegment(selected_segment, face_candidate);
            if (!path.empty()) {
                cout << "Placing segment with path: ";
                for (int p : path) cout << p << " ";
                cout << " in face " << face_candidate << endl;
                placeSegment(selected_segment, face_candidate, path);
                cout << "After placement: laying vertices: ";
                for (int v : laying_vertices) cout << v << " ";
                cout << ", edges: ";
                for (auto& e : laying_edges) cout << "(" << e.first << "," << e.second << ") ";
                cout << ", faces count: " << faces.size() << endl;
                placed = true;
                break;
            }
        }
        
        if (!placed) {
            cout << "No suitable face and path found for selected segment, graph is not planar." << endl;
            return false;
        }
    }

    cout << "Planarity check completed: graph is planar." << endl;
    return true;
}

vector<int> LayingGraph::findFirstCycle() {
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);
    vector<int> depth(n, 0);

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            stack<pair<int, int>> st; // (vertex, parent)
            st.push({i, -1});
            visited[i] = true;
            parent[i] = -1;
            depth[i] = 0;

            while (!st.empty()) {
                auto [u, prev] = st.top();
                st.pop();

                for (int v = 0; v < n; v++) {
                    if (graph[u][v] && u != v) {
                        if (!visited[v]) {
                            visited[v] = true;
                            parent[v] = u;
                            depth[v] = depth[u] + 1;
                            st.push({v, u});
                        } else if (v != prev) {
                            // Found cycle
                            vector<int> cycle;
                            int a = u, b = v;
                            
                            // Bring both to same depth
                            while (depth[a] > depth[b]) {
                                cycle.push_back(a);
                                a = parent[a];
                            }
                            while (depth[b] > depth[a]) {
                                cycle.push_back(b);
                                b = parent[b];
                            }
                            
                            // Find common ancestor
                            while (a != b) {
                                cycle.push_back(a);
                                cycle.push_back(b);
                                a = parent[a];
                                b = parent[b];
                            }
                            cycle.push_back(a);
                            
                            // Remove duplicates and ensure minimum length 3
                            set<int> unique_cycle(cycle.begin(), cycle.end());
                            cycle.assign(unique_cycle.begin(), unique_cycle.end());
                            
                            if (cycle.size() >= 3) {
                                // Close the cycle
                                cycle.push_back(cycle[0]);
                                return cycle;
                            }
                        }
                    }
                }
            }
        }
    }
    return {};
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
                laying_vertices.find(u) != laying_vertices.end() &&
                laying_vertices.find(v) != laying_vertices.end() &&
                laying_edges.find({u, v}) == laying_edges.end()) {
                
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
    vector<int> suitable_faces;

    for (size_t i = 0; i < faces.size(); i++) {
        const auto& face = faces[i];
        
        // Для сегментов типа 0 (ребро) проверяем, что обе контактные вершины находятся в грани
        // и что они соседние в цикле грани (или могут быть соединены через грань)
        if (seg.type == 0) {
            bool start_found = false;
            bool end_found = false;
            
            for (int v : face) {
                if (v == seg.u) start_found = true;
                if (v == seg.v) end_found = true;
            }
            
            if (start_found && end_found) {
                // Дополнительная проверка: вершины должны быть соседними в грани
                // или между ними должен быть путь по грани, не содержащий других контактных вершин
                for (size_t j = 0; j < face.size() - 1; j++) {
                    if ((face[j] == seg.u && face[j+1] == seg.v) ||
                        (face[j] == seg.v && face[j+1] == seg.u)) {
                        suitable_faces.push_back(i);
                        break;
                    }
                }
                
                // Если не нашли как соседние, все равно добавляем грань
                // (возможно, вершины не соседние, но находятся в одной грани)
                if (suitable_faces.empty() || suitable_faces.back() != i) {
                    suitable_faces.push_back(i);
                }
            }
        } 
        // Для сегментов типа 1 (компонента) проверяем, что все контактные вершины в грани
        else {
            set<int> face_vertices(face.begin(), face.end());
            bool suitable = true;
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
    }
    
    return suitable_faces;
}

vector<int> LayingGraph::findPathInSegment(const Segment& seg, int face_id) {
    cout << "Finding path for segment type " << seg.type << " with " 
         << seg.contact_vertices.size() << " contact vertices" << endl;
    
    if (seg.type == 0) {
        return {seg.u, seg.v};
    }

    const auto& contacts = seg.contact_vertices;
    if (contacts.empty()) {
        return {};
    }

    // Если только одна контактная вершина
    if (contacts.size() == 1) {
        int contact = *contacts.begin();
        
        for (int target : seg.vertices) {
            if (target == contact) continue;
            
            set<int> allowed = seg.vertices;
            allowed.insert(contact);
            
            vector<int> path = bfs(contact, target, allowed);
            if (!path.empty()) {
                return path;
            }
        }
        return {};
    }

    // Для нескольких контактных вершин
    vector<int> contact_list(contacts.begin(), contacts.end());
    set<int> allowed = seg.vertices;
    allowed.insert(contacts.begin(), contacts.end());

    cout << "Multiple contacts: ";
    for (int c : contact_list) cout << c << " ";
    cout << endl;

    // Пробуем все пары контактных вершин
    for (size_t i = 0; i < contact_list.size(); i++) {
        for (size_t j = i + 1; j < contact_list.size(); j++) {
            int start = contact_list[i];
            int end = contact_list[j];
            
            if (laying_edges.find({min(start, end), max(start, end)}) != laying_edges.end()) {
                cout << "Edge (" << start << "," << end << ") already laid, skipping" << endl;
                continue;
            }
            
            cout << "Trying path from " << start << " to " << end << endl;
            vector<int> path = bfs(start, end, allowed);
            
            if (!path.empty()) {
                // Проверяем, что путь содержит хотя бы одну вершину сегмента
                bool has_segment_vertex = false;
                for (int v : path) {
                    if (seg.vertices.find(v) != seg.vertices.end()) {
                        has_segment_vertex = true;
                        break;
                    }
                }
                if (!has_segment_vertex) {
                    cout << "Path does not contain any segment vertex, skipping" << endl;
                    continue;
                }
                
                // Проверяем, что путь содержит хотя бы одно новое ребро
                bool has_new_edge = false;
                for (size_t k = 0; k < path.size() - 1; k++) {
                    int u = path[k], v = path[k + 1];
                    if (laying_edges.find({min(u, v), max(u, v)}) == laying_edges.end()) {
                        has_new_edge = true;
                        break;
                    }
                }
                if (!has_new_edge) {
                    cout << "Path contains only already laid edges, skipping" << endl;
                    continue;
                }
                
                cout << "Found path between contacts: ";
                for (int v : path) cout << v << " ";
                cout << endl;
                return path;
            }
        }
    }

    // Если не нашли путь между контактами, ищем путь через внутренние вершины сегмента
    cout << "No direct path between contacts, trying path through segment vertices..." << endl;
    
    for (int start : contact_list) {
        for (int end : contact_list) {
            if (start == end) continue;
            
            for (int intermediate : seg.vertices) {
                if (intermediate == start || intermediate == end) continue;
                
                set<int> allowed1 = seg.vertices;
                allowed1.insert(start);
                vector<int> path1 = bfs(start, intermediate, allowed1);
                
                if (!path1.empty()) {
                    set<int> allowed2 = seg.vertices;
                    allowed2.insert(end);
                    vector<int> path2 = bfs(intermediate, end, allowed2);
                    
                    if (!path2.empty()) {
                        vector<int> full_path = path1;
                        full_path.insert(full_path.end(), path2.begin() + 1, path2.end());
                        
                        // Проверяем, что путь содержит хотя бы одно новое ребро
                        bool has_new_edge = false;
                        for (size_t k = 0; k < full_path.size() - 1; k++) {
                            int u = full_path[k], v = full_path[k + 1];
                            if (laying_edges.find({min(u, v), max(u, v)}) == laying_edges.end()) {
                                has_new_edge = true;
                                break;
                            }
                        }
                        if (!has_new_edge) {
                            continue;
                        }
                        
                        cout << "Found path through segment: ";
                        for (int v : full_path) cout << v << " ";
                        cout << endl;
                        return full_path;
                    }
                }
            }
        }
    }

    cout << "No valid path found for segment" << endl;
    return {};
}

void LayingGraph::placeSegment(const Segment& seg, int face_id, const vector<int>& path) {
    for (int v : path) {
        laying_vertices.insert(v);
    }
    for (size_t i = 0; i < path.size() - 1; i++) {
        int u = path[i], v = path[i + 1];
        laying_edges.insert({min(u, v), max(u, v)});
    }

    splitFace(face_id, path);
}

void LayingGraph::splitFace(int face_id, const vector<int>& path) {
    if (face_id >= faces.size() || path.size() < 2) return;

    vector<int> face = faces[face_id];
    int start = path[0];
    int end = path[path.size() - 1];
    
    cout << "Splitting face " << face_id << ": ";
    for (int v : face) cout << v << " ";
    cout << " with path: ";
    for (int v : path) cout << v << " ";
    cout << endl;

    // Находим start в грани
    int start_idx = -1;
    for (int i = 0; i < face.size() - 1; i++) {
        if (face[i] == start) {
            start_idx = i;
            break;
        }
    }

    if (start_idx == -1) {
        cout << "ERROR: Start vertex " << start << " not found in face" << endl;
        return;
    }

    // Находим end в грани
    int end_idx = -1;
    for (int i = 0; i < face.size() - 1; i++) {
        if (face[i] == end) {
            end_idx = i;
            break;
        }
    }

    vector<int> new_face1, new_face2;

    if (end_idx != -1) {
        // Обе контактные вершины найдены в грани
        cout << "Both contacts found in face" << endl;
        
        // Определяем направление обхода грани
        if (start_idx < end_idx) {
            // Грань 1: от start до end по грани + путь в обратном порядке
            for (int i = start_idx; i <= end_idx; i++) {
                new_face1.push_back(face[i]);
            }
            for (int i = path.size() - 2; i >= 1; i--) {
                new_face1.push_back(path[i]);
            }

            // Грань 2: от end до start через оставшуюся часть грани + путь в прямом порядке
            for (int i = end_idx; i < face.size() - 1; i++) {
                new_face2.push_back(face[i]);
            }
            for (int i = 0; i <= start_idx; i++) {
                new_face2.push_back(face[i]);
            }
            for (int i = 1; i < path.size() - 1; i++) {
                new_face2.push_back(path[i]);
            }
        } else {
            // === ОШИБКА БЫЛА ЗДЕСЬ (start_idx > end_idx) ===
            
            // Грань 1: Идем start -> конец массива -> начало массива -> end.
            // Мы пришли в end. Нам нужно вернуться в start.
            // Путь path: start -> ... -> end.
            // Значит, добавляем путь В ОБРАТНОМ ПОРЯДКЕ (path reverse).
            
            for (int i = start_idx; i < face.size() - 1; i++) {
                new_face1.push_back(face[i]);
            }
            for (int i = 0; i <= end_idx; i++) {
                new_face1.push_back(face[i]);
            }
            // ИСПРАВЛЕНИЕ: Был прямой цикл, меняем на обратный
            for (int i = path.size() - 2; i >= 1; i--) { 
                new_face1.push_back(path[i]);
            }

            // Грань 2: Идем end -> start напрямую.
            // Мы пришли в start. Нам нужно вернуться в end.
            // Путь path: start -> ... -> end.
            // Значит, добавляем путь В ПРЯМОМ ПОРЯДКЕ (path forward).

            for (int i = end_idx; i <= start_idx; i++) {
                new_face2.push_back(face[i]);
            }
            // ИСПРАВЛЕНИЕ: Был обратный цикл, меняем на прямой
            for (int i = 1; i < path.size() - 1; i++) {
                new_face2.push_back(path[i]);
            }
        }
    } else {
        // Только start найден в грани, end - новая вершина
        cout << "Only start contact found, end is new vertex" << endl;
        
        // Создаем новую малую грань [start, end, start]
        new_face1.push_back(start);
        new_face1.push_back(end);
        new_face1.push_back(start);

        // Оставляем исходную грань без изменений
        new_face2 = face;

        // Добавляем новую грань, не удаляя исходную
        faces.push_back(new_face1);

        cout << "Created new face: ";
        for (int v : new_face1) cout << v << " ";
        cout << endl;
        
        return;
    }

    // Замыкаем грани
    if (!new_face1.empty() && new_face1.front() != new_face1.back()) {
        new_face1.push_back(new_face1.front());
    }
    if (!new_face2.empty() && new_face2.front() != new_face2.back()) {
        new_face2.push_back(new_face2.front());
    }

    // Проверяем, что новые грани корректны
    if (new_face1.size() < 3 || new_face2.size() < 3) {
        cout << "ERROR: New faces are too small" << endl;
        return;
    }

    // Заменяем старую грань
    faces.erase(faces.begin() + face_id);
    faces.push_back(new_face1);
    faces.push_back(new_face2);

    cout << "Created new faces:" << endl;
    cout << "Face " << faces.size() - 2 << ": ";
    for (int v : new_face1) cout << v << " ";
    cout << endl;
    cout << "Face " << faces.size() - 1 << ": ";
    for (int v : new_face2) cout << v << " ";
    cout << endl;
}

vector<set<int>> LayingGraph::findConnectedComponents(const set<int>& vertices) {
    vector<set<int>> components;
    set<int> visited;

    for (int v : vertices) {
        if (visited.find(v) == visited.end()) {
            set<int> component;
            queue<int> q;
            q.push(v);
            visited.insert(v);

            while(!q.empty()) {
                int u = q.front();
                q.pop();
                component.insert(u);

                for (int w = 0; w < n; w++) {
                    if (graph[u][w] && 
                        vertices.find(w) != vertices.end() && 
                        visited.find(w) == visited.end()) {
                        
                        visited.insert(w);
                        q.push(w);
                    }
                }
            }
            components.push_back(component);
        }
    }
    
    return components;
}

vector<int> LayingGraph::bfs(int start, int end, const set<int>& allowed_vertices) {
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    queue<int> q;
    
    q.push(start);
    visited[start] = true;
    parent[start] = -1;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        if (u == end) {
            // Восстанавливаем путь от end к start
            vector<int> path;
            int current = end;
            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }
            reverse(path.begin(), path.end());
            return path;
        }
        
        for (int v = 0; v < n; v++) {
            if (graph[u][v] && allowed_vertices.find(v) != allowed_vertices.end() && !visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    
    return {}; // Путь не найден
}