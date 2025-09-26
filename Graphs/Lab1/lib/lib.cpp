#include "lib.h"
#include <climits>
#include <fstream>
#include <iostream>

using namespace std;

const int INF = INT_MAX;

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
      
        for (int i = 0; i < n; i++) {
          add_vertex(i);
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int weight;
                file >> weight;
                if (weight != 0) {
                    add_edge(i, j, weight);
                }
            }
        }
    
        file.close();
    }
}

Graph::~Graph() {
    for (int i = 0; i < matrix_size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] vertices;
}

int Graph::find_vertex_index(int vertex) {
    for (int i = 0; i < vertex_count; i++) {
        if (vertices[i] == vertex) {
            return i;
        }
    }
    return -1;
}

int Graph::size() { return vertex_count; }

int Graph::weight(int vertex1, int vertex2) {
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
    if (find_vertex_index(vertex) != -1) {
        cout << "Error: Vertex already exists" << endl;
        return;
    }

    // if (vertex_count >= matrix_size) {
    //     expand_matrix();
    // }

    vertices[vertex_count] = vertex;
    vertex_count++;
}

void Graph::add_edge(int vertex1, int vertex2, int weight) {
    int i = find_vertex_index(vertex1);
    int j = find_vertex_index(vertex2);

    if (i == -1 || j == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    matrix[i][j] = weight;
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

    matrix[i][j] = 0;
    if (!is_directed) {
        matrix[j][i] = 0;
    }
}

// Все ребра между всеми точками
void Graph::list_of_edges() {
    for (int i = 0; i < vertex_count; i++) {
        for (int j = (is_directed ? 0 : i); j < vertex_count; j++) {
            if (matrix[i][j] != 0) {
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

    for (int j = 0; j < vertex_count; j++) {
        if (matrix[idx][j] != 0) {
            cout << vertex << " " << vertices[j] << " " 
            << matrix[idx][j] << endl;
        }
    }
}

void Graph::list_of_vertices() {
    for (int i = 0; i < vertex_count; i++) {
        cout << vertices[i] << endl;
    }
    cout << endl;
}

bool Graph::is_connected() {
    if (vertex_count == 0) return true; // Пустой граф связен по определению

    bool* visited = new bool[vertex_count]();
    int* path = new int[vertex_count];
    int top = -1;

    path[++top] = 0;
    visited[0] = true;
    int count = 1;

    while (top >= 0) {
        int current = path[top--];

        for (int j = 0; j < vertex_count; j++) {
            bool has_edge = false;

            if (!is_directed) {
                has_edge = (matrix[current][j] != 0 || matrix[j][current]);
            } else {
                has_edge = (matrix[current][j] != 0);
            }

            if (has_edge && !visited[j]) {
                visited[j] = true;
                path[++top] = j;
                count++;
            }
        }
    }
    delete[] visited;
    delete[] path;
    
    return count == vertex_count;
}

void Graph::connected_components() {
    bool* visited = new bool[vertex_count]();

    for (int i = 0; i < vertex_count; i++) {
        if (!visited[i]) {
            int* path = new int[vertex_count];
            int top = -1;

            path[++top] = i;
            visited[i] = true;

            cout << "[";
            bool first = true;

            while (top >= 0) {
                int current = path[top--];

                if (!first) {
                    cout << ", ";
                }
                first = false;
                cout << vertices[current];

                for (int j = 0; j < vertex_count; j++) {
                    bool has_edge = false;
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
    int* dist = new int[vertex_count];// Минимальные расстояния до каждой вершины (в начале все максимально неопределённые).
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

Graph::distances_from_vertex(int start) {
    int start_idx = find_vertex_index(start);

    if (start_idx == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    // Инициализация вспомогательных массивов
    int* dist = new int[vertex_count];// Минимальные расстояния до каждой вершины (в начале все максимально неопределённые).
    int* prev = new int[vertex_count]; // Запоминает предыдущую вершину на кратчайшем пути для восстановления итогового маршрута.
    bool* visited = new bool[vertex_count](); // Флаги посещённых вершин (все false)

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
        if (i < vertex_count - 1) cout << "     ";
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

// Дерево