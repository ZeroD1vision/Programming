/**
 * @file main.cpp
 * @brief Главный файл приложения для поиска MST алгоритмом Борувки
 * 
 * Принимает параметры командной строки:
 * - Обязательный: имя входного файла или номер теста (1-10)
 * - Опционально: количество потоков (-n), имя выходного файла (-o)
 * 
 * Примеры использования:
 *   ./program 1                    # Использовать тест 1
 *   ./program input.txt            # Использовать конкретный файл
 *   ./program 1 -n 8 -o result.txt # Тест 1, 8 потоков, вывод в result.txt
 */

#include "lib/lib.h"
#include <iostream>
#include <fstream>

using namespace std;

/**
 * @brief Точка входа в приложение
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код завершения программы (0 - успех, 1 - ошибка)
 * 
 * Обрабатывает аргументы командной строки, запускает алгоритм Борувки
 * и сохраняет результаты в указанный файл.
 */
int main(int argc, char* argv[]) {
    string input_file, output_file = "ans.txt";
    int n_threads = 4;

    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-n" && i + 1 < argc) {
            n_threads = stoi(argv[++i]);
        } else if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else {
            // Проверяем, является ли аргумент числом от 1 до 10
            try {
                int test_num = stoi(arg);
                if (test_num >= 1 && test_num <= 10) {
                    input_file = getTestFileName(test_num);
                } else {
                    input_file = arg;
                }
            } catch (const exception&) {
                input_file = arg; // если не число, считаем, что это уже имя файла
            }
        }
    }

    if (input_file.empty()) {
        cerr << "Usage: " << argv[0] << " <input_file|test_number(1-10)> [-n threads] [-o output_file]" << endl;
        cerr << "Examples:" << endl;
        cerr << "  " << argv[0] << " 1                    # Use test file 1" << endl;
        cerr << "  " << argv[0] << " input.txt            # Use specific file" << endl;
        cerr << "  " << argv[0] << " 1 -n 8 -o result.txt # Use test file 1 with 8 threads" << endl;
        return 1;
    }

    try {
        auto graph = readGraph(input_file);
        cout << "Graph loaded: " << graph.size() << " vertices" << endl;
        
        ParallelBoruvka boruvka(graph, n_threads);
        double duration;
        auto mst = boruvka.findMST(duration);
        int total_weight = boruvka.calculateTotalWeight(mst);

        // Запись результатов
        ofstream fout(output_file);
        fout << "Time: " << duration << " ms, Threads: " << n_threads << endl;
        fout << "Total weight: " << total_weight << endl;
        fout << "Edges in MST:" << endl;
        for (const auto& edge : mst) {
            int u = edge.first;
            int v = edge.second;
            int weight = 0;
            
            for (const auto& neighbor : graph[u]) {
                if (neighbor.first == v) {
                    weight = neighbor.second;
                    break;
                }
            }
            fout << edge.first << " - " << edge.second << " : " << weight << endl;
        }
        fout.close();

        cout << "MST found with total weight: " << total_weight << endl;
        cout << "Execution time: " << duration << " ms" << endl;
        cout << "Results written to: " << output_file << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}