#pragma once
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

#ifndef LIB_H
#define LIB_H

#include <Windows.h>

 // Контекст устройства (общий для всех фигур)
extern HDC hdc;

// Класс места нахождения
class Location
{
protected:
    // координаты
    int x;
    int y;

public:
    Location(int InitX, int InitY); // конструктор
    ~Location();                    // деструктор

    // Получение координат
    int GetX();
    int GetY();

    // Установка новых координат
    void SetX(int NewX);
    void SetY(int NewY);
};

// Класс точки с прародителем - место положение
class Point : public Location
{
protected:
    bool visible; // видимость

public:
    Point(int InitX, int InitY); // конструктор
    ~Point();                    // деструктор

    bool GetVisible();                // Получение состояния видимости
    void SetVisible(bool NewVisible); // Установка состояния видимости

    virtual void Show(); // "Показать точку"
    virtual void Hide(); // "Спрятать" точку

    void MoveTo(int NewX, int NewY); // Переместить точку
    void Drag(int Step);
};

// Класс круга
class Circle : public Point
{
protected:
    int radius;

public:
    Circle(int InitX, int InitY, int InitRadius);
    ~Circle();

    void Show();
    void Hide();
    void Expand(int DeltaRad);
    void Reduse(int DeltaRad);

    void MoveTo(int NewX, int NewY);
};

// Класс отвертки
class Screwdriver : public Point
{
protected:
    int length;
    int width;
    bool visible;

public:
    Screwdriver(int InitX, int InitY, int InitLength, int InitWidth);
    ~Screwdriver();

    void Show();
    void Hide();

    int GetWidth() {
        return width;
    }

    int GetLength() {
        return length;
    }
};

// Базовый класс для фонариков с общей функциональностью
class BaseFlashlight : public Point
{
protected:
    int bodyWidth;
    int bodyHeight;
    int headWidth;
    int headHeight;
    bool broken; // состояние фонарика

public:
    BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~BaseFlashlight();

    int GetBodyWidth() { return bodyWidth; }
    int GetBodyHeight() { return bodyHeight; }
    int GetHeadWidth() { return headWidth; }
    int GetHeadHeight() { return headHeight; }

    bool IsBroken() { return broken; }
};

// Целый прямоугольный фонарик
class RectFlashlight : public BaseFlashlight
{
public:
    RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RectFlashlight();

    void Show();
    void Hide();
    void MoveTo(int NewX, int NewY);
};

// Целый круглый фонарик
class RoundFlashlight : public BaseFlashlight
{
public:
    RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RoundFlashlight();

    void Show();
    void Hide();
};

// Поврежденный фонарик (универсальный класс)
class BrokenFlashlight : public BaseFlashlight
{
private:
    int flashlightType; // 0 - прямоугольный, 1 - круглый
    void DrawRectHead();   // Головка для прямоугольного
    void DrawRoundHead();  // Головка для круглого
public:
    BrokenFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight, int Type);
    ~BrokenFlashlight();

    void Show();
    void Hide();
};

class Stone : public Point {
protected:
    int width, height;
public:
    Stone(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Stone();

    void Show();
    void Hide();
    int GetStoneWidth() { return width; }
    int GetStoneHeight() { return height; }
};
#endif

