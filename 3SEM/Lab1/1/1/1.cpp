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
 * Last Revision : 26.09.2025                                                  *
 * Comment       : Библиотека для работы с фигурами - фонарик                  *
 ******************************************************************************/

#include <iostream>
#include <windows.h>
#include "Header.h"

using namespace std;

// Макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*Г Л О Б А Л Ь Н Ы Е  К О Н С Т А Н Т Ы  И  П Е Р Е М Е Н Н Ы Е*/
/****************************************************************/

HDC hdc;

// Функция проверки столкновения между фонариком и конфликтным объектом
bool IsCollision(BaseFlashlight& flashlight, Conflict& conflict)
{
    int flashX = flashlight.GetX();
    int flashY = flashlight.GetY();
    int flashWidth = flashlight.GetBodyWidth();
    int flashHeight = flashlight.GetBodyHeight();

    int conflictX = conflict.GetX();
    int conflictY = conflict.GetY();
    int conflictWidth = conflict.GetWidth();
    int conflictHeight = conflict.GetHeight();

    if (flashX < conflictX + conflictWidth &&
        flashX + flashWidth > conflictX &&
        flashY < conflictY + conflictHeight &&
        flashY + flashHeight > conflictY)
    {
        return true;
    }
    return false;
}

// Функция для обработки движения
bool HandleMovement(int& x, int& y, int step)
{
    int prev_x = x;
    int prev_y = y;

    if (KEY_DOWN(VK_LEFT)) { x -= step; }
    if (KEY_DOWN(VK_RIGHT)) { x += step; }
    if (KEY_DOWN(VK_UP)) { y -= step; }
    if (KEY_DOWN(VK_DOWN)) { y += step; }

    return (prev_x != x) || (prev_y != y);
}

/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/

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


    const int USUAL_FLASHLIGHT = 0;
    const int BROKEN_FLASHLIGHT = 1;

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
    int ScrewWidth = 15, ScrewHeight = 60;
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
    BrokenRectFlashlight brokenRect(rect_x, rect_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    BrokenRoundFlashlight brokenRound(round_x, round_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);

    // Другие фигуры
    Point point(point_x, point_y);
    Circle circle(circle_x, circle_y, Radius);
    Screwdriver screwdriver(screw_x, screw_y, ScrewWidth, ScrewHeight);
    Stone stone(stone_x, stone_y, StoneWidth, StoneHeight);

    // Состояния фонариков
    int rect_state = USUAL_FLASHLIGHT;
    int round_state = USUAL_FLASHLIGHT;

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
    rectFlashlight.Show();
    roundFlashlight.Show();
    screwdriver.Show();
    stone.Show();

    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "1 - Circle" << endl;
    cout << "2 - RectFlashlight" << endl;
    cout << "3 - Point" << endl;
    cout << "4 - RoundFlashlight" << endl;
    cout << "Note: Screwdriver is immovable (repair tool)" << endl;

    while (true)
    {
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        // Выбор фигуры
        if (KEY_DOWN(49)) // 1 - Circle
        {
            current_figure = CIRCLE;
            fig_x = circle.GetX();
            fig_y = circle.GetY();
        }
        if (KEY_DOWN(50)) // 2 - RectFlashlight
        {
            current_figure = RECT_FLASHLIGHT;
            fig_x = rectFlashlight.GetX();
            fig_y = rectFlashlight.GetY();
        }
        if (KEY_DOWN(51)) // 3 - Point
        {
            current_figure = POINT;
            fig_x = point.GetX();
            fig_y = point.GetY();
        }
        if (KEY_DOWN(52)) // 4 - RoundFlashlight
        {
            current_figure = ROUND_FLASHLIGHT;
            fig_x = roundFlashlight.GetX();
            fig_y = roundFlashlight.GetY();
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

            // Скрываем текущий фонарик перед перемещением
            if (rect_state == USUAL_FLASHLIGHT) {
                rectFlashlight.Hide();
            }
            else {
                brokenRect.Hide();
            }

            // Перемещаем оба варианта фонарика
            rectFlashlight.MoveTo(fig_x, fig_y);
            brokenRect.MoveTo(fig_x, fig_y);

            // Проверяем столкновения
            if (rect_state == USUAL_FLASHLIGHT) {
                if (IsCollision(rectFlashlight, stone)) {
                    rect_state = BROKEN_FLASHLIGHT;
                }
            }
            else {
                if (IsCollision(brokenRect, screwdriver)) {
                    rect_state = USUAL_FLASHLIGHT;
                }
            }

            // Показываем нужный фонарик
            if (rect_state == USUAL_FLASHLIGHT) {
                rectFlashlight.Show();
            }
            else {
                brokenRect.Show();
            }

            // Перерисовываем статические объекты
            stone.Show();
            screwdriver.Show();

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

            // Скрываем текущий фонарик перед перемещением
            if (round_state == USUAL_FLASHLIGHT) {
                roundFlashlight.Hide();
            }
            else {
                brokenRound.Hide();
            }

            // Перемещаем оба варианта фонарика
            roundFlashlight.MoveTo(fig_x, fig_y);
            brokenRound.MoveTo(fig_x, fig_y);

            // Проверяем столкновения
            if (round_state == USUAL_FLASHLIGHT) {
                if (IsCollision(roundFlashlight, stone)) {
                    round_state = BROKEN_FLASHLIGHT;
                }
            }
            else {
                if (IsCollision(brokenRound, screwdriver)) {
                    round_state = USUAL_FLASHLIGHT;
                }
            }

            // Показываем нужный фонарик
            if (round_state == USUAL_FLASHLIGHT) {
                roundFlashlight.Show();
            }
            else {
                brokenRound.Show();
            }

            // Перерисовываем статические объекты
            stone.Show();
            screwdriver.Show();

            Sleep(DELAY);
            break;
        }
    }

    //===============================================================
    ReleaseDC(hwnd, hdc);
    return 0;
}