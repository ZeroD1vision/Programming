#include "lib/lib.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>

using namespace std;

string getTestPath(int testNumber, const string& testName = "gpg_t1_") {
    ostringstream filename;
    filename << testName << setw(3) << setfill('0') << testNumber << ".txt";

    string path = "tests/matrix/" + filename.str();
    if (filesystem::exists(path)) {
        return path;
    }

    return filename.str();
}

void ParseArguments(int argc, char* argv[], string& inputFile, string& outputFile) {
    inputFile = "";
    outputFile = "ans.txt";

    // Начинаем с 1, потому что argv[0] - это имя программы
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[i + 1]; // Следующий аргумент - имя выходного файла
            i++; // Пропускаем следующий аргумент, так как его уже обработали
        } else if (arg == "-t" && i + 1 < argc) {
            // Флаг -t: следующий аргумент - номер теста
            int testNumber = stoi(argv[i + 1]);
            inputFile = getTestPath(testNumber);  // Преобразуем номер в путь
            i++; // Пропускаем следующий аргумент
        } else if (inputFile.empty()) {
            inputFile = arg;
        } else {
            cerr << "Warning: Unknown argument: " << arg << endl;
        }
    }

    if (inputFile.empty()) {
        cerr << "Error: Input file name or test number is required" << endl;
        cerr << "Usage: " << argv[0] << " <input_file> [-o <output_file>]" << endl;
        cerr << "Or: " << argv[0] << " -t <test_number> [-o <output_file>]" << endl;
        exit(1);
    }
}

void writeResultsToFile(const Graph& graph, const string& filename) {
    ofstream out(filename);
    if (!out) {
        cerr << "Error opening file!" << endl;
        return;
    }
    // a) Суммарный поток
    out << "Maximum flow value: " << graph.getMaxFlow() << "\n\n";
    // b) Источник и сток
    out << "Source: " << (graph.getSource() + 1) << ", sink: " << (graph.getSink() + 1) << "\n\n";
    // c) Список рёбер
    out << "Flow: \n";
    const vector<vector<int>>& flows = graph.getFlows();
    const vector<vector<int>>& capacities = graph.getCapacities();
    int n = capacities.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // if (flows[i][j] > 0 || capacities[i][j]> 0){
            if (flows[i][j] > 0){
                out << i + 1 << "->" << j + 1 << ": " << flows[i][j] << "\n";
            }
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
    graph.readMatrix(inputFile);

    // Вычисление максимального потока
    int maxFlow = graph.getMaxFlow(graph.getSource(), graph.getSink());
    cout << "Computed max flow: " << maxFlow << endl;
    
    // Запись результатов
    writeResultsToFile(graph, outputFile);
    return 0;
}