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
    virtual void Show();
    virtual void Hide();
    void SetX(int NewX);
    void SetY(int NewY);
    void MoveTo(int NewX, int NewY);
    void Drag(int Step);
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

    virtual void Show();
    virtual void Hide();
    void Expand(int DeltaRad);
    void Reduse(int DeltaRad);
};

// Класс фонарика
class Flashlight : public Point
{
protected:
    int bodyWidth; // Толщина ручки
    int bodyHeight; // Высота ручки
    int headWidth; // Ширина головки
    int headHeight; // Высота головки
    bool visible; // Видимость

public:
    Flashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight, 
               int InitHeadWidth, int InitHeadHeight);
    ~Flashlight();

    virtual void Show();
    virtual void Hide();
};
// class Flashlight : public MyRectangle
// {
// protected:
//     int headWidth;
//     int headHeight;
//     bool visible;

// public:
//     Flashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight, 
//                int InitHeadWidth, int InitHeadHeight);
//     ~Flashlight();

//     virtual void Show();
//     virtual void Hide();
// };

#endif

