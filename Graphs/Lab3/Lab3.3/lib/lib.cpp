#include "lib.h"
#include <climits>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <stack>

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

bool Graph::isBipartite(vector<int>& colors) {
    stack<int> s;
    colors.assign(size_, -1); // 1. Начальная вершина в синий
    // 2. Проверим все смежные с текущей
    for (int i = 0; i < size_; i++) {
        // 3. Выбираем соседнюю непосещенную
        if (colors[i] == -1) {
            // 4. Красим в другой цвет, текущую сохраняем в стек
            colors[i] = 0; // Меняем цвет
            s.push(i); // Добавим в очередь

            // 5. Пока стек не пуст
            while (!s.empty()) { // Пока есть неперекрашенные соседи
                int u = s.top(); // Забираем из очереди
                s.pop(); // Удалим из очереди

                for (int v = 0; v < size_; v++) {
                    if (matrix[u][v]) { // Существует связь
                        if (colors[v] == -1) { // Непосещенные
                            colors[v] = 1 - colors[u]; // Гениально
                            s.push(v); // Добавим в очередь
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

// ==================== АЛГОРИТМ КУНА  ====================
pair<int, vector<pair<int, int>>> Graph::findMaxMatching(const vector<int>& colors) {
    vector<int> U, V;
    for (int i = 0; i < size_; i++) {
        if (colors[i] == 0) U.push_back(i);
        else V.push_back(i);
    }

    int nLeft = U.size(); // Кол-во в U
    int nRight = V.size(); // Кол-во в V

    // Создаём mapping из исходных индексов в индексы в массиве V
    vector<int> indexInV(size_, -1);
    for (int i = 0; i < nRight; i++) {
        indexInV[V[i]] = i;
    }

    // Массивы для хранения сматченности (биективные)
    // массив размером |V| (не n!), где индексы — это упорядоченные позиции V 
    // (например, если V = {3,5,7}, то локальные индексы 0=3, 1=5, 2=7).
    vector<int> matchU(nLeft, -1); // для вершин U: matchU[i] = индекс в V
    vector<int> matchV(nRight, -1); // для вершин V: matchV[j] = индекс в U

    // Основной цикл алгоритма Куна
    int matchingSize = 0;
    for (int u = 0; u < nLeft; u++) {
        vector<bool> visited(nRight, false); // для каждого u свой массив посещений
        if (dfs(u, visited, matchU, matchV, U, V, indexInV, colors)) {
            matchingSize++;
        }
    }

    // Поиск списка ребер
    vector<pair<int, int>> matchingEdges;
    for (int u = 0; u < nLeft; u++) {
        if (matchU[u] != -1) {
            matchingEdges.push_back({U[u], V[matchU[u]]});
        }
    }

    return {matchingSize, matchingEdges};
}

bool Graph::dfs(int u, vector<bool>& visited, vector<int>& matchU, 
                vector<int>& matchV, const vector<int>& U, 
                const vector<int>& V, const vector<int>& indexInV, 
                const vector<int>& colors) {
    int originalU = U[u]; // исходный номер вершины U (он построен линейно U = {0,1,2} 
                          // в отличие от V = {1,3,5})
                          // indexInU маппинг не нужен

    for (int v = 0; v < size_; v++) {
        if (matrix[originalU][v] && colors[v] == 1) {
            // Переводим оригинальный v
            // в его локальный индекс в V (позицию в matchV).
            // Если v не в V, vIndex = -1
            int v_index = indexInV[v]; // для доступа к массиву matchV
            // matchV[vIndex] хранит индекс вершины из U, сматченной с v (или -1, если свободна).

            if (v_index != -1 && !visited[v_index]) {
                visited[v_index] = true;

                // Если V свободна ИЛИ можно переподвесить её текущую пару
                if (matchV[v_index] == -1 || dfs(matchV[v_index], visited, matchU, matchV, U, V, indexInV, colors)) {
                    // Инвертируем ребра (спрашиваем Мурзика может ли он расстаться с Василисой)
                    matchU[u] = v_index; // Добавляем ребро U-V
                    matchV[v_index] = u; // Добавляем ребро V-U
                    return true; // Успешно нашли увеличивающую цепь
                } 
            }
        }
    }
    return false; // Не нашли увеличивающую цепь
}