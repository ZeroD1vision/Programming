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

    // Уменьшенные размеры для большего пространства
    int Radius = 20;
    int RectBodyWidth = 30, RectBodyHeight = 90;
    int RectHeadWidth = 45, RectHeadHeight = 60;
    int RoundBodyWidth = 35, RoundBodyHeight = 70;
    int RoundHeadWidth = 50, RoundHeadHeight = 40;
    int ScrewLength = 40, ScrewWidth = 12;
    int StoneWidth = 50, StoneHeight = 40;
    int BatteryWidth = 20, BatteryHeight = 35;

    // -----------------------//
    // ===== КООРДИНАТЫ ===== //
    // -----------------------//

    int x0 = 50, y0 = 50;

    // Два фонарика слева
    int rect_x = x0 + 50, rect_y = y0 + 300;    // прямоугольный сверху слева
    int round_x = x0 + 50, round_y = y0 + 450;  // круглый снизу слева

    // Другие фигуры
    int circle_x = x0 + 200, circle_y = y0 + 400;
    int point_x = x0 + 100, point_y = y0 + 300;

    // Объекты на сцене
    int screw_x = x0 + 500, screw_y = y0 + 500;  // отвертка справа снизу
    int stone_x = x0 + 300, stone_y = y0 + 400;  // камень в центре
    int battery_x = x0 + 500, battery_y = y0 + 300;  // батарейка справа сверху

    // -------------------------------//
    // ===== ОБЪЕКТЫ НА "СЦЕНЕ" ===== //
    // -------------------------------//

    // Фонарики
    RectFlashlight rectFlashlight(rect_x, rect_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    BrokenRectFlashlight brokenRect(rect_x, rect_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    LitRectFlashlight litRectFlashlight(rect_x, rect_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);

    RoundFlashlight roundFlashlight(round_x, round_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    BrokenRoundFlashlight brokenRound(round_x, round_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    LitRoundFlashlight litRoundFlashlight(round_x, round_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);

    // Указатели на текущие фонарики
    BaseFlashlight* currentRect = &rectFlashlight;
    BaseFlashlight* currentRound = &roundFlashlight;

    // Другие фигуры
    Point point(point_x, point_y);
    Circle circle(circle_x, circle_y, Radius);
    Screwdriver screwdriver(screw_x, screw_y, ScrewWidth, ScrewLength);
    Stone stone(stone_x, stone_y, StoneWidth, StoneHeight);
    Battery battery(battery_x, battery_y, BatteryWidth, BatteryHeight);  // Батарейка

    // -------------------------------//
    // ===== СИСТЕМА СОСТОЯНИЙ ===== //
    // -------------------------------//

    // Для прямоугольного фонарика
    const int RECT_TRANSITIONS_COUNT = 3;
    const int RECT_NOT_TRANSITIONS_COUNT = 1;

    BaseFlashlight* from_states_rect[RECT_TRANSITIONS_COUNT + RECT_NOT_TRANSITIONS_COUNT] =
    {
        &rectFlashlight,    // обычный -> камень
        &brokenRect,        // сломанный -> отвертка  
        &rectFlashlight,    // обычный -> батарейка
        &litRectFlashlight  // горящий -> отсутствие батарейки
    };

    Conflict* conflicts_rect[RECT_TRANSITIONS_COUNT + RECT_NOT_TRANSITIONS_COUNT] =
    {
        &stone,        // переход 0: столкновение с камнем
        &screwdriver,  // переход 1: столкновение с отверткой
        &battery,      // переход 2: столкновение с батарейкой
        &battery       // переход 3: отсутствие столкновения с батарейкой
    };

    BaseFlashlight* to_states_rect[RECT_TRANSITIONS_COUNT + RECT_NOT_TRANSITIONS_COUNT] =
    {
        &brokenRect,       // переход 0: обычный -> сломанный
        &rectFlashlight,   // переход 1: сломанный -> обычный
        &litRectFlashlight,// переход 2: обычный -> горящий
        &rectFlashlight    // переход 3: горящий -> обычный
    };

    // Для круглого фонарика
    const int ROUND_TRANSITIONS_COUNT = 3;
    const int ROUND_NOT_TRANSITIONS_COUNT = 1;

    BaseFlashlight* from_states_round[ROUND_TRANSITIONS_COUNT + ROUND_NOT_TRANSITIONS_COUNT] =
    {
        &roundFlashlight,    // обычный -> камень
        &brokenRound,        // сломанный -> отвертка
        &roundFlashlight,    // обычный -> батарейка
        &litRoundFlashlight  // горящий -> отсутствие батарейки
    };

    Conflict* conflicts_round[ROUND_TRANSITIONS_COUNT + ROUND_NOT_TRANSITIONS_COUNT] =
    {
        &stone,        // переход 0: столкновение с камнем
        &screwdriver,  // переход 1: столкновение с отверткой
        &battery,      // переход 2: столкновение с батарейкой
        &battery       // переход 3: отсутствие столкновения с батарейкой
    };

    BaseFlashlight* to_states_round[ROUND_TRANSITIONS_COUNT + ROUND_NOT_TRANSITIONS_COUNT] =
    {
        &brokenRound,       // переход 0: обычный -> сломанный
        &roundFlashlight,   // переход 1: сломанный -> обычный
        &litRoundFlashlight,// переход 2: обычный -> горящий
        &roundFlashlight    // переход 3: горящий -> обычный
    };

    // -------------------------- //
    // ===== ДЛЯ УПРАВЛЕНИЯ ===== //
    // -------------------------- //

    int fig_x, fig_y;
    int current_figure = 0;

    bool collision;
    bool valid_transition;

    //  К О Н Е Ц  П Е Р Е М Е Н Н Ы Е
    //===============================================================

    // Показываем стартовые фигуры
    point.Show();
    circle.Show();
    currentRect->Show();
    currentRound->Show();
    screwdriver.Show();
    stone.Show();
    battery.Show();

    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "1 - Circle" << endl;
    cout << "2 - RectFlashlight" << endl;
    cout << "3 - Point" << endl;
    cout << "4 - RoundFlashlight" << endl;

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

            // Проверяем переходы для прямоугольного фонарика
            for (int i = 0; i < RECT_TRANSITIONS_COUNT + RECT_NOT_TRANSITIONS_COUNT; i++)
            {
                collision = IsCollision(*currentRect, *conflicts_rect[i]);

                valid_transition = (i < RECT_TRANSITIONS_COUNT&& collision) ||
                    (i == RECT_TRANSITIONS_COUNT && !collision);

                if (currentRect == from_states_rect[i] && valid_transition)
                {
                    currentRect = to_states_rect[i];
                    break;
                }
            }

            // Обновляем координаты всех прямоугольных фонариков
            rect_x = fig_x;
            rect_y = fig_y;
            rectFlashlight.SetX(rect_x);
            rectFlashlight.SetY(rect_y);
            brokenRect.SetX(rect_x);
            brokenRect.SetY(rect_y);
            litRectFlashlight.SetX(rect_x);
            litRectFlashlight.SetY(rect_y);

            // Перерисовываем конфликтные объекты
            stone.Show();
            screwdriver.Show();
            battery.Show();
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

            // Проверяем переходы для круглого фонарика
            for (int i = 0; i < ROUND_TRANSITIONS_COUNT + ROUND_NOT_TRANSITIONS_COUNT; i++)
            {
                collision = IsCollision(*currentRound, *conflicts_round[i]);

                valid_transition = (i < ROUND_TRANSITIONS_COUNT&& collision) ||
                    (i == ROUND_TRANSITIONS_COUNT && !collision);

                if (currentRound == from_states_round[i] && valid_transition)
                {
                    currentRound = to_states_round[i];
                    break;
                }
            }

            // Обновляем координаты всех круглых фонариков
            round_x = fig_x;
            round_y = fig_y;
            roundFlashlight.SetX(round_x);
            roundFlashlight.SetY(round_y);
            brokenRound.SetX(round_x);
            brokenRound.SetY(round_y);
            litRoundFlashlight.SetX(round_x);
            litRoundFlashlight.SetY(round_y);

            // Перерисовываем конфликтные объекты
            stone.Show();
            screwdriver.Show();
            battery.Show();
            currentRound->Show();

            Sleep(DELAY);
            break;
        }
    }

    //===============================================================
    ReleaseDC(hwnd, hdc);
    return 0;
}