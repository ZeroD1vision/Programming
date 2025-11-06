#include "lib.h"
#include <climits>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

// ==================== КОНСТРУКТОР И ИНИЦИАЛИЗАЦИЯ ====================

Graph::Graph(const string& filename) {
    ifstream file(filename);
    file >> size_;
    matrix.resize(size_, vector<int>(size_));
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            file >> matrix[i][j];
        }
    }
}

// ==================== ОСНОВНЫЕ ФУНКЦИИ ====================

bool Graph::isBipartite (vector<int>& colors) {
    queue<int> q;
    colors.assign(size_, -1); // 1. Начальная вершина в синий
    // 2. Проверим все смежные с текущей 
    for (int i = 0; i < size_; i++) {
        // 3. Выбираем соседнюю непосещенную
        if (colors[i] == -1) {
            // 4. Красим в другой цвет, текущую сохраняем в стек
            colors[i] = 0; // Меняем цвет
            q.push(i); // Добавим в очередь

            // 5. Пока стек не пуст
            while (!q.empty()) { // Пока есть неперекрашенные соседи
                int u = q.front(); // Забираем из очереди
                q.pop(); // Удалим из очереди 

                for (int v = 0; v < size_; v++) {
                    if (matrix[u][v]) { // Существует связь
                        if (colors[v] == -1) { // Непосещенные
                            colors[v] = 1 - colors[u]; // Гениально
                            q.push(v); // Добавим в очередь
                        } else if (colors[u] == colors[v]) { // Если среди них вершина с тем же цветом 
                            return false; // — не двудольный
                        }
                    }
                }
            } 
        }
    }
    return true;
}