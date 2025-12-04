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

/****************************************************************/
/*           Ф У Н К Ц И Я  П Р О В Е Р К И  С Т О Л К Н О В Е Н И Я       */
/****************************************************************/

bool IsCollision(Conflict& conflict, BaseFlashlight& flashlight)
{
    // Получаем координаты и размеры конфликтного объекта
    int conflictX = conflict.GetX();
    int conflictY = conflict.GetY();
    int conflictWidth = conflict.GetWidth();
    int conflictHeight = conflict.GetHeight();

    // Получаем координаты и размеры фонарика
    int flashlightX = flashlight.GetX();
    int flashlightY = flashlight.GetY();
    int bodyWidth = flashlight.GetBodyWidth();
    int bodyHeight = flashlight.GetBodyHeight();

    // Проверка столкновения по прямоугольникам
    bool bodyCollision = !(flashlightX + bodyWidth < conflictX ||
        flashlightX > conflictX + conflictWidth ||
        flashlightY + bodyHeight < conflictY ||
        flashlightY > conflictY + conflictHeight);

    return bodyCollision;
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
    const int FLASHLIGHT = 2;
    const int POINT = 3;
    const int SQUARE = 4;

    int STEP = 5;
    const int DELAY = 33;

    const int TRANSITIONS_COUNT = 12;        // 10 переходов со столкновением
    const int NOT_TRANSITIONS_COUNT = 2;     // 2 перехода без столкновения
    const int TOTAL_STATES = TRANSITIONS_COUNT + NOT_TRANSITIONS_COUNT;

    const int FLASHLIGHT_COUNT = 8;          // 6 состояний фонарика
    const int CONFLICT_COUNT = 6;            // 5 конфликтных объектов

    //  К О Н Е Ц  К О Н С Т А Н Т Ы
    //===============================================================

    //===============================================================
    //  Н А Ч А Л О  П Е Р Е М Е Н Н Ы Е

    // --------------------//
    // ===== РАЗМЕРЫ ===== //
    // --------------------//

    // Уменьшенные размеры как в первом проекте
    int Radius = 20;                    // Круг
    int SquareSize = 40;                // Квадрат
    int BatteryWidth = 15, BatteryHeight = 25;  // Батарейка

    // Прямоугольный фонарик (как в первом проекте)
    int RectBodyWidth = 30, RectBodyHeight = 90;
    int RectHeadWidth = 45, RectHeadHeight = 60;

    // Круглый фонарик (как в первом проекте)  
    int RoundBodyWidth = 35, RoundBodyHeight = 70;
    int RoundHeadWidth = 50, RoundHeadHeight = 40;

    // Другие объекты (уменьшенные)
    int ScrewLength = 50, ScrewWidth = 12;
    int StoneWidth = 50, StoneHeight = 40;

    // -----------------------//
    // ===== КООРДИНАТЫ ===== //
    // -----------------------//

    // Начальные координаты - сдвинуты ниже по вертикали
    int x0 = 50, y0 = 150;  // y0 увеличен с 50 до 150

    // Фонарик в центре (сдвинут вниз)
    int flashlight_x = x0 + 200, flashlight_y = y0 + 200;

    // Конфликтные объекты по кругу (все сдвинуты вниз)
    int circle_x = x0 + 100, circle_y = y0 + 100;      // Круг слева сверху
    int square_x = x0 + 300, square_y = y0 + 100;      // Квадрат справа сверху
    int stone_x = x0 + 100, stone_y = y0 + 300;        // Камень слева снизу  
    int screw_x = x0 + 300, screw_y = y0 + 300;        // Отвертка справа снизу
    int battery_x = x0 + 200, battery_y = y0 + 50;     // Батарейка сверху по центру

    // Точка для управления
    int point_x = x0 + 50, point_y = y0 + 150;

    int mud_x = x0 + 400, mud_y = y0 + 200;

    // -------------------------------//
    // ===== ОБЪЕКТЫ НА "СЦЕНЕ" ===== //
    // -------------------------------//

    // Фонарики (6 состояний)
    RectFlashlight rectFlashlight(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    RoundFlashlight roundFlashlight(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    BrokenRectFlashlight brokenRect(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    BrokenRoundFlashlight brokenRound(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    LitRectFlashlight litRectFlashlight(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    LitRoundFlashlight litRoundFlashlight(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);

    DirtyRectFlashlight dirtyRect(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    DirtyRoundFlashlight dirtyRound(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    MudPuddle mudPuddle(mud_x, mud_y, 60, 40);  // Грязевая лужа

    // Указатель на текущий фонарик
    BaseFlashlight* currentFlashlight = &rectFlashlight;

    // Другие фигуры
    Point point(point_x, point_y);
    Circle circle(circle_x, circle_y, Radius);
    Screwdriver screwdriver(screw_x, screw_y, ScrewWidth, ScrewLength);
    Stone stone(stone_x, stone_y, StoneWidth, StoneHeight);
    Square square(square_x, square_y, SquareSize, SquareSize);
    Battery battery(battery_x, battery_y, BatteryWidth, BatteryHeight);  // Батарейка

    // Массивы для удобного управления
    BaseFlashlight* allFlashlights[FLASHLIGHT_COUNT] = {
        &rectFlashlight, &brokenRect, &litRectFlashlight, &dirtyRect,
        &roundFlashlight, &brokenRound, &litRoundFlashlight, &dirtyRound
    };

    Conflict* allConflicts[CONFLICT_COUNT] = {
        &stone, &screwdriver, &circle, &square, &battery, &mudPuddle
    };

    // -------------------------------//
    // ===== СИСТЕМА ПЕРЕХОДОВ ===== //
    // -------------------------------//

    // УНИВЕРСАЛЬНЫЕ МАССИВЫ ПЕРЕХОДОВ (12 состояний)
    BaseFlashlight* from_states[TOTAL_STATES] =
    {
        // ТРАНЗИТИВНЫЕ ПЕРЕХОДЫ (при столкновении) - 10 переходов
        &rectFlashlight,      // 0: обычный прямоугольный -> камень
        &brokenRect,          // 1: сломанный прямоугольный -> отвертка  
        &rectFlashlight,      // 2: обычный прямоугольный -> батарейка
        &litRectFlashlight,   // 3: горящий прямоугольный -> отсутствие батарейки
        &rectFlashlight,      // 4: обычный прямоугольный -> круг (переход формы)

        &roundFlashlight,     // 5: обычный круглый -> камень
        &brokenRound,         // 6: сломанный круглый -> отвертка
        &roundFlashlight,     // 7: обычный круглый -> батарейка
        &litRoundFlashlight,  // 8: горящий круглый -> отсутствие батарейки
        &roundFlashlight,     // 9: обычный круглый -> квадрат (переход формы)
        // НОВЫЕ ПЕРЕХОДЫ для грязного фонарика:
        &rectFlashlight,          // 12: грязный прямоугольный → батарейка
        &roundFlashlight,          // 13: грязный круглый → батарейка

        // НЕТРАНЗИТИВНЫЕ ПЕРЕХОДЫ (без столкновения) - 2 перехода
        &litRectFlashlight,   // 10: горящий прямоугольный -> обычный (без батарейки)
        &litRoundFlashlight   // 11: горящий круглый -> обычный (без батарейки)
    };

    Conflict* conflicts[TOTAL_STATES] =
    {
        // ТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &stone,        // 0: прямоугольный с камнем
        &screwdriver,  // 1: сломанный прямоугольный с отверткой
        &battery,      // 2: прямоугольный с батарейкой
        &battery,      // 3: горящий прямоугольный без батарейки
        &circle,       // 4: прямоугольный с кругом

        &stone,        // 5: круглый с камнем
        &screwdriver,  // 6: сломанный круглый с отверткой
        &battery,      // 7: круглый с батарейкой
        &battery,      // 8: горящий круглый без батарейки
        &square,       // 9: круглый с квадратом
        // НОВЫЕ:
        & mudPuddle,  // 12: грязный прямоугольный с батарейкой
        & mudPuddle,   // 13: грязный круглый с батарейкой

        // НЕТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &battery,      // 10: горящий прямоугольный без батарейки
        &battery       // 11: горящий круглый без батарейки
    };

    BaseFlashlight* to_states[TOTAL_STATES] =
    {
        // ТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &brokenRect,        // 0: прямоугольный -> сломанный прямоугольный
        &rectFlashlight,    // 1: сломанный прямоугольный -> прямоугольный
        &litRectFlashlight, // 2: прямоугольный -> горящий прямоугольный
        &rectFlashlight,    // 3: горящий прямоугольный -> прямоугольный
        &roundFlashlight,   // 4: прямоугольный -> круглый

        &brokenRound,       // 5: круглый -> сломанный круглый
        &roundFlashlight,   // 6: сломанный круглый -> круглый
        &litRoundFlashlight,// 7: круглый -> горящий круглый
        &roundFlashlight,   // 8: горящий круглый -> круглый
        &rectFlashlight,    // 9: круглый -> прямоугольный
        // НОВЫЕ:
        & dirtyRect,        // 12: грязный прямоугольный → горящий прямоугольный
        & dirtyRound,       // 13: грязный круглый → горящий круглый

        // НЕТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &rectFlashlight,    // 10: горящий прямоугольный -> обычный прямоугольный
        &roundFlashlight    // 11: горящий круглый -> обычный круглый
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
    for (int i = 0; i < CONFLICT_COUNT; i++)
    {
        allConflicts[i]->Show();
    }
    currentFlashlight->Show();

    // Выводим инструкции
    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "2 - Flashlight (starts rectangular)" << endl;
    cout << "3 - Point" << endl;

    while (true)
    {
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        // Выбор фигуры
        if (KEY_DOWN(50)) // 2 - Фонарик
        {
            current_figure = FLASHLIGHT;
            fig_x = currentFlashlight->GetX();
            fig_y = currentFlashlight->GetY();
        }
        if (KEY_DOWN(51)) // 3 - Точка
        {
            current_figure = POINT;
            fig_x = point.GetX();
            fig_y = point.GetY();
        }

        // Обработка управления
        switch (current_figure)
        {
        case FLASHLIGHT: // Управление фонариком
            if (!HandleMovement(fig_x, fig_y, STEP)) { break; }

            currentFlashlight->Hide();

            // Обновляем координаты всех фонариков
            for (int i = 0; i < FLASHLIGHT_COUNT; i++)
            {
                allFlashlights[i]->SetX(fig_x);
                allFlashlights[i]->SetY(fig_y);
            }

            flashlight_x = fig_x;
            flashlight_y = fig_y;

            // Проверяем переходы
            for (int i = 0; i < TOTAL_STATES; i++)
            {
                collision = IsCollision(*conflicts[i], *currentFlashlight);

                // Для горящих фонариков проверяем ОТСУТСТВИЕ батарейки
                if (i == 3 || i == 8 || i == 12 || i == 13)
                {
                    valid_transition = !collision;  // Нет столкновения с батарейкой
                }
                else
                {
                    valid_transition = collision;   // Есть столкновение
                }

                if (currentFlashlight == from_states[i] && valid_transition)
                {
                    currentFlashlight = to_states[i];
                    break;
                }
            }

            // Перерисовываем все конфликтные объекты
            for (int i = 0; i < CONFLICT_COUNT; i++)
            {
                allConflicts[i]->Show();
            }

            currentFlashlight->Show();
            Sleep(DELAY);
            break;
        }
    }

    //===============================================================
    ReleaseDC(hwnd, hdc);
    return 0;
}