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


/* =============== БАЗОВЫЙ КЛАСС Flashlight =============== */

// Конструктор базового фонарика (класса BaseFlashlight)
BaseFlashlight::BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight) : Point(InitX, InitY)
{
    bodyWidth = InitBodyWidth;
    bodyHeight = InitBodyHeight;
    headWidth = InitHeadWidth;
    headHeight = InitHeadHeight;
    broken = false;
}

// Деструктор (класса BaseFlashlight)
BaseFlashlight::~BaseFlashlight() {}

/*// Проверка столкновения (класса BaseFlashlight)
bool BaseFlashlight::IsCollision(Conflict& conflict)
{
    if (x < conflict.GetX() + conflict.GetWidth() &&
        x + bodyWidth > conflict.GetX() &&
        y < conflict.GetY() + conflict.GetHeight() &&
        y + bodyHeight > conflict.GetY())
    {
        return true;
    }
    return false;
}*/

/* =============== ПРЯМОУГОЛЬНЫЙ ФОНАРИК =============== */

// Конструктор прямоугольного фонарика (класса RectFlashlight)
RectFlashlight::RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
}

// Деструктор (класса RectFlashlight)
RectFlashlight::~RectFlashlight() {}

// "Показать" прямоугольный фонарик (класс RectFlashlight)
// "Показать" прямоугольный фонарик (класс RectFlashlight)
// "Показать" прямоугольный фонарик (класс RectFlashlight)
void RectFlashlight::Show()
{
    visible = true;

    // Создаем кисти и перья
    HBRUSH grayBrush = CreateSolidBrush(RGB(80, 80, 80));
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    // Корпус фонарика (серый)
    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // ТЕНИ для прямоугольного фонарика
    HBRUSH darkShadowBrush = CreateSolidBrush(RGB(40, 40, 40));  // Темная тень
    HBRUSH mediumShadowBrush = CreateSolidBrush(RGB(60, 60, 60)); // Средняя тень

    // Темная тень с правой стороны
    SelectObject(hdc, darkShadowBrush);
    Rectangle(hdc, x + bodyWidth - 5, y + 3, x + bodyWidth, y + bodyHeight - 3);

    // Средняя тень с нижней стороны
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, x + 3, y + bodyHeight - 5, x + bodyWidth - 3, y + bodyHeight);

    // Головка фонарика (тоже серая)
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    SelectObject(hdc, grayBrush);
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // ТЕНИ для головки
    // Темная тень с правой стороны головки
    SelectObject(hdc, darkShadowBrush);
    Rectangle(hdc, headX + headWidth - 4, headY + 2, headX + headWidth, headY + headHeight - 2);

    // Средняя тень с нижней стороны головки
    SelectObject(hdc, mediumShadowBrush);
    Rectangle(hdc, headX + 2, headY + headHeight - 4, headX + headWidth - 2, headY + headHeight);

    // Кнопка фонарика (красная)
    SelectObject(hdc, redBrush);
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(blackPen);
    DeleteObject(redBrush);
    DeleteObject(grayBrush);
    DeleteObject(darkShadowBrush);
    DeleteObject(mediumShadowBrush);
}

// "Спрятать" прямоугольный фонарик (класс RectFlashlight)
void RectFlashlight::Hide()
{
    visible = true;

    // Создаем кисти и перья
    HBRUSH grayBrush = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    // Корпус фонарика (серый)
    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка фонарика (тоже серая)
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // Кнопка фонарика (красная)
    SelectObject(hdc, redBrush);
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(blackPen);
    DeleteObject(redBrush);
    DeleteObject(grayBrush);
}

/* =============== КРУГЛЫЙ ФОНАРИК =============== */

// Конструктор круглого фонарика (класса RoundFlashlight)
RoundFlashlight::RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
}

// Деструктор (класса RoundFlashlight)
RoundFlashlight::~RoundFlashlight() {}

// "Показать" круглый фонарик (класс RoundFlashlight)
void RoundFlashlight::Show()
{
    visible = true;

    // Создаем кисти и перья
    HBRUSH grayBrush = CreateSolidBrush(RGB(80, 80, 80));
    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    // Корпус фонарика (овал, серый)
    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка фонарика (треугольник, серый)
    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;
    Polygon(hdc, triangle, 3);

    // Кнопка фонарика (зеленый круг)
    SelectObject(hdc, greenBrush);
    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    DeleteObject(blackPen);
    DeleteObject(greenBrush);
    DeleteObject(grayBrush);
}

// "Спрятать" круглый фонарик (класс RoundFlashlight)
void RoundFlashlight::Hide()
{
    visible = true;

    // Создаем кисти и перья
    HBRUSH grayBrush = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH greenBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    // Корпус фонарика (овал, серый)
    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка фонарика (треугольник, серый)
    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;
    Polygon(hdc, triangle, 3);

    // Кнопка фонарика (зеленый круг)
    SelectObject(hdc, greenBrush);
    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    DeleteObject(blackPen);
    DeleteObject(greenBrush);
    DeleteObject(grayBrush);
}

/* =============== ПОВРЕЖДЕННЫЙ ФОНАРИК =============== */

// Конструктор сломанного прямоугольного фонарика (класса BrokenRectFlashlight)
BrokenRectFlashlight::BrokenRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RectFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
    broken = true;
}

// Деструктор (класса BrokenRectFlashlight)
BrokenRectFlashlight::~BrokenRectFlashlight() {}

// "Показать" сломанный прямоугольный фонарик (класс BrokenRectFlashlight)
void BrokenRectFlashlight::Show()
{
    visible = true;

    // Создаем кисти и перья для сломанного фонарика
    HBRUSH grayBrush = CreateSolidBrush(RGB(100, 100, 100));  // Темнее для сломанного
    HBRUSH darkGrayBrush = CreateSolidBrush(RGB(60, 60, 60));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(40, 40, 40));  // Темные трещины
    HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));    // Красные для серьезных повреждений

    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);

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

    // ТРЕЩИНЫ НА ПРЯМОУГОЛЬНОМ ФОНАРИКЕ
    SelectObject(hdc, crackPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH)); // Без заливки для трещин

    // Основная трещина через корпус
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.3, NULL);
    LineTo(hdc, x + bodyWidth * 0.5, y + bodyHeight * 0.6);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.4);

    // Вертикальная трещина
    MoveToEx(hdc, x + bodyWidth * 0.6, y + 5, NULL);
    LineTo(hdc, x + bodyWidth * 0.6, y + bodyHeight - 5);

    // Трещина на головке
    MoveToEx(hdc, headX + headWidth * 0.3, headY + 5, NULL);
    LineTo(hdc, headX + headWidth * 0.7, headY + headHeight - 5);

    // Сломанная кнопка
    SelectObject(hdc, redPen);
    MoveToEx(hdc, buttonX, buttonY, NULL);
    LineTo(hdc, buttonX + buttonWidth, buttonY + buttonHeight);
    MoveToEx(hdc, buttonX + buttonWidth, buttonY, NULL);
    LineTo(hdc, buttonX, buttonY + buttonHeight);

    // Удаляем созданные объекты
    DeleteObject(blackPen);
    DeleteObject(crackPen);
    DeleteObject(redPen);
    DeleteObject(grayBrush);
    DeleteObject(darkGrayBrush);
}

void BrokenRectFlashlight::Hide()
{
    visible = false;

    // Создаем белые кисти и перья для замазывания
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);

        // ЗАМАЗЫВАЕМ СЛОМАННЫЙ ПРЯМОУГОЛЬНЫЙ ФОНАРИК

        // Основной корпус
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

        // Закрашиваем трещины дополнительными линиями
        // Основная трещина через корпус
        MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.3, NULL);
        LineTo(hdc, x + bodyWidth * 0.5, y + bodyHeight * 0.6);
        LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.4);

        // Вертикальная трещина
        MoveToEx(hdc, x + bodyWidth * 0.6, y + 5, NULL);
        LineTo(hdc, x + bodyWidth * 0.6, y + bodyHeight - 5);

        // Трещина на головке
        MoveToEx(hdc, headX + headWidth * 0.3, headY + 5, NULL);
        LineTo(hdc, headX + headWidth * 0.7, headY + headHeight - 5);

        // Сломанная кнопка
        MoveToEx(hdc, buttonX, buttonY, NULL);
        LineTo(hdc, buttonX + buttonWidth, buttonY + buttonHeight);
        MoveToEx(hdc, buttonX + buttonWidth, buttonY, NULL);
        LineTo(hdc, buttonX, buttonY + buttonHeight);

    // Дополнительно замазываем всю область прямоугольником на всякий случай
    Rectangle(hdc, x, y - headHeight, x + bodyWidth, y + bodyHeight);

    // Удаляем созданные объекты
    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
}
/*
void BrokenFlashlight::DrawRectHead()
{
    // Обычная прямоугольная головка (как была)
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);
}

void BrokenFlashlight::DrawRoundHead()
{
    // Сломанная/деформированная головка для круглого фонарика

    // Вариант 1: 
    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;
    Polygon(hdc, triangle, 3);
} */

/* =============== СЛОМАННЫЙ КРУГЛЫЙ ФОНАРИК =============== */

BrokenRoundFlashlight::BrokenRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : RoundFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
    broken = true;
}

BrokenRoundFlashlight::~BrokenRoundFlashlight() {}

void BrokenRoundFlashlight::Show()
{
    visible = true;

    // Создаем кисти и перья для сломанного фонарика
    HBRUSH grayBrush = CreateSolidBrush(RGB(100, 100, 100));  // Темнее для сломанного
    HBRUSH darkGrayBrush = CreateSolidBrush(RGB(60, 60, 60));
    HPEN blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(40, 40, 40));  // Темные трещины
    HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));    // Красные для серьезных повреждений

    SelectObject(hdc, grayBrush);
    SelectObject(hdc, blackPen);

    // СЛОМАННЫЙ КРУГЛЫЙ ФОНАРИК

    // Корпус фонарика (овал)
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка фонарика (треугольник) - слегка деформированная
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
    SelectObject(hdc, darkGrayBrush); // Темнее для сломанной кнопки
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    // ТРЕЩИНЫ НА КРУГЛОМ ФОНАРИКЕ
    SelectObject(hdc, crackPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH)); // Без заливки для трещин

    // Основная трещина через корпус
    MoveToEx(hdc, x + bodyWidth * 0.3, y + bodyHeight * 0.2, NULL);
    LineTo(hdc, x + bodyWidth * 0.7, y + bodyHeight * 0.8);

    // Вторая трещина
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.7, NULL);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.3);

    // Трещина на головке
    MoveToEx(hdc, triangle[0].x, triangle[0].y, NULL);
    LineTo(hdc, triangle[1].x, triangle[1].y);
    LineTo(hdc, triangle[2].x, triangle[2].y);
    LineTo(hdc, triangle[0].x, triangle[0].y); // Замыкаем треугольник трещиной

    // Сломанная кнопка
    SelectObject(hdc, redPen);
    MoveToEx(hdc, buttonX - buttonRadius / 2, buttonY - buttonRadius / 2, NULL);
    LineTo(hdc, buttonX + buttonRadius / 2, buttonY + buttonRadius / 2);
}
void BrokenRoundFlashlight::Hide()
{
    visible = false;

    // Создаем белые кисти и перья для замазывания
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    SelectObject(hdc, whiteBrush);
    SelectObject(hdc, whitePen);
    // ЗАМАЗЫВАЕМ СЛОМАННЫЙ КРУГЛЫЙ ФОНАРИК

    // Корпус фонарика (овал)
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // Головка фонарика (треугольник)
    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2 - 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2 + 2;
    triangle[2].y = y - 3;
    Polygon(hdc, triangle, 3);

    // Кнопка фонарика
    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    // Закрашиваем трещины дополнительными линиями
    // Основная трещина через корпус
    MoveToEx(hdc, x + bodyWidth * 0.3, y + bodyHeight * 0.2, NULL);
    LineTo(hdc, x + bodyWidth * 0.7, y + bodyHeight * 0.8);

    // Вторая трещина
    MoveToEx(hdc, x + bodyWidth * 0.2, y + bodyHeight * 0.7, NULL);
    LineTo(hdc, x + bodyWidth * 0.8, y + bodyHeight * 0.3);

    // Трещина на головке
    MoveToEx(hdc, triangle[0].x, triangle[0].y, NULL);
    LineTo(hdc, triangle[1].x, triangle[1].y);
    LineTo(hdc, triangle[2].x, triangle[2].y);
    LineTo(hdc, triangle[0].x, triangle[0].y);

    // Сломанная кнопка
    MoveToEx(hdc, buttonX - buttonRadius / 2, buttonY - buttonRadius / 2, NULL);
    LineTo(hdc, buttonX + buttonRadius / 2, buttonY + buttonRadius / 2);

    // Дополнительно замазываем всю область прямоугольником на всякий случай
    Rectangle(hdc, x, y - headHeight, x + bodyWidth, y + bodyHeight);

    // Удаляем созданные объекты
    DeleteObject(whitePen);
    DeleteObject(whiteBrush);
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