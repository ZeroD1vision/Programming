/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Windows Console Application                                 *
 * Project name  : Pt_1                                                        *
 * File name     : lib.h                                                       *
 * Language      : CPP                                                         *
 * Programmers   : Нарзиев Артемий Тимурович                                   *
 * Modified By   :                                                             *
 * Created       : 13.09.2025                                                  *
 * Last Revision : 26.09.2025                                                  *
 * Comment       : Библиотека для работы с фигурами - фонарик                  *
 ******************************************************************************/

 #include <iostream>
#include <windows.h>
#include "lib/lib.h"

using namespace std;

// Макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

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

    Flashlight flashlight(x0, y0, 40, 120, 60, 80);
    flashlight.Show();

    // cout << "All figures are shown!" << endl;

    // cout << "To drag use arrows, to exit - ESC" << endl;
    // cout << "Choose the figure to drag:" << endl;
    // cout << "1 - Point" << endl;
    // cout << "2 - Circle" << endl;
    // cout << "3 - Rectangle" << endl;
    // cout << "4 - Flashlight" << endl;
    
    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "1 - Circle" << endl;
    cout << "2 - Flashlight" << endl;

    // int choice;
    // cin >> choice;

    // switch (choice)
    // {
    // case 1:
    //     point.Drag(5);
    //     break;
    // case 2:
    //     circle.Drag(5);
    //     break;
    // case 3:
    //     rect.Drag(5);
    //     break;
    // case 4:
    //     flashlight.Drag(5);
    //     break;
    // default:
    //     cout << "Incorrect choice" << endl;
    // }

    //                   РЕАЛИЗАЦИЯ ДВИЖЕНИЯ                 //

    int FigX; // Координата х фигуры
    int FigY; // Координата у фигуры
    int choice = 1; // Нажатая кнопка
    int Step = 5; // Шаг для перемещения
    
    // Цикл для выбора и перемещения фигур
    while (true)
    {
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        // Если выбрана 1
        if (KEY_DOWN(49)) { 
            choice = 1;
            FigX = circle.GetX(); // Берем х циркуля
            FigY = circle.GetY(); // Берем у циркуля
        }

        // Если выбрана 2
        if (KEY_DOWN(50)) { 
            choice = 2;
            FigX = flashlight.GetX(); // Берем х циркуля
            FigY = flashlight.GetY(); // Берем у циркуля
        }

        if (KEY_DOWN(51)) { 
            choice = 3;
            FigX = point.GetX(); // Берем х циркуля
            FigY = point.GetY(); // Берем у циркуля
        }

        switch (choice) {   
            case (1):
                // Стрелка влево
                if (KEY_DOWN(VK_LEFT))
                {
                    FigX = FigX - Step;
                    circle.MoveTo(FigX, FigY);
                    Sleep(100);
                }

                // Стрелка вправо
                if (KEY_DOWN(VK_RIGHT))
                {
                    FigX = FigX + Step;
                    circle.MoveTo(FigX, FigY);
                    Sleep(100);
                }

                // Стрелка вверх
                if (KEY_DOWN(VK_UP))
                {
                    FigY = FigY - Step;
                    circle.MoveTo(FigX, FigY);
                    Sleep(100);
                }

                // Стрелка вниз
                if (KEY_DOWN(VK_DOWN))
                {
                    FigY = FigY + Step;
                    circle.MoveTo(FigX, FigY);
                    Sleep(100);
                }
                break;

            case(2):
                // Стрелка влево
                if (KEY_DOWN(VK_LEFT))
                {
                    FigX = FigX - Step;
                    flashlight.MoveTo(FigX, FigY);
                    Sleep(100);
                }

                // Стрелка вправо
                if (KEY_DOWN(VK_RIGHT))
                {
                    FigX = FigX + Step;
                    flashlight.MoveTo(FigX, FigY);
                    Sleep(100);
                }

                // Стрелка вверх
                if (KEY_DOWN(VK_UP))
                {
                    FigY = FigY - Step;
                    flashlight.MoveTo(FigX, FigY);
                    Sleep(100);
                }

                // Стрелка вниз
                if (KEY_DOWN(VK_DOWN))
                {
                    FigY = FigY + Step;
                    flashlight.MoveTo(FigX, FigY);
                    Sleep(100);
                }

            case (3):
                // Стрелка влево
                if (KEY_DOWN(VK_LEFT))
                {
                    FigX = FigX - Step;
                    point.MoveTo(FigX, FigY);
                    Sleep(100);
                }
                // Стрелка вправо
                if (KEY_DOWN(VK_RIGHT))
                {
                    FigX = FigX + Step;
                    point.MoveTo(FigX, FigY);
                    Sleep(100);
                }
                // Стрелка вверх
                if (KEY_DOWN(VK_UP))
                {
                    FigY = FigY - Step;
                    point.MoveTo(FigX, FigY);
                    Sleep(100);
                }
                // Стрелка вниз
                if (KEY_DOWN(VK_DOWN))
                {
                    FigY = FigY + Step;
                    point.MoveTo(FigX, FigY);
                    Sleep(100);
                }
                break;

            default:
                break;
        }
    }

        // if (KEY_DOWN(VK_LEFT))
        // {
        //     FigX = FigX - Step;
        //     MoveTo(FigX, FigY);
        //     Sleep(100);
        // }

        // if (KEY_DOWN(VK_RIGHT))
        // {
        //     FigX = FigX + Step;
        //     MoveTo(FigX, FigY);
        //     Sleep(100);
        // }

        // if (KEY_DOWN(VK_UP))
        // {
        //     FigY = FigY - Step;
        //     MoveTo(FigX, FigY);
        //     Sleep(100);
        // }

        // if (KEY_DOWN(VK_DOWN))
        // {
        //     FigY = FigY + Step;
        //     MoveTo(FigX, FigY);
        //     Sleep(100);
        // }

    // Очистка
    cout << "Hiding everything..." << endl;
    point.Hide();
    circle.Hide();
    flashlight.Hide();

    ReleaseDC(hwnd, hdc);
    cout << "Programm ended with code 0." << endl;
    return 0;
}