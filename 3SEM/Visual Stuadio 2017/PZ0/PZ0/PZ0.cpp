/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Windows Console Application                                 *
 * Project name  : Pt_1                                                        *
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

HDC hdc;    // Объявим контекст устройства
            // Контекст устройства это структура, содержащая
            // описание видеокарты на вашем компьютере
            // и всех необходимых графических элементов

/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/

int main()
{
    //===============================================================
    //  Н А Ч А Л О  М О Л И Т В А
    // Получим дескриптор консольного окна
    HWND hwnd = GetConsoleWindow();

    // если дескриптор НЕ существует
    if (hwnd == NULL)
    {
        cout << "Console Window Descriptor NOT FOUND !\n";
        return 1; // ERROR
    } // if (hwnd == NULL)

    // дескриптор существует

    // получим контекст устройства для консольного окна
    hdc = GetWindowDC(hwnd);

    // если контекст НЕ существует
    if (hdc == 0)
    {
        cout << "Handle Device Context NOT FOUND !\n";
        return 2; // ERROR
    } // if (hdc == 0)

    // контекст существует - можем работать
    // К О Н Е Ц  М О Л И Т В А 
    //===============================================================

    //===============================================================
    // Н А Ч А Л О  К О Н С Т А Н Т Ы

    // константы выбранных фигур
    const int CIRCLE = 1;
    const int RECT_FLASHLIGHT = 2;
    const int POINT = 3;
    const int ROUND_FLASHLIGHT = 4;
    const int SCREWDRIVER = 5;

    // константы состояний фонариков
    const int USUAL_FLASHLIGHT = 0;    // обычный фонарик
    const int BROKEN_FLASHLIGHT = -1;  // сломанный

    int STEP = 5;           // шаг в Drag
    const int DELAY = 50;   // задержка

    // Размеры камня
    const int STONE_WIDTH = 80;
    const int STONE_HEIGHT = 60;

    //  К О Н Е Ц  К О Н С Т А Н Т Ы
    //===============================================================

    //===============================================================
    //  Н А Ч А Л О  П Е Р Е М Е Н Н Ы Е

      // --------------------//
     // ===== РАЗМЕРЫ ===== //
    // --------------------//

    // для круга (класса Circle)
    int Radius = 30;    // начальное значение радиуса КРУГА

    // для прямоугольного фонарика (класса RectFlashlight)
    int RectBodyWidth = 40;     // ширина корпуса
    int RectBodyHeight = 120;   // высота корпуса
    int RectHeadWidth = 60;     // ширина головки
    int RectHeadHeight = 80;    // высота головки

    // для круглого фонарика (класса RoundFlashlight)
    int RoundBodyWidth = 50;    // ширина корпуса
    int RoundBodyHeight = 100;  // высота корпуса
    int RoundHeadWidth = 70;    // ширина головки
    int RoundHeadHeight = 60;   // высота головки

    // для отвертки (класса Screwdriver)
    int ScrewLength = 60;       // длина отвертки
    int ScrewWidth = 15;        // ширина отвертки


      // -----------------------//
     // ===== КООРДИНАТЫ ===== //
    // -----------------------//

    // стартовые координаты
    int x0 = 100;
    int y0 = 100;

    // текущие координаты прямоугольного фонарика
    int rect_x = x0 + 300;
    int rect_y = y0 + 300;

    // текущие координаты круглого фонарика
    int round_x = x0 + 500;
    int round_y = y0 + 300;

    // текущие координаты круга
    int circle_x = x0 + 200;
    int circle_y = y0 + 200;

    // текущие координаты точки
    int point_x = x0 + 100;
    int point_y = y0 + 100;

    // текущие координаты отвертки
    int screw_x = x0 + 400;
    int screw_y = y0 + 400;

    // координаты камня
    int stone_x = x0 + 400;
    int stone_y = y0 + 200;


    // -------------------------------//
   // ===== ОБЪЕКТЫ НА "СЦЕНЕ" ===== //
  // -------------------------------//

  // фонарики
    RectFlashlight rectFlashlight(rect_x, rect_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    RoundFlashlight roundFlashlight(round_x, round_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    BrokenFlashlight brokenRect(rect_x, rect_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight, 0);
    BrokenFlashlight brokenRound(round_x, round_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight, 1);

    // другие фигуры
    Point point(point_x, point_y);
    Circle circle(circle_x, circle_y, Radius);
    Screwdriver screwdriver(screw_x, screw_y, ScrewLength, ScrewWidth);

    // камень
    Stone stone(stone_x, stone_y, STONE_WIDTH, STONE_HEIGHT);


    // -------------------------------//
   // ===== СОСТОЯНИЕ ОБЪЕКТОВ ===== //
  // -------------------------------//

    int rect_state = USUAL_FLASHLIGHT;
    int round_state = USUAL_FLASHLIGHT;


    // -------------------------- //
   // ===== ДЛЯ УПРАВЛЕНИЯ ===== //
  // -------------------------- //

    int fig_x, fig_y;           // координаты текущей фигуры
    int current_figure = 0;     // нажатая клавиша (1 - круг, 2 - прямоугольный фонарик, 3 - точка, 4 - круглый фонарик, 5 - отвертка)

    //  К О Н Е Ц  П Е Р Е М Е Н Н Ы Е
    //===============================================================

    // Показываем стартовые фигуры
    point.Show();           // точка
    circle.Show();          // круг
    rectFlashlight.Show();  // прямоугольный фонарик
    roundFlashlight.Show(); // круглый фонарик
    screwdriver.Show();     // отвертка
    stone.Show();           // камень

    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "1 - Circle" << endl;
    cout << "2 - RectFlashlight" << endl;
    cout << "3 - Point" << endl;
    cout << "4 - RoundFlashlight" << endl;
    cout << "5 - Screwdriver (repair tool)" << endl;

    while (true)
    {
        // при нажатии escape выходим
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        // Выбор фигуры - круг
        if (KEY_DOWN(49))
        {
            current_figure = CIRCLE;    // изменяем текущую фигуру

            // и изменяем координаты
            fig_x = circle.GetX();
            fig_y = circle.GetY();
        } // if (KEY_DOWN(49))

        // Выбор фигуры - прямоугольный фонарик
        if (KEY_DOWN(50))
        {
            current_figure = RECT_FLASHLIGHT;    // изменяем текущую фигуру

            // и изменяем координаты
            fig_x = rect_x;
            fig_y = rect_y;
        } // if (KEY_DOWN(50))

        // Выбор фигуры - точка
        if (KEY_DOWN(51))
        {
            current_figure = POINT;    // изменяем текущую фигуру

            // и изменяем координаты
            fig_x = point.GetX();
            fig_y = point.GetY();
        } // if (KEY_DOWN(51))

        // Выбор фигуры - круглый фонарик
        if (KEY_DOWN(52))
        {
            current_figure = ROUND_FLASHLIGHT;    // изменяем текущую фигуру

            // и изменяем координаты
            fig_x = round_x;
            fig_y = round_y;
        } // if (KEY_DOWN(52))

        // Выбор фигуры - отвертка
        if (KEY_DOWN(53))
        {
            current_figure = SCREWDRIVER;    // изменяем текущую фигуру

            // и изменяем координаты
            fig_x = screw_x;
            fig_y = screw_y;
        } // if (KEY_DOWN(53))

        // Обработка управления для выбранной фигуры
        switch (current_figure)
        {
        case CIRCLE: // Управление кругом
            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;
                circle.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            } // if (KEY_DOWN(VK_LEFT))

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;
                circle.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            } // if (KEY_DOWN(VK_RIGHT))

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;
                circle.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            } // if (KEY_DOWN(VK_UP))

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;
                circle.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            } // if (KEY_DOWN(VK_DOWN))
            break;

        case RECT_FLASHLIGHT: // Управление прямоугольным фонариком
            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;
            } // if (KEY_DOWN(VK_LEFT))

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;
            } // if (KEY_DOWN(VK_RIGHT))

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;
            } // if (KEY_DOWN(VK_UP))

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;
            } // if (KEY_DOWN(VK_DOWN))

            // СКРЫВАЕМ ОБЕ ВЕРСИИ ФОНАРИКА ПЕРЕД ПЕРЕМЕЩЕНИЕМ
            rectFlashlight.Hide();
            brokenRect.Hide();

            // Обновляем координаты прямоугольного фонарика
            rect_x = fig_x;
            rect_y = fig_y;
            rectFlashlight.SetX(rect_x);
            rectFlashlight.SetY(rect_y);
            brokenRect.SetX(rect_x);
            brokenRect.SetY(rect_y);

            // Проверяем столкновения с камнем
            if (rect_x < stone_x + STONE_WIDTH &&
                rect_x + RectBodyWidth > stone_x &&
                rect_y < stone_y + STONE_HEIGHT &&
                rect_y + RectBodyHeight > stone_y)
            {
                rect_state = BROKEN_FLASHLIGHT; // сломанный
            } // if (столкнулся с камнем)

            // Перемещаем и отображаем прямоугольный фонарик в зависимости от состояния
            switch (rect_state)
            {
            case USUAL_FLASHLIGHT: // обычный фонарик
                brokenRect.Hide(); // скрываем сломанный фонарик

                stone.Show();      // показываем камень

                // и перемещаем обычный фонарик
                rectFlashlight.MoveTo(rect_x, rect_y);
                break;

            case BROKEN_FLASHLIGHT: // сломанный фонарик
                rectFlashlight.Hide(); // прячем обычный фонарик

                stone.Show();          // показываем камень

                // и перемещаем сломанный фонарик
                brokenRect.MoveTo(rect_x, rect_y);
                break;
            }

            Sleep(DELAY);
            break;

        case POINT: // Управление точкой
            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;
                point.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            } // if (KEY_DOWN(VK_LEFT))

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;
                point.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            } // if (KEY_DOWN(VK_RIGHT))

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;
                point.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            } // if (KEY_DOWN(VK_UP))

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;
                point.MoveTo(fig_x, fig_y);
                Sleep(DELAY);
            } // if (KEY_DOWN(VK_DOWN))
            break;

        case ROUND_FLASHLIGHT: // Управление круглым фонариком
            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;
            } // if (KEY_DOWN(VK_LEFT))

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;
            } // if (KEY_DOWN(VK_RIGHT))

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;
            } // if (KEY_DOWN(VK_UP))

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;
            } // if (KEY_DOWN(VK_DOWN))

            roundFlashlight.Hide();
            brokenRound.Hide();

            // Обновляем координаты круглого фонарика
            round_x = fig_x;
            round_y = fig_y;
            roundFlashlight.SetX(round_x);
            roundFlashlight.SetY(round_y);
            brokenRound.SetX(round_x);
            brokenRound.SetY(round_y);

            // Проверяем столкновения с камнем
            if (round_x < stone_x + STONE_WIDTH &&
                round_x + RoundBodyWidth > stone_x &&
                round_y < stone_y + STONE_HEIGHT &&
                round_y + RoundBodyHeight > stone_y)
            {
                round_state = BROKEN_FLASHLIGHT; // сломанный
            } // if (столкнулся с камнем)

            // Перемещаем и отображаем круглый фонарик в зависимости от состояния
            switch (round_state)
            {
            case USUAL_FLASHLIGHT: // обычный фонарик
                stone.Show();       // показываем 
                roundFlashlight.Show(); // показываем обычный фонарик
                break;

            case BROKEN_FLASHLIGHT: // сломанный фонарик
                stone.Show();           // показываем 
                brokenRound.Show();     // показываем сломанный фонарик
                break;
            }

            Sleep(DELAY);
            break;

        case SCREWDRIVER: // Управление отверткой
            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;
            } // if (KEY_DOWN(VK_LEFT))

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;
            } // if (KEY_DOWN(VK_RIGHT))

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;
            } // if (KEY_DOWN(VK_UP))

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;
            } // if (KEY_DOWN(VK_DOWN))

            // Обновляем координаты отвертки
            screw_x = fig_x;
            screw_y = fig_y;

            // Проверяем столкновения для починки
            // только сломанные фонарики можно починить
            if (rect_state == BROKEN_FLASHLIGHT)
            {
                // ЕСЛИ отвертка столкнулась с прямоугольным фонариком!
                if (screw_x < rect_x + RectBodyWidth &&
                    screw_x + ScrewWidth > rect_x &&
                    screw_y < rect_y + RectBodyHeight &&
                    screw_y + ScrewLength > rect_y)
                {
                    rectFlashlight.SetX(rect_x);
                    rectFlashlight.SetY(rect_y);
                    brokenRect.Hide();
                    rect_state = USUAL_FLASHLIGHT; // чиним фонарик
                } // if (столкнулся с прямоугольным фонариком)
            } // if (rect_state == BROKEN_FLASHLIGHT)

            if (round_state == BROKEN_FLASHLIGHT)
            {
                // ЕСЛИ отвертка столкнулась с круглым фонариком!
                if (screw_x < round_x + RoundBodyWidth &&
                    screw_x + ScrewWidth > round_x &&
                    screw_y < round_y + RoundBodyHeight &&
                    screw_y + ScrewLength > round_y)
                {
                    roundFlashlight.SetX(round_x);
                    roundFlashlight.SetY(round_y);
                    brokenRound.Hide();
                    round_state = USUAL_FLASHLIGHT; // чиним фонарик
                } // if (столкнулся с круглым фонариком)
            } // if (round_state == BROKEN_FLASHLIGHT)

            // Перемещаем отвертку
            screwdriver.MoveTo(screw_x, screw_y);

            // Перерисовываем все объекты
            stone.Show();           // камень
            if (rect_state == USUAL_FLASHLIGHT)
            {
                rectFlashlight.Show();  // обычный прямоугольный фонарик
            }
            else
            {
                brokenRect.Show();      // сломанный прямоугольный фонарик
            }
            if (round_state == USUAL_FLASHLIGHT)
            {
                roundFlashlight.Show(); // обычный круглый фонарик
            }
            else
            {
                brokenRound.Show();     // сломанный круглый фонарик
            }

            Sleep(DELAY);
            break;
        }
    }

    //===============================================================
    ReleaseDC(hwnd, hdc);

    return 0;
}