#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>

using namespace std;

class Graph {
private:
    bool is_directed; // Флаг: ориентированный граф (true) или неориентированный (false)
    int matrix_size;  // Текущий размер матрицы (выделенная память)
    int vertex_count; // Текущее количество вершин в графе
    int *vertices;    // Массив номеров вершин
    int **matrix;     // Динамическая матрица смежности для хранения весов рёбер

    int find_vertex_index(int vertex); // Найти индекс вершины по ее номеру
    void expand_matrix();              // Увеличить размер матрицы при необходимости

public:
    /**
     * @brief Конструктор графа
     * @param filename Путь к файлу с матрицей смежности (nullptr для пустого графа)
     * @param directed Тип графа: true - ориентированный, false - неориентированный
     */
    Graph(const char *filename, bool directed);
    // Деструктор - освобождает всю выделенную память
    ~Graph();

    // Основные методы по заданию
    int size();                                          // Количество вершин в графе
    int weight(int vertex1, int vertex2);                // Вес ребра между двумя вершинами
    bool is_edge(int vertex1, int vertex2);              // Проверка существования ребра
    void add_vertex(int vertex);                         // Добавление вершины
    void add_edge(int vertex1, int vertex2, int weight); // Добавление ребра с весом
    void remove_vertex(int vertex);                      // Удаление вершины
    void remove_edge(int vertex1, int vertex2);          // Удаление ребра
    
    // Метод list_of_edges из задания: без аргументов - все ребра, с аргументом - ребра из вершины
    void list_of_edges();
    void list_of_edges(int vertex);

    // Дополнительные методы для командной строки
    bool is_connected();                    // Проверка связности графа
    void list_of_vertices();                // Вывод всех вершин
    void connected_components();            // Вывод компонент связности
    void shortest_path(int start, int end); // Кратчайший путь между двумя вершинами
    void distances_from_vertex(int start);  // Расстояния от вершины до всех остальных
    void all_shortest_paths();              // Все кратчайшие пути (для примера вывода)
    void min_spanning_tree();               // Минимальное остовное дерево
};

#endif