/**
 * @file lib.h
 * @brief Заголовочный файл для параллельной реализации алгоритма Борувки
 * 
 * Содержит объявления классов и функций для поиска минимального остовного дерева
 * с использованием многопоточности.
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

const int INF = numeric_limits<int>::max();

struct Segment {
    int type; // 0 - A), 1 - Б) типы
    set<int> cantact_vertices; // Контактные вершины

    // Тип А) Ребро, оба конца которого принадлежат GPlane, но само оно не принадлежит
    int u, v; 

    // Тип Б) Компонента, дпоолняющая уложенный граф
    set<int> vertices; // Точки 
    set<pair<int, int>> edges; // Ребра, соединяющие с уложенным графом
};

class LayingGraph {
public: 
    LayingGraph(const vector<vector<int>>& adj_matrix); // Конструктор
    bool isLaying(); // Запускает весь алгоритм: находит начальный цикл, сегменты, инкрементально размещает их, проверяя на пересечения. Если всё размещено без конфликтов — граф планарен.
    vector<vector<int>> getFaces() { return faces; }; // Взять все грани (список всех граней, внутри список точек грани)

private:
    int n;
    vector<vector<int>> graph; // Список смежности (из adj_matrix)

    set<pair<int, int>> laying_edges; // Уложенные грани
    set<int> laying_vertices; // Уложенные точки
    vector<vector<int>> faces; // Грани уложенные (состоит из точек границы кажой грани)

    // Вспомогательные функции
    vector<int> findFirstCycle(); // Поиск начального цикла
    vector<Segment> findSegments(); // Поиск сегментов
    vector<int> findFacesForSegments(const Segment& seg); // Найти грани вмещающие сегмент (вернет их ID)
    vector<int> findPathInSegment(const Segment& seg, int face_id); // Найти путь от контакта до контакта сегмента в грани
    void placeSegment(const Segment& seg, int face_id, const vector<int>& path); // Интегрирует сегмент в укладку
    void splitFace(int face_id, const vector<int>& path); // Когда добавляется путь (ребро/сегмент), грань делится

    vector<int> bfs(int start; const set<int>& allowed_vertices); // Поиск путей в сегментах или гранях
    bool isConnected(const set<int>& vertices); // Сегменты должны быть связными для размещения
    vector<set<int>> findConnectedComponents(const set<int>& vertices); // Для рамзещения в разных местах разных компонент по отдельности (если одна не плананрна граф не планарен)
}

#endif