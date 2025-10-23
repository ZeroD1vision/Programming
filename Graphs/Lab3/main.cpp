#include "lib/lib.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

void writeResultsToFile(const Graph& graph, const string& filename) {
    ofstream out(filename);
    if (!out) {
        cerr << "Error opening file!" << endl;
        return;
    }
    // a) Суммарный поток
    out << "Maximum flow value: " << graph.getMaxFlow() << "\n\n";
    // b) Источник и сток
    out << "Source: " << graph.getSource() << ", sink: " << graph.getSink() << "\n\n";
    // c) Список рёбер
    out << "Flow: \n";
    const ... flows = graph.getFlows();
    const ... capacities = graph.getCapacities();
    int n = capacities.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            out << i << "->" << j << ": " << flows[i][j] << "\n";
        }
    }
    out.close();
    cout << "Results successfully written to " << filename << endl;
}

int main(int argc, char* argv[]) {
    Graph graph;
    
    string inputFile, outputFile;
    ParseArguments(argc, argv, inputFile, outputFile);

    cout << "Input file: " << inputFile << endl;
    cout << "Output file: " << outputFile << endl;

    // Чтение матрицы и автоматическое определение источника/стока
    graph.ReadMatrix(inputFile);

    // Вычисление максимального потока
    int maxFlow = graph.getMaxFlow(graph.getSource(), graph.getSink());
    cout << "Computed max flow: " << maxFlow << endl;
    
    // Запись результатов
    writeResultsToFile(graph, outputFile);
    return 0;
}