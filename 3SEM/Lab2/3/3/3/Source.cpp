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

using namespace std;

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/* =============== КЛАСС Location =============== */
Location::Location(int InitX, int InitY) { x = InitX; y = InitY; }
Location::~Location() {}
int Location::GetX() { return x; }
int Location::GetY() { return y; }
void Location::SetX(int NewX) { x = NewX; }
void Location::SetY(int NewY) { y = NewY; }

/* =============== КЛАСС Point =============== */
Point::Point(int InitX, int InitY) : Location(InitX, InitY) { visible = false; }
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
Circle::Circle(int InitX, int InitY, int InitRadius) : Conflict(InitX, InitY, InitRadius*2, InitRadius * 2) {
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

/* =============== БАЗОВЫЙ КЛАСС Flashlight =============== */
BaseFlashlight::BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight) : Point(InitX, InitY) {
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
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
RectFlashlight::~RectFlashlight() {}

void RectFlashlight::Show() {
    visible = true;
    HBRUSH darkMetalBrush = CreateSolidBrush(RGB(70, 70, 80));
    HBRUSH mediumMetalBrush = CreateSolidBrush(RGB(90, 90, 100));
    HBRUSH lightMetalBrush = CreateSolidBrush(RGB(110, 110, 120));
    HBRUSH darkHeadBrush = CreateSolidBrush(RGB(60, 60, 70));
    HBRUSH mediumHeadBrush = CreateSolidBrush(RGB(80, 80, 90));
    HBRUSH lightHeadBrush = CreateSolidBrush(RGB(100, 100, 110));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(50, 50, 60));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(60, 60, 70));

    // Тени
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    Rectangle(hdc, x + 3, y + 3, x + bodyWidth + 3, y + bodyHeight + 3);
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, x + 1, y + 1, x + bodyWidth + 1, y + bodyHeight + 1);

    // Корпус
    HPEN softDarkPen = CreatePen(PS_SOLID, 1, RGB(65, 65, 75));
    HPEN softMediumPen = CreatePen(PS_SOLID, 1, RGB(85, 85, 95));
    SelectObject(hdc, darkMetalBrush);
    SelectObject(hdc, softDarkPen);
    Rectangle(hdc, x, y, x + bodyWidth * 0.3, y + bodyHeight);
    SelectObject(hdc, mediumMetalBrush);
    Rectangle(hdc, x + bodyWidth * 0.3, y, x + bodyWidth * 0.7, y + bodyHeight);
    SelectObject(hdc, lightMetalBrush);
    Rectangle(hdc, x + bodyWidth * 0.7, y, x + bodyWidth, y + bodyHeight);

    // Головка
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    SelectObject(hdc, shadowBrush);
    Rectangle(hdc, headX + 2, headY + 2, headX + headWidth + 2, headY + headHeight + 2);
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, headX + 1, headY + 1, headX + headWidth + 1, headY + headHeight + 1);
    SelectObject(hdc, darkHeadBrush);
    SelectObject(hdc, softDarkPen);
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight * 0.2);
    SelectObject(hdc, mediumHeadBrush);
    Rectangle(hdc, headX, headY + headHeight * 0.2, headX + headWidth, headY + headHeight * 0.5);
    SelectObject(hdc, lightHeadBrush);
    Rectangle(hdc, headX, headY + headHeight * 0.5, headX + headWidth, headY + headHeight * 0.8);
    SelectObject(hdc, CreateSolidBrush(RGB(120, 120, 130)));
    Rectangle(hdc, headX, headY + headHeight * 0.8, headX + headWidth, headY + headHeight);

    // Кнопка
    int buttonWidth = 14;
    int buttonHeight = 20;
    int buttonX = x + bodyWidth - 28;
    int buttonY = y + bodyHeight - 35;
    SelectObject(hdc, shadowBrush);
    Rectangle(hdc, buttonX + 2, buttonY + 2, buttonX + buttonWidth + 2, buttonY + buttonHeight + 2);
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, buttonX + 1, buttonY + 1, buttonX + buttonWidth + 1, buttonY + buttonHeight + 1);
    HBRUSH lightRedBrush = CreateSolidBrush(RGB(220, 40, 40));
    HBRUSH mediumRedBrush = CreateSolidBrush(RGB(180, 20, 20));
    HBRUSH darkRedBrush = CreateSolidBrush(RGB(140, 0, 0));
    SelectObject(hdc, lightRedBrush);
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight * 0.3);
    SelectObject(hdc, mediumRedBrush);
    Rectangle(hdc, buttonX, buttonY + buttonHeight * 0.3, buttonX + buttonWidth, buttonY + buttonHeight * 0.7);
    SelectObject(hdc, darkRedBrush);
    Rectangle(hdc, buttonX, buttonY + buttonHeight * 0.7, buttonX + buttonWidth, buttonY + buttonHeight);
    HBRUSH indicatorBrush = CreateSolidBrush(RGB(255, 255, 200));
    SelectObject(hdc, indicatorBrush);
    Ellipse(hdc, buttonX + buttonWidth * 0.3, buttonY + buttonHeight * 0.4,
        buttonX + buttonWidth * 0.7, buttonY + buttonHeight * 0.6);

    // Удаляем объекты
    DeleteObject(softDarkPen); DeleteObject(softMediumPen);
    DeleteObject(darkMetalBrush); DeleteObject(mediumMetalBrush); DeleteObject(lightMetalBrush);
    DeleteObject(darkHeadBrush); DeleteObject(mediumHeadBrush); DeleteObject(lightHeadBrush);
    DeleteObject(lightRedBrush); DeleteObject(mediumRedBrush); DeleteObject(darkRedBrush);
    DeleteObject(indicatorBrush); DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
}

void RectFlashlight::Hide() {
    visible = false;
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, x - 15, headY - 15, x + bodyWidth + 20, y + bodyHeight + 20);
    DeleteObject(whitePen); DeleteObject(whiteBrush);
}

/* =============== КРУГЛЫЙ ФОНАРИК =============== */
RoundFlashlight::RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}
RoundFlashlight::~RoundFlashlight() {}

void RoundFlashlight::Show() {
    visible = true;
    HBRUSH darkBaseBrush = CreateSolidBrush(RGB(80, 80, 90));
    HBRUSH mediumBaseBrush = CreateSolidBrush(RGB(100, 100, 110));
    HBRUSH lightBaseBrush = CreateSolidBrush(RGB(120, 120, 130));
    HBRUSH darkShadeBrush = CreateSolidBrush(RGB(210, 200, 180));
    HBRUSH mediumShadeBrush = CreateSolidBrush(RGB(230, 220, 200));
    HBRUSH lightShadeBrush = CreateSolidBrush(RGB(250, 240, 220));
    HBRUSH shadeAccentBrush = CreateSolidBrush(RGB(180, 160, 140));
    HBRUSH darkEmeraldBrush = CreateSolidBrush(RGB(0, 100, 80));
    HBRUSH mediumEmeraldBrush = CreateSolidBrush(RGB(0, 150, 120));
    HBRUSH lightEmeraldBrush = CreateSolidBrush(RGB(0, 200, 160));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(60, 60, 70));
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(70, 70, 80));
    HBRUSH shadeShadowBrush = CreateSolidBrush(RGB(180, 170, 160));

    // Основание
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    Ellipse(hdc, x + 3, y + 3, x + bodyWidth + 3, y + bodyHeight + 3);
    SelectObject(hdc, mediumShadowBrush);
    Ellipse(hdc, x + 1, y + 1, x + bodyWidth + 1, y + bodyHeight + 1);
    HPEN softDarkPen = CreatePen(PS_SOLID, 1, RGB(75, 75, 85));
    HPEN softMediumPen = CreatePen(PS_SOLID, 1, RGB(95, 95, 105));
    SelectObject(hdc, darkBaseBrush);
    SelectObject(hdc, softDarkPen);
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);
    SelectObject(hdc, mediumBaseBrush);
    Ellipse(hdc, x + bodyWidth * 0.1, y + bodyHeight * 0.1,
        x + bodyWidth * 0.9, y + bodyHeight * 0.9);
    SelectObject(hdc, lightBaseBrush);
    Ellipse(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.2,
        x + bodyWidth * 0.8, y + bodyHeight * 0.8);

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

    // Основной абажур
    SelectObject(hdc, darkShadeBrush);
    Polygon(hdc, shade, 7);

    // Кнопка
    int buttonRadius = 10;
    int buttonX = x + bodyWidth - 25;
    int buttonY = y + bodyHeight - 25;
    SelectObject(hdc, shadowBrush);
    Ellipse(hdc, buttonX - buttonRadius + 1, buttonY - buttonRadius + 1,
        buttonX + buttonRadius + 1, buttonY + buttonRadius + 1);
    SelectObject(hdc, darkEmeraldBrush);
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);
    SelectObject(hdc, mediumEmeraldBrush);
    Ellipse(hdc, buttonX - buttonRadius * 0.7, buttonY - buttonRadius * 0.7,
        buttonX + buttonRadius * 0.7, buttonY + buttonRadius * 0.7);
    SelectObject(hdc, lightEmeraldBrush);
    Ellipse(hdc, buttonX - buttonRadius * 0.4, buttonY - buttonRadius * 0.4,
        buttonX + buttonRadius * 0.4, buttonY + buttonRadius * 0.4);

    // Удаляем объекты
    DeleteObject(softDarkPen); DeleteObject(softMediumPen);
    DeleteObject(darkBaseBrush); DeleteObject(mediumBaseBrush); DeleteObject(lightBaseBrush);
    DeleteObject(darkShadeBrush); DeleteObject(mediumShadeBrush); DeleteObject(lightShadeBrush);
    DeleteObject(shadeAccentBrush); DeleteObject(darkEmeraldBrush); DeleteObject(mediumEmeraldBrush);
    DeleteObject(lightEmeraldBrush); DeleteObject(shadowBrush); DeleteObject(mediumShadowBrush);
    DeleteObject(shadeShadowBrush);
}

void RoundFlashlight::Hide() {
    visible = false;
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, x - 20, headY - 20, x + bodyWidth + 25, y + bodyHeight + 25);
    DeleteObject(whitePen); DeleteObject(whiteBrush);
}

/* =============== СЛОМАННЫЙ ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */
BrokenRectFlashlight::BrokenRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RectFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {
    broken = true;
}
BrokenRectFlashlight::~BrokenRectFlashlight() {}

void BrokenRectFlashlight::Show() {
    visible = true;
    HBRUSH darkMetalBrush = CreateSolidBrush(RGB(60, 60, 65));
    HBRUSH mediumMetalBrush = CreateSolidBrush(RGB(75, 75, 80));
    HBRUSH lightMetalBrush = CreateSolidBrush(RGB(90, 90, 95));
    HBRUSH darkHeadBrush = CreateSolidBrush(RGB(50, 50, 55));
    HBRUSH mediumHeadBrush = CreateSolidBrush(RGB(65, 65, 70));
    HBRUSH lightHeadBrush = CreateSolidBrush(RGB(80, 80, 85));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(40, 40, 45));
    HPEN softDarkPen = CreatePen(PS_SOLID, 1, RGB(55, 55, 60));

    // Корпус
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, softDarkPen);
    Rectangle(hdc, x + 2, y + 2, x + bodyWidth + 2, y + bodyHeight + 2);
    SelectObject(hdc, darkMetalBrush);
    Rectangle(hdc, x, y, x + bodyWidth * 0.3, y + bodyHeight);
    SelectObject(hdc, mediumMetalBrush);
    Rectangle(hdc, x + bodyWidth * 0.3, y, x + bodyWidth * 0.7, y + bodyHeight);
    SelectObject(hdc, lightMetalBrush);
    Rectangle(hdc, x + bodyWidth * 0.7, y, x + bodyWidth, y + bodyHeight);

    // Головка
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    SelectObject(hdc, shadowBrush);
    Rectangle(hdc, headX + 1, headY + 1, headX + headWidth + 1, headY + headHeight + 1);
    SelectObject(hdc, darkHeadBrush);
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight * 0.3);
    SelectObject(hdc, mediumHeadBrush);
    Rectangle(hdc, headX, headY + headHeight * 0.3, headX + headWidth, headY + headHeight * 0.6);
    SelectObject(hdc, lightHeadBrush);
    Rectangle(hdc, headX, headY + headHeight * 0.6, headX + headWidth, headY + headHeight);

    // Трещины
    HPEN crackPen = CreatePen(PS_SOLID, 2, RGB(30, 30, 35));
    HPEN deepCrackPen = CreatePen(PS_SOLID, 1, RGB(120, 30, 30));
    SelectObject(hdc, crackPen);
    MoveToEx(hdc, x + bodyWidth * 0.2, y + 5, NULL);
    LineTo(hdc, x + bodyWidth * 0.4, y + bodyHeight * 0.3);
    LineTo(hdc, x + bodyWidth * 0.6, y + bodyHeight * 0.6);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight - 5);
    MoveToEx(hdc, x + bodyWidth * 0.5, y + 10, NULL);
    LineTo(hdc, x + bodyWidth * 0.5, y + bodyHeight - 10);
    SelectObject(hdc, deepCrackPen);
    MoveToEx(hdc, headX + headWidth * 0.2, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.8, headY + headHeight - 5);
    MoveToEx(hdc, headX + headWidth * 0.8, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.2, headY + headHeight - 5);

    // Кнопка
    int buttonWidth = 14;
    int buttonHeight = 20;
    int buttonX = x + bodyWidth - 28;
    int buttonY = y + bodyHeight - 35;
    HBRUSH brokenButtonBrush = CreateSolidBrush(RGB(80, 20, 20));
    SelectObject(hdc, brokenButtonBrush);
    SelectObject(hdc, softDarkPen);
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, deepCrackPen);
    MoveToEx(hdc, buttonX, buttonY, NULL);
    LineTo(hdc, buttonX + buttonWidth, buttonY + buttonHeight);
    MoveToEx(hdc, buttonX + buttonWidth, buttonY, NULL);
    LineTo(hdc, buttonX, buttonY + buttonHeight);

    DeleteObject(softDarkPen); DeleteObject(crackPen); DeleteObject(deepCrackPen);
    DeleteObject(darkMetalBrush); DeleteObject(mediumMetalBrush); DeleteObject(lightMetalBrush);
    DeleteObject(darkHeadBrush); DeleteObject(mediumHeadBrush); DeleteObject(lightHeadBrush);
    DeleteObject(brokenButtonBrush); DeleteObject(shadowBrush);
}

void BrokenRectFlashlight::Hide() {
    visible = false;
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, x - 25, headY - 25, x + bodyWidth + 30, y + bodyHeight + 30);
    DeleteObject(whitePen); DeleteObject(whiteBrush);
}

/* =============== СЛОМАННЫЙ КРУГЛЫЙ ФОНАРИК =============== */
BrokenRoundFlashlight::BrokenRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RoundFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {
    broken = true;
}
BrokenRoundFlashlight::~BrokenRoundFlashlight() {}

void BrokenRoundFlashlight::Show() {
    visible = true;
    HBRUSH darkBaseBrush = CreateSolidBrush(RGB(70, 70, 75));
    HBRUSH mediumBaseBrush = CreateSolidBrush(RGB(85, 85, 90));
    HBRUSH lightBaseBrush = CreateSolidBrush(RGB(100, 100, 105));
    HBRUSH darkShadeBrush = CreateSolidBrush(RGB(180, 170, 160));
    HBRUSH mediumShadeBrush = CreateSolidBrush(RGB(200, 190, 180));
    HBRUSH lightShadeBrush = CreateSolidBrush(RGB(220, 210, 200));
    HBRUSH shadowBrush = CreateSolidBrush(RGB(50, 50, 55));
    HPEN softDarkPen = CreatePen(PS_SOLID, 1, RGB(65, 65, 70));

    // Основание
    SelectObject(hdc, shadowBrush);
    SelectObject(hdc, softDarkPen);
    Ellipse(hdc, x + 2, y + 2, x + bodyWidth + 2, y + bodyHeight + 2);
    SelectObject(hdc, darkBaseBrush);
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);
    SelectObject(hdc, mediumBaseBrush);
    Ellipse(hdc, x + bodyWidth * 0.15, y + bodyHeight * 0.15,
        x + bodyWidth * 0.85, y + bodyHeight * 0.85);
    SelectObject(hdc, lightBaseBrush);
    Ellipse(hdc, x + bodyWidth * 0.25, y + bodyHeight * 0.25,
        x + bodyWidth * 0.75, y + bodyHeight * 0.75);

    // Абажур
    POINT damagedShade[7];
    int shadeTop = y - headHeight;
    int shadeBottom = y;
    damagedShade[0].x = x + bodyWidth / 2 - headWidth * 0.35;
    damagedShade[0].y = shadeTop;
    damagedShade[1].x = x + bodyWidth / 2 + headWidth * 0.25;
    damagedShade[1].y = shadeTop;
    damagedShade[2].x = x + bodyWidth / 2 + headWidth * 0.4;
    damagedShade[2].y = shadeTop + headHeight * 0.3;
    damagedShade[3].x = x + bodyWidth / 2 + headWidth * 0.45;
    damagedShade[3].y = shadeTop + headHeight * 0.6;
    damagedShade[4].x = x + bodyWidth / 2 + headWidth * 0.35;
    damagedShade[4].y = shadeBottom;
    damagedShade[5].x = x + bodyWidth / 2 - headWidth * 0.4;
    damagedShade[5].y = shadeBottom;
    damagedShade[6].x = x + bodyWidth / 2 - headWidth * 0.45;
    damagedShade[6].y = shadeTop + headHeight * 0.6;

    // Тень абажура
    POINT shadowShade[7];
    for (int i = 0; i < 7; i++) {
        shadowShade[i].x = damagedShade[i].x + 1;
        shadowShade[i].y = damagedShade[i].y + 1;
    }
    HBRUSH shadeShadowBrush = CreateSolidBrush(RGB(160, 150, 140));
    SelectObject(hdc, shadeShadowBrush);
    Polygon(hdc, shadowShade, 7);

    // Основной абажур
    SelectObject(hdc, darkShadeBrush);
    Polygon(hdc, damagedShade, 7);

    // Трещины
    HPEN crackPen = CreatePen(PS_SOLID, 2, RGB(140, 130, 120));
    HPEN deepCrackPen = CreatePen(PS_SOLID, 1, RGB(100, 40, 40));
    SelectObject(hdc, crackPen);
    MoveToEx(hdc, x + bodyWidth * 0.3, y + bodyHeight * 0.2, NULL);
    LineTo(hdc, x + bodyWidth * 0.7, y + bodyHeight * 0.8);
    SelectObject(hdc, deepCrackPen);
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.7, NULL);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.3);

    DeleteObject(softDarkPen); DeleteObject(crackPen); DeleteObject(deepCrackPen);
    DeleteObject(darkBaseBrush); DeleteObject(mediumBaseBrush); DeleteObject(lightBaseBrush);
    DeleteObject(darkShadeBrush); DeleteObject(mediumShadeBrush); DeleteObject(lightShadeBrush);
    DeleteObject(shadowBrush); DeleteObject(shadeShadowBrush);
}

void BrokenRoundFlashlight::Hide() {
    visible = false;
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);
    Rectangle(hdc, x - 35, y - headHeight - 35, x + bodyWidth + 40, y + bodyHeight + 40);
    DeleteObject(whitePen); DeleteObject(whiteBrush);
}