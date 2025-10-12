#include "lib.h"
#include <climits>
#include <fstream>
#include <iostream>

using namespace std;

RBT::RBT() {
    listnode = new Node(0);
    listnode->color = BLACK;
    root = listnode;
}

RBT::~RBT() {
    deleteTree(root);
    delete listnode;
}

// Удаление узла и его поддерева
void RBT::deleteTree(Node* node) {
    if (node != listnode) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

// РОТАЦИИ

// Левая ротация
// 1. B становится на место A (тоже его родителем).
// 2. A становится левым ребенком B.
// 3. Если у B был левый ребенок, он становится правым ребенком A.
// 4. Обновляем указатель у родителя A, чтобы он теперь указывал на B.
// !. Правый ребенок B не меняется, он остается правым ребенком B.
void RBT::leftRotate(Node* x) {
    Node* y = x->right; // устанавливаем y — правого ребенка x

    if (y->left != listnode) { // Если левый для у есть
        x->right = y->left; // левый ребенок у становится правым для x
        y->left->parent = x; // обновляем правого родителя у на х
    }

    y->parent = x->parent; // y становится на место x

    if (x->parent == listnode) // Если х был корнем
        root = y; // y — корень
    else if (x == x->parent->left) // Если х был левым ребенком
        x->parent->left = y; // у станет левым для родителя х
    else
        x->parent->right = y; // наоборот

    y->left = x; // x становится левым ребенком y
    x->parent = y;
}

// Правая ротация
void RBT::rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    
    if (x->right != listnode) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == listnode) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    
    x->right = y;
    y->parent = x;
}

// Делает балансировку и перекрашивание для соблюдения свойств RBT.
// Св-ва RBT:
// 1. Корень всегда черный.
// 2. Красный узел не может иметь красных детей.
// 3. У всех путей от корня до листа — одинаковое количество черных узлов.
void RBT::fixInsert(Node*z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) { // Праотец красный
                y->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else { // Праотец черный
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                z->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

// Обычная вставка в RBT
bool RBT::insert(int value) {
    Node* z = new Node(value);
    z->left = listnode;
    z->right = listnode;
    z->parent = listnode;

    Node* y = listnode;  // родитель потенциального места вставки.
    Node* x = root;

    while (x != listnode) {
        y = x; // Запоминаем текущий узел как потенциального родителя
        if (z->data < x->data) { // Если значение меньше — идем налево
            x = x->left;
        } else if (z->data > x->data) { // Если больше — направо.
            x = x->right;
        } else {
            // Дубликат не допускается
            delete z;
            return false;
        }
    }

    z->parent = y; // Указываем родителя нового узла
    if (x == listnode) {
        root = z; // Дерево пустое, новый узел — корень
    } else if (z->data < y->data) {
        y->left = z; // Вставляем слева
    } else {
        y->right = z; // Вставляем справа
    }

    fixInsert(z); // Выполняем балансировку

    return true;
}

// После удаления узла появляется "двойная чернота", которая нарушает баланс.
// Цель — устранить двойную черноту
void RBT::fixRemove(Node* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* y = x->parent->right;

            // Проверка цвета брата
            if (y->color == RED) { 
                y->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                y = x->parent->right;
            }

            // Проверяем детей брата
            if (y->left->color == BLACK && y->right->color == BLACK) {
                y->color = RED; // Перекрашиваем с Черного в Красный
                x = x->parent; // Двигаемся вверх
            } else {
                // Проверим правого ребенка брата
                if (y->right->color == BLACK) { 
                    y->left->color = BLACK;
                    y->color = RED;
                    rightRotate(y);
                    y = x->parent->right;
                }

                y->color = x->parent->color;
                x->parent->color = BLACK;
                y->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            // Симметричный случай
            Node* w = x->parent->left;
            
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

bool RBT::remove(int value) {
    Node* z = root;
    // Находим узел для удаления
    while (z != listnode) {
        if (value == z->data) {
            break;
        } else if (value < z->data) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (z == listnode) return false; // Узел не найден

    // когда удаляемый узел имеет двух детей, y — его преемник
    Node* y = z; // узел, который будет фактически удалён
    Node* x;
    Color y_original_color = y->color;

    // Если у z нет левого ребенка
    if (z->left == listnode) {
        x = z->right; // x — правый ребенок z
        transplant(z, z->right);
    } else if (z->right == listnode) {
    // Если у z нет правого ребенка
        x = z->left; // x — левый ребенок z
        transplant(z, z->left);
    } else {
        // Если у z оба ребенка (сложна)
        y = minimum(z->right); // y — минимальный в правом поддереве z, который заменит удаляемый
        y_original_color = y->color;
        x = y->right;
        
        // Если y — прямой потомок z
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right); // удаляем y из его текущего места
            y->right = z->right;
            y->right->parent = y;
        }
        
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color; // Цвет y копирует цвет удаляемого z
    }
    
    delete z;
  
    // Если удаленный узел был черным
    if (y_original_color == BLACK) {
        fixRemove(x); // восстановим свойства RB-дерева
    }
    
    return true;
}

void RBT::transplant(Node* u, Node* v) {
    if (u->parent == listnode) {
        // 1. Если у узла u родитель - listnode, значит u - корень дерева
        root = v; // делаем v новым корнем
    } else if (u == u->parent->left) {
        // 2. Если u - левый потомок своего родителя
        u->parent->left = v; // заменяем у родителя левый указатель на v
    } else {
        // 3. Если u - правый потомок
        u->parent->right = v; // заменяем у родителя правый указатель на v
    }
    
    // 4. Обновляем указатель у v, чтобы его родитель был u->parent
    v->parent = u->parent;
}

// Нахождение минимального узла
Node* RBT::minimum(Node* node) {
    while (node->left != listnode) {
        node = node->left;
    }
    return node;
}

// Нахождение максимального узла  
Node* RBT::maximum(Node* node) {
    while (node->right != listnode) {
        node = node->right;
    }
    return node;
}
