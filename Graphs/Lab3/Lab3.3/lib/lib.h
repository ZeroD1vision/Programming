#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <utility>

using namespace std;

class Graph {
private:
    int size_;
    vector<vector<int>> matrix;

public:
    Graph(const string& filename);
    bool isBipartite(vector<int>& colors);

    int getSize() const { return size_; }
    vector<vector<int>> getMatrix() const { return matrix; };
};

#endif