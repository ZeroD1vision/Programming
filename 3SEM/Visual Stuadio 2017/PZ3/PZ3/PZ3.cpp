/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Windows Console Application                                 *
 * Project name  : Pt_1                                                        *
 * File name     : main.cpp                                                    *
 * Language      : CPP                                                         *
 * Programmers   : Нарзиев Артемий Тимурович                                   *
 * Modified By   :                                                             *
 * Created       : 13.09.2025                                                  *
 * Last Revision : 24.10.2025                                                  *
 * Comment       : ЛР №3. Фигура: фонарик                                      *
 ******************************************************************************/

#include <windows.h>
#include <iostream>
using namespace std;

#include "Header.h"

// Макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*Г Л О Б А Л Ь Н Ы Е  К О Н С Т А Н Т Ы  И  П Е Р Е М Е Н Н Ы Е*/
/****************************************************************/

HDC hdc;    // Объявим контекст устройства

bool IsCollision(BaseFlashlight& flashlight, Conflict& conflict) 
{
    int x = flashlight.GetX();
    int y = flashlight.GetY();
    int width = flashlight.GetBodyWidth();
    int height = flashlight.GetBodyHeight();

    // Проверка столкновения по прямоугольникам
    if (x < conflict.GetX() + conflict.GetWidth() &&
        x + width > conflict.GetX() &&
        y < conflict.GetY() + conflict.GetHeight() &&
        y + height > conflict.GetY())
    {
        return true;
    }
    return false;
}
/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/

// ф-я для отработки нажатий стрелочек
bool HandleMovement(int& x, int& y, int step)
{
    int prev_x = x;
    int prev_y = y;

    // влево
    if (KEY_DOWN(VK_LEFT))
    {
        x = x - step;
    }
    // вправо
    if (KEY_DOWN(VK_RIGHT))
    {
        x = x + step;
    }
    // вверх
    if (KEY_DOWN(VK_UP))
    {
        y = y - step;
    }
    // вниз
    if (KEY_DOWN(VK_DOWN))
    {
        y = y + step;
    }

    return (prev_x != x) || (prev_y != y);
}

int main()
{
    //===============================================================
    //  Н А Ч А Л О  М О Л И Т В А
    HWND hwnd = GetConsoleWindow();
    if (hwnd == NULL)
    {
        cout << "Console Window Descriptor NOT FOUND !\n";
        return 1;
    }

    hdc = GetWindowDC(hwnd);
    if (hdc == 0)
    {
        cout << "Handle Device Context NOT FOUND !\n";
        return 2;
    }
    // К О Н Е Ц  М О Л И Т В А 
    //===============================================================

    //===============================================================
    // Н А Ч А Л О  К О Н С Т А Н Т Ы

    const int CIRCLE = 1;
    const int RECT_FLASHLIGHT = 2;
    const int POINT = 3;
    const int ROUND_FLASHLIGHT = 4;
    const int SCREWDRIVER = 5;

    int STEP = 5;
    const int DELAY = 50;

    //  К О Н Е Ц  К О Н С Т А Н Т Ы
    //===============================================================

    //===============================================================
    //  Н А Ч А Л О  П Е Р Е М Е Н Н Ы Е

    // --------------------//
    // ===== РАЗМЕРЫ ===== //
    // --------------------//

    int Radius = 30;
    int RectBodyWidth = 40, RectBodyHeight = 120;
    int RectHeadWidth = 60, RectHeadHeight = 80;
    int RoundBodyWidth = 50, RoundBodyHeight = 100;
    int RoundHeadWidth = 70, RoundHeadHeight = 60;
    int ScrewLength = 60, ScrewWidth = 15;
    int StoneWidth = 80, StoneHeight = 60;

    // -----------------------//
    // ===== КООРДИНАТЫ ===== //
    // -----------------------//

    int x0 = 100, y0 = 100;
    int rect_x = x0 + 300, rect_y = y0 + 300;
    int round_x = x0 + 500, round_y = y0 + 300;
    int circle_x = x0 + 200, circle_y = y0 + 200;
    int point_x = x0 + 100, point_y = y0 + 100;
    int screw_x = x0 + 400, screw_y = y0 + 400;
    int stone_x = x0 + 400, stone_y = y0 + 200;

    // -------------------------------//
    // ===== ОБЪЕКТЫ НА "СЦЕНЕ" ===== //
    // -------------------------------//

    // Фонарики
    RectFlashlight rectFlashlight(rect_x, rect_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    RoundFlashlight roundFlashlight(round_x, round_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    BrokenFlashlight brokenRect(rect_x, rect_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight, 0);
    BrokenFlashlight brokenRound(round_x, round_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight, 1);

    // Указатели на текущие фонарики
    BaseFlashlight* currentRect = &rectFlashlight;
    BaseFlashlight* currentRound = &roundFlashlight;

    // Другие фигуры
    Point point(point_x, point_y);
    Circle circle(circle_x, circle_y, Radius);
    Screwdriver screwdriver(screw_x, screw_y, ScrewLength, ScrewWidth);
    Stone stone(stone_x, stone_y, StoneWidth, StoneHeight);

    // -------------------------- //
    // ===== ДЛЯ УПРАВЛЕНИЯ ===== //
    // -------------------------- //

    int fig_x, fig_y;
    int current_figure = 0;

    //  К О Н Е Ц  П Е Р Е М Е Н Н Ы Е
    //===============================================================

    // Показываем стартовые фигуры
    point.Show();
    circle.Show();
    currentRect->Show();
    currentRound->Show();
    screwdriver.Show();
    stone.Show();

    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "1 - Circle" << endl;
    cout << "2 - RectFlashlight" << endl;
    cout << "3 - Point" << endl;
    cout << "4 - RoundFlashlight" << endl;
    cout << "5 - Screwdriver" << endl;

    while (true)
    {
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        // Выбор фигуры
        if (KEY_DOWN(49))
        {
            current_figure = CIRCLE;
            fig_x = circle.GetX();
            fig_y = circle.GetY();
        }
        if (KEY_DOWN(50))
        {
            current_figure = RECT_FLASHLIGHT;
            fig_x = rect_x;
            fig_y = rect_y;
        }
        if (KEY_DOWN(51))
        {
            current_figure = POINT;
            fig_x = point.GetX();
            fig_y = point.GetY();
        }
        if (KEY_DOWN(52))
        {
            current_figure = ROUND_FLASHLIGHT;
            fig_x = round_x;
            fig_y = round_y;
        }
        if (KEY_DOWN(53))
        {
            current_figure = SCREWDRIVER;
            fig_x = screw_x;
            fig_y = screw_y;
        }

        // Обработка управления
        switch (current_figure)
        {
        case CIRCLE: // Управление кругом
            if (HandleMovement(fig_x, fig_y, STEP))
            {
                circle.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            }
            break;

        case RECT_FLASHLIGHT: // Управление прямоугольным фонариком
            if (!HandleMovement(fig_x, fig_y, STEP)) { break; }

            currentRect->Hide();

            // Обновляем координаты всех прямоугольных фонариков
            rect_x = fig_x;
            rect_y = fig_y;
            rectFlashlight.SetX(rect_x);
            rectFlashlight.SetY(rect_y);
            brokenRect.SetX(rect_x);
            brokenRect.SetY(rect_y);

            // Проверяем столкновения с использованием IsCollision
            if (IsCollision(*currentRect, stone))
            {
                currentRect = &brokenRect;  // сломанный
            }
            else if (currentRect == &brokenRect && IsCollision(*currentRect, screwdriver))
            {
                currentRect = &rectFlashlight;  // починили
            }

            // Перерисовываем конфликтные объекты
            stone.Show();
            screwdriver.Show();
            currentRect->Show();
            Sleep(DELAY);
            break;

        case POINT: // Управление точкой
            if (HandleMovement(fig_x, fig_y, STEP))
            {
                point.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            }
            break;

        case ROUND_FLASHLIGHT: // Управление круглым фонариком
            if (!HandleMovement(fig_x, fig_y, STEP)) { break; }

            currentRound->Hide();

            // Обновляем координаты всех круглых фонариков
            round_x = fig_x;
            round_y = fig_y;
            roundFlashlight.SetX(round_x);
            roundFlashlight.SetY(round_y);
            brokenRound.SetX(round_x);
            brokenRound.SetY(round_y);

            // Проверяем столкновения с использованием IsCollision
            if (IsCollision(*currentRound, stone))
            {
                currentRound = &brokenRound;  // сломанный
            }
            else if (currentRound == &brokenRound && IsCollision(*currentRound, screwdriver))
            {
                currentRound = &roundFlashlight;  // починили
            }

            // Перерисовываем конфликтные объекты
            stone.Show();
            screwdriver.Show();
            currentRound->Show();
            Sleep(DELAY);
            break;

        case SCREWDRIVER: // Управление отверткой
            if (HandleMovement(fig_x, fig_y, STEP))
            {
                screw_x = fig_x;
                screw_y = fig_y;
                screwdriver.MoveTo(screw_x, screw_y);

                // Перерисовываем все объекты
                stone.Show();
                currentRect->Show();
                currentRound->Show();
                Sleep(DELAY);
            }
            break;
        }
    }

    //===============================================================
    ReleaseDC(hwnd, hdc);
    return 0;
}