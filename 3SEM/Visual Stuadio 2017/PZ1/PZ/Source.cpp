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

#include "Header.h"
#include <iostream>

using namespace std;
extern HDC hdc;

// Макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*           Р Е А Л И З А Ц И Я  М Е Т О Д О В                 */
/****************************************************************/

/* =============== КЛАСС Location =============== */

Location::Location(int InitX, int InitY)
{
    x = InitX;
    y = InitY;
}

Location::~Location() {}

int Location::GetX() { return x; }
int Location::GetY() { return y; }

void Location::SetX(int NewX) { x = NewX; }
void Location::SetY(int NewY) { y = NewY; }

void Location::MoveTo(int NewX, int NewY)
{
    Hide();
    x = NewX;
    y = NewY;
    Show();
}

void Location::Show() { /* Ничего не делаем, реализация в наследниках */ }
void Location::Hide() { /* Ничего не делаем, реализация в наследниках */ }

/* =============== КЛАСС Point =============== */

Point::Point(int InitX, int InitY) : Location(InitX, InitY)
{
    visible = false;
}

Point::~Point() {}

bool Point::GetVisible() { return visible; }
void Point::SetVisible(bool NewVisible) { visible = NewVisible; }

void Point::Show()
{
    visible = true;
    SetPixel(hdc, x, y, RGB(255, 0, 0));
    SetPixel(hdc, x, y + 1, RGB(255, 0, 0));
    SetPixel(hdc, x + 1, y, RGB(255, 0, 0));
    SetPixel(hdc, x + 1, y + 1, RGB(255, 0, 0));
}

void Point::Hide()
{
    visible = false;
    SetPixel(hdc, x, y, RGB(0, 0, 255));
    SetPixel(hdc, x, y + 1, RGB(0, 0, 255));
    SetPixel(hdc, x + 1, y, RGB(0, 0, 255));
    SetPixel(hdc, x + 1, y + 1, RGB(0, 0, 255));
}

void Point::MoveTo(int NewX, int NewY)
{
    Hide();
    x = NewX;
    y = NewY;
    Show();
}

void Point::Drag(int Step)
{
    int FigX = x;
    int FigY = y;

    while (true)
    {
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        if (KEY_DOWN(VK_LEFT))
        {
            FigX = FigX - Step;
            MoveTo(FigX, FigY);
            Sleep(100);
        }

        if (KEY_DOWN(VK_RIGHT))
        {
            FigX = FigX + Step;
            MoveTo(FigX, FigY);
            Sleep(100);
        }

        if (KEY_DOWN(VK_UP))
        {
            FigY = FigY - Step;
            MoveTo(FigX, FigY);
            Sleep(100);
        }

        if (KEY_DOWN(VK_DOWN))
        {
            FigY = FigY + Step;
            MoveTo(FigX, FigY);
            Sleep(100);
        }
    }
}

/* =============== КЛАСС Circle =============== */

// Конструктор круга (класса Circle)
Circle::Circle(int InitX, int InitY, int InitRadius) :
    Point(InitX, InitY) {
    radius = InitRadius;
}

// Деконструктор
Circle::~Circle() {}

// Показать круг
void Circle::Show() {
    visible = true;	// значение видимости на правду

    // создаём ручку для рисования
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, Pen);

    // рисуем красный круг
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // удаляем ручку
    DeleteObject(Pen);
}

// Спрятать круг
void Circle::Hide() {
    visible = false;	// значение видимости на фальш

    // создаём ручку для рисования
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    SelectObject(hdc, Pen);

    // рисуем синий круг
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // удаляем ручку
    DeleteObject(Pen);
}

// Расширить круг
void Circle::Expand(int DeltaRad) {
    Hide();	// прячем круг

    // изменяем радиус
    radius = radius + DeltaRad;
    if (radius < 2) { radius = 2; } // проверка для Reduce

    Show();	// возвращаем круг
}

// Сжать круг
void Circle::Reduse(int DeltaRad) {
    Expand(-DeltaRad);
}

/* =============== КЛАСС Flashlight =============== */

// Конструктор фонарика
Flashlight::Flashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : Point(InitX, InitY)
{
    bodyWidth = InitBodyWidth;
    bodyHeight = InitBodyHeight;
    headWidth = InitHeadWidth;
    headHeight = InitHeadHeight;
    visible = false;
}

// Деструктор фонарика
Flashlight::~Flashlight() {}

// Показать фонарик
void Flashlight::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

    SelectObject(hdc, Pen);

    // 1. Рисуем корпус фонарика (прямоугольник)
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // 2. Рисуем головку фонарика (прямоугольник)
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // 3. Рисуем кнопку фонарика (маленький прямоугольник)
    int buttonWidth = 15;
    int buttonHeight = 20;
    int buttonX = x + bodyWidth - 25;
    int buttonY = y + bodyHeight - 30;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(Pen);
}

void Flashlight::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

    SelectObject(hdc, Pen);

    // Стираем все компоненты фонарика
    // 1. Корпус
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // 2. Головка
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // 3. Кнопка
    int buttonWidth = 15;
    int buttonHeight = 20;
    int buttonX = x + bodyWidth - 25;
    int buttonY = y + bodyHeight - 30;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(Pen);
}