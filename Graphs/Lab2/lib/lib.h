#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int value) : data(value), color(RED), left(nullptr), 
        right(nullptr), parent(nullptr) {};
};

class RBT {
private:
    Node* root;
    Node* listnode; // (всегда черный)

    // Вспомогательные методы
    void leftRotate(Node* x); // Параметр - относительно кого ротируем
    void rightRotate(Node* y);
    void fixInsert(Node* z); // Делает балансировку и перекрашивание
    void fixRemove(Node* x);
    void transplant(Node* u, Node* v); // Заменяем узел при удалении, 
                                       // чтобы не потерять поддерево
    Node* minimum(Node* node);
    Node* maximum(Node* node);
    void deleteTree(Node* node);

public:
    RBT();
    ~RBT();

    // Основные операции
    bool insert(int value); // Просто вставляет узел в дерево по правилам бинарного поиска.
    bool remove(int value);
    Node* search(int value);

    // Получение min max
    int findMin();
    int findMax();
    std::vector<int> getMin10();
    std::vector<int> getMax10();

    void printTree();
};
#endif