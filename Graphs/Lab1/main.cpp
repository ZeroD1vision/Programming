#include <iostream>
#include <fstream>
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
            cout << "Enter filename and is it directed or not (Yes/No): ";
            cin >> filename >> directed;
            if (gptr) delete gptr;
            gptr = new Graph(filename.c_str(), directed);
            cout << "Graph was readed from file" << endl;
        }
        return 0;
    }
}