/**
 * @file lib.cpp
 * @brief Реализация алгоритма проверки планарности Gamma
 * 
 * Содержит реализацию основных методов класса LayingGraph для проверки
 * планарности графа и построения плоской укладки.
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

// ==================== ЦВЕТА ДЛЯ ОТЛАДОЧНОГО ВЫВОДА ====================
#define DEBUG_COLOR COLOR_GRAY
#define SEGMENT_COLOR COLOR_MAGENTA
#define FACE_COLOR COLOR_CYAN
#define ERROR_COLOR COLOR_RED
#define SUCCESS_COLOR COLOR_GREEN

#define DEBUG_FUNC(x) cout << DEBUG_COLOR << "[DEBUG][" << __func__ << "] " << x << COLOR_RESET << endl
#define SEGMENT_DEBUG(x) cout << SEGMENT_COLOR << "[SEGMENT] " << x << COLOR_RESET << endl
#define FACE_DEBUG(x) cout << FACE_COLOR << "[FACE] " << x << COLOR_RESET << endl

// ==================== РЕАЛИЗАЦИЯ МЕТОДОВ LayingGraph ====================

/**
 * @brief Конструктор класса LayingGraph
 * @param adj_matrix Матрица смежности графа
 * 
 * Инициализирует объект для проверки планарности переданного графа.
 */
LayingGraph::LayingGraph(const vector<vector<int>>& adj_matrix) 
    : graph(adj_matrix), n(adj_matrix.size()) {}

/**
 * @brief Основной метод проверки планарности
 * @return true если граф планарен, false в противном случае
 * 
 * Реализует алгоритм Gamma:
 * 1. Находит начальный цикл для базовой укладки
 * 2. Итеративно находит и укладывает сегменты
 * 3. Проверяет возможность укладки каждого сегмента
 * 4. Разделяет грани при укладке новых путей
 */
bool LayingGraph::isLaying() {
    DEBUG_FUNC("Starting planarity check for graph with " << n << " vertices");
    
    // Граф с <= 2 вершинами всегда планарен
    if (n <= 2) {
        DEBUG_FUNC("Small graph (n<=2), automatically planar");
        return true;
    }

    // ==================== ШАГ 1: ПОИСК НАЧАЛЬНОГО ЦИКЛА ====================
    DEBUG_FUNC("Step 1: Finding initial cycle...");
    vector<int> first_cycle = findFirstCycle();
    if (first_cycle.empty()) {
        DEBUG_FUNC("No cycle found - graph is a tree, planar");
        return true; // Граф - дерево, планарен
    }
    
    // Формируем отладочную строку с вершинами цикла
    stringstream cycle_ss;
    cycle_ss << "Initial cycle found: ";
    for (int v : first_cycle) cycle_ss << v << " ";
    DEBUG_FUNC(cycle_ss.str());

    // ==================== ИНИЦИАЛИЗАЦИЯ УКЛАДКИ ====================
    faces.push_back(first_cycle);
    
    // Создаем внешнюю грань (обратный порядок вершин)
    vector<int> outer_face = first_cycle;
    reverse(outer_face.begin(), outer_face.end());
    faces.push_back(outer_face);
    
    // Добавляем вершины и ребра цикла в уложенную часть
    for (int v : first_cycle) {
        laying_vertices.insert(v);
    }
    for (int i = 0; i < first_cycle.size(); i++) {
        int u = first_cycle[i];
        int v = first_cycle[(i + 1) % first_cycle.size()];
        laying_edges.insert({min(u, v), max(u, v)});
    }

    // ==================== ИТЕРАТИВНАЯ УКЛАДКА СЕГМЕНТОВ ====================
    int iteration = 0;
    while (true) {
        iteration++;
        DEBUG_FUNC("=== Iteration " << iteration << " ===");
        
        // ==================== ШАГ 2-3: ПОИСК СЕГМЕНТОВ ====================
        vector<Segment> segments = findSegments();
        if (segments.empty()) {
            DEBUG_FUNC("No more segments to lay - graph is planar");
            break; // Все сегменты уложены - граф планарен
        }
        
        SEGMENT_DEBUG("Found " << segments.size() << " segments");
        
        Segment selected_segment;
        int selected_face = -1;
        bool found = false;
        int min_suitable_count = INF;

        // ==================== ШАГ 4-6: ВЫБОР СЕГМЕНТА И ГРАНИ ====================
        for (const auto& seg : segments) {
            // Формируем отладочную информацию о сегменте
            stringstream seg_ss;
            seg_ss << "Type=" << seg.type << ", Contacts=[";
            for (int c : seg.contact_vertices) seg_ss << c << " ";
            seg_ss << "]";
            if (seg.type == 0) {
                seg_ss << " Edge: " << seg.u << "-" << seg.v;
            } else {
                seg_ss << " Vertices: ";
                for (int v : seg.vertices) seg_ss << v << " ";
            }
            SEGMENT_DEBUG(seg_ss.str());

            // Находим подходящие грани для сегмента
            vector<int> suitable_faces = findFacesForSegment(seg);
            
            // ==================== ШАГ 5: ПРОВЕРКА ВОЗМОЖНОСТИ УКЛАДКИ ====================
            if (suitable_faces.empty()) {
                DEBUG_FUNC("No suitable faces for segment - graph not planar");
                return false; // Не можем уложить сегмент - граф не планарен
            }

            // Выбираем сегмент с минимальным количеством подходящих граней
            size_t suitable_faces_count = suitable_faces.size();
            if (!found || suitable_faces_count < min_suitable_count) {
                selected_segment = seg;
                min_suitable_count = suitable_faces_count;
                selected_face = suitable_faces[0];
                found = true;
            }
        }

        // ==================== ШАГ 7: УКЛАДКА ВЫБРАННОГО СЕГМЕНТА ====================
        DEBUG_FUNC("Selected segment type " << selected_segment.type << " with " 
                  << min_suitable_count << " suitable faces");
        
        // Пробуем уложить сегмент в одну из подходящих граней
        bool placed = false;
        for (int face_candidate : findFacesForSegment(selected_segment)) {
            FACE_DEBUG("Trying face " << face_candidate << " for segment");
            
            vector<int> path = findPathInSegment(selected_segment, face_candidate);
            if (!path.empty()) {
                stringstream path_ss;
                path_ss << "Path found: ";
                for (int p : path) path_ss << p << " ";
                DEBUG_FUNC(path_ss.str());
                
                placeSegment(selected_segment, face_candidate, path);
                placed = true;
                break;
            }
        }
        
        if (!placed) {
            DEBUG_FUNC("Failed to place segment in any face - graph not planar");
            return false;
        }
    }

    DEBUG_FUNC("Planarity check completed successfully");
    return true;
}

/**
 * @brief Находит первый цикл в графе для начальной укладки
 * @return Вектор вершин цикла или пустой вектор если цикл не найден
 * 
 * Использует DFS для поиска циклов. Найденный цикл служит основой
 * для начальной укладки графа.
 */
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
                            // Найден цикл - восстанавливаем его
                            vector<int> cycle;
                            int a = u, b = v;
                            
                            // Поднимаем вершины до общего уровня
                            while (depth[a] > depth[b]) {
                                cycle.push_back(a);
                                a = parent[a];
                            }
                            while (depth[b] > depth[a]) {
                                cycle.push_back(b);
                                b = parent[b];
                            }
                            
                            // Находим общего предка
                            while (a != b) {
                                cycle.push_back(a);
                                cycle.push_back(b);
                                a = parent[a];
                                b = parent[b];
                            }
                            cycle.push_back(a);
                            
                            // Убираем дубликаты и проверяем длину
                            set<int> unique_cycle(cycle.begin(), cycle.end());
                            cycle.assign(unique_cycle.begin(), unique_cycle.end());
                            
                            if (cycle.size() >= 3) {
                                cycle.push_back(cycle[0]); // Замыкаем цикл
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

/**
 * @brief Находит все сегменты для текущей укладки
 * @return Вектор найденных сегментов
 * 
 * Сегменты бывают двух типов:
 * - Тип 0: Ребра между уложенными вершинами, но не уложенные
 * - Тип 1: Компоненты связности неуложенных вершин
 */
vector<Segment> LayingGraph::findSegments() {
    vector<Segment> segments;
    
    // Находим неуложенные вершины
    set<int> not_laying_vertices;
    for (int i = 0; i < n; i++) {
        if (laying_vertices.find(i) == laying_vertices.end()) {
            not_laying_vertices.insert(i);
        }
    }

    // ==================== ПОИСК СЕГМЕНТОВ ТИПА 0 (РЕБРА) ====================
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            // Ребро существует, обе вершины уложены, но ребро не уложено
            if (graph[u][v] && 
                laying_vertices.find(u) != laying_vertices.end() &&
                laying_vertices.find(v) != laying_vertices.end() &&
                laying_edges.find({u, v}) == laying_edges.end()) {
                
                Segment seg;
                seg.type = 0;
                seg.u = u;
                seg.v = v;
                seg.contact_vertices = {u, v};
                segments.push_back(seg);
            }
        }
    }

    // ==================== ПОИСК СЕГМЕНТОВ ТИПА 1 (КОМПОНЕНТЫ) ====================
    vector<set<int>> components = findConnectedComponents(not_laying_vertices);

    for (const auto& comp : components) {
        Segment seg;
        seg.type = 1;
        seg.vertices = comp;

        // Находим контактные вершины и связи с уложенной частью
        for (int u : comp) {
            for (int v = 0; v < n; v++) {
                if (graph[u][v] && laying_vertices.find(v) != laying_vertices.end()) {
                    seg.contact_vertices.insert(v);
                    seg.edges.insert({min(u, v), max(u, v)});
                }
            }
        }

        // Добавляем внутренние ребра компоненты
        for (int u : comp) {
            for (int v : comp) {
                if (graph[u][v] && u < v) {
                    seg.edges.insert({u, v});
                }
            }
        }

        if (!seg.contact_vertices.empty()) {
            segments.push_back(seg);
        }
    }
    
    return segments;
}

/**
 * @brief Находит грани, подходящие для укладки сегмента
 * @param seg Сегмент для укладки
 * @return Вектор индексов подходящих граней
 * 
 * Для сегмента типа 0 проверяет, что обе контактные вершины находятся в грани.
 * Для сегмента типа 1 проверяет, что все контактные вершины находятся в грани.
 */
vector<int> LayingGraph::findFacesForSegment(const Segment& seg) {
    vector<int> suitable_faces;

    for (size_t i = 0; i < faces.size(); i++) {
        const auto& face = faces[i];
        
        if (seg.type == 0) {
            // Для ребра проверяем наличие обеих вершин в грани
            bool start_found = false, end_found = false;
            
            for (int v : face) {
                if (v == seg.u) start_found = true;
                if (v == seg.v) end_found = true;
            }
            
            if (start_found && end_found) {
                suitable_faces.push_back(i);
            }
        } else {
            // Для компоненты проверяем наличие всех контактных вершин в грани
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

/**
 * @brief Находит путь для укладки сегмента в указанной грани
 * @param seg Сегмент для укладки
 * @param face_id Индекс грани
 * @return Путь укладки в виде последовательности вершин
 * 
 * Для сегментов типа 0 путь состоит из двух вершин ребра.
 * Для сегментов типа 1 ищет путь между контактными вершинами через сегмент.
 */
vector<int> LayingGraph::findPathInSegment(const Segment& seg, int face_id) {
    DEBUG_FUNC("Finding path for segment type " << seg.type);
    
    if (seg.type == 0) {
        return {seg.u, seg.v}; // Путь для ребра - просто само ребро
    }

    const auto& contacts = seg.contact_vertices;
    if (contacts.empty()) {
        return {};
    }

    // Одна контактная вершина - ищем путь к любой вершине сегмента
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

    // Несколько контактных вершин - ищем путь между ними
    vector<int> contact_list(contacts.begin(), contacts.end());
    set<int> allowed = seg.vertices;
    allowed.insert(contacts.begin(), contacts.end());

    // Пробуем все пары контактных вершин
    for (size_t i = 0; i < contact_list.size(); i++) {
        for (size_t j = i + 1; j < contact_list.size(); j++) {
            int start = contact_list[i];
            int end = contact_list[j];
            
            // Пропускаем уже уложенные ребра
            if (laying_edges.find({min(start, end), max(start, end)}) != laying_edges.end()) {
                continue;
            }
            
            vector<int> path = bfs(start, end, allowed);
            
            if (!path.empty()) {
                // Проверяем, что путь содержит вершины сегмента и новые ребра
                bool has_segment_vertex = false;
                bool has_new_edge = false;
                
                for (int v : path) {
                    if (seg.vertices.find(v) != seg.vertices.end()) {
                        has_segment_vertex = true;
                        break;
                    }
                }
                
                for (size_t k = 0; k < path.size() - 1; k++) {
                    int u = path[k], v = path[k + 1];
                    if (laying_edges.find({min(u, v), max(u, v)}) == laying_edges.end()) {
                        has_new_edge = true;
                        break;
                    }
                }
                
                if (has_segment_vertex && has_new_edge) {
                    return path;
                }
            }
        }
    }

    // Пробуем путь через внутренние вершины сегмента
    for (int start : contact_list) {
        for (int end : contact_list) {
            if (start == end) continue;
            
            for (int intermediate : seg.vertices) {
                if (intermediate == start || intermediate == end) continue;
                
                // Ищем путь start -> intermediate -> end
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
                        return full_path;
                    }
                }
            }
        }
    }

    return {};
}

/**
 * @brief Укладывает сегмент в указанную грань
 * @param seg Сегмент для укладки
 * @param face_id Индекс грани
 * @param path Путь укладки
 * 
 * Добавляет вершины и ребра пути в уложенную часть графа
 * и разделяет грань на две новые грани.
 */
void LayingGraph::placeSegment(const Segment& seg, int face_id, const vector<int>& path) {
    // Добавляем вершины и ребра пути
    for (int v : path) {
        laying_vertices.insert(v);
    }
    for (size_t i = 0; i < path.size() - 1; i++) {
        int u = path[i], v = path[i + 1];
        laying_edges.insert({min(u, v), max(u, v)});
    }

    // Разделяем грань
    splitFace(face_id, path);
}

/**
 * @brief Разделяет грань при укладке нового пути
 * @param face_id Индекс разделяемой грани
 * @param path Путь, разделяющий грань
 * 
 * Создает две новые грани из исходной, разделенной путем.
 * Обрабатывает различные случаи расположения контактных вершин.
 */
void LayingGraph::splitFace(int face_id, const vector<int>& path) {
    if (face_id >= faces.size() || path.size() < 2) return;

    vector<int> face = faces[face_id];
    int start = path[0];
    int end = path[path.size() - 1];
    
    FACE_DEBUG("Splitting face " << face_id << " with path from " << start << " to " << end);

    // Находим индексы контактных вершин в грани
    int start_idx = -1, end_idx = -1;
    for (int i = 0; i < face.size() - 1; i++) {
        if (face[i] == start) start_idx = i;
        if (face[i] == end) end_idx = i;
    }

    if (start_idx == -1) {
        cout << ERROR_COLOR << "[ERROR] Start vertex " << start << " not found in face" << COLOR_RESET << endl;
        return;
    }

    vector<int> new_face1, new_face2;

    if (end_idx != -1) {
        // Обе контактные вершины найдены в грани
        if (start_idx < end_idx) {
            // Грань 1: от start до end по грани + путь в обратном порядке
            for (int i = start_idx; i <= end_idx; i++) {
                new_face1.push_back(face[i]);
            }
            for (int i = path.size() - 2; i >= 1; i--) {
                new_face1.push_back(path[i]);
            }

            // Грань 2: от end до start через оставшуюся часть + путь в прямом порядке
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
            // start_idx > end_idx
            for (int i = start_idx; i < face.size() - 1; i++) {
                new_face1.push_back(face[i]);
            }
            for (int i = 0; i <= end_idx; i++) {
                new_face1.push_back(face[i]);
            }
            for (int i = path.size() - 2; i >= 1; i--) {
                new_face1.push_back(path[i]);
            }

            for (int i = end_idx; i <= start_idx; i++) {
                new_face2.push_back(face[i]);
            }
            for (int i = 1; i < path.size() - 1; i++) {
                new_face2.push_back(path[i]);
            }
        }
    } else {
        // Только start найден в грани
        new_face1.push_back(start);
        new_face1.push_back(end);
        new_face1.push_back(start);

        new_face2 = face;
        faces.push_back(new_face1);
        return;
    }

    // Замыкаем грани
    if (!new_face1.empty() && new_face1.front() != new_face1.back()) {
        new_face1.push_back(new_face1.front());
    }
    if (!new_face2.empty() && new_face2.front() != new_face2.back()) {
        new_face2.push_back(new_face2.front());
    }

    // Проверяем корректность новых граней
    if (new_face1.size() < 3 || new_face2.size() < 3) {
        cout << ERROR_COLOR << "[ERROR] New faces are too small" << COLOR_RESET << endl;
        return;
    }

    // Заменяем старую грань двумя новыми
    faces.erase(faces.begin() + face_id);
    faces.push_back(new_face1);
    faces.push_back(new_face2);
}

/**
 * @brief Находит компоненты связности в множестве вершин
 * @param vertices Множество вершин
 * @return Вектор компонент связности
 * 
 * Использует BFS для поиска компонент связности.
 */
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

/**
 * @brief Поиск в ширину с ограничением по допустимым вершинам
 * @param start Начальная вершина
 * @param end Конечная вершина
 * @param allowed_vertices Множество допустимых вершин
 * @return Кратчайший путь или пустой вектор если путь не найден
 */
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
            // Восстанавливаем путь
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
    
    return {};
}