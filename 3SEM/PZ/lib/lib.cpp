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

#include "lib.h"
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

void Location::MoveTo(int NewX, int NewY)
{
    Hide();
    x = NewX;
    y = NewY;
    Show();
}

void Location::Drag(int Step)
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
    Location::Drag(Step);
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

/* =============== КЛАСС MyRectangle =============== */

MyRectangle::MyRectangle(int InitX, int InitY, int InitWidth, int InitHeight) : Point(InitX, InitY)
{
    width = InitWidth;
    height = InitHeight;
}

MyRectangle::~MyRectangle() {}

void MyRectangle::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, Pen);

    Rectangle(hdc, x, y, x + width, y + height);

    DeleteObject(Pen);
}

void MyRectangle::Hide() 
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    SelectObject(hdc, Pen);

    Rectangle(hdc, x, y, x + width, y + height);

    DeleteObject(Pen);  
}

/* =============== КЛАСС Flashlight =============== */

Flashlight::Flashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight, 
                       int InitHeadWidth, int InitHeadHeight) 
    : MyRectangle(InitX, InitY, InitBodyWidth, InitBodyHeight)
{
    headWidth = InitHeadWidth;
    headHeight = InitHeadHeight;
}

Flashlight::~Flashlight() {}

void Flashlight::Show()
{
    visible = true;
    // 1. Создаем и показываем корпус фонарика (используем базовый класс)
    MyRectangle::Show();
    
    // 2. Создаем и показываем головку фонарика (прямоугольник)
    int headX = x + (width - headWidth) / 2;
    int headY = y - headHeight;
    MyRectangle head(headX, headY, headWidth, headHeight);
    head.Show();
    
    // 3. Создаем и показываем кнопку фонарика (маленький прямоугольник)
    int buttonWidth = 15;
    int buttonHeight = 20;
    int buttonX = x + width - 25;
    int buttonY = y + height - 30;
    MyRectangle button(buttonX, buttonY, buttonWidth, buttonHeight);
    button.Show();
}

void Flashlight::Hide()
{
    visible = false;
    // 1. Создаем и скрываем корпус фонарика
    MyRectangle::Hide();
    
    // 2. Создаем и скрываем головку фонарика
    int headX = x + (width - headWidth) / 2;
    int headY = y - headHeight;
    MyRectangle head(headX, headY, headWidth, headHeight);
    head.Hide();
    
    // 3. Создаем и скрываем кнопку фонарика
    int buttonX = x + width - 25;
    int buttonY = y + height - 30;
    int buttonWidth = 15;
    int buttonHeight = 20;
    MyRectangle button(buttonX, buttonY, buttonWidth, buttonHeight);
    button.Hide();
}
// Flashlight::Flashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight, 
//                        int InitHeadWidth, int InitHeadHeight) 
//     : Point(InitX, InitY)
// {
//     bodyWidth = InitBodyWidth;
//     bodyHeight = InitBodyHeight;
//     headWidth = InitHeadWidth;
//     headHeight = InitHeadHeight;
// }

// Flashlight::~Flashlight() {}

// void Flashlight::Show() {
//     visible = true;
//     HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));  // Красный перо
//     HPEN OldPen = (HPEN)SelectObject(hdc, Pen);  // Сохранить старый перо
    
//     // Корпус: рисовать как четыре линии (прямоугольник)
//     MoveToEx(hdc, x, y, NULL);  // Начать слева-сверху
//     LineTo(hdc, x + bodyWidth, y);  // Вверхняя сторона
//     LineTo(hdc, x + bodyWidth, y + bodyHeight);  // Правая сторона
//     LineTo(hdc, x, y + bodyHeight);  // Нижняя сторона
//     LineTo(hdc, x, y);  // Левая сторона (закрыть цикл)
    
//     // Головка (трапеция): четыре линии
//     int headLeftX = x + (bodyWidth - headWidth) / 2;  // Левая верхняя
//     int headCenterY = y;  // Верх головы совпадает с корпусом
//     int headBottomY = y - headHeight;  // Снизу головы (выше корпуса)
//     MoveToEx(hdc, headLeftX, headCenterY, NULL);  // Начать слева-сверху головы
//     LineTo(hdc, x + (bodyWidth + headWidth) / 2, headCenterY);  // Верх
//     LineTo(hdc, x + bodyWidth, headBottomY);  // Правая наклонная
//     LineTo(hdc, x, headBottomY);  // Низ
//     LineTo(hdc, headLeftX, headCenterY);  // Левая наклонная (закрыть)
    
//     // Кнопка: четыре линии (маленький прямоугольник)
//     MoveToEx(hdc, x + bodyWidth - 20, y + bodyHeight - 30, NULL);  // Левый верх кнопки
//     LineTo(hdc, x + bodyWidth - 5, y + bodyHeight - 30);  // Верх
//     LineTo(hdc, x + bodyWidth - 5, y + bodyHeight - 10);  // Правая
//     LineTo(hdc, x + bodyWidth - 20, y + bodyHeight - 10);  // Низ
//     LineTo(hdc, x + bodyWidth - 20, y + bodyHeight - 30);  // Левая (закрыть)
    
//     SelectObject(hdc, OldPen);  // Восстановить старый перо
//     DeleteObject(Pen);  // Удалить созданное перо
// }

// void Flashlight::Hide() {
//     visible = false;
//     HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));  // Синее перо
//     HPEN OldPen = (HPEN)SelectObject(hdc, Pen);
    
//     // Корпус: четыре линии в чёрном
//     MoveToEx(hdc, x, y, NULL);
//     LineTo(hdc, x + bodyWidth, y);
//     LineTo(hdc, x + bodyWidth, y + bodyHeight);
//     LineTo(hdc, x, y + bodyHeight);
//     LineTo(hdc, x, y);
    
//     // Головка: четыре линии в чёрном
//     int headLeftX = x + (bodyWidth - headWidth) / 2;
//     int headCenterY = y;
//     int headBottomY = y - headHeight;
//     MoveToEx(hdc, headLeftX, headCenterY, NULL);
//     LineTo(hdc, x + (bodyWidth + headWidth) / 2, headCenterY);
//     LineTo(hdc, x + bodyWidth, headBottomY);
//     LineTo(hdc, x, headBottomY);
//     LineTo(hdc, headLeftX, headCenterY);
    
//     // Кнопка: четыре линии в чёрном
//     MoveToEx(hdc, x + bodyWidth - 20, y + bodyHeight - 30, NULL);
//     LineTo(hdc, x + bodyWidth - 5, y + bodyHeight - 30);
//     LineTo(hdc, x + bodyWidth - 5, y + bodyHeight - 10);
//     LineTo(hdc, x + bodyWidth - 20, y + bodyHeight - 10);
//     LineTo(hdc, x + bodyWidth - 20, y + bodyHeight - 30);
    
//     SelectObject(hdc, OldPen);
//     DeleteObject(Pen);
// }

// // Вспомогательная функция для рисования линии пикселями (простой цикл для диагонали)
// // Использует SetPixel для каждого пикселя на линии (упрощённо, без оптимизации)
// void drawLinePixel(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
//     int dx = abs(x2 - x1);
//     int dy = abs(y2 - y1);
//     int sx = (x1 < x2) ? 1 : -1;
//     int sy = (y1 < y2) ? 1 : -1;
//     int err = dx - dy;

//     while (true) {
//         SetPixel(hdc, x1, y1, color);
//         if (x1 == x2 && y1 == y2) break;
//         int e2 = 2 * err;
//         if (e2 > -dy) {
//             err -= dy;
//             x1 += sx;
//         }
//         if (e2 < dx) {
//             err += dx;
//             y1 += sy;
//         }
//     }
// }

// void Flashlight::Hide() {
//     visible = false;
//     HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));  // Синяя ручка (для совместимости, хотя не используется для линий)
//     HPEN OldPen = (HPEN)SelectObject(hdc, Pen);
    
//     // Корпус: рисуем как прямоугольник (из класса MyRectangle или GDI Rectangle)
//     // Предполагаем, что у Flashlight есть bodyWidth и bodyHeight
//     Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);  // Синий прямоугольник для корпуса
    
//     // Кнопка: рисуем как прямоугольник
//     // Предполагаем размеры кнопки (например, 15x20, как в вашем коде)
//     int buttonWidth = 15;
//     int buttonHeight = 20;
//     Rectangle(hdc, x + bodyWidth - buttonWidth, y + bodyHeight - buttonHeight, 
//               x + bodyWidth, y + bodyHeight);  // Синий прямоугольник для кнопки
    
//     // Головка: две наклонные линии, нарисованные циклом с SetPixel
//     // Расчёты координат как в вашем коде
//     int headLeftX = x + (bodyWidth - headWidth) / 2;
//     int headRightX = x + (bodyWidth + headWidth) / 2;
//     int headCenterY = y;
//     int headBottomY = y - headHeight;
    
//     // Первая линия: от (headLeftX, headCenterY) к (headRightX, headCenterY) — горизонтальная, но можно нарисовать пикселями
//     drawLinePixel(hdc, headLeftX, headCenterY, headRightX, headCenterY, RGB(0, 0, 255));
    
//     // Вторая линия: от (headRightX, headCenterY) к (x + bodyWidth, headBottomY) — наклонная
//     drawLinePixel(hdc, headRightX, headCenterY, x + bodyWidth, headBottomY, RGB(0, 0, 255));
    
//     // Третья линия: от (x + bodyWidth, headBottomY) к (x, headBottomY) — горизонтальная
//     drawLinePixel(hdc, x + bodyWidth, headBottomY, x, headBottomY, RGB(0, 0, 255));
    
//     // Четвёртая линия: от (x, headBottomY) к (headLeftX, headCenterY) — наклонная
//     drawLinePixel(hdc, x, headBottomY, headLeftX, headCenterY, RGB(0, 0, 255));
    
//     // Восстанавливаем старую ручку и удаляем
//     SelectObject(hdc, OldPen);
//     DeleteObject(Pen);
// }