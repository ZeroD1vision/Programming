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

    virtual void Show() override; // "Показать точку"
    virtual void Hide() override; // "Спрятать" точку

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

    virtual void Show() override;
    virtual void Hide() override;
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

    virtual void Show() override;
    virtual void Hide() override;
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

    virtual void Show() override = 0; // чистая виртуальная функция
    virtual void Hide() override = 0;

    // Проверка столкновения со стеной
    bool CheckWallCollision(int wallLeft, int wallTop, int wallRight, int wallBottom);
    // Проверка столкновения с отверткой
    bool CheckScrewdriverCollision(Screwdriver* screwdriver);
};

// Целый прямоугольный фонарик
class RectFlashlight : public BaseFlashlight
{
public:
    RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RectFlashlight();

    virtual void Show();
    virtual void Hide();
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

    virtual void Show() override;
    virtual void Hide() override;
};


#endif

