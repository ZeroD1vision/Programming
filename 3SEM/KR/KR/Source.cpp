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
 * Last Revision : 24.10.2025                                                  *
 * Comment       : ЛР №3. Фигура: фонарик                                      *
 ******************************************************************************/

#include "Header.h"
#include <iostream>
#include <cmath>

using namespace std;

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/* =============== КЛАСС Location =============== */
Location::Location(int InitX, int InitY) {
    x = InitX;
    y = InitY;
}
Location::~Location() {}
int Location::GetX() { return x; }
int Location::GetY() { return y; }
void Location::SetX(int NewX) { x = NewX; }
void Location::SetY(int NewY) { y = NewY; }

/* =============== КЛАСС Point =============== */
Point::Point(int InitX, int InitY) : Location(InitX, InitY) {
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
    SetPixel(hdc, x + 1, y + 1, RGB(255, 0, 0));
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
Circle::Circle(int InitX, int InitY, int InitRadius) :
    Conflict(InitX, InitY, InitRadius * 2, InitRadius * 2) {
    radius = InitRadius;
}

Circle::~Circle() {}

void Circle::Show() {
    visible = true;

    // Цвета для градиента и теней
    HBRUSH darkRedBrush = CreateSolidBrush(RGB(139, 0, 0));
    HBRUSH mediumRedBrush = CreateSolidBrush(RGB(205, 0, 0));
    HBRUSH lightRedBrush = CreateSolidBrush(RGB(255, 69, 0));
    HBRUSH highlightBrush = CreateSolidBrush(RGB(255, 99, 71));

    HPEN darkRedPen = CreatePen(PS_SOLID, 2, RGB(128, 0, 0));
    HPEN mediumRedPen = CreatePen(PS_SOLID, 2, RGB(178, 34, 34));
    HPEN lightRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

    HBRUSH shadowBrush = CreateSolidBrush(RGB(100, 40, 40));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(140, 80, 80));

    // Тени
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, darkRedPen);
    Ellipse(hdc, x - radius + 3, y - radius + 3, x + radius + 3, y + radius + 3);

    SelectObject(hdc, mediumShadowBrush);
    Ellipse(hdc, x - radius + 1, y - radius + 1, x + radius + 1, y + radius + 1);

    // Основной круг с градиентом
    SelectObject(hdc, darkRedBrush);
    SelectObject(hdc, darkRedPen);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    SelectObject(hdc, mediumRedBrush);
    SelectObject(hdc, mediumRedPen);
    Ellipse(hdc, x - radius * 0.7, y - radius * 0.7, x + radius * 0.7, y + radius * 0.7);

    SelectObject(hdc, lightRedBrush);
    SelectObject(hdc, lightRedPen);
    Ellipse(hdc, x - radius * 0.4, y - radius * 0.4, x + radius * 0.4, y + radius * 0.4);

    // Блик
    SelectObject(hdc, highlightBrush);
    Ellipse(hdc, x - radius * 0.2, y - radius * 0.2, x + radius * 0.1, y + radius * 0.1);

    // Контур
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    SelectObject(hdc, darkRedPen);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // Удаляем объекты
    DeleteObject(darkRedPen); DeleteObject(mediumRedPen); DeleteObject(lightRedPen);
    DeleteObject(darkRedBrush); DeleteObject(mediumRedBrush); DeleteObject(lightRedBrush);
    DeleteObject(highlightBrush); DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
}

void Circle::Hide() {
    visible = false;
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);
    Ellipse(hdc, x - radius - 2, y - radius - 2, x + radius + 5, y + radius + 5);
    DeleteObject(whitePen); DeleteObject(whiteBrush);
}

void Circle::Expand(int DeltaRad) {
    Hide();
    radius = radius + DeltaRad;
    if (radius < 2) radius = 2;
    Show();
}

void Circle::Reduse(int DeltaRad) { Expand(-DeltaRad); }

/* =============== КЛАСС Conflict =============== */
Conflict::Conflict(int InitX, int InitY, int InitWidth, int InitHeight)
    : Point(InitX, InitY) {
    width = InitWidth;
    height = InitHeight;
}
Conflict::~Conflict() {}

/* =============== КЛАСС Square =============== */
Square::Square(int InitX, int InitY, int InitWidth, int InitHeight)
    : Conflict(InitX, InitY, InitWidth, InitHeight) {}
Square::~Square() {}

void Square::Show() {
    visible = true;
    HBRUSH darkBlueBrush = CreateSolidBrush(RGB(0, 0, 139));
    HBRUSH mediumBlueBrush = CreateSolidBrush(RGB(0, 0, 205));
    HBRUSH lightBlueBrush = CreateSolidBrush(RGB(30, 144, 255));
    HPEN darkBluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 128));
    HPEN mediumBluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 200));
    HPEN lightBluePen = CreatePen(PS_SOLID, 1, RGB(70, 130, 255));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(40, 40, 100));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(80, 80, 140));

    // Тени
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    Rectangle(hdc, x + 4, y + 4, x + width + 4, y + height + 4);
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, x + 2, y + 2, x + width + 2, y + height + 2);

    // Основной квадрат
    SelectObject(hdc, darkBlueBrush);
    SelectObject(hdc, darkBluePen);
    Rectangle(hdc, x, y + height / 2, x + width, y + height);
    SelectObject(hdc, mediumBlueBrush);
    Rectangle(hdc, x, y + height / 4, x + width, y + height / 2);
    SelectObject(hdc, lightBlueBrush);
    Rectangle(hdc, x, y, x + width, y + height / 4);

    // Блики
    HPEN highlightPen = CreatePen(PS_SOLID, 1, RGB(100, 150, 255));
    HBRUSH highlightBrush = CreateSolidBrush(RGB(100, 150, 255));
    SelectObject(hdc, highlightBrush);
    SelectObject(hdc, highlightPen);
    Rectangle(hdc, x + 2, y + 2, x + width - 2, y + 4);
    Rectangle(hdc, x + 2, y + 2, x + 4, y + height - 2);

    // Контур
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    SelectObject(hdc, darkBluePen);
    Rectangle(hdc, x, y, x + width, y + height);

    // Удаляем объекты
    DeleteObject(darkBluePen); DeleteObject(mediumBluePen); DeleteObject(lightBluePen);
    DeleteObject(highlightPen); DeleteObject(darkBlueBrush); DeleteObject(mediumBlueBrush);
    DeleteObject(lightBlueBrush); DeleteObject(highlightBrush);
    DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
}

void Square::Hide() {
    visible = false;
    HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);
    Rectangle(hdc, x - 2, y - 2, x + width + 6, y + height + 6);
    DeleteObject(whitePen); DeleteObject(whiteBrush);
}

/* =============== КЛАСС Screwdriver =============== */
Screwdriver::Screwdriver(int InitX, int InitY, int InitWidth, int InitHeight)
    : Conflict(InitX, InitY, InitWidth, InitHeight) {}
Screwdriver::~Screwdriver() {}

void Screwdriver::Show() {
    visible = true;
    HBRUSH handleBrush = CreateSolidBrush(RGB(139, 69, 19));
    HBRUSH metalBrush = CreateSolidBrush(RGB(192, 192, 192));
    HPEN darkPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, handleBrush);
    SelectObject(hdc, darkPen);
    Rectangle(hdc, x, y, x + width, y - height + 5);
    SelectObject(hdc, metalBrush);
    Rectangle(hdc, x + 3, y, x + width / 2 + 3, y + height);
    POINT tip[3];
    tip[0].x = x + width / 4 + 3; tip[0].y = y + height;
    tip[1].x = x + 3; tip[1].y = y + height + 10;
    tip[2].x = x + width / 2 + 3; tip[2].y = y + height + 10;
    Polygon(hdc, tip, 3);
    DeleteObject(darkPen); DeleteObject(metalBrush); DeleteObject(handleBrush);
}

void Screwdriver::Hide() {
    visible = true;
    HBRUSH handleBrush = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH metalBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN darkPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, handleBrush);
    SelectObject(hdc, darkPen);
    Rectangle(hdc, x, y, x + width, y - height + 5);
    SelectObject(hdc, metalBrush);
    Rectangle(hdc, x + 3, y, x + width / 2 + 3, y + height);
    POINT tip[3];
    tip[0].x = x + width / 4 + 3; tip[0].y = y + height;
    tip[1].x = x + 3; tip[1].y = y + height + 10;
    tip[2].x = x + width / 2 + 3; tip[2].y = y + height + 10;
    Polygon(hdc, tip, 3);
    DeleteObject(darkPen); DeleteObject(metalBrush); DeleteObject(handleBrush);
}

/* =============== КЛАСС Stone =============== */
Stone::Stone(int InitX, int InitY, int InitWidth, int InitHeight)
    : Conflict(InitX, InitY, InitWidth, InitHeight) {}
Stone::~Stone() {}

void Stone::Show() {
    visible = true;
    HBRUSH darkStoneBrush = CreateSolidBrush(RGB(80, 80, 80));
    HBRUSH mediumStoneBrush = CreateSolidBrush(RGB(120, 120, 120));
    HBRUSH lightStoneBrush = CreateSolidBrush(RGB(160, 160, 160));
    HPEN stonePen = CreatePen(PS_SOLID, 2, RGB(60, 60, 60));
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(40, 40, 40));
    HPEN highlightPen = CreatePen(PS_SOLID, 1, RGB(180, 180, 180));
    HBRUSH stoneShadowBrush = CreateSolidBrush(RGB(60, 60, 60));

    // Тень
    SelectObject(hdc, stoneShadowBrush);
    SelectObject(hdc, stonePen);
    POINT shadowShape[8];
    shadowShape[0].x = x + width / 4 + 2; shadowShape[0].y = y + 2;
    shadowShape[1].x = x + width * 3 / 4 + 2; shadowShape[1].y = y + 2;
    shadowShape[2].x = x + width + 2; shadowShape[2].y = y + height / 3 + 2;
    shadowShape[3].x = x + width * 3 / 4 + 2; shadowShape[3].y = y + height * 2 / 3 + 2;
    shadowShape[4].x = x + width + 2; shadowShape[4].y = y + height + 2;
    shadowShape[5].x = x + width / 2 + 2; shadowShape[5].y = y + height + 2;
    shadowShape[6].x = x + 2; shadowShape[6].y = y + height * 2 / 3 + 2;
    shadowShape[7].x = x + width / 4 + 2; shadowShape[7].y = y + height / 3 + 2;
    Polygon(hdc, shadowShape, 8);

    // Основной камень
    SelectObject(hdc, darkStoneBrush);
    POINT stoneShape[8];
    stoneShape[0].x = x + width / 4; stoneShape[0].y = y;
    stoneShape[1].x = x + width * 3 / 4; stoneShape[1].y = y;
    stoneShape[2].x = x + width; stoneShape[2].y = y + height / 3;
    stoneShape[3].x = x + width * 3 / 4; stoneShape[3].y = y + height * 2 / 3;
    stoneShape[4].x = x + width; stoneShape[4].y = y + height;
    stoneShape[5].x = x + width / 2; stoneShape[5].y = y + height;
    stoneShape[6].x = x; stoneShape[6].y = y + height * 2 / 3;
    stoneShape[7].x = x + width / 4; stoneShape[7].y = y + height / 3;
    Polygon(hdc, stoneShape, 8);

    // Блик и трещины
    SelectObject(hdc, highlightPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    MoveToEx(hdc, x + width / 3, y + height / 5, NULL);
    LineTo(hdc, x + width * 2 / 3, y + height / 6);
    LineTo(hdc, x + width * 3 / 4, y + height / 4);
    SelectObject(hdc, crackPen);
    MoveToEx(hdc, x + width / 3, y + height / 4, NULL);
    LineTo(hdc, x + width / 2, y + height / 2);
    MoveToEx(hdc, x + width * 2 / 3, y + height / 3, NULL);
    LineTo(hdc, x + width / 3, y + height * 3 / 4);

    DeleteObject(stonePen); DeleteObject(crackPen); DeleteObject(highlightPen);
    DeleteObject(darkStoneBrush); DeleteObject(mediumStoneBrush); DeleteObject(lightStoneBrush);
    DeleteObject(stoneShadowBrush);
}

void Stone::Hide() {
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);
    POINT stoneShape[8];
    stoneShape[0].x = x + width / 4; stoneShape[0].y = y;
    stoneShape[1].x = x + width * 3 / 4; stoneShape[1].y = y;
    stoneShape[2].x = x + width; stoneShape[2].y = y + height / 3;
    stoneShape[3].x = x + width * 3 / 4; stoneShape[3].y = y + height * 2 / 3;
    stoneShape[4].x = x + width; stoneShape[4].y = y + height;
    stoneShape[5].x = x + width / 2; stoneShape[5].y = y + height;
    stoneShape[6].x = x; stoneShape[6].y = y + height * 2 / 3;
    stoneShape[7].x = x + width / 4; stoneShape[7].y = y + height / 3;
    Polygon(hdc, stoneShape, 8);
    DeleteObject(Pen);
}

/* =============== КЛАСС Battery =============== */
Battery::Battery(int InitX, int InitY, int InitWidth, int InitHeight)
    : Conflict(InitX, InitY, InitWidth, InitHeight) {}
Battery::~Battery() {}

void Battery::Show()
{
    visible = true;

    // Создаем градиентные кисти для более продвинутой батарейки
    HBRUSH darkSilverBrush = CreateSolidBrush(RGB(150, 150, 160));
    HBRUSH mediumSilverBrush = CreateSolidBrush(RGB(180, 180, 190));
    HBRUSH lightSilverBrush = CreateSolidBrush(RGB(210, 210, 220));

    HBRUSH darkGreenBrush = CreateSolidBrush(RGB(0, 100, 0));
    HBRUSH mediumGreenBrush = CreateSolidBrush(RGB(0, 150, 0));
    HBRUSH lightGreenBrush = CreateSolidBrush(RGB(0, 200, 0));
    HBRUSH highlightGreenBrush = CreateSolidBrush(RGB(100, 255, 100));

    HPEN darkPen = CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
    HPEN mediumPen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
    HPEN lightPen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));

    HBRUSH shadowBrush = CreateSolidBrush(RGB(100, 100, 110));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(120, 120, 130));

    // Тени
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, darkPen);
    Rectangle(hdc, x + 3, y + 3, x + width + 3, y + height + 3);

    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, x + 1, y + 1, x + width + 1, y + height + 1);

    // Основной корпус батарейки
    SelectObject(hdc, darkSilverBrush);
    SelectObject(hdc, darkPen);
    Rectangle(hdc, x, y, x + width, y + height);

    SelectObject(hdc, mediumSilverBrush);
    Rectangle(hdc, x + width * 0.1, y + height * 0.1,
        x + width * 0.9, y + height * 0.9);

    SelectObject(hdc, lightSilverBrush);
    Rectangle(hdc, x + width * 0.2, y + height * 0.2,
        x + width * 0.8, y + height * 0.8);

    // Верхний контакт
    int contactWidth = width / 3;
    int contactHeight = height / 6;
    SelectObject(hdc, darkSilverBrush);
    Rectangle(hdc, x + (width - contactWidth) / 2, y - contactHeight,
        x + (width + contactWidth) / 2, y);

    SelectObject(hdc, mediumSilverBrush);
    Rectangle(hdc, x + (width - contactWidth) / 2 + 1, y - contactHeight + 1,
        x + (width + contactWidth) / 2 - 1, y - 1);

    // Индикатор заряда с градиентом
    int indicatorWidth = width * 0.6;
    int indicatorHeight = height * 0.4;
    int indicatorX = x + (width - indicatorWidth) / 2;
    int indicatorY = y + height * 0.3;

    SelectObject(hdc, darkGreenBrush);
    Rectangle(hdc, indicatorX, indicatorY,
        indicatorX + indicatorWidth, indicatorY + indicatorHeight);

    SelectObject(hdc, mediumGreenBrush);
    Rectangle(hdc, indicatorX + 2, indicatorY + 2,
        indicatorX + indicatorWidth - 2, indicatorY + indicatorHeight * 0.7);

    SelectObject(hdc, lightGreenBrush);
    Rectangle(hdc, indicatorX + 4, indicatorY + 4,
        indicatorX + indicatorWidth - 4, indicatorY + indicatorHeight * 0.5);

    SelectObject(hdc, highlightGreenBrush);
    Rectangle(hdc, indicatorX + 6, indicatorY + 6,
        indicatorX + indicatorWidth - 6, indicatorY + indicatorHeight * 0.3);

    // Знак плюса
    HPEN plusPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, plusPen);
    int centerX = x + width / 2;
    int centerY = y + height / 2;

    // Горизонтальная линия плюса
    MoveToEx(hdc, centerX - indicatorWidth / 4, centerY, NULL);
    LineTo(hdc, centerX + indicatorWidth / 4, centerY);

    // Вертикальная линия плюса
    MoveToEx(hdc, centerX, centerY - indicatorHeight / 4, NULL);
    LineTo(hdc, centerX, centerY + indicatorHeight / 4);

    // Контур батарейки
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    SelectObject(hdc, darkPen);
    Rectangle(hdc, x, y - contactHeight, x + width, y + height);

    // Удаляем объекты
    DeleteObject(plusPen);
    DeleteObject(darkPen); DeleteObject(mediumPen); DeleteObject(lightPen);
    DeleteObject(darkSilverBrush); DeleteObject(mediumSilverBrush); DeleteObject(lightSilverBrush);
    DeleteObject(darkGreenBrush); DeleteObject(mediumGreenBrush); DeleteObject(lightGreenBrush);
    DeleteObject(highlightGreenBrush);
    DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
}

void Battery::Hide()
{
    visible = false;
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

    SelectObject(hdc, whitePen);
    SelectObject(hdc, whiteBrush);

    // Замазываем всю область батарейки с запасом для контакта
    int contactHeight = height / 6;
    Rectangle(hdc, x - 5, y - contactHeight - 5, x + width + 10, y + height + 10);

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}

/* =============== БАЗОВЫЙ КЛАСС Flashlight =============== */
BaseFlashlight::BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight) : Point(InitX, InitY) {
    bodyWidth = InitBodyWidth;
    bodyHeight = InitBodyHeight;
    headWidth = InitHeadWidth;
    headHeight = InitHeadHeight;
}
BaseFlashlight::~BaseFlashlight() {}

void BaseFlashlight::Show()
{
    visible = true;
    PrintLight();   // свет
    PrintBody();    // корпус
    PrintHead();    // головка
    PrintCrack();   // трещины
    PrintButton();  // кнопка
}

void BaseFlashlight::Hide()
{
    visible = false;
    // Создаем белые кисти и перья для замазывания
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);

    // Замазываем всю область фонарика с большим запасом для света
    int left = x - headWidth / 2;
    int top = y - headHeight * 2;
    int right = x + bodyWidth + headWidth / 2;
    int bottom = y + bodyHeight + 5;

    Rectangle(hdc, left, top, right, bottom);

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}

/* =============== ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */
RectFlashlight::RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
RectFlashlight::~RectFlashlight() {}

void RectFlashlight::PrintBody()
{
    HBRUSH darkMetalBrush = CreateSolidBrush(RGB(70, 70, 80));
    HBRUSH mediumMetalBrush = CreateSolidBrush(RGB(90, 90, 100));
    HBRUSH lightMetalBrush = CreateSolidBrush(RGB(110, 110, 120));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(50, 50, 60));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(60, 60, 70));
    HPEN softDarkPen = CreatePen(PS_SOLID, 1, RGB(65, 65, 75));
    HPEN softMediumPen = CreatePen(PS_SOLID, 1, RGB(85, 85, 95));

    // Тени
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    Rectangle(hdc, x + 3, y + 3, x + bodyWidth + 3, y + bodyHeight + 3);
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, x + 1, y + 1, x + bodyWidth + 1, y + bodyHeight + 1);

    // Корпус с градиентом
    SelectObject(hdc, darkMetalBrush);
    SelectObject(hdc, softDarkPen);
    Rectangle(hdc, x, y, x + bodyWidth * 0.3, y + bodyHeight);
    SelectObject(hdc, mediumMetalBrush);
    Rectangle(hdc, x + bodyWidth * 0.3, y, x + bodyWidth * 0.7, y + bodyHeight);
    SelectObject(hdc, lightMetalBrush);
    Rectangle(hdc, x + bodyWidth * 0.7, y, x + bodyWidth, y + bodyHeight);

    DeleteObject(softDarkPen); DeleteObject(softMediumPen);
    DeleteObject(darkMetalBrush); DeleteObject(mediumMetalBrush); DeleteObject(lightMetalBrush);
    DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
}

void RectFlashlight::PrintHead()
{
    HBRUSH darkHeadBrush = CreateSolidBrush(RGB(60, 60, 70));
    HBRUSH mediumHeadBrush = CreateSolidBrush(RGB(80, 80, 90));
    HBRUSH lightHeadBrush = CreateSolidBrush(RGB(100, 100, 110));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(50, 50, 60));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(60, 60, 70));
    HPEN softDarkPen = CreatePen(PS_SOLID, 1, RGB(65, 65, 75));

    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;

    // Тени
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    Rectangle(hdc, headX + 2, headY + 2, headX + headWidth + 2, headY + headHeight + 2);
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, headX + 1, headY + 1, headX + headWidth + 1, headY + headHeight + 1);

    // Головка с градиентом
    SelectObject(hdc, darkHeadBrush);
    SelectObject(hdc, softDarkPen);
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight * 0.2);
    SelectObject(hdc, mediumHeadBrush);
    Rectangle(hdc, headX, headY + headHeight * 0.2, headX + headWidth, headY + headHeight * 0.5);
    SelectObject(hdc, lightHeadBrush);
    Rectangle(hdc, headX, headY + headHeight * 0.5, headX + headWidth, headY + headHeight * 0.8);
    SelectObject(hdc, CreateSolidBrush(RGB(120, 120, 130)));
    Rectangle(hdc, headX, headY + headHeight * 0.8, headX + headWidth, headY + headHeight);

    DeleteObject(softDarkPen);
    DeleteObject(darkHeadBrush); DeleteObject(mediumHeadBrush); DeleteObject(lightHeadBrush);
    DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
}

void RectFlashlight::PrintButton()
{
    HBRUSH lightRedBrush = CreateSolidBrush(RGB(220, 40, 40));
    HBRUSH mediumRedBrush = CreateSolidBrush(RGB(180, 20, 20));
    HBRUSH darkRedBrush = CreateSolidBrush(RGB(140, 0, 0));
    HBRUSH indicatorBrush = CreateSolidBrush(RGB(255, 255, 200));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(50, 50, 60));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(60, 60, 70));

    int buttonWidth = 14;
    int buttonHeight = 20;
    int buttonX = x + bodyWidth - 28;
    int buttonY = y + bodyHeight - 35;

    // Тени
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    Rectangle(hdc, buttonX + 2, buttonY + 2, buttonX + buttonWidth + 2, buttonY + buttonHeight + 2);
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, buttonX + 1, buttonY + 1, buttonX + buttonWidth + 1, buttonY + buttonHeight + 1);

    // Кнопка с градиентом
    SelectObject(hdc, lightRedBrush);
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight * 0.3);
    SelectObject(hdc, mediumRedBrush);
    Rectangle(hdc, buttonX, buttonY + buttonHeight * 0.3, buttonX + buttonWidth, buttonY + buttonHeight * 0.7);
    SelectObject(hdc, darkRedBrush);
    Rectangle(hdc, buttonX, buttonY + buttonHeight * 0.7, buttonX + buttonWidth, buttonY + buttonHeight);

    // Индикатор
    SelectObject(hdc, indicatorBrush);
    Ellipse(hdc, buttonX + buttonWidth * 0.3, buttonY + buttonHeight * 0.4,
        buttonX + buttonWidth * 0.7, buttonY + buttonHeight * 0.6);

    DeleteObject(lightRedBrush); DeleteObject(mediumRedBrush); DeleteObject(darkRedBrush);
    DeleteObject(indicatorBrush); DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
}

void RectFlashlight::PrintLight()
{
    // Обычный фонарик не светит
}

void RectFlashlight::PrintCrack()
{
    // Обычный фонарик не имеет трещин
}

void RectFlashlight::Hide() {
    // Просто вызываем Hide из BaseFlashlight - он уже делает всё правильно
    BaseFlashlight::Hide();
}

/* =============== СЛОМАННЫЙ ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */
BrokenRectFlashlight::BrokenRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RectFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
BrokenRectFlashlight::~BrokenRectFlashlight() {}

void BrokenRectFlashlight::PrintCrack()
{
    HPEN crackPen = CreatePen(PS_SOLID, 2, RGB(30, 30, 35));
    HPEN deepCrackPen = CreatePen(PS_SOLID, 1, RGB(120, 30, 30));

    SelectObject(hdc, crackPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // Трещины на корпусе
    MoveToEx(hdc, x + bodyWidth * 0.2, y + 5, NULL);
    LineTo(hdc, x + bodyWidth * 0.4, y + bodyHeight * 0.3);
    LineTo(hdc, x + bodyWidth * 0.6, y + bodyHeight * 0.6);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight - 5);
    MoveToEx(hdc, x + bodyWidth * 0.5, y + 10, NULL);
    LineTo(hdc, x + bodyWidth * 0.5, y + bodyHeight - 10);

    // Трещины на головке
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    SelectObject(hdc, deepCrackPen);
    MoveToEx(hdc, headX + headWidth * 0.2, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.8, headY + headHeight - 5);
    MoveToEx(hdc, headX + headWidth * 0.8, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.2, headY + headHeight - 5);

    // Сломанная кнопка
    int buttonWidth = 14;
    int buttonHeight = 20;
    int buttonX = x + bodyWidth - 28;
    int buttonY = y + bodyHeight - 35;
    MoveToEx(hdc, buttonX, buttonY, NULL);
    LineTo(hdc, buttonX + buttonWidth, buttonY + buttonHeight);
    MoveToEx(hdc, buttonX + buttonWidth, buttonY, NULL);
    LineTo(hdc, buttonX, buttonY + buttonHeight);

    DeleteObject(crackPen); DeleteObject(deepCrackPen);
}

void BrokenRectFlashlight::Hide()
{
    visible = false;
    // Создаем белые кисти и перья для замазывания
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);

    // Замазываем трещины дополнительными линиями
    // Основная трещина через корпус
    MoveToEx(hdc, x + bodyWidth * 0.2, y + 5, NULL);
    LineTo(hdc, x + bodyWidth * 0.4, y + bodyHeight * 0.3);
    LineTo(hdc, x + bodyWidth * 0.6, y + bodyHeight * 0.6);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight - 5);
    MoveToEx(hdc, x + bodyWidth * 0.5, y + 10, NULL);
    LineTo(hdc, x + bodyWidth * 0.5, y + bodyHeight - 10);

    // Трещина на головке
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    MoveToEx(hdc, headX + headWidth * 0.2, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.8, headY + headHeight - 5);
    MoveToEx(hdc, headX + headWidth * 0.8, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.2, headY + headHeight - 5);

    // Сломанная кнопка
    int buttonWidth = 14;
    int buttonHeight = 20;
    int buttonX = x + bodyWidth - 28;
    int buttonY = y + bodyHeight - 35;
    MoveToEx(hdc, buttonX, buttonY, NULL);
    LineTo(hdc, buttonX + buttonWidth, buttonY + buttonHeight);
    MoveToEx(hdc, buttonX + buttonWidth, buttonY, NULL);
    LineTo(hdc, buttonX, buttonY + buttonHeight);

    // Замазываем основную область фонарика
    BaseFlashlight::Hide();

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}

/* =============== ГОРЯЩИЙ ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */
LitRectFlashlight::LitRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RectFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
LitRectFlashlight::~LitRectFlashlight() {}

void LitRectFlashlight::PrintLight()
{
    // Свет из головки (теплые оттенки желтого)
    HBRUSH darkYellowBrush = CreateSolidBrush(RGB(255, 220, 100));
    HBRUSH mediumYellowBrush = CreateSolidBrush(RGB(255, 240, 160));
    HBRUSH lightYellowBrush = CreateSolidBrush(RGB(255, 250, 200));
    HBRUSH highlightBrush = CreateSolidBrush(RGB(255, 255, 230));

    HPEN yellowPen = CreatePen(PS_SOLID, 1, RGB(255, 220, 100));
    HPEN lightYellowPen = CreatePen(PS_SOLID, 1, RGB(255, 240, 180));

    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;

    // Рисуем луч света (трапеция) с градиентом
    POINT light[4];
    light[0].x = headX + headWidth * 0.2;
    light[0].y = headY;
    light[1].x = headX + headWidth * 0.8;
    light[1].y = headY;
    light[2].x = headX + headWidth * 1.5;
    light[2].y = headY - headHeight * 2;
    light[3].x = headX - headWidth * 0.5;
    light[3].y = headY - headHeight * 2;

    // Основной луч
    SelectObject(hdc, darkYellowBrush);
    SelectObject(hdc, yellowPen);
    Polygon(hdc, light, 4);

    // Внутренняя часть луча (более светлая)
    POINT innerLight[4];
    for (int i = 0; i < 4; i++) {
        innerLight[i].x = (light[i].x + headX + headWidth / 2) / 2;
        innerLight[i].y = (light[i].y + headY) / 2;
    }
    SelectObject(hdc, mediumYellowBrush);
    SelectObject(hdc, lightYellowPen);
    Polygon(hdc, innerLight, 4);

    // Яркое ядро луча
    POINT coreLight[4];
    for (int i = 0; i < 4; i++) {
        coreLight[i].x = (innerLight[i].x + headX + headWidth / 2) / 2;
        coreLight[i].y = (innerLight[i].y + headY) / 2;
    }
    SelectObject(hdc, lightYellowBrush);
    Polygon(hdc, coreLight, 4);

    // Центральная точка (самое яркое)
    POINT centerLight[4];
    for (int i = 0; i < 4; i++) {
        centerLight[i].x = (coreLight[i].x + headX + headWidth / 2) / 2;
        centerLight[i].y = (coreLight[i].y + headY) / 2;
    }
    SelectObject(hdc, highlightBrush);
    Polygon(hdc, centerLight, 4);

    // Лучики света
    HPEN rayPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 180));
    SelectObject(hdc, rayPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // Рисуем несколько лучей
    for (int i = 0; i < 8; i++) {
        double angle = 3.14159 * i / 4;
        int rayLength = headHeight * 2.5;
        MoveToEx(hdc, headX + headWidth / 2, headY, NULL);
        LineTo(hdc, headX + headWidth / 2 + cos(angle) * rayLength,
            headY + sin(angle) * rayLength);
    }

    DeleteObject(yellowPen); DeleteObject(lightYellowPen); DeleteObject(rayPen);
    DeleteObject(darkYellowBrush); DeleteObject(mediumYellowBrush); DeleteObject(lightYellowBrush);
    DeleteObject(highlightBrush);
}

void LitRectFlashlight::Hide()
{
    visible = false;

    // Создаем белые кисти и перья для замазывания
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);

    // Замазываем область света отдельно (трапецию)
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;

    POINT light[4];
    light[0].x = headX + headWidth * 0.2;
    light[0].y = headY;
    light[1].x = headX + headWidth * 0.8;
    light[1].y = headY;
    light[2].x = headX + headWidth * 1.5;
    light[2].y = headY - headHeight * 2;
    light[3].x = headX - headWidth * 0.5;
    light[3].y = headY - headHeight * 2;

    // Замазываем область света
    Polygon(hdc, light, 4);

    // Замазываем лучики
    for (int i = 0; i < 8; i++) {
        double angle = 3.14159 * i / 4;
        int rayLength = headHeight * 2.5;
        MoveToEx(hdc, headX + headWidth / 2, headY, NULL);
        LineTo(hdc, headX + headWidth / 2 + cos(angle) * rayLength,
            headY + sin(angle) * rayLength);
    }

    // Замазываем основную область фонарика
    BaseFlashlight::Hide();

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}

/* =============== КРУГЛЫЙ ФОНАРИК =============== */
RoundFlashlight::RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
RoundFlashlight::~RoundFlashlight() {}

void RoundFlashlight::PrintBody()
{
    HBRUSH darkBaseBrush = CreateSolidBrush(RGB(80, 80, 90));
    HBRUSH mediumBaseBrush = CreateSolidBrush(RGB(100, 100, 110));
    HBRUSH lightBaseBrush = CreateSolidBrush(RGB(120, 120, 130));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(60, 60, 70));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(70, 70, 80));
    HPEN softDarkPen = CreatePen(PS_SOLID, 1, RGB(75, 75, 85));
    HPEN softMediumPen = CreatePen(PS_SOLID, 1, RGB(95, 95, 105));

    // Основание с тенями
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, softDarkPen);
    Ellipse(hdc, x + 3, y + 3, x + bodyWidth + 3, y + bodyHeight + 3);
    SelectObject(hdc, mediumShadowBrush);
    Ellipse(hdc, x + 1, y + 1, x + bodyWidth + 1, y + bodyHeight + 1);
    SelectObject(hdc, darkBaseBrush);
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);
    SelectObject(hdc, mediumBaseBrush);
    Ellipse(hdc, x + bodyWidth * 0.1, y + bodyHeight * 0.1,
        x + bodyWidth * 0.9, y + bodyHeight * 0.9);
    SelectObject(hdc, lightBaseBrush);
    Ellipse(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.2,
        x + bodyWidth * 0.8, y + bodyHeight * 0.8);

    DeleteObject(softDarkPen); DeleteObject(softMediumPen);
    DeleteObject(darkBaseBrush); DeleteObject(mediumBaseBrush); DeleteObject(lightBaseBrush);
    DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
}

void RoundFlashlight::PrintHead()
{
    HBRUSH darkShadeBrush = CreateSolidBrush(RGB(210, 200, 180));
    HBRUSH mediumShadeBrush = CreateSolidBrush(RGB(230, 220, 200));
    HBRUSH lightShadeBrush = CreateSolidBrush(RGB(250, 240, 220));
    HBRUSH shadeShadowBrush = CreateSolidBrush(RGB(180, 170, 160));

    // Абажур
    POINT shade[7];
    int shadeTop = y - headHeight;
    int shadeBottom = y;
    shade[0].x = x + bodyWidth / 2 - headWidth * 0.3; shade[0].y = shadeTop;
    shade[1].x = x + bodyWidth / 2 + headWidth * 0.3; shade[1].y = shadeTop;
    shade[2].x = x + bodyWidth / 2 + headWidth * 0.45; shade[2].y = shadeTop + headHeight * 0.3;
    shade[3].x = x + bodyWidth / 2 + headWidth * 0.5; shade[3].y = shadeTop + headHeight * 0.6;
    shade[4].x = x + bodyWidth / 2 + headWidth * 0.45; shade[4].y = shadeBottom;
    shade[5].x = x + bodyWidth / 2 - headWidth * 0.45; shade[5].y = shadeBottom;
    shade[6].x = x + bodyWidth / 2 - headWidth * 0.5; shade[6].y = shadeTop + headHeight * 0.6;

    // Тень абажура
    POINT shadowShade[7];
    for (int i = 0; i < 7; i++) {
        shadowShade[i].x = shade[i].x + 2;
        shadowShade[i].y = shade[i].y + 2;
    }
    SelectObject(hdc, shadeShadowBrush);
    Polygon(hdc, shadowShade, 7);

    // Основной абажур с градиентом
    SelectObject(hdc, darkShadeBrush);
    Polygon(hdc, shade, 7);

    // Внутренняя часть абажура
    POINT innerShade[7];
    for (int i = 0; i < 7; i++) {
        innerShade[i].x = (shade[i].x + x + bodyWidth / 2) / 2;
        innerShade[i].y = (shade[i].y + y - headHeight / 2) / 2;
    }
    SelectObject(hdc, mediumShadeBrush);
    Polygon(hdc, innerShade, 7);

    // Центральная часть абажура
    POINT centerShade[7];
    for (int i = 0; i < 7; i++) {
        centerShade[i].x = (innerShade[i].x + x + bodyWidth / 2) / 2;
        centerShade[i].y = (innerShade[i].y + y - headHeight / 2) / 2;
    }
    SelectObject(hdc, lightShadeBrush);
    Polygon(hdc, centerShade, 7);

    DeleteObject(darkShadeBrush); DeleteObject(mediumShadeBrush); DeleteObject(lightShadeBrush);
    DeleteObject(shadeShadowBrush);
}

void RoundFlashlight::PrintButton()
{
    HBRUSH darkEmeraldBrush = CreateSolidBrush(RGB(0, 100, 80));
    HBRUSH mediumEmeraldBrush = CreateSolidBrush(RGB(0, 150, 120));
    HBRUSH lightEmeraldBrush = CreateSolidBrush(RGB(0, 200, 160));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(60, 60, 70));

    int buttonRadius = 10;
    int buttonX = x + bodyWidth - 25;
    int buttonY = y + bodyHeight - 25;

    // Тень кнопки
    SelectObject(hdc, shadowBrush);
    Ellipse(hdc, buttonX - buttonRadius + 1, buttonY - buttonRadius + 1,
        buttonX + buttonRadius + 1, buttonY + buttonRadius + 1);

    // Кнопка с градиентом
    SelectObject(hdc, darkEmeraldBrush);
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);
    SelectObject(hdc, mediumEmeraldBrush);
    Ellipse(hdc, buttonX - buttonRadius * 0.7, buttonY - buttonRadius * 0.7,
        buttonX + buttonRadius * 0.7, buttonY + buttonRadius * 0.7);
    SelectObject(hdc, lightEmeraldBrush);
    Ellipse(hdc, buttonX - buttonRadius * 0.4, buttonY - buttonRadius * 0.4,
        buttonX + buttonRadius * 0.4, buttonY + buttonRadius * 0.4);

    DeleteObject(darkEmeraldBrush); DeleteObject(mediumEmeraldBrush); DeleteObject(lightEmeraldBrush);
    DeleteObject(shadowBrush);
}

void RoundFlashlight::PrintLight()
{
    // Обычный фонарик не светит
}

void RoundFlashlight::PrintCrack()
{
    // Обычный фонарик не имеет трещин
}

/* =============== СЛОМАННЫЙ КРУГЛЫЙ ФОНАРИК =============== */
BrokenRoundFlashlight::BrokenRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RoundFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
BrokenRoundFlashlight::~BrokenRoundFlashlight() {}

void BrokenRoundFlashlight::PrintCrack()
{
    HPEN crackPen = CreatePen(PS_SOLID, 2, RGB(140, 130, 120));
    HPEN deepCrackPen = CreatePen(PS_SOLID, 1, RGB(100, 40, 40));

    SelectObject(hdc, crackPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // Трещины
    MoveToEx(hdc, x + bodyWidth * 0.3, y + bodyHeight * 0.2, NULL);
    LineTo(hdc, x + bodyWidth * 0.7, y + bodyHeight * 0.8);
    SelectObject(hdc, deepCrackPen);
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.7, NULL);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.3);

    DeleteObject(crackPen); DeleteObject(deepCrackPen);
}

void BrokenRoundFlashlight::Hide()
{
    visible = false;

    // Создаем белые кисти и перья для замазывания
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);

    // Замазываем трещины дополнительными линиями
    // Основная трещина через корпус
    MoveToEx(hdc, x + bodyWidth * 0.3, y + bodyHeight * 0.2, NULL);
    LineTo(hdc, x + bodyWidth * 0.7, y + bodyHeight * 0.8);

    // Вторая трещина
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.7, NULL);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.3);

    // Сломанная кнопка
    int buttonRadius = 10;
    int buttonX = x + bodyWidth - 25;
    int buttonY = y + bodyHeight - 25;
    MoveToEx(hdc, buttonX - buttonRadius / 2, buttonY - buttonRadius / 2, NULL);
    LineTo(hdc, buttonX + buttonRadius / 2, buttonY + buttonRadius / 2);

    // Замазываем основную область фонарика
    BaseFlashlight::Hide();

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}

/* =============== ГОРЯЩИЙ КРУГЛЫЙ ФОНАРИК =============== */
LitRoundFlashlight::LitRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RoundFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
LitRoundFlashlight::~LitRoundFlashlight() {}

void LitRoundFlashlight::PrintLight()
{
    // Мягкий свет из круглого фонарика (теплые желтые оттенки)
    HBRUSH darkWarmBrush = CreateSolidBrush(RGB(255, 230, 150));
    HBRUSH mediumWarmBrush = CreateSolidBrush(RGB(255, 240, 180));
    HBRUSH lightWarmBrush = CreateSolidBrush(RGB(255, 250, 210));
    HBRUSH highlightBrush = CreateSolidBrush(RGB(255, 255, 230));

    HPEN warmPen = CreatePen(PS_SOLID, 1, RGB(255, 230, 150));
    HPEN lightWarmPen = CreatePen(PS_SOLID, 1, RGB(255, 240, 190));

    // Рисуем рассеянный луч света (треугольник вниз) с градиентом
    POINT light[3];
    light[0].x = x + bodyWidth / 2;
    light[0].y = y - headHeight; // начало луча
    light[1].x = x + bodyWidth / 2 - headWidth * 1.2;
    light[1].y = y + headHeight * 2.5; // левая точка основания (шире)
    light[2].x = x + bodyWidth / 2 + headWidth * 1.2;
    light[2].y = y + headHeight * 2.5; // правая точка основания

    // Основной луч
    SelectObject(hdc, darkWarmBrush);
    SelectObject(hdc, warmPen);
    Polygon(hdc, light, 3);

    // Внутренняя часть луча
    POINT innerLight[3];
    for (int i = 0; i < 3; i++) {
        innerLight[i].x = (light[i].x + x + bodyWidth / 2) / 2;
        innerLight[i].y = (light[i].y + y) / 2;
    }
    SelectObject(hdc, mediumWarmBrush);
    SelectObject(hdc, lightWarmPen);
    Polygon(hdc, innerLight, 3);

    // Ядро луча
    POINT coreLight[3];
    for (int i = 0; i < 3; i++) {
        coreLight[i].x = (innerLight[i].x + x + bodyWidth / 2) / 2;
        coreLight[i].y = (innerLight[i].y + y) / 2;
    }
    SelectObject(hdc, lightWarmBrush);
    Polygon(hdc, coreLight, 3);

    // Центральная часть (самое яркое)
    POINT centerLight[3];
    for (int i = 0; i < 3; i++) {
        centerLight[i].x = (coreLight[i].x + x + bodyWidth / 2) / 2;
        centerLight[i].y = (coreLight[i].y + y) / 2;
    }
    SelectObject(hdc, highlightBrush);
    Polygon(hdc, centerLight, 3);

    // Рассеянные лучики
    HPEN rayPen = CreatePen(PS_SOLID, 1, RGB(255, 245, 200));
    SelectObject(hdc, rayPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // Рисуем мягкие лучи
    for (int i = 0; i < 12; i++) {
        double angle = 3.14159 * i / 6 - 3.14159 / 2; // направлены вниз
        int rayLength = headHeight * 3;
        MoveToEx(hdc, x + bodyWidth / 2, y - headHeight / 2, NULL);
        LineTo(hdc, x + bodyWidth / 2 + cos(angle) * rayLength,
            y - headHeight / 2 + sin(angle) * rayLength);
    }

    DeleteObject(warmPen); DeleteObject(lightWarmPen); DeleteObject(rayPen);
    DeleteObject(darkWarmBrush); DeleteObject(mediumWarmBrush); DeleteObject(lightWarmBrush);
    DeleteObject(highlightBrush);
}

void LitRoundFlashlight::Hide()
{
    visible = false;

    // Создаем белые кисти и перья для замазывания
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);

    // Замазываем область света отдельно (треугольник вниз)
    POINT light[3];
    light[0].x = x + bodyWidth / 2;
    light[0].y = y - headHeight;
    light[1].x = x + bodyWidth / 2 - headWidth * 1.2;
    light[1].y = y + headHeight * 2.5;
    light[2].x = x + bodyWidth / 2 + headWidth * 1.2;
    light[2].y = y + headHeight * 2.5;

    // Замазываем область света
    Polygon(hdc, light, 3);

    // Замазываем лучики
    for (int i = 0; i < 12; i++) {
        double angle = 3.14159 * i / 6 - 3.14159 / 2;
        int rayLength = headHeight * 3;
        MoveToEx(hdc, x + bodyWidth / 2, y - headHeight / 2, NULL);
        LineTo(hdc, x + bodyWidth / 2 + cos(angle) * rayLength,
            y - headHeight / 2 + sin(angle) * rayLength);
    }

    // Замазываем основную область фонарика
    BaseFlashlight::Hide();

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}


/* =============== КЛАСС MudPuddle =============== */
MudPuddle::MudPuddle(int InitX, int InitY, int InitWidth, int InitHeight)
    : Conflict(InitX, InitY, InitWidth, InitHeight) {}
MudPuddle::~MudPuddle() {}

void MudPuddle::Show()
{
    visible = true;

    // Просто коричневый овал
    HBRUSH mudBrush = CreateSolidBrush(RGB(139, 90, 43));
    HPEN mudPen = CreatePen(PS_SOLID, 1, RGB(101, 67, 33));

    SelectObject(hdc, mudBrush);
    SelectObject(hdc, mudPen);
    Ellipse(hdc, x, y, x + width, y + height);

    // Внутренний более светлый овал
    HBRUSH lightMudBrush = CreateSolidBrush(RGB(160, 120, 80));
    SelectObject(hdc, lightMudBrush);
    Ellipse(hdc, x + 5, y + 5, x + width - 5, y + height - 5);

    DeleteObject(mudBrush); DeleteObject(lightMudBrush); DeleteObject(mudPen);
}

void MudPuddle::Hide()
{
    visible = false;
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);
    Ellipse(hdc, x - 5, y - 5, x + width + 5, y + height + 5);

    DeleteObject(whitePen); DeleteObject(whiteBrush);
}

/* =============== ГРЯЗНЫЙ ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */
DirtyRectFlashlight::DirtyRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RectFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
DirtyRectFlashlight::~DirtyRectFlashlight() {}

void DirtyRectFlashlight::PrintBody()
{
    RectFlashlight::PrintBody();  // Рисуем обычный корпус

    // Просто коричневые пятна поверх
    HBRUSH mudBrush = CreateSolidBrush(RGB(120, 80, 40));
    SelectObject(hdc, mudBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));

    // 3 пятна грязи
    Ellipse(hdc, x + 10, y + 20, x + 25, y + 35);
    Ellipse(hdc, x + bodyWidth - 20, y + 10, x + bodyWidth - 5, y + 25);
    Ellipse(hdc, x + 15, y + bodyHeight - 25, x + 30, y + bodyHeight - 10);

    DeleteObject(mudBrush);
}

void DirtyRectFlashlight::PrintHead()
{
    RectFlashlight::PrintHead();  // Обычная головка

    // Полоска грязи снизу головки
    HBRUSH mudBrush = CreateSolidBrush(RGB(100, 70, 40));
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;

    SelectObject(hdc, mudBrush);
    Rectangle(hdc, headX, headY + headHeight - 10, headX + headWidth, headY + headHeight);

    DeleteObject(mudBrush);
}

void DirtyRectFlashlight::PrintButton()
{
    RectFlashlight::PrintButton();  // Обычная кнопка
}

void DirtyRectFlashlight::Hide() { BaseFlashlight::Hide(); }

/* =============== ГРЯЗНЫЙ КРУГЛЫЙ ФОНАРИК =============== */
DirtyRoundFlashlight::DirtyRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RoundFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
DirtyRoundFlashlight::~DirtyRoundFlashlight() {}

void DirtyRoundFlashlight::PrintBody()
{
    RoundFlashlight::PrintBody();  // Обычный корпус

    // Две полоски грязи
    HBRUSH mudBrush = CreateSolidBrush(RGB(110, 75, 45));
    SelectObject(hdc, mudBrush);

    // Горизонтальная полоса
    Rectangle(hdc, x + 10, y + bodyHeight / 2 - 5, x + bodyWidth - 10, y + bodyHeight / 2 + 5);
    // Вертикальная полоса
    Rectangle(hdc, x + bodyWidth / 2 - 5, y + 10, x + bodyWidth / 2 + 5, y + bodyHeight - 10);

    DeleteObject(mudBrush);
}

void DirtyRoundFlashlight::PrintHead()
{
    RoundFlashlight::PrintHead();  // Обычная головка
}

void DirtyRoundFlashlight::PrintButton()
{
    RoundFlashlight::PrintButton();  // Обычная кнопка
}

void DirtyRoundFlashlight::Hide() { BaseFlashlight::Hide(); }