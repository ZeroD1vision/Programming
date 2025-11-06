#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <vector>
#include <filesystem>
#include <sstream>
#include <string>
#include "lib/lib.h"

int main() {
    string inputFile = "tests/matrix/gpg_t3_001.txt";
    string outputFile = "result.txt";

    Graph g(inputFile);
    int n = g.getSize();
    auto matrix = g.getMatrix();

    vector<int> colors;
    bool bipartite = g.isBipartite(colors);

    ofstream fout(outputFile);

    if(bipartite) {
        fout << "Граф двудольный\n";

        // Формируем матрицу двудольного графа (часто в виде двух частей: U и V)
        vector<int> U, V;
        for(int i=0; i<n; i++) {
            if(colors[i] == 1) U.push_back(i);
            else V.push_back(i);
        }
        int nLeft = U.size();
        int nRight = V.size();

        // Создаем матрицу для алгоритма
        vector<vector<int>> bipartiteMatrix(nLeft, vector<int>(nRight, 0));
        for(int i=0; i<nLeft; i++) {
            for(int j=0; j<nRight; j++) {
                if(matrix[U[i]][V[j]] != 0) {
                    bipartiteMatrix[i][j] = 1;
                }
            }
        }

        // BipartiteMatcher matcher(bipartiteMatrix, nLeft);
        // int maxMatch = matcher.maxMatching();

        // // Ребра паросочетания
        // auto matchingEdges = matcher.getMatchingEdges();

        // // Записи
        // fout << "Количество паросочетаний: " << maxMatch << "\n";
        // fout << "Размер максимального паросочетания: " << maxMatch << "\n";
        // fout << "Ребра, входящие в паросочетание:\n";
        // for(auto& edge : matchingEdges) {
        //     int u = U[edge.first];
        //     int v = V[edge.second];
        //     fout << u << " - " << v << "\n";
        // }

    } else {
        fout << "Граф не двудольный\n";
    }

    fout.close();
    return 0;
}