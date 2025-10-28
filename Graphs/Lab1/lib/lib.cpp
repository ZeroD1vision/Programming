#include "lib.h"
#include <climits>
#include <fstream>
#include <iostream>

using namespace std;

// Константа INF представляет "бесконечность" для алгоритмов на графах
// Используется для обозначения недостижимых вершин или очень больших расстояний
const int INF = INT_MAX;

/**
* @brief Конструктор графа - создает граф из файла или пустой граф
* @param filename Путь к файлу с матрицей смежности (nullptr для пустого графа)
* @param directed Тип графа: true - ориентированный, false - неориентированный
*/
Graph::Graph(const char *filename, bool directed)
    : is_directed(directed),  // Флаг ориентации графа (true - стрелки, false - линии)
      vertex_count(0),        // Текущее количество вершин в графе
      matrix_size(0),         // Текущий физический размер матрицы смежности
      matrix(nullptr),        // Указатель на двумерный массив матрицы смежности
      vertices(nullptr) {     // Указатель на массив идентификаторов вершин
    // Работа с файлом - если передан путь к файлу
    if (filename && filename[0] != '\0') {
        ifstream file(filename);  // Поток для чтения из файла
        if (!file.is_open()) {
            cout << "Error: Cannot open file" << filename << endl;
            return;
        }
      
        // Читаем число точек из файла
        int n; // Число вершин в графе, прочитанное из первой строки файла
        file >> n;
      
        // Выделение места и инициализация матриц
        matrix_size = n + 10; // Размер матрицы с запасом +10 для будущего добавления вершин
        vertices = new int[matrix_size];  // Динамический массив для хранения идентификаторов вершин
        matrix = new int *[matrix_size];  // Двумерный динамический массив для матрицы смежности
        for (int i = 0; i < matrix_size; i++) {
            matrix[i] = new int[matrix_size](); // Выделяем и инициализируем нулями каждую строку матрицы
            vertices[i] = -1;                   // -1 означает что ячейка массива вершин пуста
        }
      
        // Добавляем вершины в граф с идентификаторами от 1 до n
        for (int i = 0; i < n; i++) {
          add_vertex(i + 1);  // Добавляем вершину с идентификатором (i+1)
        }

        // Читаем матрицу смежности из файла и добавляем рёбра
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int weight;  // Вес ребра между вершинами i и j из файла
                file >> weight;
                // Если вес не нулевой, добавляем ребро между вершинами i и j
                if (weight != 0) {
                    add_edge(i + 1, j + 1, weight);  // +1 потому что вершины нумеруются с 1
                }
            }
        }
    
        file.close();  // Закрываем файл после чтения

        // Вывод результата загрузки графа
        cout << "Граф " << (is_directed ? "ориентированный" : "неориентированный") << endl;
    }
}

// Деструктор графа - освобождает всю выделенную память
Graph::~Graph() {
    // Освобождаем память: сначала каждую строку матрицы, затем массив указателей
    for (int i = 0; i < matrix_size; i++) {
        delete[] matrix[i];  // Удаляем каждую строку матрицы смежности
    }
    delete[] matrix;     // Удаляем массив указателей на строки матрицы
    delete[] vertices;   // Удаляем массив идентификаторов вершин
}

// Поиск индекса вершины в массиве vertices по её идентификатору
int Graph::find_vertex_index(int vertex) {
    // Линейный поиск вершины в массиве vertices
    for (int i = 0; i < vertex_count; i++) {
        if (vertices[i] == vertex) {
            return i;  // Возвращаем индекс найденной вершины
        }
    }
    return -1;  // Вершина не найдена
}

// Увеличение размера матрицы при нехватке места для новых вершин
void Graph::expand_matrix() {
    // Новый размер = текущий + 10 (увеличиваем с запасом)
    int new_size = matrix_size + 10;
    int** new_matrix = new int*[new_size];  // Новая матрица большего размера
    int* new_vertices = new int[new_size];  // Новый массив вершин большего размера
    
    // Инициализируем новую матрицу и массив вершин
    for (int i = 0; i < new_size; i++) {
        new_matrix[i] = new int[new_size]();  // Выделяем и инициализируем нулями новую строку матрицы
        new_vertices[i] = -1;                 // Помечаем ячейку как пустую
        
        // Копируем данные из старой матрицы (только для существующих ячеек)
        if (i < matrix_size) {
            new_vertices[i] = vertices[i];  // Копируем идентификатор вершины
            for (int j = 0; j < matrix_size; j++) {
                new_matrix[i][j] = matrix[i][j];  // Копируем веса рёбер
            }
        }
    }
    
    // Освобождаем память старой матрицы
    for (int i = 0; i < matrix_size; i++) {
        delete[] matrix[i];  // Удаляем каждую строку старой матрицы
    }
    delete[] matrix;     // Удаляем массив указателей старой матрицы
    delete[] vertices;   // Удаляем старый массив вершин
    
    // Обновляем указатели и размер
    matrix = new_matrix;         // Перенаправляем указатель на новую матрицу
    vertices = new_vertices;     // Перенаправляем указатель на новый массив вершин
    matrix_size = new_size;      // Обновляем размер матрицы
}

// Возвращает текущее количество вершин в графе
int Graph::size() { 
    return vertex_count;  // Количество добавленных вершин
}

// Возвращает вес ребра между двумя вершинами
int Graph::weight(int vertex1, int vertex2) {
    // Находим индексы вершин в массиве vertices
    int i = find_vertex_index(vertex1);  // Индекс первой вершины
    int j = find_vertex_index(vertex2);  // Индекс второй вершины

    if (i == -1 || j == -1) {
        cout << "Error: Vertex not found" << endl;
        return 0;  // Возвращаем 0 если вершины не найдены
    }

    return matrix[i][j];  // Возвращаем вес из матрицы смежности
}

// Проверяет существует ли ребро между двумя вершинами
bool Graph::is_edge(int vertex1, int vertex2) {
    int i = find_vertex_index(vertex1);  // Индекс первой вершины
    int j = find_vertex_index(vertex2);  // Индекс второй вершины

    if (i == -1 || j == -1) {
        return false;  // Если вершины не существуют, ребра нет
    }

    return matrix[i][j] != 0;  // Ребро существует если вес не равен 0
}

// Добавляет новую вершину в граф
void Graph::add_vertex(int vertex) {
    // Проверяем, не существует ли уже такая вершина
    if (find_vertex_index(vertex) != -1) {
        cout << "Error: Vertex already exists" << endl;
        return;
    }

    // Если массив полон, увеличиваем его размер
    if (vertex_count >= matrix_size) {
        expand_matrix();  // Увеличиваем размер матрицы и массива вершин
    }

    // Добавляем вершину в конец массива
    vertices[vertex_count] = vertex;  // Сохраняем идентификатор вершины
    vertex_count++;  // Увеличиваем счётчик вершин
}

// Добавляет ребро между двумя вершинами с заданным весом
void Graph::add_edge(int vertex1, int vertex2, int weight) {
    // Находим индексы вершин в массиве vertices
    int i = find_vertex_index(vertex1);  // Индекс начальной вершины
    int j = find_vertex_index(vertex2);  // Индекс конечной вершины

    if (i == -1 || j == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    // Устанавливаем вес в матрице смежности
    matrix[i][j] = weight;  // Записываем вес ребра в матрицу
    // Если граф неориентированный, добавляем обратное ребро
    if (!is_directed) {
        matrix[j][i] = weight;  // Для неориентированного графа ребро двустороннее
    }
}

// Удаляет вершину из графа
void Graph::remove_vertex(int vertex) {
    int idx = find_vertex_index(vertex);  // Индекс удаляемой вершины
    if (idx == -1) {
        cout << "Error: vertex not found" << endl;
        return;
    }

    // Удаление из массива вершин - сдвигаем элементы влево
    for (int i = idx; i < vertex_count - 1; i++) {
        vertices[i] = vertices[i + 1];  // Сдвигаем идентификаторы вершин
    }
    vertices[vertex_count - 1] = -1;  // Последнюю ячейку помечаем как пустую

    // Удаление из матрицы (удаляем столбец и строку)
    // Удаление столбца - сдвигаем столбцы влево
    for (int i = 0; i < vertex_count; i++) {
        for (int j = idx; j < vertex_count - 1; j++) {
            matrix[i][j] = matrix[i][j + 1];  // Сдвигаем элементы столбца
        }
        matrix[i][vertex_count - 1] = 0;  // Последний столбец обнуляем
    }

    // Удаление строки - сдвигаем строки вверх
    for (int i = idx; i < vertex_count - 1; i++) {
        for (int j = 0; j < vertex_count; j++) {
            matrix[i][j] = matrix[i + 1][j];  // Сдвигаем элементы строки
        }
    }

    // Обнуляем последнюю строку матрицы
    for (int j = 0; j < vertex_count; j++) {
        matrix[vertex_count - 1][j] = 0;
    }

    vertex_count--;  // Уменьшаем количество вершин
}

// Удаляет ребро между двумя вершинами
void Graph::remove_edge(int vertex1, int vertex2) {
    int i = find_vertex_index(vertex1);  // Индекс первой вершины
    int j = find_vertex_index(vertex2);  // Индекс второй вершины

    if (i == -1 || j == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    matrix[i][j] = 0;  // Обнуляем вес = удаляем ребро
    if (!is_directed) {
        matrix[j][i] = 0;  // Для неориентированного графа удаляем и обратное ребро
    }
}

// Выводит список всех рёбер графа
void Graph::list_of_edges() {
    // Проходим по всем возможным парам вершин
    for (int i = 0; i < vertex_count; i++) {
        // Для неориентированного графа начинаем с i, чтобы избежать дубликатов
        // Для ориентированного просматриваем все пары
        for (int j = (is_directed ? 0 : i); j < vertex_count; j++) {
            if (matrix[i][j] != 0) {
                // Выводим: вершина1 вершина2 вес
                cout << vertices[i] << " " << vertices[j] << " " 
                << matrix[i][j] << endl;
            }
        }
    }
}

// Выводит все рёбра исходящие из конкретной вершины
void Graph::list_of_edges(int vertex) {
    int idx = find_vertex_index(vertex);  // Индекс заданной вершины
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

// Выводит список всех вершин графа
void Graph::list_of_vertices() {
    // Вывод всех вершин в порядке их хранения в массиве
    for (int i = 0; i < vertex_count; i++) {
        cout << vertices[i] << endl;  // Выводим идентификатор вершины
    }
    cout << endl;  // Пустая строка в конце
}

// Проверяет связность графа (для неориентированного) или слабую связность (для ориентированного)
bool Graph::is_connected() {
    if (vertex_count == 0) return true;  // Пустой граф считается связным
    
    // Для ориентированного графа используем алгоритм для слабой связности
    // Создаем неориентированную версию матрицы для обхода
    int** undir_matrix = new int*[vertex_count];  // Временная неориентированная матрица
    for (int i = 0; i < vertex_count; i++) {
        undir_matrix[i] = new int[vertex_count];  // Выделяем строку временной матрицы
        for (int j = 0; j < vertex_count; j++) {
            // Для слабой связности считаем граф неориентированным
            // Ребро есть если есть прямое или обратное ребро
            undir_matrix[i][j] = (matrix[i][j] != 0 || matrix[j][i] != 0) ? 1 : 0;
        }
    }
    
    // Массивы для обхода графа в глубину
    bool* visited = new bool[vertex_count]();  // Массив посещенных вершин (инициализирован false)
    int* path = new int[vertex_count];         // Стек для вершин (реализация DFS через стек)
    int top = -1;                              // Вершина стека (индекс верхнего элемента)

    // Начинаем обход с вершины 0 (первой в массиве)
    path[++top] = 0;      // Помещаем первую вершину в стек
    visited[0] = true;    // Отмечаем первую вершину как посещенную
    int count = 1;        // Счетчик посещенных вершин

    // Обход в глубину (DFS) с использованием стека
    while (top >= 0) {
        int current = path[top--];  // Извлекаем вершину из стека

        // Проверяем всех соседей текущей вершины
        for (int j = 0; j < vertex_count; j++) {
            if (undir_matrix[current][j] != 0 && !visited[j]) {
                visited[j] = true;      // Отмечаем соседа как посещенного
                path[++top] = j;        // Добавляем соседа в стек
                count++;                // Увеличиваем счетчик посещенных
            }
        }
    }
    
    // Освобождаем память временных массивов
    for (int i = 0; i < vertex_count; i++) {
        delete[] undir_matrix[i];  // Удаляем каждую строку временной матрицы
    }
    delete[] undir_matrix;  // Удаляем массив указателей временной матрицы
    delete[] visited;       // Удаляем массив посещенных вершин
    delete[] path;          // Удаляем стек
    
    // Граф связен если мы посетили все вершины
    return count == vertex_count;
}

// Находит и выводит все компоненты связности графа
void Graph::connected_components() {
    // Для ориентированного графа выводим слабо связные компоненты
    if (is_directed) {
        cout << "Weakly connected components:" << endl;
    } else {
        cout << "Connected components:" << endl;
    }
    
    // Создаем неориентированную версию матрицы для обхода
    int** undir_matrix = new int*[vertex_count];  // Временная неориентированная матрица
    for (int i = 0; i < vertex_count; i++) {
        undir_matrix[i] = new int[vertex_count];  // Выделяем строку временной матрицы
        for (int j = 0; j < vertex_count; j++) {
            // Ребро есть если есть прямое или обратное ребро в исходном графе
            undir_matrix[i][j] = (matrix[i][j] != 0 || matrix[j][i] != 0) ? 1 : 0;
        }
    }
    
    bool* visited = new bool[vertex_count]();  // Массив посещенных вершин

    // Обходим все вершины, находя новые компоненты связности
    for (int i = 0; i < vertex_count; i++) {
        if (!visited[i]) {  // Если вершина еще не посещена, начинаем новую компоненту
            int* path = new int[vertex_count];  // Стек для DFS
            int top = -1;                       // Вершина стека

            path[++top] = i;     // Добавляем текущую вершину в стек
            visited[i] = true;   // Отмечаем как посещенную

            cout << "[";         // Начинаем вывод компоненты
            bool first = true;   // Флаг для определения первого элемента в выводе

            // Обход компоненты связности
            while (top >= 0) {
                int current = path[top--];  // Извлекаем вершину из стека

                if (!first) {
                    cout << ", ";  // Разделитель между вершинами
                }
                first = false;
                cout << vertices[current];  // Выводим идентификатор вершины

                // Добавляем всех непосещенных соседей в стек
                for (int j = 0; j < vertex_count; j++) {
                    if (undir_matrix[current][j] != 0 && !visited[j]) {
                        visited[j] = true;   // Отмечаем соседа как посещенного
                        path[++top] = j;     // Добавляем соседа в стек
                    }
                }
            }

            cout << "]" << endl;  // Заканчиваем вывод компоненты
            delete[] path;        // Освобождаем память стека
        }   
    }

    // Освобождаем память временных массивов
    for (int i = 0; i < vertex_count; i++) {
        delete[] undir_matrix[i];  // Удаляем каждую строку временной матрицы
    }
    delete[] undir_matrix;  // Удаляем массив указателей временной матрицы
    delete[] visited;       // Удаляем массив посещенных вершин
}

// Находит кратчайший путь между двумя вершинами с помощью алгоритма Дейкстры
void Graph::shortest_path(int start, int end) {
    // Находим индексы в массивах для начала и конца
    int start_idx = find_vertex_index(start);  // Индекс начальной вершины
    int end_idx = find_vertex_index(end);      // Индекс конечной вершины

    // Если не нашли — прерываем выполнение.
    if (start_idx == -1 || end_idx == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    // Инициализация вспомогательных массивов для алгоритма Дейкстры
    int* dist = new int[vertex_count];  // Минимальные расстояния от start до каждой вершины
    int* prev = new int[vertex_count];  // Массив предыдущих вершин на кратчайшем пути
    bool* visited = new bool[vertex_count]();  // Флаги посещённых вершин (инициализированы false)

    // Инициализация массивов: все расстояния бесконечны, предыдущих вершин нет
    for (int i = 0; i < vertex_count; i++) {
        dist[i] = INF;    // Изначально расстояние до всех вершин неизвестно (бесконечность)
        prev[i] = -1;     // Предыдущая вершина неизвестна
        visited[i] = false;  // Все вершины не посещены
    }

    dist[start_idx] = 0;  // Расстояние от стартовой вершины до самой себя равно 0

    // Основной цикл алгоритма Дейкстры
    while (true) {
        int current = -1;     // Текущая обрабатываемая вершина
        int min_dist = INF;   // Минимальное расстояние среди непосещенных вершин

        // Найдем непосещённую вершину с минимальным расстоянием
        for (int i = 0; i < vertex_count; i++) {
            if ((dist[i] < min_dist) && !visited[i]) {
                min_dist = dist[i];  // Обновляем минимальное расстояние
                current = i;         // Запоминаем вершину с минимальным расстоянием
            }
        }

        // Все вершины обработаны или недостижимы
        if (current == -1) break;
        
        // Помечаем текущую вершину посещённой
        visited[current] = true;

        // Обновляем расстояния до всех соседей текущей вершины
        for (int i = 0; i < vertex_count; i++) {
            // Если есть ребро, вершина не посещена и текущее расстояние не бесконечно
            if (!visited[i] && matrix[current][i] != 0 
                && dist[current] != INF) {
                int current_dist = matrix[current][i] + dist[current];  // Новое расстояние
                // Если найден более короткий путь, обновляем
                if (current_dist < dist[i]) {
                    dist[i] = current_dist;  // Обновляем расстояние
                    prev[i] = current;       // Запоминаем предыдущую вершину
                }
            }
        }
    }
    
    // Восстановление пути от end к start
    if (dist[end_idx] == INF) {
        cout << "No path from " << start << "to " << end << endl;
    }
    else {
        int count = 0;  // Количество вершин на пути
        int* path = new int[vertex_count];  // Массив для хранения вершин пути

        // Просчитываем путь от конца до начала по ссылкам prev
        for (int i = end_idx; i != -1; i = prev[i]) {
            path[count++] = i;  // Добавляем вершины в обратном порядке
        }

        // Переворачиваем массив пути чтобы получить путь от start к end
        for (int left = 0, right = count - 1; left < right; ++left, --right) {
            swap(path[left], path[right]);  // Меняем местами элементы
        }

        // Выводим путь в формате v1-v2-v3-...-vn
        for (int i = 0; i < count; i++) {
            cout << vertices[path[i]];  // Выводим идентификатор вершины
            if (i != count - 1) cout << "-";  // Разделитель между вершинами
        }
        cout << endl;

        delete[] path;  // Освобождаем память массива пути
    }

    // Освобождаем память вспомогательных массивов
    delete[] dist;
    delete[] prev;
    delete[] visited;
}

// Выводит кратчайшие расстояния от заданной вершины до всех остальных
void Graph::distances_from_vertex(int start) {
    int start_idx = find_vertex_index(start);  // Индекс начальной вершины
    if (start_idx == -1) {
        cout << "Error: Vertex not found" << endl;
        return;
    }

    // Инициализация вспомогательных массивов для алгоритма Дейкстры
    int* dist = new int[vertex_count];  // Минимальные расстояния до каждой вершины
    int* prev = new int[vertex_count];  // Массив предыдущих вершин (не используется в выводе)
    bool* visited = new bool[vertex_count]();  // Флаги посещённых вершин

    // Инициализация массивов
    for (int i = 0; i < vertex_count; i++) {
        dist[i] = INF;    // Изначально все расстояния бесконечны
        prev[i] = -1;     // Предыдущие вершины неизвестны
        visited[i] = false;  // Все вершины не посещены
    }

    dist[start_idx] = 0;  // Расстояние до самой себя равно 0

    // Основной цикл алгоритма Дейкстры
    while (true) {
        int current = -1;     // Текущая обрабатываемая вершина
        int min_dist = INF;   // Минимальное расстояние среди непосещенных

        // Поиск непосещенной вершины с минимальным расстоянием
        for (int i = 0; i < vertex_count; i++) {
            if ((dist[i] < min_dist) && !visited[i]) {
                min_dist = dist[i];  // Обновляем минимальное расстояние
                current = i;         // Запоминаем вершину
            }
        }

        // Все вершины обработаны или недостижимы
        if (current == -1) break;
        
        // Помечаем текущую вершину посещённой
        visited[current] = true;

        // Обновляем расстояния до соседей
        for (int i = 0; i < vertex_count; i++) {
            if (!visited[i] && matrix[current][i] != 0 
                && dist[current] != INF) {
                int current_dist = matrix[current][i] + dist[current];  // Новое расстояние
                if (current_dist < dist[i]) {
                    dist[i] = current_dist;  // Обновляем расстояние
                    prev[i] = current;       // Запоминаем предыдущую вершину
                }
            }
        }
    }
    
    // Вывод кратчайших расстояний от start до всех вершин
    cout << "Shortest paths lengths from " << start << ":" << endl;
    for (int i = 0; i < vertex_count; i++) {
        cout << "  " << start << " - " << vertices[i] << " : ";
        if (dist[i] == INT_MAX) {
            cout << "+Infinity";  // Вершина недостижима
        } else {
            cout << dist[i];      // Выводим расстояние
        }
        if (i < vertex_count - 1) cout << "     "; // Разделитель между расстояниями
    }
    cout << endl;
    
    // Освобождаем память (prev не используется в выводе, но удаляем для чистоты)
    delete[] dist;
    delete[] visited;
}

// Выводит кратчайшие пути между всеми парами вершин
void Graph::all_shortest_paths() {
    // Для каждой вершины вычисляем и выводим расстояния до всех остальных
    for (int i = 0; i < vertex_count; i++) {
        distances_from_vertex(vertices[i]);  // Вызываем для каждой вершины
    }
}

// Строит минимальное остовное дерево с помощью алгоритма Краскала
void Graph::min_spanning_tree() {
    // Шаг 0: Определение направленности графа по матрице
    bool directed = false;  // Флаг ориентированности (определяется автоматически)
    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < vertex_count; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                directed = true;  // Найдено асимметричное ребро - граф ориентированный
                break;
            }
        }
        if (directed) break;
    }
    if (directed) {
        cout << "Graph is directed, but MST will be computed treating it as undirected" << endl;
    }

    // Максимальное возможное количество ребер в полном графе
    int max_possible_edges = vertex_count * (vertex_count - 1) / 2;

    // Инициализация массивов для алгоритма Краскала
    int edge_count = 0;  // Счетчик фактического количества ребер в графе
    int count = 0;       // Счетчик ребер вошедших в минимальное остовное дерево

    // Массив для хранения всех ребер графа: каждый элемент [вес, вершина1, вершина2]
    int** edges = new int*[max_possible_edges]();
    for (int i = 0; i < max_possible_edges; i++) {
        edges[i] = new int[3];  // Каждое ребро хранится как массив из 3 элементов
    }
    
    int* components = new int[vertex_count];        // Массив компонент связности для вершин
    int* mst_edges = new int[max_possible_edges];  // Индексы ребер вошедших в MST

    // Шаг 1: Находим все ребра графа (только уникальные для неориентированного)
    for (int i = 0; i < vertex_count; i++) {
        for (int j = i + 1; j < vertex_count; j++) { // j > i — чтобы не брать дважды одно ребро
            if (matrix[i][j] != 0 || matrix[j][i] != 0) {
                // Определяем вес ребра (берем минимальный если есть оба направления)
                int w = 0;
                if (matrix[i][j] != 0 && matrix[j][i] != 0) {
                    w = std::min(matrix[i][j], matrix[j][i]);  // Берем минимальный вес
                }
                else if (matrix[i][j] != 0) {
                    w = matrix[i][j];  // Используем прямое ребро
                }
                else {
                    w = matrix[j][i];  // Используем обратное ребро
                }

                edges[edge_count][0] = w;  // Сохраняем вес ребра
                edges[edge_count][1] = i;  // Сохраняем индекс первой вершины
                edges[edge_count][2] = j;  // Сохраняем индекс второй вершины
                edge_count++;               // Увеличиваем счетчик ребер
            }
        }
    }

    // Проверка: если в графе нет ребер, MST построить невозможно
    if (edge_count == 0) {
        cout << "No edges found. Impossible to build MST" << endl;
        for (int i = 0; i < edge_count; i++) delete[] edges[i];
        delete[] edges;
        return;
    }

    // Шаг 2: Сортировка ребер по весу (пузырьковая сортировка)
    for (int i = 0; i < edge_count - 1; i++) {
        for (int j = 0; j < edge_count - i - 1; j++) {
            if (edges[j][0] > edges[j + 1][0]) {
                // Меняем местами ребра (все три компоненты)
                swap(edges[j][0], edges[j + 1][0]);  // Вес
                swap(edges[j][1], edges[j + 1][1]);  // Первая вершина
                swap(edges[j][2], edges[j + 1][2]);  // Вторая вершина
            }
        }
    }

    // Шаг 3: Инициализация системы непересекающихся множеств
    // Каждой вершине присваиваем номер ее компоненты
    for (int i = 0; i < vertex_count; i++) {
        components[i] = i;  // Изначально каждая вершина в своей компоненте
    }

    // Шаг 4: Основной цикл построения MST (алгоритм Краскала)
    for (int i = 0; i < edge_count; i++) {
        // Извлекаем информацию о текущем ребре
        int weight = edges[i][0];  // Вес ребра
        int u = edges[i][1];       // Индекс первой вершины ребра
        int v = edges[i][2];       // Индекс второй вершины ребра

        // Если вершины принадлежат разным компонентам, ребро не создает цикл
        if (components[u] != components[v]) {
            mst_edges[count] = i;  // Сохраняем индекс ребра в MST
            count++;                // Увеличиваем счетчик ребер в MST

            // Объединяем компоненты: все вершины компоненты v переводим в компоненту u
            int old_component = components[v];   // Компонента которая будет поглощена
            int new_component = components[u];   // Компонента которая будет расширена

            // Перебираем все вершины и переводим вершины из старой компоненты в новую
            for (int k = 0; k < vertex_count; k++) {
                if (components[k] == old_component) {
                    components[k] = new_component;  // Меняем компоненту вершины
                }
            }
        } 
        // Иначе - ребро создает цикл, пропускаем его
    }

    // Проверка на связность графа
    // Если не все вершины в одной компоненте - MST не существует
    int first_component = components[0];  // Компонента первой вершины
    for (int i = 1; i < vertex_count; i++) {
        if (components[i] != first_component) {
            cout << "Graph is not connected. MST does not exist." << endl;
            // Освобождаем память перед выходом
            for (int j = 0; j < max_possible_edges; j++) delete[] edges[j];
            delete[] edges;
            delete[] components;
            delete[] mst_edges;
            
            return;
        }
    }

    // Шаг 5: Вывод результата - веса и ребер MST
    int total_weight = 0;  // Суммарный вес минимального остовного дерева
    cout << "Weight of minimum spanning tree: ";
    // Вычисляем общий вес MST
    for (int i = 0; i < count; i++) {
        total_weight += edges[mst_edges[i]][0];  // Суммируем веса всех ребер MST
    }
    cout << total_weight << endl;

    // Выводим все ребра минимального остовного дерева
    cout << "Minimum Spanning Tree edges:" << endl;
    for (int i = 0; i < count; i++) {
        int idx = mst_edges[i];           // Индекс ребра в массиве edges
        int u = edges[idx][1];            // Индекс первой вершины
        int v = edges[idx][2];            // Индекс второй вершины
        int w = edges[idx][0];            // Вес ребра
        cout << vertices[u] << " - " << vertices[v] << " : " << w << endl;  // Форматированный вывод
    }

    // Очистка памяти
    for (int i = 0; i < max_possible_edges; i++) delete[] edges[i];  // Удаляем каждое ребро
    delete[] edges;        // Удаляем массив указателей на ребра
    delete[] components;   // Удаляем массив компонент связности
    delete[] mst_edges;    // Удаляем массив индексов ребер MST
}