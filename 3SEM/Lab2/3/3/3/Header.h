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

// Класс конфликтных объектов (имеют размеры)
class Conflict : public Point
{
protected:
    int width;
    int height;

public:
    Conflict(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Conflict();

    int GetWidth() { return width; }
    int GetHeight() { return height; }
    void SetWidth(int NewWidth) { width = NewWidth; }
    void SetHeight(int NewHeight) { height = NewHeight; }
};

class Square : public Conflict
{
public:
    Square(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Square();

    void Show();
    void Hide();
};

// Класс круга
class Circle : public Conflict
{
protected:
    int radius;

public:
    Circle(int InitX, int InitY, int InitRadius);
    ~Circle();

    virtual void Show() override;
    virtual void Hide() override;
    void Expand(int DeltaRad);
    void Reduse(int DeltaRad);
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
    void Break() { broken = true; }
    void Repair() { broken = false; }
};

// Целый прямоугольный фонарик
class RectFlashlight : public BaseFlashlight
{
public:
    RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RectFlashlight();

    virtual void Show() override;
    virtual void Hide() override;
};

// Сломанный прямоугольный фонарик (наследуется от RectFlashlight)
class BrokenRectFlashlight : public RectFlashlight
{
public:
    BrokenRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~BrokenRectFlashlight();

    virtual void Show() override;
    virtual void Hide() override;
};

// Целый круглый фонарик
class RoundFlashlight : public BaseFlashlight
{
public:
    RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RoundFlashlight();

    virtual void Show() override;
    virtual void Hide() override;
};

// Сломанный круглый фонарик (наследуется от RoundFlashlight)
class BrokenRoundFlashlight : public RoundFlashlight
{
public:
    BrokenRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~BrokenRoundFlashlight();

    virtual void Show() override;
    virtual void Hide() override;
};

// Класс отвертки (недвижимый)
class Screwdriver : public Conflict
{
public:
    Screwdriver(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Screwdriver();

    void Show();
    void Hide();
};

// Класс камня
class Stone : public Conflict
{
public:
    Stone(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Stone();

    void Show();
    void Hide();
};

#endif