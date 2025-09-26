#include <iostream>
#include <windows.h>
#include "lib/lib.h"

using namespace std;

// Глобальная переменная контекста устройства
HDC hdc;

/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/
int main()
{
    // Стартовые координаты
    int x0 = 200;
    int y0 = 300;

    // Получим дескриптор консольного окна
    HWND hwnd = GetConsoleWindow();

    if (hwnd == NULL)
    {
        cout << "Console Window Descriptor NOT FOUND !\n";
        return 1;
    }

    // Получим контекст устройства для консольного окна
    hdc = GetWindowDC(hwnd);

    if (hdc == 0)
    {
        cout << "Handle Device Context NOT FOUND !\n";
        return 2;
    }

    // Демонстрация работы с фигурами
    Point point(50, 50);
    point.Show();

    Circle circle(150, 150, 30);
    circle.Show();

    MyRectangle rect(250, 100, 80, 60);
    rect.Show();

    Flashlight flashlight(x0, y0, 40, 120, 60, 80);
    flashlight.Show();

    cout << "Все фигуры отображены!" << endl;

    cout << "Для перетаскивания фигур используйте стрелки, для выхода - ESC" << endl;
    cout << "Выберите фигуру для перетаскивания:" << endl;
    cout << "1 - Точка" << endl;
    cout << "2 - Круг" << endl;
    cout << "3 - Прямоугольник" << endl;
    cout << "5 - Фонарик" << endl;

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        point.Drag(5);
        break;
    case 2:
        circle.Drag(5);
        break;
    case 3:
        rect.Drag(5);
        break;
    case 5:
        flashlight.Drag(5);
        break;
    default:
        cout << "Неверный выбор" << endl;
    }

    // Очистка
    cout << "Очистка экрана..." << endl;
    point.Hide();
    circle.Hide();
    rect.Hide();
    flashlight.Hide();

    ReleaseDC(hwnd, hdc);
    cout << "Программа завершена." << endl;
    return 0;
}