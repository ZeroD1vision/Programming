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

    virtual void Show();
    virtual void Hide();
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
    int damageLevel; // уровень повреждения (0-3)

public:
    BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
                   int InitHeadWidth, int InitHeadHeight);
    ~BaseFlashlight();

    bool IsBroken() { return broken; }
    void Break() { broken = true; }
    void Repair() { broken = false; damageLevel = 0; }
    void SetDamage(int level) { damageLevel = level; if (level >= 3) broken = true; }
    int GetDamage() { return damageLevel; }

    virtual void Show() = 0; // чистая виртуальная функция
    virtual void Hide() = 0;

    // Проверка столкновения со стеной
    bool CheckWallCollision(int wallLeft, int wallTop, int wallRight, int wallBottom);
    // Проверка столкновения с отверткой
    bool CheckScrewdriverCollision(Screwdriver* screwdriver);
};

// Первый тип фонарика - прямоугольный
class RectFlashlight : public BaseFlashlight
{
public:
    RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RectFlashlight();

    virtual void Show();
    virtual void Hide();

private:
    void DrawCracks(); // Рисует трещины на корпусе
    void DrawStains(); // Рисует пятна на корпусе
};

// Второй тип фонарика - круглый
class RoundFlashlight : public BaseFlashlight
{
public:
    RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
                    int InitHeadWidth, int InitHeadHeight);
    ~RoundFlashlight();

    virtual void Show();
    virtual void Hide();
private:
    void DrawCracks(); // Рисует трещины на корпусе
    void DrawBrokenPieces(); // Рисует отколовшиеся куски
};


#endif

