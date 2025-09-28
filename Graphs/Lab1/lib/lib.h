#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>

using namespace std;

class Graph {
private:
    bool is_directed; // Ориентированный граф
    int matrix_size;  // Размер матрицы
    int vertex_count; // Количество точек
    int *vertices;    // Массив индексов точек
    int **matrix;     // Матрица смежности

    int find_vertex_index(int vertex); // Найти индекс вершины по ее номеру

public:
    Graph(const char *filename, bool directed);
    ~Graph();

    // Основные методы по заданию
    int size(); // Количество вершин в графе
    int weight(int vertex1, int vertex2); // Вес ребра между двумя вершинами
    bool is_edge(int vertex1, int vertex2); // Проверка существования ребра
    void add_vertex(int vertex); // Добавление вершины
    void add_edge(int vertex1, int vertex2, int weight); // Добавление ребра с весом
    void remove_vertex(int vertex); // Удаление вершины
    void remove_edge(int vertex1, int vertex2); // Удаление ребра
    
    // Метод list_of_edges из задания: без аргументов - все ребра, с аргументом - ребра из вершины
    void list_of_edges();
    void list_of_edges(int vertex);

    // Дополнительные методы для командной строки
    bool is_connected(); // Проверка связности графа
    void list_of_vertices(); // Вывод всех вершин
    void connected_components(); // Вывод компонент связности
    void shortest_path(int start, int end); // Кратчайший путь между двумя вершинами
    void distances_from_vertex(int start); // Расстояния от вершины до всех остальных
    void all_shortest_paths();     // Все кратчайшие пути (для примера вывода)
    void min_spanning_tree(); // Минимальное остовное дерево
};

#endif