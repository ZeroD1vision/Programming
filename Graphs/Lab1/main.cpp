#include <iostream>
#include <fstream>
#include <iomanip>
#include "lib/lib.h"

using namespace std;

int main() {
    Graph* gptr = nullptr; // Указатель на объект графа в динамической памяти
    string command;

    while (true) {
        cout << "Enter command: ";
        cin >> command;

        if (command == "create") {
            bool directed;
            cout << "Directed? (1/0): ";
            cin >> directed;
            if (gptr) delete gptr; // Удаляем из памяти если есть
            gptr = new Graph("", directed);
            cout << "Empty graph created" << endl;
        }
        else if (command == "read") {
            string filename;
            bool directed;
            cout << "Enter filename and is it directed or not (1/0): ";
            cin >> filename >> directed;
            if (gptr) delete gptr;
            gptr = new Graph(filename.c_str(), directed);
            cout << "Graph was readed from file" << endl;
        }
        else if (command == "add_vertex") {
            // Добавление вершины
            int v;
            cout << "Enter vertex: ";
            cin >> v;
            if (gptr) gptr->add_vertex(v);
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "add_edge") {
            int v1, v2, w;
            cout << "Enter vertices and weight: ";
            cin >> v1 >> v2 >> w;
            if (gptr) gptr->add_edge(v1, v2, w);
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "remove_vertex") {
            // Удаление вершины
            int v;
            cout << "Enter vertex: ";
            cin >> v;
            if (gptr) gptr->remove_vertex(v);
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "remove_edge") {
            // Удаление ребра
            int v1, v2;
            cout << "Enter vertices: ";
            cin >> v1 >> v2;
            if (gptr) gptr->remove_edge(v1, v2);
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "list_vertices") {
            // Вывод всех вершин
            if (gptr) gptr->list_of_vertices();
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "list_edges") {
            // Вывод всех ребер (list_of_edges без аргументов)
            if (gptr) gptr->list_of_edges();
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "list_vertex_edges") {
            // Вывод ребер для конкретной вершины (list_of_edges с аргументом)
            int v;
            cout << "Enter vertex: ";
            cin >> v;
            if (gptr) gptr->list_of_edges(v);
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "is_connected") {
            // Проверка связности графа
            if (gptr) cout << (gptr->is_connected() ? "Yes" : "No") << endl;
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "connected_components") {
            // Вывод компонент связности
            if (gptr) gptr->connected_components();
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "shortest_path") {
            // Кратчайший путь между двумя вершинами
            int start, end;
            cout << "Enter start and end vertices: ";
            cin >> start >> end;
            if (gptr) gptr->shortest_path(start, end);
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "distances") {
            // Расстояния от одной вершины до всех остальных
            int start;
            cout << "Enter start vertex: ";
            cin >> start;
            if (gptr) gptr->distances_from_vertex(start);
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "all_distances") {
            // Все кратчайшие расстояния (для каждой пары вершин)
            if (gptr) {
                for (int i = 0; i < gptr->size(); i++) {
                    // Вызываем distances_from_vertex для каждой вершины
                    if (gptr) gptr->distances_from_vertex(i + 1);  // Вершины нумеруются с 1
                }
            }
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "mst") {
            // Минимальное остовное дерево
            if (gptr) gptr->min_spanning_tree();
            else cout << "Error: Graph not created" << endl;
        }
        else if (command == "exit") {
            // Выход из программы
            if (gptr) delete gptr;
            break;
        }
        else {
            // Неизвестная команда
            cout << "Unknown command" << endl;
        }
    }
    
    return 0;
}