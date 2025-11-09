#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <vector>
#include <filesystem>
#include <sstream>
#include <string>
#include "lib/lib.h"

// Функция для формирования пути к файлу теста
string getTestFilePath(int testNumber) {
    stringstream ss;
    ss << "tests/matrix/gpg_t3_";
    ss << setw(3) << setfill('0') << testNumber;
    ss << ".txt";
    return ss.str();
}

// Функция для проверки существования файла
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

int main() {
    int testNumber;
    
    // Запрос номера теста у пользователя
    cout << "Enter test file number (1-11): ";
    cin >> testNumber;
    
    // Проверка корректности ввода
    if (testNumber < 1 || testNumber > 11) {
        cerr << "Error: test number must be from 1 to 11" << endl;
        return 1;
    }
    
    // Формирование путей к файлам
    string inputFile = getTestFilePath(testNumber);
    string outputFile = "result.txt";
    
    // Проверка существования файла
    if (!fileExists(inputFile)) {
        cerr << "Error: test file not found: " << inputFile << endl;
        return 1;
    }
    
    cout << "Testing: " << inputFile << endl;
    
    try {
        // Загрузка и обработка графа
        Graph g(inputFile);
        vector<int> colors;
        bool bipartite = g.isBipartite(colors);
        
        ofstream fout(outputFile);
        
        // a) Проверка двудольности
        if (bipartite) {
            fout << "Graph is bipartite\n";
            
            // Поиск максимального паросочетания
            auto [matchingSize, matchingEdges] = g.findMaxMatching(colors);
            
            // c) Размер максимального паросочетания
            fout << "Size of maximum matching: " << matchingSize << ".\n";
            
            // d) Рёбра паросочетания
            fout << "Maximum matching:\n{";
            for (size_t i = 0; i < matchingEdges.size(); i++) {
                fout << "(" << matchingEdges[i].first << ", " << matchingEdges[i].second << ")";
                if (i != matchingEdges.size() - 1) {
                    fout << ", ";
                }
            }
            fout << "}\n";
            
        } else {
            fout << "Graph is not bipartite.\n";
        }
        
        fout.close();
        cout << "Result is written to file: " << outputFile << endl;
        
    } catch (const exception& e) {
        cerr << "Error in operating graph: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}