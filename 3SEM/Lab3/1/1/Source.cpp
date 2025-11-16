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

// Конструктор класса Point
Point::Point(int InitX, int InitY) : Location(InitX, InitY)
{
    visible = false;
}

// Деструктор (класса Point)
Point::~Point() {}

// Получение видимости точки (класса Point)
bool Point::GetVisible() { return visible; }

// Изменение видимости точки (класса Point)
void Point::SetVisible(bool NewVisible) { visible = NewVisible; }

// Показать точку (класса Point)
void Point::Show()
{
    visible = true;
    SetPixel(hdc, x, y, RGB(255, 0, 0));
    SetPixel(hdc, x, y + 1, RGB(255, 0, 0));
    SetPixel(hdc, x + 1, y, RGB(255, 0, 0));
    SetPixel(hdc, x + 1, y + 1, RGB(255, 0, 0));
}

// Спрятать точку (класса Point)
void Point::Hide()
{
    visible = false;
    SetPixel(hdc, x, y, RGB(0, 0, 255));
    SetPixel(hdc, x, y + 1, RGB(0, 0, 255));
    SetPixel(hdc, x + 1, y, RGB(0, 0, 255));
    SetPixel(hdc, x + 1, y + 1, RGB(0, 0, 255));
}

// Перемещение точки (класса Point)
void Point::MoveTo(int NewX, int NewY)
{
    Hide();
    x = NewX;
    y = NewY;
    Show();
}

// Перетаскивание точки (класса Point)
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

/* =============== КЛАСС Conflict =============== */

// Конструктор конфликтного объекта (класса Conflict)
Conflict::Conflict(int InitX, int InitY, int InitWidth, int InitHeight)
    : Point(InitX, InitY) {
    width = InitWidth;
    height = InitHeight;
}

// Деструктор (класса Conflict)
Conflict::~Conflict() {}

/* =============== БАЗОВЫЙ КЛАСС Flashlight =============== */

// Конструктор базового фонарика (класса BaseFlashlight)
BaseFlashlight::BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight) : Point(InitX, InitY)
{
    bodyWidth = InitBodyWidth;
    bodyHeight = InitBodyHeight;
    headWidth = InitHeadWidth;
    headHeight = InitHeadHeight;
}

// Деструктор (класса BaseFlashlight)
BaseFlashlight::~BaseFlashlight() {}

void BaseFlashlight::Show()
{
    visible = true;
    PrintLight();   // свет
    PrintButton();  // сначала кнопка (она позади)
    PrintBody();    // корпус
    PrintHead();    // головка
    PrintCrack();   // трещины
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
    int left = x - headWidth/2;
    int top = y - headHeight * 2; // дополнительное место для света
    int right = x + bodyWidth + headWidth/2;
    int bottom = y + bodyHeight; // дополнительное место для света

    Rectangle(hdc, left, top, right, bottom);

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}


/* =============== ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */

// Конструктор прямоугольного фонарика (класса RectFlashlight)
RectFlashlight::RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}

// Деструктор (класса RectFlashlight)
RectFlashlight::~RectFlashlight() {}

void RectFlashlight::PrintBody()
{
    HBRUSH grayBrush = CreateSolidBrush(RGB(80, 80, 80));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    DeleteObject(blackPen);
    DeleteObject(grayBrush);
}

void RectFlashlight::PrintHead()
{
    HBRUSH grayBrush = CreateSolidBrush(RGB(80, 80, 80));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;

    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    DeleteObject(blackPen);
    DeleteObject(grayBrush);
}

void RectFlashlight::PrintButton()
{
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;

    SelectObject(hdc, redBrush);
    SelectObject(hdc, blackPen);
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(blackPen);
    DeleteObject(redBrush);
}

void RectFlashlight::PrintLight()
{
    // Обычный фонарик не светит
}

void RectFlashlight::PrintCrack()
{
    // Обычный фонарик не имеет трещин
}

/* =============== СЛОМАННЫЙ ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */

BrokenRectFlashlight::BrokenRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RectFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}

BrokenRectFlashlight::~BrokenRectFlashlight() {}

void BrokenRectFlashlight::PrintCrack()
{
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(40, 40, 40));
    HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

    SelectObject(hdc, crackPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // Основная трещина через корпус
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.3, NULL);
    LineTo(hdc, x + bodyWidth * 0.5, y + bodyHeight * 0.6);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.4);

    // Вертикальная трещина
    MoveToEx(hdc, x + bodyWidth * 0.6, y + 5, NULL);
    LineTo(hdc, x + bodyWidth * 0.6, y + bodyHeight - 5);

    // Трещина на головке
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    MoveToEx(hdc, headX + headWidth * 0.3, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.7, headY + headHeight - 5);

    // Сломанная кнопка
    SelectObject(hdc, redPen);
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    MoveToEx(hdc, buttonX, buttonY, NULL);
    LineTo(hdc, buttonX + buttonWidth, buttonY + buttonHeight);
    MoveToEx(hdc, buttonX + buttonWidth, buttonY, NULL);
    LineTo(hdc, buttonX, buttonY + buttonHeight);

    DeleteObject(crackPen);
    DeleteObject(redPen);
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
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.3, NULL);
    LineTo(hdc, x + bodyWidth * 0.5, y + bodyHeight * 0.6);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.4);

    // Вертикальная трещина
    MoveToEx(hdc, x + bodyWidth * 0.6, y + 5, NULL);
    LineTo(hdc, x + bodyWidth * 0.6, y + bodyHeight - 5);

    // Трещина на головке
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    MoveToEx(hdc, headX + headWidth * 0.3, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.7, headY + headHeight - 5);

    // Сломанная кнопка
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
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
    // Желтый свет из головки
    HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
    HPEN yellowPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;

    // Рисуем луч света (трапецию)
    POINT light[4];
    light[0].x = headX + headWidth * 0.2;
    light[0].y = headY;
    light[1].x = headX + headWidth * 0.8;
    light[1].y = headY;
    light[2].x = headX + headWidth * 1.5;
    light[2].y = headY - headHeight * 2;
    light[3].x = headX - headWidth * 0.5;
    light[3].y = headY - headHeight * 2;

    SelectObject(hdc, yellowBrush);
    SelectObject(hdc, yellowPen);
    Polygon(hdc, light, 4);

    DeleteObject(yellowPen);
    DeleteObject(yellowBrush);
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

    // Замазываем основную область фонарика
    BaseFlashlight::Hide();

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}


/* =============== КРУГЛЫЙ ФОНАРИК =============== */

// Конструктор круглого фонарика (класса RoundFlashlight)
RoundFlashlight::RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight) {}

// Деструктор (класса RoundFlashlight)
RoundFlashlight::~RoundFlashlight() {}

void RoundFlashlight::PrintBody()
{
    HBRUSH grayBrush = CreateSolidBrush(RGB(80, 80, 80));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    DeleteObject(blackPen);
    DeleteObject(grayBrush);
}

void RoundFlashlight::PrintHead()
{
    HBRUSH grayBrush = CreateSolidBrush(RGB(80, 80, 80));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;

    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);
    Polygon(hdc, triangle, 3);

    DeleteObject(blackPen);
    DeleteObject(grayBrush);
}

void RoundFlashlight::PrintButton()
{
    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;

    SelectObject(hdc, greenBrush);
    SelectObject(hdc, blackPen);
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    DeleteObject(blackPen);
    DeleteObject(greenBrush);
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
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(40, 40, 40));
    HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

    SelectObject(hdc, crackPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // Основная трещина через корпус
    MoveToEx(hdc, x + bodyWidth * 0.3, y + bodyHeight * 0.2, NULL);
    LineTo(hdc, x + bodyWidth * 0.7, y + bodyHeight * 0.8);

    // Вторая трещина
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.7, NULL);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.3);

    // Сломанная кнопка
    SelectObject(hdc, redPen);
    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    MoveToEx(hdc, buttonX - buttonRadius / 2, buttonY - buttonRadius / 2, NULL);
    LineTo(hdc, buttonX + buttonRadius / 2, buttonY + buttonRadius / 2);

    DeleteObject(crackPen);
    DeleteObject(redPen);
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
    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
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
    // Желтый свет из головки (направлен вниз)
    HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
    HPEN yellowPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

    // Рисуем луч света (треугольник) вниз
    POINT light[3];
    light[0].x = x + bodyWidth / 2;
    light[0].y = y - headHeight; // начало луча (нижняя точка головки)
    light[1].x = x + bodyWidth / 2 - headWidth;
    light[1].y = y + headHeight * 2; // левая точка основания луча (ниже)
    light[2].x = x + bodyWidth / 2 + headWidth;
    light[2].y = y + headHeight * 2; // правая точка основания луча

    SelectObject(hdc, yellowBrush);
    SelectObject(hdc, yellowPen);
    Polygon(hdc, light, 3);

    DeleteObject(yellowPen);
    DeleteObject(yellowBrush);
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
    light[1].x = x + bodyWidth / 2 - headWidth;
    light[1].y = y + headHeight * 2;
    light[2].x = x + bodyWidth / 2 + headWidth;
    light[2].y = y + headHeight * 2;

    // Замазываем область света
    Polygon(hdc, light, 3);

    // Замазываем основную область фонарика
    BaseFlashlight::Hide();

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}

/* =============== КЛАСС Screwdriver =============== */

// Конструктор отвертки (класса Screwdriver)
Screwdriver::Screwdriver(int InitX, int InitY, int InitWidth, int InitLength)
    : Conflict(InitX, InitY, InitWidth, InitLength)
{
}

// Деструктор (класса Screwdriver)
Screwdriver::~Screwdriver() {}

// "Показать" отвертку (класс Screwdriver)
void Screwdriver::Show()
{
    visible = true;

    // Создаем кисти и перья
    HBRUSH handleBrush = CreateSolidBrush(RGB(139, 69, 19)); // Коричневый
    HBRUSH metalBrush = CreateSolidBrush(RGB(192, 192, 192)); // Серебряный
    HPEN darkPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // Черный для контуров

    // Рисуем ручку
    SelectObject(hdc, handleBrush);
    SelectObject(hdc, darkPen);
    Rectangle(hdc, x, y, x + width, y - height + 5);

    // Рисуем металлическую часть (прямоугольник)
    SelectObject(hdc, metalBrush);
    Rectangle(hdc, x + 3, y, x + width / 2 + 3, y + height);

    // Рисуем наконечник (треугольник)
    POINT tip[3];
    tip[0].x = x + width / 4 + 3;
    tip[0].y = y + height;
    tip[1].x = x + 3;
    tip[1].y = y + height + 10;
    tip[2].x = x + width / 2 + 3;
    tip[2].y = y + height + 10;
    Polygon(hdc, tip, 3);

    // Удаляем объекты
    DeleteObject(darkPen);
    DeleteObject(metalBrush);
    DeleteObject(handleBrush);
}

void Screwdriver::Hide()
{
    visible = true;

    // Создаем кисти и перья
    HBRUSH handleBrush = CreateSolidBrush(RGB(255, 255, 255)); // Коричневый
    HBRUSH metalBrush = CreateSolidBrush(RGB(255, 255, 255)); // Серебряный
    HPEN darkPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); // Черный для контуров

    // Рисуем ручку
    SelectObject(hdc, handleBrush);
    SelectObject(hdc, darkPen);
    Rectangle(hdc, x, y, x + width, y - height + 5);

    // Рисуем металлическую часть (прямоугольник)
    SelectObject(hdc, metalBrush);
    Rectangle(hdc, x + 3, y, x + width / 2 + 3, y + height);

    // Рисуем наконечник (треугольник)
    POINT tip[3];
    tip[0].x = x + width / 4 + 3;
    tip[0].y = y + height;
    tip[1].x = x + 3;
    tip[1].y = y + height + 10;
    tip[2].x = x + width / 2 + 3;
    tip[2].y = y + height + 10;
    Polygon(hdc, tip, 3);

    // Удаляем объекты
    DeleteObject(darkPen);
    DeleteObject(metalBrush);
    DeleteObject(handleBrush);
}

/* =============== КЛАСС Stone =============== */

// Конструктор камня (класса Stone)
Stone::Stone(int InitX, int InitY, int InitWidth, int InitHeight) :
    Conflict(InitX, InitY, InitWidth, InitHeight) {}

// Деструктор (класса Stone)
Stone::~Stone() {}

// "Показать" камень (класс Stone)
void Stone::Show() {
    visible = true;

    // Создаем кисть для заливки камня
    HBRUSH stoneBrush = CreateSolidBrush(RGB(120, 120, 120)); // Серый цвет
    HPEN stonePen = CreatePen(PS_SOLID, 2, RGB(80, 80, 80)); // Темно-серый для контура

    // Выбираем кисть и перо
    SelectObject(hdc, stoneBrush);
    SelectObject(hdc, stonePen);

    // Рисуем камень как неровный многоугольник (заливается и контур)
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

    // Теперь рисуем трещины (только линии, без заливки)
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));
    SelectObject(hdc, crackPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH)); // Убираем заливку для трещин

    MoveToEx(hdc, x + width / 3, y + height / 4, NULL);
    LineTo(hdc, x + width / 2, y + height / 2);
    MoveToEx(hdc, x + width * 2 / 3, y + height / 3, NULL);
    LineTo(hdc, x + width / 3, y + height * 3 / 4);

    // Удаляем созданные объекты
    DeleteObject(crackPen);
    DeleteObject(stonePen);
    DeleteObject(stoneBrush);
}

void Stone::Hide() {
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

/* =============== КЛАСС Battery =============== */

Battery::Battery(int InitX, int InitY, int InitWidth, int InitHeight)
    : Conflict(InitX, InitY, InitWidth, InitHeight) {}

Battery::~Battery() {}

void Battery::Show()
{
    visible = true;

    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HBRUSH silverBrush = CreateSolidBrush(RGB(192, 192, 192));
    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));

    SelectObject(hdc, blackPen);

    // Основной корпус батарейки
    SelectObject(hdc, silverBrush);
    Rectangle(hdc, x, y, x + width, y + height);

    // Верхний контакт
    int contactWidth = width / 3;
    int contactHeight = height / 8;
    Rectangle(hdc, x + (width - contactWidth) / 2, y - contactHeight,
        x + (width + contactWidth) / 2, y);

    // Индикатор заряда
    SelectObject(hdc, greenBrush);
    int indicatorWidth = width * 0.6;
    int indicatorHeight = height * 0.4;
    Rectangle(hdc, x + (width - indicatorWidth) / 2, y + height * 0.3,
        x + (width + indicatorWidth) / 2, y + height * 0.3 + indicatorHeight);

    DeleteObject(blackPen);
    DeleteObject(silverBrush);
    DeleteObject(greenBrush);
}

void Battery::Hide()
{
    visible = false;
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

    SelectObject(hdc, whitePen);
    SelectObject(hdc, whiteBrush);

    // Замазываем всю область батарейки
    Rectangle(hdc, x, y - height / 8, x + width, y + height);

    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}