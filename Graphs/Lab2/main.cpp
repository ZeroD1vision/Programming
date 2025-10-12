#include "lib/lib.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

void printMenu() {
    cout << "\n=== Red-Black Tree Commands ===" << endl;
    cout << "search <value>  - Search for an element" << endl;
    cout << "insert <value>  - Insert an element" << endl;
    cout << "remove <value>  - Remove an element" << endl;
    cout << "min             - Find minimum element" << endl;
    cout << "min10           - Find 10 smallest elements" << endl;
    cout << "max             - Find maximum element" << endl;
    cout << "max10           - Find 10 largest elements" << endl;
    cout << "print           - Print all elements (for debug)" << endl;
    cout << "exit            - Exit program" << endl;
    cout << "===============================" << endl;
}

int main() {
    RBT tree;
    
    // Чтение данных из файла
    string filename;
    cout << "Enter input filename: ";
    cin >> filename;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return 1;
    }
    
    // Предполагаем, что файл содержит числа по одному на строке
    int value;
    vector<int> numbers;
    while (file >> value) {
        numbers.push_back(value);
    }
    file.close();
    
    // Вставляем все числа в дерево
    auto start = high_resolution_clock::now();
    for (int num : numbers) {
        tree.insert(num);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    
    cout << "Tree built from " << numbers.size() << " elements in " 
         << duration.count() << " ms" << endl;
    
    // Командный интерфейс
    string command;
    int inputValue;
    
    printMenu();
    
    while (true) {
        cout << "\nEnter command: ";
        cin >> command;
        
        // if (command == "search") {
        //     cin >> inputValue;
            
        //     start = high_resolution_clock::now();
        //     Node* foundNode = tree.search(inputValue);  // ИЗМЕНЕНО: получаем указатель
        //     end = high_resolution_clock::now();
        //     duration = duration_cast<milliseconds>(end - start);
            
        //     if (foundNode != nullptr) {
        //         cout << "Element " << inputValue << " found at address " << foundNode 
        //              << " with value " << foundNode->data 
        //              << " in " << duration.count() << " microseconds" << endl;
        //     } else {
        //         cout << "Element " << inputValue << " not found" 
        //              << " in " << duration.count() << " microseconds" << endl;
        //     }
                 
        if (command == "insert") {
            cin >> inputValue;
            
            start = high_resolution_clock::now();
            bool success = tree.insert(inputValue);
            end = high_resolution_clock::now();
            duration = duration_cast<milliseconds>(end - start);
            
            if (success) {
                cout << "Element " << inputValue << " inserted successfully in " 
                     << duration.count() << " microseconds" << endl;
            } else {
                cout << "Element " << inputValue << " already exists. Insertion failed in " 
                     << duration.count() << " microseconds" << endl;
            }
            
        } else if (command == "remove") {
            cin >> inputValue;
            
            start = high_resolution_clock::now();
            bool success = tree.remove(inputValue);
            end = high_resolution_clock::now();
            duration = duration_cast<milliseconds>(end - start);
            
            if (success) {
                cout << "Element " << inputValue << " removed successfully in " 
                     << duration.count() << " microseconds" << endl;
            } else {
                cout << "Element " << inputValue << " not found. Removal failed in " 
                     << duration.count() << " microseconds" << endl;
            }
            
        // } else if (command == "min") {
        //     start = high_resolution_clock::now();
        //     int minVal = tree.findMin();
        //     end = high_resolution_clock::now();
        //     duration = duration_cast<microseconds>(end - start);
            
        //     if (minVal != -1) {
        //         cout << "Minimum element: " << minVal 
        //              << " found in " << duration.count() << " microseconds" << endl;
        //     } else {
        //         cout << "Tree is empty" << endl;
        //     }
            
        // } else if (command == "min10") {
        //     start = high_resolution_clock::now();
        //     vector<int> min10 = tree.getMin10();
        //     end = high_resolution_clock::now();
        //     duration = duration_cast<microseconds>(end - start);
            
        //     cout << "10 smallest elements: ";
        //     for (int val : min10) {
        //         cout << val << " ";
        //     }
        //     cout << "\nFound in " << duration.count() << " microseconds" << endl;
            
        // } else if (command == "max") {
        //     start = high_resolution_clock::now();
        //     int maxVal = tree.findMax();
        //     end = high_resolution_clock::now();
        //     duration = duration_cast<microseconds>(end - start);
            
        //     if (maxVal != -1) {
        //         cout << "Maximum element: " << maxVal 
        //              << " found in " << duration.count() << " microseconds" << endl;
        //     } else {
        //         cout << "Tree is empty" << endl;
        //     }
            
        // } else if (command == "max10") {
        //     start = high_resolution_clock::now();
        //     vector<int> max10 = tree.getMax10();
        //     end = high_resolution_clock::now();
        //     duration = duration_cast<microseconds>(end - start);
            
        //     cout << "10 largest elements: ";
        //     for (int val : max10) {
        //         cout << val << " ";
        //     }
        //     cout << "\nFound in " << duration.count() << " microseconds" << endl;
            
        // } else if (command == "print") {
        //     tree.printTree();
            
        } else if (command == "exit") {
            cout << "Goodbye!" << endl;
            break;
            
        } else {
            cout << "Unknown command. Type 'help' to see available commands." << endl;
        }
    }
    
    return 0;
}