#include "lib.h"
#include <climits>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ==================== КОНСТРУКТОР И ИНИЦИАЛИЗАЦИЯ ====================

Map::Map(vector<vector<int>>& heights) : heights_(heights) {
    rows = heights.size();
    cols = heights[0].size();

    parents_.resize(rows, vector<pair<int, int>>(cols, {-1, -1}));
};

// ==================== ПОЛУЧЕНИЕ СОСЕДНИХ КЛЕТОК ====================

vector<pair<int, int>> Map::getNeighbors(int i, int j) {
    vector<pair<int, int>> neighbors;
    vector<pair<int, int>> directions = {{0,1}, {1,0}, {-1,0}, {0,-1}};

    for (auto [di, dj] : directions) {
        int ni = i + di;
        int nj = j + dj;
        if (ni >= 0 && ni < rows && nj >= 0 
                    && nj < cols && heights_[ni][nj] != 0) {
            neighbors.push_back({ni, nj});
        }
    }
    return neighbors;
}

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

/**
 * Вычисляет общую стоимость пути для проверки корректности
 * @param path - вектор точек пути
 * @return общая стоимость пути
 */
int Map::calculatePathCost(const vector<pair<int,int>>& path) {
    int cost = 0;
    for (int i = 0; i < path.size()-1; i++) {
        int i1 = path[i].first, j1 = path[i].second;
        int i2 = path[i+1].first, j2 = path[i+1].second;
        cost += getCost(i1, j1, i2, j2);
    }
    return cost;
}

/**
 * Вычисляет стоимость перехода между двумя клетками
 * @return 1 + абсолютная разница высот
 */
int Map::getCost(int i1, int j1, int i2, int j2) {
    return (1 + abs(heights_[i1][j1] - heights_[i2][j2]));
}

/**
 * Эвристическая функция Чебышева для A*
 * @return максимальное расстояние по осям X и Y
 */
int Map::euristic(int i1, int j1, int i2, int j2) {
    return max(abs(i1 - i2), abs(j1 - j2));
}

// ==================== ВОССТАНОВЛЕНИЕ ПУТИ ====================

/**
 * Восстанавливает путь от конечной точки до начальной
 * используя массив родителей
 * @return вектор точек пути от начала до конца
 */
vector<pair<int, int>> Map::recreatePath(pair<int, int> start, pair<int, int> end) {
    vector<pair<int, int>> path;
    auto [i, j] = end;

    if (parents_[i][j] == make_pair(-1, -1)) {
        return path; // Не нашли
    }

    while (start.first != i || start.second != j) {
        path.push_back({i, j});
        auto [pi, pj] = parents_[i][j];
        i = pi;
        j = pj;
    }

    path.push_back(start);
    
    reverse(path.begin(), path.end());
    return path;
}

// ==================== АЛГОРИТМ ДЕЙКСТРЫ ====================

/**
 * Реализация алгоритма Дейкстры для поиска кратчайшего пути
 * @param start - начальная точка
 * @param end - конечная точка  
 * @param visited_count - счетчик посещенных узлов (выходной параметр)
 * @return длина кратчайшего пути или -1 если путь не найден
 */
int Map::calcLengthDijkstra(pair<int, int> start, pair<int, int> end, int& visited_count) {
    visited_count = 0;
    int r = rows;
    int c = cols;
    int start_i = start.first;
    int start_j = start.second;
    int end_i = end.first;
    int end_j = end.second;

    // Инициализация массивов
    vector<vector<bool>> visited(r, vector<bool>(c, false));
    vector<vector<pair<int, int>>> prev(r, vector<pair<int, int>>(c, {-1, -1}));
    vector<vector<int>> dist(r, vector<int>(c, INT_MAX));
    
    // Вектор для хранения узлов в виде (i, j)
    vector<pair<int, int>> heap;

    dist[start_i][start_j] = 0;
    prev[start_i][start_j] = {start_i, start_j};
    heap.push_back({start_i, start_j});

    while (!heap.empty()) {
        int min_idx = 0;
        int min_dist = INT_MAX;

        // Поиск узла с минимальным расстоянием
        for (int idx = 0; idx < heap.size(); idx++) {
            auto [i, j] = heap[idx];
            if (dist[i][j] < min_dist) {
                min_dist = dist[i][j];
                min_idx = idx;
            }
        }

        auto [i, j] = heap[min_idx];
        heap.erase(heap.begin() + min_idx);

        if (visited[i][j]) continue;
        visited[i][j] = true;
        visited_count++;

        // Если достигли конечной точки
        if (i == end_i && j == end_j) {
            parents_ = prev;
            return dist[i][j];
        }

        // Обработка соседей
        vector<pair<int, int>> neighbors = getNeighbors(i, j);
        for (auto [ni, nj] : neighbors) {
            if (visited[ni][nj]) continue;

            int cost = getCost(i, j, ni, nj);
            int new_dist = dist[i][j] + cost;

            if (new_dist < dist[ni][nj]) {
                dist[ni][nj] = new_dist;
                prev[ni][nj] = {i, j};

                if(!visited[ni][nj]) {
                    heap.push_back({ni, nj});
                }
            }
        }
    }

    parents_ = prev;
    return -1; // Не найден
};

// ==================== АЛГОРИТМ A* ====================

/**
 * Реализация алгоритма A* с эвристикой Чебышева
 * @param start - начальная точка
 * @param end - конечная точка
 * @param visited_count - счетчик посещенных узлов (выходной параметр)  
 * @return длина кратчайшего пути или -1 если путь не найден
 */
int Map::calcLengthAStar(pair<int, int> start, pair<int, int> end, int& visited_count) {
    visited_count = 0;
    int r = rows;
    int c = cols;
    int start_i = start.first;
    int start_j = start.second;
    int end_i = end.first;
    int end_j = end.second;

    vector<vector<pair<int, int>>> prev(r, vector<pair<int, int>>(c, {-1, -1}));
    vector<vector<bool>> visited(r, vector<bool>(c, false));
    vector<vector<int>> dist(r, vector<int>(c, INT_MAX));
    
    vector<pair<int, int>> heap;

    dist[start_i][start_j] = 0;
    prev[start_i][start_j] = {start_i, start_j};
    heap.push_back({start_i, start_j});

    while (!heap.empty()) {
        int min_idx = 0;
        int min_dist = INT_MAX;

        // Выбор узла с минимальной оценкой f(x) = g(x) + h(x)
        for (int idx = 0; idx < heap.size(); idx++) {
            auto [i, j] = heap[idx];
            int current_euristic = euristic(i, j, end_i, end_j);
            int current_dist = dist[i][j] + current_euristic;
            if (current_dist < min_dist) {
                min_dist = current_dist;
                min_idx = idx;
            }
        }

        auto [i, j] = heap[min_idx];
        heap.erase(heap.begin() + min_idx);

        if (visited[i][j]) continue;
        visited[i][j] = true;
        visited_count++;

        // Если достигли конечной точки
        if (end_i == i && end_j == j) {
            parents_ = prev;
            return dist[i][j];
        }

        // Обновление расстояний до соседей
        vector<pair<int, int>> neighbors = getNeighbors(i, j);
        for (auto [ni, nj] : neighbors) {
            if (visited[ni][nj]) continue;

            int cost = getCost(i, j, ni, nj);
            int new_dist = dist[i][j] + cost;

            if (new_dist < dist[ni][nj]) {
                dist[ni][nj] = new_dist;
                prev[ni][nj] = {i, j};

                if (!visited[ni][nj]) {
                    heap.push_back({ni, nj});
                }
            }
        }
    }

    parents_ = prev;
    return -1;
}

// ==================== КОММЕНТАРИИ К АЛГОРИТМАМ ====================

/*
АЛГОРИТМ ДЕЙКСТРЫ:
1. Пока открытый список не пуст:
2.    Взять узел с минимальной стоимостью g(x) из открытого списка
3.    Если этот узел является целевым, завершить
4.    Поместить узел в закрытый список
5.    Для каждого соседа этого узла:
6.       Если сосед в закрытом списке, пропустить
7.       Вычислить стоимость g для соседа (текущий g + стоимость перехода)
8.       Если сосед не в открытом списке, добавить его
9.       Иначе, если найден более короткий путь, обновить его стоимость

АЛГОРИТМ A*:
1. Пока открытый список не пуст:
2.    Взять узел с минимальной оценкой f(x) = g(x) + h(x) из открытого списка
3.    Если этот узел является целевым, завершить
4.    Поместить узел в закрытый список  
5.    Для каждого соседа этого узла:
6.       Если сосед в закрытом списке, пропустить
7.       Вычислить стоимость g для соседа (текущий g + стоимость перехода)
8.       Если сосед не в открытом списке, добавить его
9.       Иначе, если найден более короткий путь, обновить его стоимость
*/