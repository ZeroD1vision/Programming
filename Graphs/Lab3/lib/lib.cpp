#include "lib.h"
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Graph::Graph() : source_(-1), sink_(-1), maxFlow_(0) {
    // Конструктор по умолчанию
}

Graph::Graph(const vector<vector<int>>& capacity) 
    : capacity_(capacity), 
      flow_(capacity.size(), vector<int>(capacity.size(), 0)), 
      maxFlow_(0), 
      source_(-1), sink_(-1) { // Инициализируем -1, чтобы проверить, если не найдём
    defineSourceAndSink();
}

void Graph::readMatrix(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        exit(1);
    }

    int n;
    file >> n;

    capacity_.resize(n, vector<int>(n));
    flow_.resize(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(file >> capacity_[i][j])) {
                cerr << "Error reading matrix element [" 
                     << i << "][" << j "]" << endl;
                exit(1);
            }
        }
    }

    file.close();
    defineSourceAndSink();
    cout << "Successfully read adjacency matrix " << n << "x" << n 
         << " from " << filename << endl;
}


void Graph::defineSourceAndSink() {
    int n = capacity_.size();
    if (n == 0) return;  // Пустой граф — ничего не делаем
    
    // Ищем source: узел без входящих рёбер
    for (int i = 0; i < n; ++i) {
        bool hasIncoming = false;
        for (int j = 0; j < n; ++j) {
            if (capacity_[j][i] > 0) {
                hasIncoming = true;
                break;
            }
        }
        if (!hasIncoming) {
            source_ = i;
            break; // Берем первого найденного
        }
    }

    // Ищем sink: узел без исходящих рёбер
    for (int i = 0; i < n; ++i) {
        bool hasOutgoing = false;
        for (int j = 0; j < n; ++j) {
            if (capacity_[i][j] > 0) {
                hasOutgoing = true;
                break; 
            }
        }
        if (!hasOutgoing) {
            sink_ = i;
            break; // Берем первого найденного
        }
    }

    cout << "Auto-detected source: " << source_ << ", sink: " 
         << sink_ << endl;
}

int Graph::getMaxFlow(int s, int t) {
    int n = capacity_.size();
    source_ = s;
    sink_ = t;

    vector<vector<int>> residual = capacity_; // Изначально остаточная сеть равна capacity
    flow_ = vector<vector<int>>(n, vector<int>(n, 0)); // Инициализируем поток f нулевым.

    vector<int> parent(n);
    maxFlow_ = 0;

    // Пока существует путь из источника в сток в остаточной сети (с положительными пропускными способностями):
    while (bfs(residual, s, t, parent)) { // Находим путь с помощью BFS и определяем минимальную остаточную способность (min_residual) на этом пути.
        int min_residual = INT_MAX;
        // Для каждого ребра (u, v) на пути:
        // Находим минимальную способность на всем пути 
        for (int v = sink_; v != source_; v = parent[v]) {
            int u = parent[v];
            if (residual[u][v] < min_residual) {
                min_residual = residual[u][v];
            }
        }
        // Находим остатки от потока на пути и обновляем поток
        for (int v = sink_; v != source_; v = parent[v]) {
            int u = parent[v];
            if (capacity_[u][v] > 0) {
                flow_[u][v] += min_residual;
            } else {
                flow_[v][u] -= min_residual;
            }
            residual[u][v] -= min_residual;
            residual[v][u] += min_residual;
        }

        // Так как сумма min ПС всех путей равна max потоку
        maxFlow_ += min_residual; 
    }

    return maxFlow_;
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
        } else if (inputFile.empty()) {
            inputFile = arg;
        } else {
            cerr << "Warning: Unknown arguments: " << arg << endl;
        }
    }

    if (inputFile.empty()) {
        cerr << "Error: Input file name is required" << endl;
        cerr << "Usage: " << argv[0] << " <input_file> [-o <output_file>]" << endl;
        exit(1);
    }
}

bool Graph::bfs(vector<vector<int>>& residual, int source, int sink, vector<int>& parent) {
    int n = capacity_.size();
    vector<bool> visited(n, false);
    vector<int> queue(n);
    int current = 0; // Текущий индекс для чтения из очереди
    int end = 0;     // Конечный элемент очереди

    visited[source] = true; 
    queue[end++] = source;
    parent[source] = -1;
    
    while (current < end) {
        int u = queue[current++];
        for (int v = 0; v < n; v++) {
            if (!visited[v] && residual[u][v] > 0) {
                parent[v] = u;
                visited[v] = true;
                queue[end++] = v;

                if (v == sink) {
                    return true;
                }
            }
        }
    }

    return false;
}