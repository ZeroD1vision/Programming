#include "lib.h"
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

/**
 * @brief Конструктор красно-черного дерева
 * @details Инициализирует пустое дерево с листовым узлом
 * Листовой узел всегда черный и указывает сам на себя
 */
RBT::RBT() {
    listnode = new Node(0);
    listnode->color = BLACK;

    listnode->left = listnode;
    listnode->right = listnode;
    listnode->parent = listnode;
    root = listnode;
}

/**
 * @brief Деструктор красно-черного дерева
 * @details Рекурсивно удаляет все узлы дерева и освобождает память
 */
RBT::~RBT() {
    deleteTree(root);
    delete listnode;
}

/**
 * @brief Рекурсивное удаление поддерева
 * @param node Корень поддерева для удаления
 * @details Выполняет пост-ордер обход для удаления всех узлов
 */
void RBT::deleteTree(Node* node) {
    if (node != listnode) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

/**
 * @brief Поиск элемента в дереве
 * @param value Значение для поиска
 * @return Указатель на найденный узел или nullptr если не найден
 * @details Выполняет бинарный поиск по дереву
 */
Node* RBT::search(int value) {
    Node* current = root;
    while (current != listnode) {
        if (value == current->data) {
            return current;  // Возвращаем указатель на найденный узел
        } else if (value < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;  // Возвращаем nullptr если элемент не найден
}

// ==================== РОТАЦИИ ДЕРЕВА ====================

// 1. B становится на место A (тоже его родителем).
// 2. A становится левым ребенком B.
// 3. Если у B был левый ребенок, он становится правым ребенком A.
// 4. Обновляем указатель у родителя A, чтобы он теперь указывал на B.
// !. Правый ребенок B не меняется, он остается правым ребенком B.
/**
 * @brief Левая ротация вокруг узла x
 * @param x Узел, вокруг которого выполняется ротация
 * @details 
 * 1. Правый ребенок y становится на место x
 * 2. x становится левым ребенком y  
 * 3. Левый ребенок y становится правым ребенком x
 * 4. Обновляются связи родителя
 */
void RBT::leftRotate(Node* x) {
    Node* y = x->right; // устанавливаем y — правого ребенка x
    x->right = y->left; // левый ребенок у становится правым для x

    if (y->left != listnode) { // Если левый для у есть
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

/**
 * @brief Правая ротация вокруг узла y
 * @param y Узел, вокруг которого выполняется ротация
 * @details Симметрична левой ротации
 */
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

/**
 * @brief Балансировка дерева после вставки
 * @param z Вновь вставленный узел (красный)
 * @details Восстанавливает свойства красно-черного дерева:
 * 1. Корень всегда черный
 * 2. Красный узел не может иметь красных детей  
 * 3. Все пути содержат одинаковое количество черных узлов
 */
void RBT::fixInsert(Node*z) {
    // cout << "Fixing: " << z->data << endl;
    // Пока родитель z красный (нарушение свойства 2)
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) { // Если брат отца красный
                                   // parent.color = BLACK;
                                   // uncle.color = BLACK;
                                   // grandparent.color = RED;
                                   // z = grandparent;
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {                // Если брат отца черный
                if (z == z->parent->right) {
                    // z - правый ребенок, нужна левая ротация
                    z = z->parent;
                    leftRotate(z);
                    // cout << "  Rotation complete." << endl;
                }
                // z - левый ребенок, нужна правая ротация
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
                // cout << "  Rotation complete." << endl;
            }
        } else { 
            // Симметричный случай, когда родитель является правым ребенком
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                    // cout << "  Rotation complete." << endl;
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
                // cout << "  Rotation complete." << endl;
            }
        }
    }
    root->color = BLACK;
    //  cout << "Fix complete." << endl; // Debug 6
}

/**
 * @brief Вставка нового элемента в дерево
 * @param value Значение для вставки
 * @return true если вставка успешна, false если элемент уже существует
 * @details Выполняет стандартную вставку в бинарное дерево поиска,
 * затем балансирует дерево с помощью fixInsert()
 */
bool RBT::insert(int value) {
    //  cout << "Inserting: " << value << endl; // Debug 1
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
    if (y == listnode) {
        root = z; // Дерево пустое, новый узел — корень
    } else if (z->data < y->data) {
        y->left = z; // Вставляем слева
    } else {
        y->right = z; // Вставляем справа
    }

    fixInsert(z); // Выполняем балансировку
    // cout << "Insert successful: " << value << endl; // Debug 3

    return true;
}

// После удаления узла появляется двойная чернота, которая нарушает баланс.
// Цель — устранить двойную черноту
/**
 * @brief Балансировка дерева после удаления
 * @param x Узел, с которого начинается балансировка
 * @details Устраняет "двойную черноту" после удаления черного узла
 */
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

/**
 * @brief Удаление элемента из дерева
 * @param value Значение для удаления
 * @return true если удаление успешно, false если элемент не найден
 * @details Использует алгоритм удаления из BST с последующей балансировкой
 */
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
        // у больше z и меньше любого другого узла в правом его поддереве
        // переносим y на место z с помощью transplant, 
        // копируем левое поддерево z в y->left, 
        // правое поддерево z в y->right, и 
        // устанавливаем цвет y как у z.
        // x — правый ребёнок y (после перемещения).
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

/**
 * @brief Замена одного поддерева другим
 * @param u Поддерево, которое заменяем
 * @param v Поддерево, которым заменяем
 * @details Обновляет связи родителя для поддержания структуры дерева
 */
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

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

/**
 * @brief Поиск минимального элемента в поддереве
 * @param node Корень поддерева
 * @return Указатель на минимальный узел
 */
Node* RBT::minimum(Node* node) {
    while (node->left != listnode) {
        node = node->left;
    }
    return node;
}

/**
 * @brief Поиск максимального элемента в поддереве
 * @param node Корень поддерева  
 * @return Указатель на максимальный узел
 */
Node* RBT::maximum(Node* node) {
    while (node->right != listnode) {
        node = node->right;
    }
    return node;
}

/**
 * @brief Поиск минимального элемента во всем дереве
 * @return Значение минимального элемента или -1 если дерево пустое
 */
int RBT::findMin() {
    if (root == listnode) return -1; // Дерево пустое
    return minimum(root)->data;
}

/**
 * @brief Поиск максимального элемента во всем дереве
 * @return Значение максимального элемента или -1 если дерево пустое
 */
int RBT::findMax() {
    if (root == listnode) return -1; // Дерево пустое
    return maximum(root)->data;
}

/**
 * @brief Рекурсивный сбор максимальных элементов
 * @param node Текущий узел
 * @param result Вектор для результатов
 * @param count Счетчик оставшихся элементов
 * @details Выполняет обратный in-order обход (справа-налево)
 */
void RBT::getAllMax(Node* node, vector <int>& result, int& count) {
    if (node == listnode || count <= 0) return;

    getAllMax(node->right, result, count);
    if (count > 0) {
        result.push_back(node->data);
        count--;
    }

    if (count > 0) {
        getAllMax(node->left, result, count);
    }
}

/**
 * @brief Рекурсивный сбор минимальных элементов
 * @param node Текущий узел
 * @param result Вектор для результатов  
 * @param count Счетчик оставшихся элементов
 * @details Выполняет in-order обход (слева-направо)
 */
void RBT::getAllMin(Node* node, vector <int>& result, int& count) {
    if (node == listnode || count <= 0) return;

    getAllMin(node->left, result, count);
    if (count > 0) {
        result.push_back(node->data);
        count--;
    }

    getAllMin(node->right, result, count);
}

/**
 * @brief Получить 10 наибольших элементов
 * @return Вектор с 10 наибольшими элементами
 */
vector <int> RBT::getMax10() {
    vector <int> result;
    int count = 10;
    getAllMax(root, result, count);
    return result;
}

/**
 * @brief Получить 10 наименьших элементов
 * @return Вектор с 10 наименьшими элементами
 */
vector <int> RBT::getMin10() {
    vector <int> result;
    int count = 10;
    getAllMin(root, result, count);
    return result;
}


// ==================== ФУНКЦИИ ВЫВОДА И ВИЗУАЛИЗАЦИИ ====================


// Вспомогательная функция для отладки
// void RBT::printTree() {
//     // Простой вывод для отладки
//     cout << "Tree elements (in-order): ";
//     vector<int> elements;
//     int count = 1000; // Большое число чтобы получить все элементы
//     getAllMax(root, elements, count);
    
//     for (int val : elements) {
//         cout << val << " ";
//     }
//     cout << endl;
// }

/**
 * @brief Вывод всех элементов дерева в порядке возрастания
 * @details Использует in-order обход для вывода отсортированных значений
 */
void RBT::printTree() {
    cout << "Tree elements (in-order): ";
    vector<int> elements;
    
    // КОРРЕКЦИЯ: ИСПОЛЬЗУЕМ ПРАВИЛЬНЫЙ ОБХОД
    inOrderTraversal(root, elements); 
    
    for (int val : elements) {
        cout << val << " ";
    }
    cout << endl;
}

/**
 * @brief In-order обход дерева
 * @param node Текущий узел
 * @param elements Вектор для сбора элементов
 * @details Рекурсивно обходит дерево в порядке: левое поддерево -> узел -> правое поддерево
 */
void RBT::inOrderTraversal(Node* node, vector<int>& elements) {
    if (node == listnode) return;

    // cout << "Traversing node: " << node->data << endl; // Debug A

    // ПРОВЕРКА ЛЕВОГО РЕБЕНКА
    if (node->left != listnode && node->left != nullptr) {
        inOrderTraversal(node->left, elements);
    } else if (node->left == nullptr) {
        cout << "FATAL: node->left is NULL at node " << node->data << endl; 
        // Это должно было быть listnode!
    }

    elements.push_back(node->data);

    // ПРОВЕРКА ПРАВОГО РЕБЕНКА
    if (node->right != listnode && node->right != nullptr) {
        inOrderTraversal(node->right, elements);
    } else if (node->right == nullptr) {
        cout << "FATAL: node->right is NULL at node " << node->data << endl; 
        // Это должно было быть listnode!
    }
}

/**
 * @brief Генерация DOT-представления поддерева
 * @param node Корень поддерева
 * @param ss Поток для записи DOT-кода
 * @param nullCount Счетчик для null-узлов
 * @details Рекурсивно генерирует описание графа в формате Graphviz DOT
 */
void RBT::generateDot(Node* node, std::stringstream& ss, int& nullCount) {
    if (node == listnode) return;
    
    // Создаем узел
    ss << "  " << node->data << " [label=\"" << node->data << "\", ";
    ss << "fillcolor=" << (node->color == RED ? "red" : "black") << ", ";
    ss << "fontcolor=white, style=filled];\n";
    
    // Левая связь
    if (node->left != listnode) {
        ss << "  " << node->data << " -> " << node->left->data << ";\n";
        generateDot(node->left, ss, nullCount);
    } else {
        ss << "  null" << nullCount << " [shape=point];\n";
        ss << "  " << node->data << " -> null" << nullCount << ";\n";
        nullCount++;
    }
    
    // Правая связь
    if (node->right != listnode) {
        ss << "  " << node->data << " -> " << node->right->data << ";\n";
        generateDot(node->right, ss, nullCount);
    } else {
        ss << "  null" << nullCount << " [shape=point];\n";
        ss << "  " << node->data << " -> null" << nullCount << ";\n";
        nullCount++;
    }
}

/**
 * @brief Визуализация дерева в формате DOT
 * @param filename Имя файла для сохранения
 * @details Создает файл в формате Graphviz DOT для визуализации дерева
 */
void RBT::visualize(const std::string& filename) {
    std::stringstream ss;
    ss << "digraph RedBlackTree {\n";
    ss << "  node [fontname=\"Arial\", fontsize=12, fontcolor=white];\n";
    ss << "  edge [fontname=\"Arial\", fontsize=10];\n";
    
    int nullCount = 0;
    if (root != listnode) {
        generateDot(root, ss, nullCount);
    } else {
        ss << "  empty [label=\"Empty Tree\", shape=plaintext];\n";
    }
    
    ss << "}\n";
    
    std::ofstream file(filename);
    file << ss.str();
    file.close();
    
    std::cout << "Tree saved to " << filename << std::endl;
    std::cout << "To generate image run: dot -Tpng " << filename << " -o tree.png" << std::endl;
}

/**
 * @brief Генерация DOT-строки для дерева
 * @return Строка с описанием дерева в формате DOT
 * @details Альтернатива visualize() для веб-визуализации или программной обработки
 */
std::string RBT::generateDotString() {
    std::stringstream ss;
    ss << "digraph RedBlackTree {\n";
    ss << "  node [fontname=\"Arial\", fontsize=12, fontcolor=white];\n";
    ss << "  edge [fontname=\"Arial\", fontsize=10];\n";
    
    int nullCount = 0;
    if (root != listnode) {
        generateDot(root, ss, nullCount);
    } else {
        ss << "  empty [label=\"Empty Tree\", shape=plaintext];\n";
    }
    
    ss << "}\n";
    return ss.str();
}