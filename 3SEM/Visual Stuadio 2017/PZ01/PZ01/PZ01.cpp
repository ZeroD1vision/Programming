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
    //===============================================================
    //  Н А Ч А Л О  М О Л И Т В А
    // Получим дескриптор консольного окна
    HWND hwnd = GetConsoleWindow();
    if (hwnd == NULL)
    {
        cout << "Console Window Descriptor NOT FOUND !\n";
        return 1;
    } // if (дескриптор окна не найден)

    // Получим контекст устройства для консольного окна
    hdc = GetWindowDC(hwnd);
    if (hdc == 0)
    {
        cout << "Handle Device Context NOT FOUND !\n";
        return 2;
    } // if (контекст устройства не найден)
    // К О Н Е Ц  М О Л И Т В А 
    //===============================================================

    //===============================================================
    // Н А Ч А Л О  К О Н С Т А Н Т Ы

    // константы выбранных фигур
    const int CIRCLE = 1;              // круг
    const int RECT_FLASHLIGHT = 2;     // прямоугольный фонарик
    const int POINT = 3;               // точка
    const int ROUND_FLASHLIGHT = 4;    // круглый фонарик
    const int SCREWDRIVER = 5;         // отвертка

    // константы состояний фонариков
    const int USUAL_FLASHLIGHT = 0;    // обычный фонарик
    const int BROKEN_FLASHLIGHT = -1;  // сломанный фонарик

    int STEP = 5;           // шаг перемещения фигур
    const int DELAY = 50;   // задержка между движениями

    // Границы стен
    const int WALL_LEFT = 50;      // левая граница стены
    const int WALL_TOP = 50;       // верхняя граница стены
    const int WALL_RIGHT = 750;    // правая граница стены
    const int WALL_BOTTOM = 550;   // нижняя граница стены

    // Размеры камня
    const int STONE_WIDTH = 80;    // ширина камня
    const int STONE_HEIGHT = 60;   // высота камня

    //  К О Н Е Ц  К О Н С Т А Н Т Ы
    //===============================================================

    //===============================================================
    //  Н А Ч А Л О  П Е Р Е М Е Н Н Ы Е

    // Координаты статичных объектов
    int stone_x = 400;  // координата X камня
    int stone_y = 200;  // координата Y камня

    // Состояния фонариков
    int rect_state = USUAL_FLASHLIGHT;   // состояние прямоугольного фонарика
    int round_state = USUAL_FLASHLIGHT;  // состояние круглого фонарика

    // Урон фонариков (для будущего расширения функционала)
    int rect_damage = 0;    // уровень урона прямоугольного фонарика
    int round_damage = 0;   // уровень урона круглого фонарика

    // Объекты на сцене
    Point point(100, 100);      // точка
    Circle circle(200, 200, 30); // круг
    RectFlashlight rectFlashlight(300, 300, 40, 120, 60, 80);       // прямоугольный фонарик
    RoundFlashlight roundFlashlight(500, 300, 50, 100, 70, 60);     // круглый фонарик
    BrokenFlashlight brokenRect(300, 300, 40, 120, 60, 80, 0);      // сломанный прямоугольный фонарик
    BrokenFlashlight brokenRound(500, 300, 50, 100, 70, 60, 1);     // сломанный круглый фонарик
    Screwdriver screwdriver(400, 400, 60, 15);                      // отвертка
    Stone stone(stone_x, stone_y, STONE_WIDTH, STONE_HEIGHT);       // камень

    // Переменные для управления
    int fig_x, fig_y;           // текущие координаты фигуры
    int current_figure = 0;     // выбранная фигура для управления
    bool was_pressed = false;   // флаг нажатия клавиши управления

    //  К О Н Е Ц  П Е Р Е М Е Н Н Ы Е
    //===============================================================

    // Рисуем стены
    HPEN wallPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, wallPen);
    Rectangle(hdc, WALL_LEFT, WALL_TOP, WALL_RIGHT, WALL_BOTTOM);
    DeleteObject(wallPen);

    // Показываем стартовые фигуры
    point.Show();           // показываем точку
    circle.Show();          // показываем круг
    rectFlashlight.Show();  // показываем прямоугольный фонарик
    roundFlashlight.Show(); // показываем круглый фонарик
    screwdriver.Show();     // показываем отвертку
    stone.Show();           // показываем камень

    cout << "To drag use arrows, to exit - ESC" << endl;
    cout << "Choose the figure to drag:" << endl;
    cout << "1 - Circle" << endl;
    cout << "2 - RectFlashlight" << endl;
    cout << "3 - Point" << endl;
    cout << "4 - RoundFlashlight" << endl;
    cout << "5 - Screwdriver (repair tool)" << endl;

    //===============================================================
    //                   РЕАЛИЗАЦИЯ ДВИЖЕНИЯ                 
    //===============================================================

    // Цикл для выбора и перемещения фигур
    while (true)
    {
        // при нажатии escape выходим
        if (KEY_DOWN(VK_ESCAPE))
        {
            break;
        } // if (нажата клавиша ESC)

        // Выбор фигуры - круг
        if (KEY_DOWN(49))
        {
            current_figure = CIRCLE;    // изменяем текущую фигуру
            fig_x = circle.GetX();      // запоминаем координату X круга
            fig_y = circle.GetY();      // запоминаем координату Y круга
        } // if (нажата клавиша 1)

        // Выбор фигуры - прямоугольный фонарик
        if (KEY_DOWN(50))
        {
            current_figure = RECT_FLASHLIGHT;   // изменяем текущую фигуру
            fig_x = rectFlashlight.GetX();      // запоминаем координату X фонарика
            fig_y = rectFlashlight.GetY();      // запоминаем координату Y фонарика
        } // if (нажата клавиша 2)

        // Выбор фигуры - точка
        if (KEY_DOWN(51))
        {
            current_figure = POINT;     // изменяем текущую фигуру
            fig_x = point.GetX();       // запоминаем координату X точки
            fig_y = point.GetY();       // запоминаем координату Y точки
        } // if (нажата клавиша 3)

        // Выбор фигуры - круглый фонарик
        if (KEY_DOWN(52))
        {
            current_figure = ROUND_FLASHLIGHT;  // изменяем текущую фигуру
            fig_x = roundFlashlight.GetX();     // запоминаем координату X фонарика
            fig_y = roundFlashlight.GetY();     // запоминаем координату Y фонарика
        } // if (нажата клавиша 4)

        // Выбор фигуры - отвертка
        if (KEY_DOWN(53))
        {
            current_figure = SCREWDRIVER;   // изменяем текущую фигуру
            fig_x = screwdriver.GetX();     // запоминаем координату X отвертки
            fig_y = screwdriver.GetY();     // запоминаем координату Y отвертки
        } // if (нажата клавиша 5)

        // Обработка управления для выбранной фигуры
        switch (current_figure)
        {
        case CIRCLE: // Управление кругом
            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;           // уменьшаем координату X
                circle.MoveTo(fig_x, fig_y);    // перемещаем круг
                Sleep(DELAY);                   // задержка
            } // if (нажата стрелка влево)

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;           // увеличиваем координату X
                circle.MoveTo(fig_x, fig_y);    // перемещаем круг
                Sleep(DELAY);                   // задержка
            } // if (нажата стрелка вправо)

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;           // уменьшаем координату Y
                circle.MoveTo(fig_x, fig_y);    // перемещаем круг
                Sleep(DELAY);                   // задержка
            } // if (нажата стрелка вверх)

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;           // увеличиваем координату Y
                circle.MoveTo(fig_x, fig_y);    // перемещаем круг
                Sleep(DELAY);                   // задержка
            } // if (нажата стрелка вниз)
            break;

        case RECT_FLASHLIGHT: // Управление прямоугольным фонариком
            was_pressed = false;  // сбрасываем флаг нажатия

            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;   // уменьшаем координату X
                was_pressed = true;     // устанавливаем флаг нажатия
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (нажата стрелка влево)

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;   // увеличиваем координату X
                was_pressed = true;     // устанавливаем флаг нажатия
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (нажата стрелка вправо)

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;   // уменьшаем координату Y
                was_pressed = true;     // устанавливаем флаг нажатия
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (нажата стрелка вверх)

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;   // увеличиваем координату Y
                was_pressed = true;     // устанавливаем флаг нажатия
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (нажата стрелка вниз)

            // если ни одна стрелка не была нажата, пропускаем обновление
            if (!was_pressed) { break; } // if (не было нажатия стрелок)

            // Проверяем столкновения только если фонарик не сломан
            if (rect_state == USUAL_FLASHLIGHT)
            {
                // Проверка столкновения со стенами
                if (fig_x < WALL_LEFT || fig_x + 40 > WALL_RIGHT ||
                    fig_y < WALL_TOP || fig_y + 120 > WALL_BOTTOM)
                {
                    rect_state = BROKEN_FLASHLIGHT; // ломаем фонарик
                } // if (фонарик вышел за границы стен)

                // Проверка столкновения с камнем
                if (fig_x < stone_x + STONE_WIDTH &&
                    fig_x + 40 > stone_x &&
                    fig_y < stone_y + STONE_HEIGHT &&
                    fig_y + 120 > stone_y)
                {
                    rect_state = BROKEN_FLASHLIGHT; // ломаем фонарик
                    roundFlashlight.Show(); // показываем круглый фонарик
                    screwdriver.Show();     // показываем отвертку
                    stone.Show();           // показываем камень
                } // if (фонарик столкнулся с камнем)
            } // if (фонарик не сломан)

            // Перемещаем и отображаем фонарик в зависимости от состояния
            if (rect_state == USUAL_FLASHLIGHT)
            {
                brokenRect.Hide();                  // скрываем сломанный фонарик
                rectFlashlight.MoveTo(fig_x, fig_y); // перемещаем целый фонарик
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (фонарик целый)
            else
            {
                rectFlashlight.Hide();              // скрываем целый фонарик
                brokenRect.MoveTo(fig_x, fig_y);    // перемещаем сломанный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // else (фонарик сломан)

            Sleep(DELAY);   // задержка
            break;

        case POINT: // Управление точкой
            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;           // уменьшаем координату X
                point.MoveTo(fig_x, fig_y);     // перемещаем точку
                Sleep(DELAY);                   // задержка
            } // if (нажата стрелка влево)

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;           // увеличиваем координату X
                point.MoveTo(fig_x, fig_y);     // перемещаем точку
                Sleep(DELAY);                   // задержка
            } // if (нажата стрелка вправо)

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;           // уменьшаем координату Y
                point.MoveTo(fig_x, fig_y);     // перемещаем точку
                Sleep(DELAY);                   // задержка
            } // if (нажата стрелка вверх)

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;           // увеличиваем координату Y
                point.MoveTo(fig_x, fig_y);     // перемещаем точку
                Sleep(DELAY);                   // задержка
            } // if (нажата стрелка вниз)
            break;

        case ROUND_FLASHLIGHT: // Управление круглым фонариком
            was_pressed = false;  // сбрасываем флаг нажатия

            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;   // уменьшаем координату X
                was_pressed = true;     // устанавливаем флаг нажатия
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (нажата стрелка влево)

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;   // увеличиваем координату X
                was_pressed = true;     // устанавливаем флаг нажатия
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (нажата стрелка вправо)

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;   // уменьшаем координату Y
                was_pressed = true;     // устанавливаем флаг нажатия
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (нажата стрелка вверх)

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;   // увеличиваем координату Y
                was_pressed = true;     // устанавливаем флаг нажатия
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (нажата стрелка вниз)

            // если ни одна стрелка не была нажата, пропускаем обновление
            if (!was_pressed) { break; } // if (не было нажатия стрелок)

            // Проверяем столкновения только если фонарик не сломан
            if (round_state == USUAL_FLASHLIGHT)
            {
                // Проверка столкновения со стенами
                if (fig_x < WALL_LEFT || fig_x + 50 > WALL_RIGHT ||
                    fig_y < WALL_TOP || fig_y + 100 > WALL_BOTTOM)
                {
                    round_state = BROKEN_FLASHLIGHT; // ломаем фонарик
                } // if (фонарик вышел за границы стен)

                // Проверка столкновения с камнем
                if (fig_x < stone_x + STONE_WIDTH &&
                    fig_x + 50 > stone_x &&
                    fig_y < stone_y + STONE_HEIGHT &&
                    fig_y + 100 > stone_y)
                {
                    round_state = BROKEN_FLASHLIGHT; // ломаем фонарик
                } // if (фонарик столкнулся с камнем)
            } // if (фонарик не сломан)

            // Перемещаем и отображаем фонарик в зависимости от состояния
            if (round_state == USUAL_FLASHLIGHT)
            {
                brokenRound.Hide();                 // скрываем сломанный фонарик
                roundFlashlight.MoveTo(fig_x, fig_y); // перемещаем целый фонарик
                rectFlashlight.Show();  // показываем прямоугольный фонарик
                roundFlashlight.Show(); // показываем круглый фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // if (фонарик целый)
            else
            {
                roundFlashlight.Hide();             // скрываем целый фонарик
                brokenRound.MoveTo(fig_x, fig_y);   // перемещаем сломанный фонарик
                screwdriver.Show();     // показываем отвертку
                stone.Show();           // показываем камень
            } // else (фонарик сломан)

            Sleep(DELAY);   // задержка
            break;

        case SCREWDRIVER: // Управление отверткой
            was_pressed = false;  // сбрасываем флаг нажатия

            // влево
            if (KEY_DOWN(VK_LEFT))
            {
                fig_x = fig_x - STEP;   // уменьшаем координату X
                was_pressed = true;     // устанавливаем флаг нажатия
            } // if (нажата стрелка влево)

            // вправо
            if (KEY_DOWN(VK_RIGHT))
            {
                fig_x = fig_x + STEP;   // увеличиваем координату X
                was_pressed = true;     // устанавливаем флаг нажатия
            } // if (нажата стрелка вправо)

            // вверх
            if (KEY_DOWN(VK_UP))
            {
                fig_y = fig_y - STEP;   // уменьшаем координату Y
                was_pressed = true;     // устанавливаем флаг нажатия
            } // if (нажата стрелка вверх)

            // вниз
            if (KEY_DOWN(VK_DOWN))
            {
                fig_y = fig_y + STEP;   // увеличиваем координату Y
                was_pressed = true;     // устанавливаем флаг нажатия
            } // if (нажата стрелка вниз)

            // если ни одна стрелка не была нажата, пропускаем обновление
            if (!was_pressed) { break; } // if (не было нажатия стрелок)

            screwdriver.MoveTo(fig_x, fig_y);   // перемещаем отвертку
            screwdriver.Show();     // показываем отвертку
            stone.Show();           // показываем камень

            // Проверяем столкновения для починки фонариков

            // Ремонт прямоугольного фонарика
            if (rect_state == BROKEN_FLASHLIGHT)
            {
                // Если отвертка попала в область сломанного фонарика (проверка пересечения прямоугольников)
                if (fig_x < brokenRect.GetX() + brokenRect.GetBodyWidth() &&   // левая граница отвертки < правой границы фонарика
                    fig_x + screwdriver.GetWidth() > brokenRect.GetX() &&      // правая граница отвертки > левой границы фонарика
                    fig_y < brokenRect.GetY() + brokenRect.GetBodyHeight() &&  // верхняя граница отвертки < нижней границы фонарика
                    fig_y + screwdriver.GetLength() + 10 > brokenRect.GetY())  // нижняя граница отвертки > верхней границы фонарика
                {
                    rect_state = USUAL_FLASHLIGHT;  // восстанавливаем состояние
                    rect_damage = 0;                // сбрасываем урон
                    brokenRect.Hide();              // скрываем сломанный фонарик
                    rectFlashlight.MoveTo(brokenRect.GetX(), brokenRect.GetY()); // перемещаем целый фонарик
                    rectFlashlight.Show();          // показываем целый фонарик
                    roundFlashlight.Show(); // показываем круглый фонарик
                    screwdriver.Show();     // показываем отвертку
                } // if (отвертка столкнулась с прямоугольным фонариком)
            } // if (прямоугольный фонарик сломан)

            // Ремонт круглого фонарика
            if (round_state == BROKEN_FLASHLIGHT)
            {
                // Если отвертка попала в область сломанного фонарика (проверка пересечения прямоугольников)
                if (fig_x < brokenRound.GetX() + brokenRound.GetBodyWidth() &&   // левая граница отвертки < правой границы фонарика
                    fig_x + screwdriver.GetWidth() > brokenRound.GetX() &&       // правая граница отвертки > левой границы фонарика
                    fig_y < brokenRound.GetY() + brokenRound.GetBodyHeight() &&  // верхняя граница отвертки < нижней границы фонарика
                    fig_y + screwdriver.GetLength() + 10 > brokenRound.GetY())   // нижняя граница отвертки > верхней границы фонарика
                {
                    round_state = USUAL_FLASHLIGHT; // восстанавливаем состояние
                    round_damage = 0;               // сбрасываем урон
                    brokenRound.Hide();             // скрываем сломанный фонарик
                    roundFlashlight.MoveTo(brokenRound.GetX(), brokenRound.GetY()); // перемещаем целый фонарик
                    roundFlashlight.Show();         // показываем целый фонарик
                    rectFlashlight.Show();  // показываем прямоугольный фонарик
                    screwdriver.Show();     // показываем отвертку
                } // if (отвертка столкнулась с круглым фонариком)
            } // if (круглый фонарик сломан)

            Sleep(DELAY);   // задержка
            break;
        } // switch (выбранная фигура)
    } // while (главный цикл программы)

    //===============================================================
    // Очистка ресурсов
    point.Hide();           // скрываем точку
    circle.Hide();          // скрываем круг
    rectFlashlight.Hide();  // скрываем прямоугольный фонарик
    roundFlashlight.Hide(); // скрываем круглый фонарик
    screwdriver.Hide();     // скрываем отвертку
    stone.Hide();           // скрываем камень
    brokenRect.Hide();      // скрываем сломанный прямоугольный фонарик
    brokenRound.Hide();     // скрываем сломанный круглый фонарик

    ReleaseDC(hwnd, hdc);   // освобождаем контекст устройства
    cout << "Program ended with code 0." << endl;
    return 0;
}