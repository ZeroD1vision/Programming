#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <utility>

using namespace std;

class Map {
private:
    int rows, cols;
    vector<vector<int>> heights_; // Массив высот каждой точки (считываем из файла)
    pair<int, int> start_, end_;
    vector<vector<pair<int, int>>> parents_; // Массив родителей для восстановления пути
    
    vector<pair<int, int>> getNeighbors(int i, int j);
    int getCost(int i1, int j1, int i2, int j2);
    int euristic(int i1, int j1, int i2, int j2);

public:
    Map(vector<vector<int>>& heights);

    int calculatePathCost(const vector<pair<int,int>>& path);
    int calcLengthDijkstra(pair<int, int> start, pair<int, int> end, int& visited_count); // Расчет длины пути
    int calcLengthAStar(pair<int, int> start, pair<int, int> end, int& visited_count);
    vector<pair<int, int>> recreatePath(pair<int, int> start, pair<int, int> end);
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getTotalNodes() const { return rows * cols; }
};

#endif