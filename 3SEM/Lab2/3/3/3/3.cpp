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
#include <cmath>
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

// Функция проверки столкновения (с указателями вместо ссылок)
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
    int headWidth = flashlight.GetHeadWidth();
    int headHeight = flashlight.GetHeadHeight();
    
    // Координаты головки фонарика
    int headX = flashlightX + (bodyWidth - headWidth) / 2;
    int headY = flashlightY - headHeight;
    
    // Проверка столкновения с корпусом фонарика
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

    if (KEY_DOWN(VK_LEFT)) { x = x - step; }
    if (KEY_DOWN(VK_RIGHT)) { x = x + step; }
    if (KEY_DOWN(VK_UP)) { y = y - step; }
    if (KEY_DOWN(VK_DOWN)) { y = y + step; }

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

    const int CIRCLE = 1;       // круг
    const int FLASHLIGHT = 2;   // универсальный фонарик
    const int POINT = 3;        // точка
    const int SCREWDRIVER = 4;  // отвертка
    const int SQUARE = 5;       // квадрат

    int STEP = 5;
    const int DELAY = 33;

    // Количество проверок на конфликтные объекты
    const int TRANSITIONS_COUNT = 6;        // увеличили до 7
    const int NOT_TRANSITIONS_COUNT = 2;    // нетранзитивные переходы

    const int FLASHLIGHT_COUNT = 4;  // rect, brokenRect, round, brokenRound
    const int CONFLICT_COUNT = 4;    // stone, screwdriver, circle, square

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
    int SquareWidth = 60, SquareHeight = 60;

    // -----------------------//
    // ===== КООРДИНАТЫ ===== //
    // -----------------------//

    int x0 = 100, y0 = 100;
    int flashlight_x = x0 + 300, flashlight_y = y0 + 300;  // единые координаты для всех фонариков
    int circle_x = x0 + 200, circle_y = y0 + 200;  // координаты круга
    int point_x = x0 + 100, point_y = y0 + 100;  // координаты точки
    int screw_x = x0 + 400, screw_y = y0 + 400;  // координаты отвертки
    int stone_x = x0 + 400, stone_y = y0 + 200;  // координаты камня
    int square_x = x0 + 600, square_y = y0 + 200;  // координаты квадрата

    // -------------------------------//
    // ===== ОБЪЕКТЫ НА "СЦЕНЕ" ===== //
    // -------------------------------//

    // Фонарики
    RectFlashlight rectFlashlight(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    RoundFlashlight roundFlashlight(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    BrokenRectFlashlight brokenRect(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    BrokenRoundFlashlight brokenRound(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);

    // Указатель на текущий фонарик
    BaseFlashlight* currentFlashlight = &rectFlashlight;  // начинаем с прямоугольного

    // Другие фигуры
    Point point(point_x, point_y);
    Circle circle(circle_x, circle_y, Radius);
    Screwdriver screwdriver(screw_x, screw_y, ScrewWidth, ScrewLength);
    Stone stone(stone_x, stone_y, StoneWidth, StoneHeight);
    Square square(square_x, square_y, SquareWidth, SquareHeight);

    // Массивы для удобного управления через циклы
    BaseFlashlight* allFlashlights[FLASHLIGHT_COUNT] = {
        &rectFlashlight, &brokenRect, &roundFlashlight, &brokenRound
    };

    Conflict* allConflicts[CONFLICT_COUNT] = {
        &stone, &screwdriver, &circle, &square
    };

    // -------------------------------//
    // ===== СИСТЕМА ПЕРЕХОДОВ ===== //
    // -------------------------------//

    const int TOTAL_STATES = TRANSITIONS_COUNT + NOT_TRANSITIONS_COUNT;

    // УНИВЕРСАЛЬНЫЕ МАССИВЫ ПЕРЕХОДОВ ДЛЯ ВСЕХ ФОНАРИКОВ
    // Состояния начальные
    BaseFlashlight* from_states[TOTAL_STATES] =
    {
        // ТРАНЗИТИВНЫЕ ПЕРЕХОДЫ (при столкновении)
        &rectFlashlight,    // 0: обычный прямоугольный -> камень
        &brokenRect,        // 1: сломанный прямоугольный -> отвертка  
        &rectFlashlight,    // 2: обычный прямоугольный -> круг
        &roundFlashlight,   // 3: обычный круглый -> камень
        &brokenRound,       // 4: сломанный круглый -> отвертка
        &roundFlashlight,   // 5: обычный круглый -> квадрат (НОВЫЙ ПЕРЕХОД)

        // НЕТРАНЗИТИВНЫЕ ПЕРЕХОДЫ (без столкновения)
        &rectFlashlight,    // 6: обычный прямоугольный -> обычный прямоугольный (без столкновения)
        &roundFlashlight    // 7: обычный круглый -> обычный круглый (без столкновения)
    };

    // Обьекты конфликтные
    Conflict* conflicts[TOTAL_STATES] =
    {
        // ТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &stone,        // переход 0
        &screwdriver,  // переход 1
        &circle,       // переход 2
        &stone,        // переход 3
        &screwdriver,  // переход 4
        &square,       // переход 5 (НОВЫЙ: круглый -> квадрат)

        // НЕТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &circle,       // переход 6 (проверяем отсутствие столкновения с кругом)
        &circle        // переход 7 (проверяем отсутствие столкновения с кругом)
    };

    // Состояния конечные
    BaseFlashlight* to_states[TOTAL_STATES] =
    {
        // ТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &brokenRect,       // переход 0: прямоугольный -> сломанный прямоугольный
        &rectFlashlight,   // переход 1: сломанный прямоугольный -> прямоугольный
        &roundFlashlight,  // переход 2: прямоугольный -> круглый
        &brokenRound,      // переход 3: круглый -> сломанный круглый
        &roundFlashlight,  // переход 4: сломанный круглый -> круглый
        &rectFlashlight,   // переход 5: круглый -> прямоугольный (через квадрат) (НОВЫЙ)

        // НЕТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &rectFlashlight,   // переход 6: прямоугольный остается прямоугольным
        &roundFlashlight   // переход 7: круглый остается круглым
    };

    // -------------------------- //
    // ===== ДЛЯ УПРАВЛЕНИЯ ===== //
    // -------------------------- //

    int fig_x, fig_y;
    int current_figure = 0;

    bool collision;
    bool valid_transition;

    bool wasCollision = false;
    bool hadCollision = false;

    //  К О Н Е Ц  П Е Р Е М Е Н Н Ы Е
    //===============================================================

    // Показываем стартовые фигуры через циклы
    point.Show();
    for (int i = 0; i < CONFLICT_COUNT; i++)
    {
        allConflicts[i]->Show();
    }
    currentFlashlight->Show();  // показываем только текущий фонарик

    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "2 - Flashlight (starts rectangular)" << endl;
    cout << "3 - Point" << endl;

    while (true)
    {
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        // Выбор фигуры
        if (KEY_DOWN(50)) // 2
        {
            current_figure = FLASHLIGHT;
            fig_x = currentFlashlight->GetX();
            fig_y = currentFlashlight->GetY();
        }
        if (KEY_DOWN(51)) // 3
        {
            current_figure = POINT;
            fig_x = point.GetX();
            fig_y = point.GetY();
        }

        // Обработка управления
        switch (current_figure)
        {
        case FLASHLIGHT: // Управление универсальным фонариком
            if (!HandleMovement(fig_x, fig_y, STEP)) { break; }

            currentFlashlight->Hide();

            // ОБНОВЛЯЕМ КООРДИНАТЫ ВСЕХ ФОНАРИКОВ ЧЕРЕЗ ЦИКЛ
            for (int i = 0; i < FLASHLIGHT_COUNT; i++)
            {
                allFlashlights[i]->SetX(fig_x);
                allFlashlights[i]->SetY(fig_y);
            }

            flashlight_x = fig_x;
            flashlight_y = fig_y;
            hadCollision = false;

            // === ДИНАМИЧЕСКИЙ ПОЛИМОРФИЗМ - проверяем столкновения === //
            for (int i = 0; i < TOTAL_STATES; i++)
            {
                // Используем нашу новую функцию IsCollision с указателями
                collision = IsCollision(*conflicts[i], *currentFlashlight);

                // Первые TRANSITIONS_COUNT переходов - должны БЫТЬ столкновения
                // Последние NOT_TRANSITIONS_COUNT переходов - должно НЕ БЫТЬ столкновения 
                valid_transition = (i < TRANSITIONS_COUNT && collision) ||
                    (i >= TRANSITIONS_COUNT && !collision);

                if (currentFlashlight == from_states[i] && valid_transition)
                {
                    currentFlashlight = to_states[i];
                    break; // применяем только первый подходящий переход
                }
                if (i < TRANSITIONS_COUNT) // Если это транзитивный переход (было столкновение)
                {
                    hadCollision = true;
                }
            }

            // ПЕРЕРИСОВЫВАЕМ КОНФЛИКТНЫЕ ОБЪЕКТЫ ТОЛЬКО ПРИ НЕОБХОДИМОСТИ:
            // 1. Если было столкновение сейчас
            // 2. Если фонарик изменился (даже если не было столкновения в этот момент)
            // 3. Если предыдущее состояние было со столкновением, а сейчас нет
            if (hadCollision || (wasCollision && !hadCollision))
            {
                for (int i = 0; i < CONFLICT_COUNT; i++)
                {
                    allConflicts[i]->Show();
                }
            }

            // Обновляем флаг предыдущего состояния столкновения
            wasCollision = hadCollision;

            currentFlashlight->Show();
            Sleep(DELAY);
            break;

        case POINT: // Управление точкой
            if (HandleMovement(fig_x, fig_y, STEP))
            {
                point.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            }
            break;
        }
    }

    //===============================================================
    ReleaseDC(hwnd, hdc);
    return 0;
}