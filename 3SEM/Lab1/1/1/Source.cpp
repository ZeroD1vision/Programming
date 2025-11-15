/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Windows Console Application                                 *
 * Project name  : Pt_1                                                        *
 * File name     : lib.cpp                                                     *
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

/* =============== КЛАСС Conflict =============== */

Conflict::Conflict(int InitX, int InitY, int InitWidth, int InitHeight)
    : Point(InitX, InitY)
{
    width = InitWidth;
    height = InitHeight;
}

Conflict::~Conflict() {}

/* =============== КЛАСС Circle =============== */

Circle::Circle(int InitX, int InitY, int InitRadius) : Point(InitX, InitY)
{
    radius = InitRadius;
}

Circle::~Circle() {}

void Circle::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, Pen);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
    DeleteObject(Pen);
}

void Circle::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    SelectObject(hdc, Pen);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
    DeleteObject(Pen);
}

void Circle::Expand(int DeltaRad)
{
    Hide();
    radius = radius + DeltaRad;
    if (radius < 2) { radius = 2; }
    Show();
}

void Circle::Reduse(int DeltaRad)
{
    Expand(-DeltaRad);
}

/* =============== БАЗОВЫЙ КЛАСС Flashlight =============== */

BaseFlashlight::BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight) : Point(InitX, InitY)
{
    bodyWidth = InitBodyWidth;
    bodyHeight = InitBodyHeight;
    headWidth = InitHeadWidth;
    headHeight = InitHeadHeight;
    broken = false;
}

BaseFlashlight::~BaseFlashlight() {}

/* =============== ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */

RectFlashlight::RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
}

RectFlashlight::~RectFlashlight() {}

void RectFlashlight::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, Pen);

    // Корпус фонарика
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка фонарика
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // Кнопка фонарика
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(Pen);
}

void RectFlashlight::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(Pen);
}

/* =============== СЛОМАННЫЙ ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */

BrokenRectFlashlight::BrokenRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RectFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
    broken = true; // Устанавливаем состояние "сломан"
}

BrokenRectFlashlight::~BrokenRectFlashlight() {}

void BrokenRectFlashlight::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

    SelectObject(hdc, Pen);

    // Основной корпус (используем унаследованные размеры)
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка фонарика
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // Кнопка
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // Трещины
    SelectObject(hdc, crackPen);
    MoveToEx(hdc, x + bodyWidth / 2, y + 5, NULL);
    LineTo(hdc, x + bodyWidth / 2, y + bodyHeight - 5);
    MoveToEx(hdc, x + 5, y + bodyHeight / 2, NULL);
    LineTo(hdc, x + bodyWidth - 5, y + bodyHeight / 2);
    MoveToEx(hdc, x + 10, y + 10, NULL);
    LineTo(hdc, x + bodyWidth - 10, y + bodyHeight - 10);

    DeleteObject(crackPen);
    DeleteObject(Pen);
}

void BrokenRectFlashlight::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    // Основной корпус
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // Кнопка
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(Pen);
}

/* =============== КРУГЛЫЙ ФОНАРИК =============== */

RoundFlashlight::RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
}

RoundFlashlight::~RoundFlashlight() {}

void RoundFlashlight::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    SelectObject(hdc, Pen);

    // Корпус фонарика (овал)
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка фонарика (треугольник)
    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;
    Polygon(hdc, triangle, 3);

    // Кнопка фонарика (круг)
    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    DeleteObject(Pen);
}

void RoundFlashlight::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;
    Polygon(hdc, triangle, 3);

    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    DeleteObject(Pen);
}

/* =============== СЛОМАННЫЙ КРУГЛЫЙ ФОНАРИК =============== */

BrokenRoundFlashlight::BrokenRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RoundFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
    broken = true; // Устанавливаем состояние "сломан"
}

BrokenRoundFlashlight::~BrokenRoundFlashlight() {}

void BrokenRoundFlashlight::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

    SelectObject(hdc, Pen);

    // Корпус фонарика (овал)
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Деформированная головка фонарика
    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2 - 2; // Смещена влево
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2 + 2; // Смещена вправо
    triangle[2].y = y - 3; // Немного поднята
    Polygon(hdc, triangle, 3);

    // Кнопка фонарика
    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    // Трещины
    SelectObject(hdc, crackPen);
    MoveToEx(hdc, x + bodyWidth * 0.3, y + bodyHeight * 0.2, NULL);
    LineTo(hdc, x + bodyWidth * 0.7, y + bodyHeight * 0.8);
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.7, NULL);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.3);

    DeleteObject(crackPen);
    DeleteObject(Pen);
}

void BrokenRoundFlashlight::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2 - 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2 + 2;
    triangle[2].y = y - 3;
    Polygon(hdc, triangle, 3);

    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    DeleteObject(Pen);
}

/* =============== КЛАСС Screwdriver =============== */

Screwdriver::Screwdriver(int InitX, int InitY, int InitWidth, int InitHeight)
    : Conflict(InitX, InitY, InitWidth, InitHeight)
{
}

Screwdriver::~Screwdriver() {}

void Screwdriver::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(139, 69, 19)); // Коричневый цвет
    HPEN ThinPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192)); // Серебряный цвет

    SelectObject(hdc, Pen);

    // Ручка отвертки
    Rectangle(hdc, x, y, x + width, y - height + 5);

    // Металлическая часть
    SelectObject(hdc, ThinPen);
    Rectangle(hdc, x + 3, y, x + width / 2 + 3, y + height);

    // Наконечник
    POINT tip[3];
    tip[0].x = x + width / 4 + 3;
    tip[0].y = y + height;
    tip[1].x = x + 3;
    tip[1].y = y + height + 10;
    tip[2].x = x + width / 2 + 3;
    tip[2].y = y + height + 10;
    Polygon(hdc, tip, 3);

    DeleteObject(Pen);
    DeleteObject(ThinPen);
}

void Screwdriver::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    // Ручка отвертки
    Rectangle(hdc, x, y, x + width, y - height + 5);

    // Металлическая часть
    SelectObject(hdc, Pen);
    Rectangle(hdc, x + 3, y, x + width / 2 + 3, y + height);

    // Наконечник
    POINT tip[3];
    tip[0].x = x + width / 4 + 3;
    tip[0].y = y + height;
    tip[1].x = x + 3;
    tip[1].y = y + height + 10;
    tip[2].x = x + width / 2 + 3;
    tip[2].y = y + height + 10;
    Polygon(hdc, tip, 3);

    DeleteObject(Pen);
}

/* =============== КЛАСС Stone =============== */

Stone::Stone(int InitX, int InitY, int InitWidth, int InitHeight)
    : Conflict(InitX, InitY, InitWidth, InitHeight)
{
}

Stone::~Stone() {}

void Stone::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(128, 128, 128)); // Серый цвет
    SelectObject(hdc, Pen);

    // Рисуем камень как неровный многоугольник
    POINT stoneShape[8];
    stoneShape[0].x = x + width / 4;
    stoneShape[0].y = y;
    stoneShape[1].x = x + width * 3 / 4;
    stoneShape[1].y = y;
    stoneShape[2].x = x + width;
    stoneShape[2].y = y + height / 3;
    stoneShape[3].x = x + width * 3 / 4;
    stoneShape[3].y = y + height * 2 / 3;
    stoneShape[4].x = x + width;
    stoneShape[4].y = y + height;
    stoneShape[5].x = x + width / 2;
    stoneShape[5].y = y + height;
    stoneShape[6].x = x;
    stoneShape[6].y = y + height * 2 / 3;
    stoneShape[7].x = x + width / 4;
    stoneShape[7].y = y + height / 3;

    Polygon(hdc, stoneShape, 8);

    // Добавляем текстуру камня
    HPEN CrackPen = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));
    SelectObject(hdc, CrackPen);

    MoveToEx(hdc, x + width / 3, y + height / 4, NULL);
    LineTo(hdc, x + width / 2, y + height / 2);
    MoveToEx(hdc, x + width * 2 / 3, y + height / 3, NULL);
    LineTo(hdc, x + width / 3, y + height * 3 / 4);

    DeleteObject(CrackPen);
    DeleteObject(Pen);
}

void Stone::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    // Стираем тем же белым цветом
    POINT stoneShape[8];
    stoneShape[0].x = x + width / 4;
    stoneShape[0].y = y;
    stoneShape[1].x = x + width * 3 / 4;
    stoneShape[1].y = y;
    stoneShape[2].x = x + width;
    stoneShape[2].y = y + height / 3;
    stoneShape[3].x = x + width * 3 / 4;
    stoneShape[3].y = y + height * 2 / 3;
    stoneShape[4].x = x + width;
    stoneShape[4].y = y + height;
    stoneShape[5].x = x + width / 2;
    stoneShape[5].y = y + height;
    stoneShape[6].x = x;
    stoneShape[6].y = y + height * 2 / 3;
    stoneShape[7].x = x + width / 4;
    stoneShape[7].y = y + height / 3;

    Polygon(hdc, stoneShape, 8);

    DeleteObject(Pen);
}