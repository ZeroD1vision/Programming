#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <vector>
#include <filesystem>
#include <sstream>
#include <string>
#include "lib/lib.h"

using namespace std;
using namespace std::chrono;


// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================


string getTestPath(int testNumber, const string& testName = "map_") {
    ostringstream filename;
    filename << testName << setw(3) << setfill('0') << testNumber << ".txt";

    string path = "tests/" + filename.str();
    if (filesystem::exists(path)) {
        return path;
    }

    return filename.str();
}

void parseArguments(int argc, char* argv[], string& inputFile, string& outputFile, 
                    pair <int, int>& start, pair<int, int>& end) {
    inputFile = "";
    outputFile = "ans.txt";
    start = {-1, -1};
    end = {-1, -1};

    for (int i = 0; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-o" && i+1 < argc) {
            outputFile = argv[i + 1];
            i++;
        } else if (arg == "-t" && i + 1 < argc) {
            int testNumber = stoi(argv[i + 1]);
            inputFile = getTestPath(testNumber);
            i++;
        } else if (arg == "-a" && i + 2 < argc) {
            start = {stoi(argv[i + 1]), stoi(argv[i + 2])};
            i += 2;
        } else if (arg == "-b" && i + 2 < argc) {
            end = {stoi(argv[i + 1]), stoi(argv[i + 2])};
            i += 2;
        } else if (inputFile.empty()) {
            inputFile = arg;
        } else {
            cerr << "Warning: Unknown argument: " << arg << endl;
        }
    }

    if (inputFile.empty()) {
        cerr << "Error: Input file name or test number is required" << endl;
        cerr << "Usage: " << argv[0] << " <input_file> -a <start_i> <start_j> -b <end_i> <end_j> [-o <output_file>]" << endl;
        cerr << "Or: " << argv[0] << " -t <test_number> -a <start_i> <start_j> -b <end_i> <end_j> [-o <output_file>]" << endl;
        exit(1);
    }

    if (start.first == -1 || start.second == -1 || end.first == -1 || end.second == -1) {
        cerr << "Error: Start and end coordinates are required" << endl;
        cerr << "Use -a <start_i> <start_j> -b <end_i> <end_j>" << endl;
        exit(1);
    }
}

vector<vector<int>> readHeights(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }

    int rows, cols;

    file >> rows;
    file >> cols;

    vector<vector<int>> heights;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<int> row;
        stringstream ss(line);
        int height;

        while (ss >> height) {
            row.push_back(height);
        }

        heights.push_back(row);
    }
    return heights;
}


// ==================== ФУНКЦИИ ВЫВОДА РЕЗУЛЬТАТОВ ====================


void writeResultsToFile(ofstream& file, const string& algorithm, 
                        int length, const vector<pair<int, int>>& path, 
                        double visited_percent, double time_ms) {
        file << algorithm << ":\n";
        file << length << " - length of path between (" << path[0].first 
             << ", " << path[0].second << ") and (" << path.back().first 
             << ", " << path.back().second << ") points.\n";
        file << "Path:\n[";

        for (size_t i = 0; i < path.size(); ++i) {
        file << "(" << path[i].first << ", " << path[i].second << ")";
        if (i < path.size() - 1) file << ", ";
        if (i > 0 && (i + 1) % 10 == 0) file << "\n";
    }
    file << "]\n";
    
    file << "Visited: " << visited_percent << "%\n";
    file << "Time: " << time_ms << " ms\n";

    file << "\n=================================================\n";
}


// ==================== ОСНОВНАЯ ФУНКЦИЯ ====================


int main(int argc, char* argv[]) {
    string inputFile, outputFile;
    pair<int, int> start, end;
    
    parseArguments(argc, argv, inputFile, outputFile, start, end);
    
    cout << "\n--------------ECHOPRINT-------------------\n" << endl;

    cout << "Input file: " << inputFile << endl;
    cout << "Output file: " << outputFile << endl;
    cout << "Start: (" << start.first << ", " << start.second << ")" << endl;
    cout << "End: (" << end.first << ", " << end.second << ")" << endl;
    
    try {
        // Чтение карты
        vector<vector<int>> heights = readHeights(inputFile);
        Map map(heights);
        
        // Проверка координат
        if (start.first < 0 || start.first >= map.getRows() || 
            start.second < 0 || start.second >= map.getCols()) {
            throw runtime_error("Invalid start coordinates");
        }
        if (end.first < 0 || end.first >= map.getRows() || 
            end.second < 0 || end.second >= map.getCols()) {
            throw runtime_error("Invalid end coordinates");
        }
        
        // Проверка, что старт и конец не на нулевых высотах
        if (heights[start.first][start.second] == 0) {
            throw runtime_error("Start point is on impassable terrain (height 0)");
        }
        if (heights[end.first][end.second] == 0) {
            throw runtime_error("End point is on impassable terrain (height 0)");
        }
        
        ofstream output(outputFile);
        if (!output.is_open()) {
            throw runtime_error("Cannot open output file: " + outputFile);
        }
        
        cout << "\n------------------------------------------\n" << endl;

        // Dijkstra
        cout << "Running Dijkstra..." << endl;
        int dijkstra_visited;
        auto start_time = high_resolution_clock::now();
        int dijkstra_length = map.calcLengthDijkstra(start, end, dijkstra_visited);
        auto end_time = high_resolution_clock::now();
        auto dijkstra_time = duration_cast<microseconds>(end_time - start_time).count() / 1000.0;
        
        vector<pair<int, int>> dijkstra_path = map.recreatePath(start, end);
        double dijkstra_percent = (dijkstra_visited * 100.0) / map.getTotalNodes();
        
        if (dijkstra_length != -1) {
            writeResultsToFile(output, "Dijkstra", dijkstra_length, dijkstra_path, 
                       dijkstra_percent, dijkstra_time);
            cout << "Dijkstra: length = " << dijkstra_length << ", visited = " << dijkstra_percent << "%, time = " << dijkstra_time << "ms" << endl;
        } else {
            output << "Dijkstra: Path not found\n---\n";
            cout << "Dijkstra: Path not found" << endl;
        }
        
        cout << "\n------------------------------------------\n" << endl;

        // A*
        cout << "Running A*..." << endl;
        int astar_visited;
        start_time = high_resolution_clock::now();
        int astar_length = map.calcLengthAStar(start, end, astar_visited);
        end_time = high_resolution_clock::now();
        auto astar_time = duration_cast<microseconds>(end_time - start_time).count() / 1000.0;
        
        vector<pair<int, int>> astar_path = map.recreatePath(start, end);
        double astar_percent = (astar_visited * 100.0) / map.getTotalNodes();

        if (astar_length != -1) {
            writeResultsToFile(output, "A*", astar_length, astar_path, 
                       astar_percent, astar_time);
            cout << "A*: length = " << astar_length << ", visited = " << astar_percent << "%, time = " << astar_time << "ms" << endl;
        } else {
            output << "A*: Path not found\n---\n";
            cout << "A*: Path not found" << endl;
        }
        

        cout << "Results successfully written to " << outputFile << endl;
        cout << "\n------------------FIXING-------------------\n" << endl;
        
        
        // ========== ОТЛАДКА И ПРОВЕРКА ЭТАЛОННОГО РЕШЕНИЯ ==========
        
        
        vector<pair<int, int>> eeniya = {
            {0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, 
            {2, 8}, {3, 8}, {3, 9}, {3, 10}, {3, 11}, {3, 12}, {3, 13}, {3, 14}, {3, 15}, {3, 16}, 
            {3, 17}, {3, 18}, {4, 18}, {5, 18}, {5, 19}, {6, 19}, {6, 20}, {7, 20}, {8, 20}, {9, 20}, 
            {10, 20}, {11, 20}, {12, 20}, {13, 20}, {13, 19}, {13, 18}, {14, 18}, {14, 17}, {15, 17}, {16, 17}, 
            {17, 17}, {18, 17}, {18, 18}, {19, 18}, {19, 19}
        };

        int pathCostEeniya = map.calculatePathCost(eeniya);
        cout << "Path cost from eeniya: " << pathCostEeniya << endl;
    
        if (!dijkstra_path.empty()) {
            int dijkstra_calculated_cost = map.calculatePathCost(dijkstra_path);
            cout << "Dijkstra calculated cost: " << dijkstra_calculated_cost << endl;
        }

        if (!astar_path.empty()) {
            int astar_calculated_cost = map.calculatePathCost(astar_path);
            cout << "A* calculated cost: " << astar_calculated_cost << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}