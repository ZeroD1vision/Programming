#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <ifstream>

using namespace std;

class Graph
{
private:
    bool is_directed; // Ориентированный граф
    int matrix_size;  // Размер матрицы
    int vertex_count; // Количество точек
    int *vertices;    // Массив индексов точек
    int **matrix;     // Матрица смежности

    int find_vertex_index(int vertex);

public:
    Graph(const char *filename, bool directed);
    ~Graph();

    int size();
    int weight(int vertex1, int vertex2);
    bool is_edge(int vertex1, int vertex2);
    void add_vertex(int vertex);
    void add_edge(int vertex1, int vertex2, int weight);
    int find_vertex_index(int vertex);
    void list_of_edges();
    void list_of_edges(int vertex);
    void remove_vertex(int vertex);
    void remove_edge(int vertex1, int vertex2);
    void connected_components();
    void shortest_path(int start, int end);
    void distances_from_vertex(int start);
    void min_spanning_tree();
};

#endif