/**
 * @file lib.h
 * @brief Заголовочный файл для реализации алгоритма проверки планарности Gamma
 * 
 * Содержит объявления классов и функций для проверки планарности графа
 * и нахождения плоской укладки с использованием алгоритма Gamma.
 */

#ifndef LIB_H
#define LIB_H

#include <set>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <limits>

using namespace std;
using namespace chrono;

// ==================== ЦВЕТА ДЛЯ ВЫВОДА ====================
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_GRAY    "\033[90m"

// Макросы для удобного цветного вывода
#define DEBUG_OUT(x) cout << COLOR_GRAY << "[DEBUG] " << x << COLOR_RESET << endl
#define INFO_OUT(x)  cout << COLOR_CYAN << "[INFO] " << x << COLOR_RESET << endl
#define WARN_OUT(x)  cout << COLOR_YELLOW << "[WARN] " << x << COLOR_RESET << endl
#define ERROR_OUT(x) cerr << COLOR_RED << "[ERROR] " << x << COLOR_RESET << endl
#define SUCCESS_OUT(x) cout << COLOR_GREEN << "[SUCCESS] " << x << COLOR_RESET << endl

// Макросы для отладочного вывода в lib.cpp
#define DEBUG_COLOR COLOR_GRAY
#define SEGMENT_COLOR COLOR_MAGENTA
#define FACE_COLOR COLOR_CYAN
#define ERROR_COLOR COLOR_RED
#define SUCCESS_COLOR COLOR_GREEN

#define DEBUG_FUNC(x) cout << DEBUG_COLOR << "[DEBUG][" << __func__ << "] " << x << COLOR_RESET << endl
#define SEGMENT_DEBUG(x) cout << SEGMENT_COLOR << "[SEGMENT] " << x << COLOR_RESET << endl
#define FACE_DEBUG(x) cout << FACE_COLOR << "[FACE] " << x << COLOR_RESET << endl

const int INF = numeric_limits<int>::max();

/**
 * @brief Структура, представляющая сегмент графа
 * 
 * Сегмент - это часть графа, которая должна быть уложена в текущую плоскую укладку.
 * Может быть двух типов:
 * - Тип 0: Ребро между двумя уже уложенными вершинами
 * - Тип 1: Компонента связности неуложенных вершин
 */
struct Segment {
    int type;                       ///< Тип сегмента: 0 - ребро, 1 - компонента
    set<int> contact_vertices;      ///< Контактные вершины (соединенные с уложенной частью)

    // Поля для типа 0 (ребро)
    int u, v; 

    // Поля для типа 1 (компонента)
    set<int> vertices;              ///< Вершины компоненты
    set<pair<int, int>> edges;      ///< Ребра компоненты и связи с уложенной частью
};

/**
 * @brief Класс для проверки планарности графа и нахождения плоской укладки
 * 
 * Реализует алгоритм Gamma для проверки планарности графа и построения
 * его плоской укладки с выделением граней.
 */
class LayingGraph {
public: 
    /**
     * @brief Конструктор класса LayingGraph
     * @param adj_matrix Матрица смежности графа
     */
    LayingGraph(const vector<vector<int>>& adj_matrix);
    
    /**
     * @brief Проверяет, является ли граф планарным
     * @return true если граф планарен, false в противном случае
     * 
     * Запускает основной алгоритм проверки планарности:
     * 1. Находит начальный цикл
     * 2. Итеративно находит и укладывает сегменты
     * 3. Проверяет возможность укладки без пересечений
     */
    bool isLaying();
    
    /**
     * @brief Возвращает список граней плоской укладки
     * @return Вектор граней, где каждая грань - вектор вершин в порядке обхода
     */
    vector<vector<int>> getFaces() { return faces; };

private:
    int n;                                  ///< Количество вершин в графе
    vector<vector<int>> graph;              ///< Матрица смежности графа
    set<pair<int, int>> laying_edges;       ///< Множество уложенных ребер
    set<int> laying_vertices;               ///< Множество уложенных вершин
    vector<vector<int>> faces;              ///< Список граней плоской укладки

    // ==================== ОСНОВНЫЕ МЕТОДЫ АЛГОРИТМА ====================
    
    /**
     * @brief Находит первый цикл для начальной укладки
     * @return Вектор вершин, образующих цикл, или пустой вектор если цикл не найден
     */
    vector<int> findFirstCycle();
    
    /**
     * @brief Находит все сегменты для текущей укладки
     * @return Вектор найденных сегментов
     */
    vector<Segment> findSegments();
    
    /**
     * @brief Находит подходящие грани для укладки сегмента
     * @param seg Сегмент для укладки
     * @return Вектор индексов подходящих граней
     */
    vector<int> findFacesForSegment(const Segment& seg);
    
    /**
     * @brief Находит путь для укладки сегмента в указанной грани
     * @param seg Сегмент для укладки
     * @param face_id Индекс грани
     * @return Путь укладки в виде последовательности вершин
     */
    vector<int> findPathInSegment(const Segment& seg, int face_id);
    
    /**
     * @brief Укладывает сегмент в указанную грань
     * @param seg Сегмент для укладки
     * @param face_id Индекс грани
     * @param path Путь укладки
     */
    void placeSegment(const Segment& seg, int face_id, const vector<int>& path);
    
    /**
     * @brief Разделяет грань при укладке нового пути
     * @param face_id Индекс разделяемой грани
     * @param path Путь, разделяющий грань
     */
    void splitFace(int face_id, const vector<int>& path);

    // ==================== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ====================
    
    /**
     * @brief Поиск в ширину с ограничением по допустимым вершинам
     * @param start Начальная вершина
     * @param end Конечная вершина
     * @param allowed_vertices Множество допустимых вершин
     * @return Кратчайший путь или пустой вектор если путь не найден
     */
    vector<int> bfs(int start, int end, const set<int>& allowed_vertices);
    
    /**
     * @brief Находит компоненты связности в множестве вершин
     * @param vertices Множество вершин
     * @return Вектор компонент связности
     */
    vector<set<int>> findConnectedComponents(const set<int>& vertices);
};

#endif