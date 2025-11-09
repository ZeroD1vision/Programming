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
    bool dfs(int u, vector<bool>& visited, vector<int>& matchU, vector<int>& matchV, 
             const vector<int>& U, const vector<int>& V, const vector<int>& indexInV, 
             const vector<int>& colors);

public:
    Graph(const string& filename);
    bool isBipartite(vector<int>& colors);
    pair<int, vector<pair<int, int>>> findMaxMatching(const vector<int>& colors);

    int getSize() const { return size_; }
    vector<vector<int>> getMatrix() const { return matrix; };
};

#endif