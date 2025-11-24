/**
 * @file main.cpp
 * @brief Главный файл приложения для проверки планарности графа
 * 
 * Принимает параметры командной строки:
 * - Обязательный: имя входного файла или номер теста (1-15)
 * - Опционально: имя выходного файла (-o)
 * 
 * Примеры использования:
 *   ./program 1                    # Использовать тест epg_t2_001.txt
 *   ./program input.txt            # Использовать конкретный файл
 *   ./program 5 -o result.txt      # Тест 5, вывод в result.txt
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include "lib/lib.h"
#include <locale> // Для темыча с виндой

using namespace std;
using namespace std::chrono;

/**
 * @brief Генерирует имя тестового файла по номеру теста
 * @param test_num Номер теста (1-15)
 * @return Полный путь к тестовому файлу в формате ../tests/epg_t2_XXX.txt
 */
string getTestFileName(int test_num) {
    stringstream ss;
    ss << "../tests/epg_t2_" << setw(3) << setfill('0') << test_num << ".txt";
    return ss.str();
}

/**
 * @brief Проверяет, является ли строка числом
 * @param str Входная строка
 * @return true если строка представляет собой число, иначе false
 */
bool isNumber(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return !str.empty();
}

/**
 * @brief Отображает прогресс-бар в консоли
 * @param current Текущее значение
 * @param total Общее значение
 * @param barWidth Ширина прогресс-бара в символах
 */
void displayProgressBar(int current, int total, int barWidth = 50) {
    float progress = static_cast<float>(current) / total;
    int pos = static_cast<int>(barWidth * progress);
    
    cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << " %\r";
    cout.flush();
}

/**
 * @brief Читает граф из файла в формате матрицы смежности
 * @param filename Имя входного файла
 * @return Граф в виде матрицы смежности
 * @throws runtime_error если файл не может быть открыт
 */
vector<vector<int>> readGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    
    cout << "📁 Reading graph from: " << filename << endl;
    
    int n;
    file >> n;
    
    vector<vector<int>> graph(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> graph[i][j];
        }
        displayProgressBar(i + 1, n, 30);
    }
    cout << endl;
    
    cout << "✅ Graph loaded: " << n << " vertices" << endl;
    return graph;
}

/**
 * @brief Записывает результаты проверки планарности в файл
 * @param filename Имя выходного файла
 * @param is_planar Результат проверки планарности
 * @param faces Список граней (если граф планарен)
 * @param duration Время выполнения алгоритма в миллисекундах
 */
void writeResult(const string& filename, bool is_planar, 
                const vector<vector<int>>& faces, long long duration) {
    ofstream file(filename);
    
    file << "Planarity Check Results" << endl;
    file << "=======================" << endl;
    file << "Execution time: " << duration << " ms" << endl << endl;
    
    if (is_planar) {
        file << "✓ Graph is PLANAR" << endl << endl;
        file << "Faces of planar embedding:" << endl;
        file << "--------------------------" << endl;
        for (size_t i = 0; i < faces.size(); i++) {
            file << "Face " << i + 1 << ": ";
            for (size_t j = 0; j < faces[i].size(); j++) {
                file << faces[i][j] + 1; // Convert to 1-based for output
                if (j < faces[i].size() - 1) file << " - ";
            }
            file << endl;
        }
    } else {
        file << "✗ Graph is NOT PLANAR" << endl;
        file << "The graph cannot be embedded in the plane without edge crossings." << endl;
    }
}

/**
 * @brief Точка входа в приложение
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код завершения программы (0 - успех, 1 - ошибка)
 * 
 * Обрабатывает аргументы командной строки, запускает проверку планарности
 * и сохраняет результаты в указанный файл с визуальным отображением прогресса.
 */
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, ".UTF-8");
    string input_file, output_file = "result.txt";
    
    cout << "=========================================" << endl;
    cout << "    Graph Planarity Checker" << endl;
    cout << "    Gamma Algorithm Implementation" << endl;
    cout << "=========================================" << endl;
    
    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else {
            // Проверяем, является ли аргумент номером теста (1-15)
            if (isNumber(arg)) {
                int test_num = stoi(arg);
                if (test_num >= 1 && test_num <= 15) {
                    input_file = getTestFileName(test_num);
                    cout << "Using test file: " << input_file << endl;
                } else {
                    input_file = arg;
                }
            } else {
                input_file = arg;
            }
        }
    }
    
    if (input_file.empty()) {
        cerr << "❌ Usage: " << argv[0] << " <input_file|test_number(1-15)> [-o output_file]" << endl;
        cerr << "Examples:" << endl;
        cerr << "  " << argv[0] << " 1                    # Use test file epg_t2_001.txt" << endl;
        cerr << "  " << argv[0] << " input.txt            # Use specific file" << endl;
        cerr << "  " << argv[0] << " 5 -o result.txt      # Use test file 5 with custom output" << endl;
        return 1;
    }
    
    try {
        // Загрузка графа
        auto graph = readGraph(input_file);
        
        // Проверка планарности
        cout << "Checking planarity using Gamma algorithm..." << endl;
        
        auto start = high_resolution_clock::now();
        LayingGraph laying_graph(graph);
        bool is_planar = laying_graph.isLaying();
        auto end = high_resolution_clock::now();
        
        auto duration = duration_cast<milliseconds>(end - start).count();
        
        vector<vector<int>> faces = laying_graph.getFaces();
        
        // Запись результатов
        writeResult(output_file, is_planar, faces, duration);
        
        // Вывод результатов в консоль
        cout << endl << "   RESULTS:" << endl;
        cout << "──────────" << endl;
        if (is_planar) {
            cout << "✅ Graph is PLANAR" << endl;
            cout << "   Number of faces: " << faces.size() << endl;
            cout << "   Execution time: " << duration << " ms" << endl;
        } else {
            cout << "❌ Graph is NOT PLANAR" << endl;
            cout << "   Execution time: " << duration << " ms" << endl;
        }
        cout << "   Results saved to: " << output_file << endl;
        
    } catch (const exception& e) {
        cerr << "   Error: " << e.what() << endl;
        return 1;
    }
    
    cout << endl << "   Program completed successfully!" << endl;
    return 0;
}