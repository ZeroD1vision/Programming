#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "lib/lib.h"

using namespace std;

// Функция для проверки, является ли строка числом
bool isNumber(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Функция для форматирования номера теста в трехзначный формат
string formatTestNumber(int num) {
    stringstream ss;
    ss << setw(3) << setfill('0') << num;
    return ss.str();
}

// Функция для вывода справки по командам
void printHelp() {
    cout << "===============================================" << endl;
    cout << "           GRAPH MANAGER - COMMANDS LIST       " << endl;
    cout << "===============================================" << endl;
    cout << "create               - Create empty graph" << endl;
    cout << "read <test_num>      - Read graph from test file (1-18)" << endl;
    cout << "read_file <filename> - Read graph from custom file" << endl;
    cout << "add_vertex <v>       - Add vertex" << endl;
    cout << "add_edge <v1 v2 w>   - Add edge with weight" << endl;
    cout << "remove_vertex <v>    - Remove vertex" << endl;
    cout << "remove_edge <v1 v2>  - Remove edge" << endl;
    cout << "list_vertices        - List all vertices" << endl;
    cout << "list_edges           - List all edges" << endl;
    cout << "list_vertex_edges <v>- List edges from vertex" << endl;
    cout << "is_connected         - Check graph connectivity" << endl;
    cout << "connected_components - Show connected components" << endl;
    cout << "shortest_path <s e>  - Find shortest path" << endl;
    cout << "distances <v>        - Find distances from vertex" << endl;
    cout << "all_distances        - Find all shortest paths" << endl;
    cout << "mst                  - Find minimum spanning tree" << endl;
    cout << "clear                - Clear screen" << endl;
    cout << "help                 - Show this help" << endl;
    cout << "exit                 - Exit program" << endl;
    cout << "===============================================" << endl;
}

// Функция для очистки экрана (кроссплатформенная)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    Graph* gptr = nullptr;
    string command;
    
    clearScreen();
    cout << "===============================================" << endl;
    cout << "           WELCOME TO GRAPH MANAGER           " << endl;
    cout << "===============================================" << endl;
    cout << "Type 'help' to see available commands" << endl;
    cout << "===============================================" << endl;

    while (true) {
        cout << "\n>>> Enter command: ";
        cin >> command;

        if (command == "create") {
            bool directed;
            cout << ">>> Directed graph? (1 for directed, 0 for undirected): ";
            cin >> directed;
            if (gptr) delete gptr;
            gptr = new Graph("", directed);
            cout << ">>> ✓ Empty graph created successfully" << endl;
        }
        else if (command == "read") {
            string testNum;
            bool directed;
            cout << ">>> Enter test number (1-18): ";
            cin >> testNum;
            cout << ">>> Directed graph? (1/0): ";
            cin >> directed;
            
            if (isNumber(testNum)) {
                int num = stoi(testNum);
                if (num >= 1 && num <= 18) {
                    string filename = "tests/matrix/bpg_" + formatTestNumber(num) + ".txt";
                    if (gptr) delete gptr;
                    gptr = new Graph(filename.c_str(), directed);
                    cout << ">>> ✓ Graph loaded from test file: " << filename << endl;
                } else {
                    cout << ">>> ✗ Error: Test number must be between 1 and 18" << endl;
                }
            } else {
                cout << ">>> ✗ Error: Please enter a valid number" << endl;
            }
        }
        else if (command == "read_file") {
            string filename;
            bool directed;
            cout << ">>> Enter filename: ";
            cin >> filename;
            cout << ">>> Directed graph? (1/0): ";
            cin >> directed;
            if (gptr) delete gptr;
            gptr = new Graph(filename.c_str(), directed);
            cout << ">>> ✓ Graph loaded from file: " << filename << endl;
        }
        else if (command == "add_vertex") {
            int v;
            cout << ">>> Enter vertex number: ";
            cin >> v;
            if (gptr) {
                gptr->add_vertex(v);
                cout << ">>> ✓ Vertex " << v << " added successfully" << endl;
            } else {
                cout << ">>> ✗ Error: Graph not created. Use 'create' or 'read' first." << endl;
            }
        }
        else if (command == "add_edge") {
            int v1, v2, w;
            cout << ">>> Enter vertices and weight (format: v1 v2 weight): ";
            cin >> v1 >> v2 >> w;
            if (gptr) {
                gptr->add_edge(v1, v2, w);
                cout << ">>> ✓ Edge (" << v1 << ", " << v2 << ") with weight " << w << " added successfully" << endl;
            } else {
                cout << ">>> ✗ Error: Graph not created. Use 'create' or 'read' first." << endl;
            }
        }
        else if (command == "remove_vertex") {
            int v;
            cout << ">>> Enter vertex to remove: ";
            cin >> v;
            if (gptr) {
                gptr->remove_vertex(v);
                cout << ">>> ✓ Vertex " << v << " removed successfully" << endl;
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "remove_edge") {
            int v1, v2;
            cout << ">>> Enter vertices to remove edge between (format: v1 v2): ";
            cin >> v1 >> v2;
            if (gptr) {
                gptr->remove_edge(v1, v2);
                cout << ">>> ✓ Edge between " << v1 << " and " << v2 << " removed successfully" << endl;
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "list_vertices") {
            if (gptr) {
                cout << ">>> Vertices in graph: ";
                gptr->list_of_vertices();
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "list_edges") {
            if (gptr) {
                cout << ">>> All edges in graph:" << endl;
                gptr->list_of_edges();
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "list_vertex_edges") {
            int v;
            cout << ">>> Enter vertex: ";
            cin >> v;
            if (gptr) {
                cout << ">>> Edges from vertex " << v << ":" << endl;
                gptr->list_of_edges(v);
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "is_connected") {
            if (gptr) {
                bool connected = gptr->is_connected();
                cout << ">>> Graph is " << (connected ? "connected" : "not connected") << endl;
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "connected_components") {
            if (gptr) {
                cout << ">>> Connected components:" << endl;
                gptr->connected_components();
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "shortest_path") {
            int start, end;
            cout << ">>> Enter start and end vertices (format: start end): ";
            cin >> start >> end;
            if (gptr) {
                cout << ">>> Shortest path from " << start << " to " << end << ":" << endl;
                gptr->shortest_path(start, end);
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "distances") {
            int start;
            cout << ">>> Enter start vertex: ";
            cin >> start;
            if (gptr) {
                gptr->distances_from_vertex(start);
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "all_distances") {
            if (gptr) {
                cout << ">>> All shortest paths:" << endl;
                gptr->all_shortest_paths();
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "mst") {
            if (gptr) {
                cout << ">>> Minimum spanning tree:" << endl;
                gptr->min_spanning_tree();
            } else {
                cout << ">>> ✗ Error: Graph not created." << endl;
            }
        }
        else if (command == "clear") {
            clearScreen();
            cout << "===============================================" << endl;
            cout << "           GRAPH MANAGER - SCREEN CLEARED     " << endl;
            cout << "===============================================" << endl;
        }
        else if (command == "help") {
            printHelp();
        }
        else if (command == "exit") {
            if (gptr) {
                delete gptr;
                cout << ">>> ✓ Graph memory freed" << endl;
            }
            cout << "===============================================" << endl;
            cout << "           THANK YOU FOR USING GRAPH MANAGER  " << endl;
            cout << "===============================================" << endl;
            break;
        }
        else {
            cout << ">>> ✗ Unknown command: '" << command << "'" << endl;
            cout << ">>> Type 'help' to see available commands" << endl;
        }
    }
    
    return 0;
}