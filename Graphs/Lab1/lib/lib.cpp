#include "lib.h"
#include <climits>
#include <fstream>
#include <iostream>

using namespace std;

const int INF = INT_MAX;

/**
* @brief Конструктор графа
* @param filename Путь к файлу с матрицей смежности (nullptr для пустого графа)
* @param directed Тип графа: true - ориентированный, false - неориентированный
*/
Graph::Graph(const char *filename, bool directed)
    : is_directed(directed), vertex_count(0), matrix_size(0), matrix(nullptr),
      vertices(nullptr) {
    // Работа с файлом
    if (filename && filename[0] != '\0') {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Cannot open file" << filename << endl;
            return;
        }
      
        // Читаем число точек из файла
        int n; // Число вершин
        file >> n;
      
        // Выделение места и инициализация матриц
        matrix_size =
            n + 10; // Берем с запасом, для больших размеров матриц (n>100)
        vertices = new int[matrix_size];
        matrix = new int *[matrix_size];
        for (int i = 0; i < matrix_size; i++) {
            matrix[i] = new int[matrix_size](); // Выделяем место для каждой строки
                                                // размером matrix_size
            vertices[i] = -1;                   // Вершины нет (индекс < 0)
        }
      
        // Добавляем вершины в граф
        for (int i = 0; i < n; i++) {
          add_vertex(i);
        }

        // Читаем матрицу смежности из файла и добавляем рёбра
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int weight;
                file >> weight;
                // Если вес не нулевой, добавляем ребро между вершинами i и j
                if (weight != 0) {
                    add_edge(i, j, weight);
                }
            }
        }
    
        file.close();
    }
}

Graph::~Graph() {
    // Освобождаем память: сначала каждую строку матрицы, затем массив указателей
    for (int i = 0; i < matrix_size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] vertices;
}

int Graph::find_vertex_index(int vertex) {
    // Поиск вершины в массиве vertices
    for (int i = 0; i < vertex_count; i++) {
        if (vertices[i] == vertex) {
            return i;
        }
    }
    return -1;
}

// Увеличение размера матрицы при нехватке места
void Graph::expand_matrix() {
    // Новый размер = текущий + 10
    int new_size = matrix_size + 10;
    int** new_matrix = new int*[new_size];
    int* new_vertices = new int[new_size];
    
    // Инициализируем новую матрицу и массив вершин
    for (int i = 0; i < new_size; i++) {
        new_matrix[i] = new int[new_size]();  // Инициализация нулями
        new_vertices[i] = -1;
        
        // Копируем данные из старой матрицы
        if (i < matrix_size) {
            new_vertices[i] = vertices[i];
            for (int j = 0; j < matrix_size; j++) {
                new_matrix[i][j] = matrix[i][j];
            }
        }
    }
    
    // Освобождаем память старой матрицы
    for (int i = 0; i < matrix_size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] vertices;
    
    // Обновляем указатели и размер
    matrix = new_matrix;
    vertices = new_vertices;
    matrix_size = new_size;
}

int Graph::size() { return vertex_count; }

int Graph::weight(int vertex1, int vertex2) {
    // Находим индексы вершин в массиве
    int i = find_vertex_index(vertex1);
    int j = find_vertex_index(vertex2);

    if (i == -1 || j == -1) {
        cout << "Error: Vertex not found" << endl;
        return 0;
    }

    return matrix[i][j];
}

bool Graph::is_edge(int vertex1, int vertex2) {
    int i = find_vertex_index(vertex1);
    int j = find_vertex_index(vertex2);

    if (i == -1 || j == -1) {
        return false;
    }

    return matrix[i][j] != 0;
}

void Graph::add_vertex(int vertex) {
    // Проверяем, не существует ли уже такая вершина
    if (find_vertex_index(vertex) != -1) {
        cout << "Error: Vertex already exists" << endl;
        return;
    }

    // Если массив полон, увеличиваем его размер
    if (vertex_count >= matrix_size) {
        expand_matrix();
    }

    // Добавляем вершину в конец массива
    vertices[vertex_count] = vertex;
    vertex_count++;
}

void Graph::add_edge(int vertex1, int vertex2, int weight) {
    // Находим индексы вершин
    int i = find_vertex_index(vertex1);
    int j = find_vertex_index(vertex2);

    if (i == -1 || j == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    // Устанавливаем вес в матрице смежности
    matrix[i][j] = weight;
    // Если граф неориентированный, добавляем обратное ребро
    if (!is_directed) {
        matrix[j][i] = weight;
    }
}

void Graph::remove_vertex(int vertex) {
    int idx = find_vertex_index(vertex);
    if (idx == -1) {
        cout << "Error: vertex not found" << endl;
        return;
    }

    // Удаление из массива точек
    for (int i = idx; i < vertex_count - 1; i++) {
        // Смещаем весь массив на единицу влево
        vertices[i] = vertices[i + 1];
    }
    vertices[vertex_count - 1] = -1;

    // Удаление из матрицы (удаляем столбец и строку)
    // Удаление столбца
    for (int i = 0; i < vertex_count; i++) {
        for (int j = idx; j < vertex_count - 1; j++) {
            matrix[i][j] = matrix[i][j + 1];
        }
        matrix[i][vertex_count - 1] = 0;
    }

    // Удаление строки
    for (int i = idx; i < vertex_count - 1; i++) {
        for (int j = 0; j < vertex_count; j++) {
            matrix[i][j] = matrix[i + 1][j];
        }
    }

    for (int j = 0; j < vertex_count; j++) {
        matrix[vertex_count - 1][j] = 0;
    }

    vertex_count--; // Уменьашем количество точек
}

void Graph::remove_edge(int vertex1, int vertex2) {
    int i = find_vertex_index(vertex1);
    int j = find_vertex_index(vertex2);

    if (i == -1 || j == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    matrix[i][j] = 0; // Обнуляем вес = удаляем ребро
    if (!is_directed) {
        matrix[j][i] = 0; // Для неориентированного графа удаляем и обратное ребро
    }
}

// Все ребра между всеми точками
void Graph::list_of_edges() {
    // Проходим по всем возможным парам вершин
    for (int i = 0; i < vertex_count; i++) {
        // Для неориентированного графа начинаем с i, чтобы избежать дубликатов
        for (int j = (is_directed ? 0 : i); j < vertex_count; j++) {
            if (matrix[i][j] != 0) {
                // Выводим: вершина1 вершина2 вес
                cout << vertices[i] << " " << vertices[j] << " " 
                << matrix[i][j] << endl;
            }
        }
    }
}

// Все ребра исходящие из конкретной точки
void Graph::list_of_edges(int vertex) {
    int idx = find_vertex_index(vertex);
    if (idx == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    // Проходим по всем вершинам и выводим рёбра из заданной вершины
    for (int j = 0; j < vertex_count; j++) {
        if (matrix[idx][j] != 0) {
            cout << vertex << " -> " << vertices[j] << ": " 
            << matrix[idx][j] << endl;
        }
    }
}

void Graph::list_of_vertices() {
    // Вывод всех вершин
    for (int i = 0; i < vertex_count; i++) {
        cout << vertices[i] << endl;
    }
    cout << endl;
}

bool Graph::is_connected() {
    if (vertex_count == 0) return true; // Пустой граф связен по определению
    
    // Массивы для поиска в глубину (DFS)
    bool* visited = new bool[vertex_count](); // Массив посещённых вершин (инициализирован false)
    int* path = new int[vertex_count];        // Стек для DFS
    int top = -1;                             // Вершина стека

    // Начинаем обход с первой вершины
    path[++top] = 0;
    visited[0] = true;
    int count = 1; // Счётчик посещённых вершин

    // Пока стек не пуст
    while (top >= 0) {
        int current = path[top--]; // Извлекаем вершину из стека

        // Проверяем всех соседей текущей вершины
        for (int j = 0; j < vertex_count; j++) {
            bool has_edge = false;

            // Для неориентированного графа проверяем рёбра в обоих направлениях
            if (!is_directed) {
                has_edge = (matrix[current][j] != 0 || matrix[j][current]);
            } else {
                has_edge = (matrix[current][j] != 0); // Для ориентированного - только исходящие
            }

            // Если есть ребро и вершина не посещена, добавляем в стек
            if (has_edge && !visited[j]) {
                visited[j] = true;
                path[++top] = j;
                count++;  // Увеличиваем счётчик посещённых вершин
            }
        }
    }
    delete[] visited;
    delete[] path;
    
    // Граф связен если все вершины были посещены
    return count == vertex_count;
}

void Graph::connected_components() {
    bool* visited = new bool[vertex_count](); // Массив посещённых вершин

    // Для каждой вершины
    for (int i = 0; i < vertex_count; i++) {
        if (!visited[i]) {
            // Начинаем новую компоненту связности
            int* path = new int[vertex_count]; // Путь для DFS
            int top = -1;

            path[++top] = i;
            visited[i] = true;

            cout << "Connected components:" << endl;
            cout << "[";
            bool first = true; // Флаг для правильной расстановки запятых

            // Поиск по компоненте в глубину
            while (top >= 0) {
                int current = path[top--];

                // Выводим вершину
                if (!first) {
                    cout << ", ";
                }
                first = false;
                cout << vertices[current];

                // Добавляем всех непосещённых соседей
                for (int j = 0; j < vertex_count; j++) {
                    bool has_edge = false;
                    // Для неориентированного графа учитываем рёбра в обоих направлениях
                    if (!is_directed) {
                        has_edge = (matrix[current][j] != 0 || matrix[j][current] != 0);
                    }
                    else {
                        has_edge = (matrix[current][j] != 0);            
                    }

                    if (has_edge && !visited[j]) {
                        visited[j] = true;
                        path[++top] = j;
                    }
                }
            }

            cout << "]" << endl;
            delete[] path;
        }   
    }

    delete[] visited;
}

void Graph::shortest_path(int start, int end) {
    // Находим индексы в массивах для начала и конца
    int start_idx = find_vertex_index(start);
    int end_idx = find_vertex_index(end);

    // Если не нашли — прерываем выполнение.
    if (start_idx == -1 || end_idx == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    // Инициализация вспомогательных массивов
    int* dist = new int[vertex_count]; // Минимальные расстояния до каждой вершины (в начале все максимально неопределённые).
    int* prev = new int[vertex_count]; // Запоминает предыдущую вершину на кратчайшем пути для восстановления итогового маршрута.
    bool* visited = new bool[vertex_count](); // Флаги посещённых вершин (все false)

    // Все вершины ставим в [INF(INT_MAX) ~ inf, -1] а начало в [0, -1]
    for (int i = 0; i < vertex_count; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = false;
    }

    dist[start_idx] = 0; // Стартовая точка в 0

    while (true) {
        int current = -1; // Текущая рассмативаемая вершина
        int min_dist = INF; // До всех точек расстояние бесконечно

        // Найдем непосещённую вершину с минимальным расстоянием
        for (int i = 0; i < vertex_count; i++) {
            if ((dist[i] < min_dist) && !visited[i]) {
                min_dist = dist[i];
                current = i;
            }
        }

        // Все вершины обработаны или недостижимы
        if (current == -1) break;
        
        // Помечаем текущую вершину посещённой
        visited[current] = true;

        for (int i = 0; i < vertex_count; i++) {
            // Если есть ребро, вершина не посещена и текущее расстояние не бесконечно
            if (!visited[i] && matrix[current][i] != 0 
                && dist[current] != INF) {
                int current_dist = matrix[current][i] + dist[current];
                // Если найден более короткий путь, обновляем
                if (current_dist < dist[i]) {
                    dist[i] = current_dist;
                    prev[i] = current;
                }
            }
        }
    }
    
    // Восстановление пути
    if (dist[end_idx] == INF) {
        cout << "No path from " << start << "to " << end << endl;
    }
    else {
        int count = 0; // Кол-во точек на пути
        int* path = new int[vertex_count]; // Список точек пути

        // Просчитываем путь от конца до начала
        for (int i = end_idx; i != -1; i = prev[i]) {
            path[count++] = i;
        }

        // Переворачиваем массив пути
        for (int left = 0, right = count - 1; left < right; ++left, --right) {
            swap(path[left], path[right]);
        }

        // Выводим путь
        for (int i = 0; i < count; i++) {
            cout << vertices[path[i]];
            if (i != count - 1) cout << "-";
        }
        cout << endl;

        delete[] path;
    }

    delete[] dist;
    delete[] prev;
    delete[] visited;
}

void Graph::distances_from_vertex(int start) {
    int start_idx = find_vertex_index(start);

    if (start_idx == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    // Инициализация вспомогательных массивов
    int* dist = new int[vertex_count];// Минимальные расстояния до каждой вершины.
    int* prev = new int[vertex_count]; // Запоминает предыдущую вершину на кратчайшем пути для восстановления итогового маршрута.
    bool* visited = new bool[vertex_count](); // Флаги посещённых вершин.

    // Все вершины ставим в [INF(INT_MAX) ~ inf, -1] а начало в [0, -1]
    for (int i = 0; i < vertex_count; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = false;
    }

    dist[start_idx] = 0; // Стартовая верршина в 0

    // Цикл для всех вершин
    while (true) {
        int current = -1;
        int min_dist = INF;

        // Найдем непосещённую вершину с минимальным расстоянием
        for (int i = 0; i < vertex_count; i++) {
            if ((dist[i] < min_dist) && !visited[i]) {
                min_dist = dist[i];
                current = i;
            }
        }

        // Все вершины обработаны или недостижимы
        if (current == -1) break;
        
        // Помечаем текущую вершину посещённой
        visited[current] = true;

        // Находим куда можем пойти и если можем меняем данные о точке
        for (int i = 0; i < vertex_count; i++) {
            if (!visited[i] && matrix[current][i] != 0 
                && dist[current] != INF) {
                int current_dist = matrix[current][i] + dist[current];
                if (current_dist < dist[i]) {
                    dist[i] = current_dist;
                    prev[i] = current;
                }
            }
        }
    }
    
    cout << "Shortest paths lengths from " << start << ":" << endl;
    for (int i = 0; i < vertex_count; i++) {
        cout << "  " << start << " - " << vertices[i] << " : ";
        if (dist[i] == INT_MAX) {
            cout << "+Infinity";
        } else {
            cout << dist[i];
        }
        if (i < vertex_count - 1) cout << "     "; // Разделитель между расстояниями
    }
    cout << endl;
    
    delete[] dist;
    delete[] visited;
}

void Graph::all_shortest_paths() {
    for (int i = 0; i < vertex_count; i++) {
        distances_from_vertex(vertices[i]);
    }
}

void Graph::min_spanning_tree() {
    if (is_directed) {
        cout << "Minimum spanning tree exists for undirected graphs only" << endl;
        return;
    }

    int max_possible_edges = vertex_count * (vertex_count - 1) / 2; // Мах кол-во еджей в графе

    // Инициализация массивов
    int edge_count = 0; // Для счета еджей
    int count = 0; // Кол-во еджей в МСТ

    int** edges = new int*[max_possible_edges](); // Массив для хранения ребер: [Вес, Вершина1, Вершина2]
    for (int i = 0; i < max_possible_edges; i++) {
        edges[i] = new int[3];
    }
    
    int* components = new int[vertex_count];
    int* mst_edges = new int[max_possible_edges];

    // Шаг 1: Находим все ребра графа
    for (int i = 0; i < vertex_count; i++) {
        for (int j = (is_directed ? 0 : i); j < vertex_count; j++) {
            if (matrix[i][j] != 0) {
                edges[edge_count][0] = matrix[i][j]; // Вес
                edges[edge_count][1] = i;            // Индекс первой вершины
                edges[edge_count][2] = j;            // Индекс второй вершины
                edge_count++;
            }
        }
    }

    // Шаг 2: Сортировка ребер по весу (пузырьковая сортировка)
    for (int i = 0; i < edge_count - 1; i++) {
        for (int j = 0; j < edge_count - i - 1; j++) {
            if (edges[j][0] > edges[j + 1][0]) {
                // Меняем местами ребра
                int * temp = edges[j];
                edges[j] = edges[i];
                edges[j + 1] = temp;
            }
        }
    }

    // Шаг 3: Каждой вершине присваиваем номер ее компоненты
    for (int i = 0; i < vertex_count; i++) {
        components[i] = i; // Изначально каждая вершина в своей компоненте
    }

    // Шаг 4: Основной цикл построения MST
    for (int i = 0; i < edge_count; i++) {
        // Найдем точки начала и конца
        // Через доступ к 2,3 полям кортежей двумерного массива
        int weight = edges[i][0]; // Вес из кортежа
        int u = edges[i][1]; // Индекс точки начала
        int v = edges[i][2]; // Индекс точки конца

        // Начало == конец компонентам => выкинем
        if (components[u] != components[v]) {
            mst_edges[count] = i; // Сохраняем индекс ребра
            count++;

            // Объединяем компоненты: все вершины компоненты v переводим в компоненту u
            int old_component = components[v];
            int new_component = components[u];

            for (int k = 0; k < vertex_count; k++) {
                if (components[k] == old_component) {
                    components[k] = new_component;
                }
            }
        } 
        // Иначе - ребро создает цикл, пропускаем его
    }
    // Шаг 5: Вывод результата
    cout << "Minimum Spanning Tree (Kruskal):" << endl;
    int total_weight = 0;
    for (int i = 0; i < count; i++) {
        int edge_idx = mst_edges[i];
        int u_idx = edges[edge_idx][1];
        int v_idx = edges[edge_idx][2];
        int weight = edges[edge_idx][0];
        
        cout << vertices[u_idx] << " - " << vertices[v_idx] << " : " << weight << endl;
        total_weight += weight;
    }
    cout << "Total weight: " << total_weight << endl;

    // Освобождение памяти
    for (int i = 0; i < max_possible_edges; i++) {
        delete[] edges[i];
    }
    delete[] edges;
    delete[] components;
    delete[] mst_edges;
}