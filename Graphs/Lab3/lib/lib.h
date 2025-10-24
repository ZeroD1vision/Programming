#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

class Graph {
private:
    int n;
    vector<vector<int>> capacity_;
    vector<vector<int>> flow_;
    vector<vector<int>> residual_;
    int sink_, source_, maxFlow_;
    void defineSourceAndSink();
    bool bfs(vector<vector<int>>& residual, int source, int sink, vector<int>& parent);

public:
    Graph(); // Конструктор по умолчанию
    Graph(const vector<vector<int>>& capacity);
    int getMaxFlow(int s, int t);
    void readMatrix(const string& filename);
    
    const vector<vector<int>>& getFlows() const { return flow_; };
    const vector<vector<int>>& getCapacities() const { return capacity_; };
    int getSink() const { return sink_; };
    int getSource() const {return source_; };
    int getMaxFlow() const { return maxFlow_; }
};

void ParseArguments(int argc, char* argv[], string& inputFile, string& outputFile);

#endif