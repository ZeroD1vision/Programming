#include "lib/lib.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

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
    cout << "visualize       - Generate tree visualization" << endl;
    cout << "exit            - Exit program" << endl;
    cout << "===============================" << endl;
}

// Функция для формирования имени файла теста
string getTestFileName(int testNumber) {
    stringstream filename;
    filename << "tests/epg_t3_";
    filename << setw(3) << setfill('0') << testNumber; // Добавляем ведущие нули
    filename << ".txt";
    return filename.str();
}

int main() {
    RBT tree;
    
    // Чтение данных из файла
    int testNumber;
    cout << "Enter test number (1-9): ";
    cin >> testNumber;
    
    // Проверяем корректность ввода
    if (testNumber < 1 || testNumber > 9) {
        cout << "Error: Test number must be between 1 and 9" << endl;
        return 1;
    }
    
    // Формируем имя файла
    string filename = getTestFileName(testNumber);
    cout << "Loading test file: " << filename << endl;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return 1;
    }
    
    int count;
    file >> count;
    vector<int> numbers;
    numbers.reserve(count);
    
    // Читаем остальные числа
    for (int i = 0; i < count; i++) {
        int value;
        if (!(file >> value)) {
            cout << "Error reading number " << i + 1 << endl;
            break;
        }
        numbers.push_back(value);
    }
    file.close();

    cout << "Read " << numbers.size() << " numbers from file" << endl;

    // ========= ОТЛАДКА ===========
    int successCount = 0;
    auto start = high_resolution_clock::now();
    for (int num : numbers) {
        if (tree.insert(num)) {
            successCount++;
        } 
        // else {
        //     cout << "Failed to insert: " << num << endl;
        // }
    }
    cout << "Read " << numbers.size() << " numbers: ";
    for (int i = 0; i < min(10, (int)numbers.size()); i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;
    
    // // Вставляем все числа в дерево
    
    // int insertedCount = 0;
    // for (int num : numbers) {
    //     if (tree.insert(num)) {
    //         insertedCount++;
    //     }
    // }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Successfully inserted " << successCount << " elements in " 
         << duration.count() << " ms" << endl;
    
    // Командный интерфейс
    string command;
    int inputValue;
    
    printMenu();
    
    while (true) {
        cout << "\nEnter command: ";
        cin >> command;
        
        if (command == "search") {
            cin >> inputValue;
            
            start = high_resolution_clock::now();
            Node* foundNode = tree.search(inputValue);
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start);
            
            if (foundNode != nullptr) {
                cout << "Element " << inputValue << " found at address " << foundNode 
                     << " with value " << foundNode->data 
                     << " in " << duration.count() << " microseconds" << endl;
            } else {
                cout << "Element " << inputValue << " not found" 
                     << " in " << duration.count() << " microseconds" << endl;
            }
                 
        } else if (command == "insert") {
            cin >> inputValue;
            
            start = high_resolution_clock::now();
            bool success = tree.insert(inputValue);
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start);
            
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
            duration = duration_cast<microseconds>(end - start);
            
            if (success) {
                cout << "Element " << inputValue << " removed successfully in " 
                     << duration.count() << " microseconds" << endl;
            } else {
                cout << "Element " << inputValue << " not found. Removal failed in " 
                     << duration.count() << " microseconds" << endl;
            }
            
        } else if (command == "min") {
            start = high_resolution_clock::now();
            int minVal = tree.findMin();
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start);
            
            if (minVal != -1) {
                cout << "Minimum element: " << minVal 
                     << " found in " << duration.count() << " microseconds" << endl;
            } else {
                cout << "Tree is empty" << endl;
            }
            
        } else if (command == "min10") {
            start = high_resolution_clock::now();
            vector<int> min10 = tree.getMin10();
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start);
            
            cout << "10 smallest elements: ";
            for (int val : min10) {
                cout << val << " ";
            }
            cout << "\nFound in " << duration.count() << " microseconds" << endl;
            
        } else if (command == "max") {
            start = high_resolution_clock::now();
            int maxVal = tree.findMax();
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start);
            
            if (maxVal != -1) {
                cout << "Maximum element: " << maxVal 
                     << " found in " << duration.count() << " microseconds" << endl;
            } else {
                cout << "Tree is empty" << endl;
            }
            
        } else if (command == "max10") {
            start = high_resolution_clock::now();
            vector<int> max10 = tree.getMax10();
            end = high_resolution_clock::now();
            duration = duration_cast<microseconds>(end - start);
            
            cout << "10 largest elements: ";
            for (int val : max10) {
                cout << val << " ";
            }
            cout << "\nFound in " << duration.count() << " microseconds" << endl;
            
        } else if (command == "print") {
            tree.printTree();
            
        } else if (command == "visualize") {
            tree.visualize();
    
            // Автоматическая генерация PNG (если установлен Graphviz)
            #ifdef _WIN32
                system("dot -Tpng tree.dot -o tree.png 2>nul");
            #else
                system("dot -Tpng tree.dot -o tree.png 2>/dev/null");
            #endif

            cout << "Tree visualization generated!" << endl;
        } else if (command == "exit") {
            cout << "Goodbye!" << endl;
            break;
            
        } else {
            cout << "Unknown command. Type 'help' to see available commands." << endl;
        }
    }
    
    return 0;
}