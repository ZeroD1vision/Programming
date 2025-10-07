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
#include "Header.h"
#include "PZ01.h"

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
    // Границы "стен"
    const int WALL_LEFT = 50;
    const int WALL_TOP = 50;
    const int WALL_RIGHT = 750;
    const int WALL_BOTTOM = 550;

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

    // Рисуем стены
    HPEN wallPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, wallPen);
    Rectangle(hdc, WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM);
    DeleteObject(wallPen);

    // Создаем фигуры
    Point point(100, 100);
    point.Show();

    Circle circle(200, 200, 30);
    circle.Show();

    // Создаем два разных фонарика
    RectFlashlight rectFlashlight(300, 300, 40, 120, 60, 80);
    rectFlashlight.Show();

    RoundFlashlight roundFlashlight(500, 300, 50, 100, 70, 60);
    roundFlashlight.Show();

    // Саздаем и прячем сразу сломанные фонарики
    BrokenFlashlight brokenRect(300, 300, 40, 120, 60, 80, 0);
    brokenRect.Hide();
    BrokenFlashlight brokenRound(500, 300, 50, 100, 70, 60, 0);
    brokenRound.Hide();

    // Отверткa
    Screwdriver screwdriver(400, 400, 60, 15);
    screwdriver.Show();

    // Флаги для отслеживания состояния фонариков
    bool rectIsBroken = false;
    bool roundIsBroken = false;

    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "1 - Circle" << endl;
    cout << "2 - RectFlashlight" << endl;
    cout << "3 - Point" << endl;
    cout << "4 - RoundFlashlight" << endl;
    cout << "5 - Screwdriver (repair tool)" << endl;
    cout << "Walls: (" << WALL_LEFT << "," << WALL_TOP << ") to ("
         << WALL_RIGHT << "," << WALL_BOTTOM << ")" << endl;
    cout << "Use screwdriver to repair broken flashlights!" << endl;

    //                   РЕАЛИЗАЦИЯ ДВИЖЕНИЯ                 //

    int FigX = 0; // Координата х фигуры
    int FigY = 0; // Координата у фигуры
    int choice = 1; // Нажатая кнопка
    int Step = 5; // Шаг для перемещения
    
    // Цикл для выбора и перемещения фигур
    while (true)
    {
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        // Круг
        if (KEY_DOWN(49)) { 
            choice = 1;
            FigX = circle.GetX(); // Берем х циркуля
            FigY = circle.GetY(); // Берем у циркуля
        }

        // Прямоугольный фонарик
        if (KEY_DOWN(50)) { 
            choice = 2;
            FigX = rectFlashlight.GetX();
            FigY = rectFlashlight.GetY();
        }
        
        // Точка
        if (KEY_DOWN(51)) { 
            choice = 3;
            FigX = point.GetX(); // Берем х циркуля
            FigY = point.GetY(); // Берем у циркуля
        }

        // Круглый фонарик
        if (KEY_DOWN(52)) {
            choice = 4;
            FigX = roundFlashlight.GetX();
            FigY = roundFlashlight.GetY();
        }

        // Отвертка
        if (KEY_DOWN(53)) {
            choice = 5;
            FigX = screwdriver.GetX();
            FigY = screwdriver.GetY();
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

            case (2):
                // Стрелка влево
                if (KEY_DOWN(VK_LEFT))
                {
                    FigX = FigX - Step;
                    if (!rectIsBroken) {
                        // Двигаем целый фонарик
                        rectFlashlight.MoveTo(FigX, FigY);
                        if (rectFlashlight.CheckWallCollision(WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM)) {
                            cout << "RectFlashlight damage: " << rectFlashlight.GetDamage() << endl;
                            if (rectFlashlight.IsBroken()) {
                                cout << "RectFlashlight broken!" << endl;
                                rectIsBroken = true;
                                // Скрываем целый и показываем разбитый
                                rectFlashlight.Hide();
                                brokenRect.MoveTo(FigX, FigY);
                                brokenRect.Show();
                            }
                        }
                    }
                    else {
                        // Двигаем разбитый фонарик
                        brokenRect.MoveTo(FigX, FigY);
                    }
                    Sleep(100);
                }

                // Стрелка вправо
                if (KEY_DOWN(VK_RIGHT)) {
                    FigX += Step;
                    if (!rectIsBroken) {
                        // Двигаем целый фонарик
                        rectFlashlight.MoveTo(FigX, FigY);
                        if (rectFlashlight.CheckWallCollision(WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM)) {
                            cout << "RectFlashlight damage: " << rectFlashlight.GetDamage() << endl;
                            if (rectFlashlight.IsBroken()) {
                                cout << "RectFlashlight broken!" << endl;
                                rectIsBroken = true;
                                // Скрываем целый и показываем разбитый
                                rectFlashlight.Hide();
                                brokenRect.MoveTo(FigX, FigY);
                                brokenRect.Show();
                            }
                        }
                    }
                    else {
                        // Двигаем разбитый фонарик
                        brokenRect.MoveTo(FigX, FigY);
                    }
                    Sleep(100);
                }

                // Стрелка вверх
                if (KEY_DOWN(VK_UP)) {
                    FigY -= Step;
                    if (!rectIsBroken) {
                        // Двигаем целый фонарик
                        rectFlashlight.MoveTo(FigX, FigY);
                        if (rectFlashlight.CheckWallCollision(WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM)) {
                            cout << "RectFlashlight damage: " << rectFlashlight.GetDamage() << endl;
                            if (rectFlashlight.IsBroken()) {
                                cout << "RectFlashlight broken!" << endl;
                                rectIsBroken = true;
                                // Скрываем целый и показываем разбитый
                                rectFlashlight.Hide();
                                brokenRect.MoveTo(FigX, FigY);
                                brokenRect.Show();
                            }
                        }
                    }
                    else {
                        // Двигаем разбитый фонарик
                        brokenRect.MoveTo(FigX, FigY);
                    }
                    Sleep(100);
                }

                // Стрелка вниз
                if (KEY_DOWN(VK_DOWN)) {
                    FigY += Step;
                    if (!rectIsBroken) {
                        // Двигаем целый фонарик
                        rectFlashlight.MoveTo(FigX, FigY);
                        if (rectFlashlight.CheckWallCollision(WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM)) {
                            cout << "RectFlashlight damage: " << rectFlashlight.GetDamage() << endl;
                            if (rectFlashlight.IsBroken()) {
                                cout << "RectFlashlight broken!" << endl;
                                rectIsBroken = true;
                                // Скрываем целый и показываем разбитый
                                rectFlashlight.Hide();
                                // BrokenFlashlight brokenRect(FigX, FigY, 40, 120, 60, 80);
                                brokenRect.MoveTo(FigX, FigY);
                                brokenRect.Show();
                            }
                        }
                    }
                    else {
                        // Двигаем разбитый фонарик
                        //BrokenFlashlight brokenRect(FigX, FigY, 40, 120, 60, 80);
                        //brokenRect.Show();
                        brokenRect.MoveTo(FigX, FigY);
                    }
                    Sleep(100);
                }
                break;

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

            // Для круглого
            case 4:
                if (KEY_DOWN(VK_LEFT)) {
                    FigX -= Step;
                    if (!roundIsBroken) {
                        // Двигаем целый фонарик
                        roundFlashlight.MoveTo(FigX, FigY);
                        if (roundFlashlight.CheckWallCollision(WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM)) {
                            cout << "RoundFlashlight damage: " << roundFlashlight.GetDamage() << endl;
                            if (roundFlashlight.IsBroken()) {
                                cout << "RoundFlashlight broken!" << endl;
                                roundIsBroken = true;
                                // Скрываем целый и показываем разбитый
                                roundFlashlight.Hide();
                                brokenRound.MoveTo(FigX, FigY);
                                brokenRound.Show();
                            }
                        }
                    }
                    else {
                        // Двигаем разбитый фонарик
                        brokenRound.MoveTo(FigX, FigY);
                    }
                    Sleep(100);
                }

                if (KEY_DOWN(VK_RIGHT)) {
                    FigX += Step;
                    if (!roundIsBroken) {
                        // Двигаем целый фонарик
                        roundFlashlight.MoveTo(FigX, FigY);
                        if (roundFlashlight.CheckWallCollision(WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM)) {
                            cout << "RoundFlashlight damage: " << roundFlashlight.GetDamage() << endl;
                            if (roundFlashlight.IsBroken()) {
                                cout << "RoundFlashlight broken!" << endl;
                                roundIsBroken = true;
                                // Скрываем целый и показываем разбитый
                                roundFlashlight.Hide();
                                brokenRound.MoveTo(FigX, FigY);
                                brokenRound.Show();
                            }
                        }
                    }
                    else {
                        // Двигаем разбитый фонарик
                        brokenRound.MoveTo(FigX, FigY);
                    }
                    Sleep(100);
                }

                if (KEY_DOWN(VK_UP)) {
                    FigY -= Step;
                    if (!roundIsBroken) {
                        // Двигаем целый фонарик
                        roundFlashlight.MoveTo(FigX, FigY);
                        if (roundFlashlight.CheckWallCollision(WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM)) {
                            cout << "RoundFlashlight damage: " << roundFlashlight.GetDamage() << endl;
                            if (roundFlashlight.IsBroken()) {
                                cout << "RoundFlashlight broken!" << endl;
                                roundIsBroken = true;
                                // Скрываем целый и показываем разбитый
                                roundFlashlight.Hide();
                                brokenRound.MoveTo(FigX, FigY);
                                brokenRound.Show();
                            }
                        }
                    }
                    else {
                        // Двигаем разбитый фонарик
                        brokenRound.MoveTo(FigX, FigY);
                    }
                    Sleep(100);
                }
                if (KEY_DOWN(VK_DOWN)) {
                    FigY += Step;
                    if (!roundIsBroken) {
                        // Двигаем целый фонарик
                        roundFlashlight.MoveTo(FigX, FigY);
                        if (roundFlashlight.CheckWallCollision(WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM)) {
                            cout << "RoundFlashlight damage: " << roundFlashlight.GetDamage() << endl;
                            if (roundFlashlight.IsBroken()) {
                                cout << "RoundFlashlight broken!" << endl;
                                roundIsBroken = true;
                                // Скрываем целый и показываем разбитый
                                roundFlashlight.Hide();
                                //BrokenFlashlight brokenRound(FigX, FigY, 50, 100, 70, 60);
                                brokenRound.MoveTo(FigX, FigY);
                                brokenRound.Show();
                            }
                        }
                    }
                    else {
                        // Двигаем разбитый фонарик
                        //BrokenFlashlight brokenRound(FigX, FigY, 50, 100, 70, 60);
                        //brokenRound.Show();
                        brokenRound.MoveTo(FigX, FigY);
                    }
                    Sleep(100);
                }
                break;

            case 5:
                if (KEY_DOWN(VK_LEFT)) {
                    FigX -= Step;
                    screwdriver.MoveTo(FigX, FigY);
                    // Проверяем столкновение с фонариками для починки
                    // Проверка починки прямоугольного фонарика
                    if (rectIsBroken) {
                        // Создаем временный разбитый фонарик для проверки столкновения
                        BrokenFlashlight tempRect(rectFlashlight.GetX(), rectFlashlight.GetY(), 40, 120, 60, 80, 0);
                        if (tempRect.CheckScrewdriverCollision(&screwdriver)) {
                            cout << "Repairing RectFlashlight!" << endl;
                            rectIsBroken = false;
                            // Скрываем разбитый и показываем целый
                            tempRect.Hide();
                            rectFlashlight.SetDamage(0);
                            rectFlashlight.Repair();
                            rectFlashlight.MoveTo(brokenRect.GetX(), brokenRect.GetY());
                            rectFlashlight.Show();
                        }
                    }

                    // Проверка починки круглого фонарика
                    if (roundIsBroken) {
                        BrokenFlashlight tempRound(roundFlashlight.GetX(), roundFlashlight.GetY(), 50, 100, 70, 60, 1);
                        if (tempRound.CheckScrewdriverCollision(&screwdriver)) {
                            cout << "Repairing RoundFlashlight!" << endl;
                            roundIsBroken = false;
                            tempRound.Hide();
                            roundFlashlight.SetDamage(0);
                            roundFlashlight.Repair();
                            roundFlashlight.MoveTo(brokenRound.GetX(), brokenRound.GetY());
                            roundFlashlight.Show();
                        }
                    }
                    Sleep(100);
                }
                if (KEY_DOWN(VK_RIGHT)) {
                    FigX += Step;
                    screwdriver.MoveTo(FigX, FigY);
                    // Проверяем столкновение с фонариками для починки
                    // Проверка починки прямоугольного фонарика
                    if (rectIsBroken) {
                        // Создаем временный разбитый фонарик для проверки столкновения
                        BrokenFlashlight tempRect(rectFlashlight.GetX(), rectFlashlight.GetY(), 40, 120, 60, 80, 0);
                        if (tempRect.CheckScrewdriverCollision(&screwdriver)) {
                            cout << "Repairing RectFlashlight!" << endl;
                            rectIsBroken = false;
                            // Скрываем разбитый и показываем целый
                            tempRect.Hide();
                            rectFlashlight.SetDamage(0);
                            rectFlashlight.Repair();
                            rectFlashlight.MoveTo(brokenRect.GetX(), brokenRect.GetY());
                            rectFlashlight.Show();
                        }
                    }

                    // Проверка починки круглого фонарика
                    if (roundIsBroken) {
                        BrokenFlashlight tempRound(roundFlashlight.GetX(), roundFlashlight.GetY(), 50, 100, 70, 60, 1);
                        if (tempRound.CheckScrewdriverCollision(&screwdriver)) {
                            cout << "Repairing RoundFlashlight!" << endl;
                            roundIsBroken = false;
                            tempRound.Hide();
                            roundFlashlight.SetDamage(0);
                            roundFlashlight.Repair();
                            roundFlashlight.MoveTo(brokenRound.GetX(), brokenRound.GetY());
                            roundFlashlight.Show();
                        }
                    }
                    Sleep(100);
                }
                if (KEY_DOWN(VK_UP)) {
                    FigY -= Step;
                    screwdriver.MoveTo(FigX, FigY);
                    // Проверяем столкновение с фонариками для починки
                    // Проверка починки прямоугольного фонарика
                    if (rectIsBroken) {
                        // Создаем временный разбитый фонарик для проверки столкновения
                        BrokenFlashlight tempRect(rectFlashlight.GetX(), rectFlashlight.GetY(), 40, 120, 60, 80, 0);
                        if (tempRect.CheckScrewdriverCollision(&screwdriver)) {
                            cout << "Repairing RectFlashlight!" << endl;
                            rectIsBroken = false;
                            // Скрываем разбитый и показываем целый
                            tempRect.Hide();
                            rectFlashlight.SetDamage(0);
                            rectFlashlight.Repair();
                            rectFlashlight.MoveTo(brokenRect.GetX(), brokenRect.GetY());
                            rectFlashlight.Show();
                        }
                    }

                    // Проверка починки круглого фонарика
                    if (roundIsBroken) {
                        BrokenFlashlight tempRound(roundFlashlight.GetX(), roundFlashlight.GetY(), 50, 100, 70, 60, 1);
                        if (tempRound.CheckScrewdriverCollision(&screwdriver)) {
                            cout << "Repairing RoundFlashlight!" << endl;
                            roundIsBroken = false;
                            tempRound.Hide();
                            roundFlashlight.SetDamage(0);
                            roundFlashlight.Repair();
                            roundFlashlight.MoveTo(brokenRound.GetX(), brokenRound.GetY());
                            roundFlashlight.Show();
                        }
                    }
                    Sleep(100);
                }
                if (KEY_DOWN(VK_DOWN)) {
                    FigY += Step;
                    screwdriver.MoveTo(FigX, FigY);
                    // Проверяем столкновение с фонариками для починки
                    // Проверка починки прямоугольного фонарика
                    if (rectIsBroken) {
                        // Создаем временный разбитый фонарик для проверки столкновения
                        BrokenFlashlight tempRect(rectFlashlight.GetX(), rectFlashlight.GetY(), 40, 120, 60, 80, 0);
                        if (tempRect.CheckScrewdriverCollision(&screwdriver)) {
                            cout << "Repairing RectFlashlight!" << endl;
                            rectIsBroken = false;
                            // Скрываем разбитый и показываем целый
                            tempRect.Hide();
                            rectFlashlight.SetDamage(0);
                            rectFlashlight.Repair();
                            rectFlashlight.MoveTo(brokenRect.GetX(), brokenRect.GetY());
                            rectFlashlight.Show();
                        }
                    }

                    // Проверка починки круглого фонарика
                    if (roundIsBroken) {
                        BrokenFlashlight tempRound(roundFlashlight.GetX(), roundFlashlight.GetY(), 50, 100, 70, 60, 1);
                        if (tempRound.CheckScrewdriverCollision(&screwdriver)) {
                            cout << "Repairing RoundFlashlight!" << endl;
                            roundIsBroken = false;
                            tempRound.Hide();
                            roundFlashlight.SetDamage(0);
                            roundFlashlight.Repair();
                            roundFlashlight.MoveTo(brokenRound.GetX(), brokenRound.GetY());
                            roundFlashlight.Show();
                        }
                    }
                    Sleep(100);
                }
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
    rectFlashlight.Hide();
    roundFlashlight.Hide();
    screwdriver.Hide();

    if (!rectIsBroken) rectFlashlight.Hide();
    if (!roundIsBroken) roundFlashlight.Hide();

    ReleaseDC(hwnd, hdc);
    cout << "Programm ended with code 0." << endl;
    return 0;
}