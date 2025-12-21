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

// Интерфейс для компонентов фонарика
class IFlashlight
{
public:
    virtual void PrintBody() = 0;              // Корпус
    virtual void PrintHead() = 0;              // Головка
    virtual void PrintButton() = 0;            // Кнопка
    virtual void PrintLight() = 0;             // Свет
    virtual void PrintCrack() = 0;             // Трещины
};

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

    virtual void Move();
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

class Square : public Conflict
{
public:
    Square(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Square();

    void Show();
    void Hide();
};

// Базовый класс для фонариков с интерфейсом
class BaseFlashlight : public Point, public IFlashlight
{
protected:
    int bodyWidth;
    int bodyHeight;
    int headWidth;
    int headHeight;

public:
    BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~BaseFlashlight();

    int GetBodyWidth() { return bodyWidth; }
    int GetBodyHeight() { return bodyHeight; }
    int GetHeadWidth() { return headWidth; }
    int GetHeadHeight() { return headHeight; }

    // Методы интерфейса IFlashlight
    virtual void PrintBody() = 0;
    virtual void PrintHead() = 0;
    virtual void PrintButton() = 0;
    virtual void PrintLight() = 0;
    virtual void PrintCrack() = 0;

    virtual void Show() override;
    virtual void Hide() override;
};

// Целый прямоугольный фонарик
class RectFlashlight : public BaseFlashlight
{
public:
    RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RectFlashlight();

    virtual void PrintBody() override;
    virtual void PrintHead() override;
    virtual void PrintButton() override;
    virtual void PrintLight() override;
    virtual void PrintCrack() override;
    virtual void Hide() override;
};

// Сломанный прямоугольный фонарик
class BrokenRectFlashlight : public RectFlashlight
{
public:
    BrokenRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~BrokenRectFlashlight();

    virtual void PrintCrack() override;
};

// Горящий прямоугольный фонарик
class LitRectFlashlight : public RectFlashlight
{
public:
    LitRectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~LitRectFlashlight();

    virtual void PrintLight() override;
    virtual void Hide() override;
};

// Целый круглый фонарик
class RoundFlashlight : public BaseFlashlight
{
public:
    RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RoundFlashlight();

    virtual void PrintBody() override;
    virtual void PrintHead() override;
    virtual void PrintButton() override;
    virtual void PrintLight() override;
    virtual void PrintCrack() override;
};

// Сломанный круглый фонарик
class BrokenRoundFlashlight : public RoundFlashlight
{
public:
    BrokenRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~BrokenRoundFlashlight();

    virtual void PrintCrack() override;
};

// Горящий круглый фонарик
class LitRoundFlashlight : public RoundFlashlight
{
public:
    LitRoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~LitRoundFlashlight();

    virtual void PrintLight() override;
    virtual void Hide() override;
};

// Класс отвертки
class Screwdriver : public Conflict
{
public:
    Screwdriver(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Screwdriver();

    virtual void Show();
    virtual void Hide();
};

// Класс камня
class Stone : public Conflict
{
public:
    Stone(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Stone();

    virtual void Show();
    virtual void Hide();
};

// Класс батарейки
class Battery : public Conflict
{
public:
    Battery(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Battery();

    virtual void Show();
    virtual void Hide();
};

// Класс мухи
class Fly : public Conflict
{
private:
    double angle;    // угол для движения по кругу
    int centerX;     // центр круга по X
    int centerY;     // центр круга по Y
    int circleRadius; // радиус круга

public:
    Fly(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Fly();

    virtual void Show();
    virtual void Hide();
    virtual void Move() override;     // движение по кругу
};

// Класс призрака
class Ghost : public Conflict
{
private:
    double angle;           // угол для движения
    int targetX;           // цель по X (фонарик)
    int targetY;           // цель по Y (фонарик)
    int chaseSpeed;        // скорость погони
    bool active;           // активен ли призрак

public:
    Ghost(int InitX, int InitY, int InitWidth, int InitHeight);
    ~Ghost();

    virtual void Show();
    virtual void Hide();
    virtual void Move() override;
    void UpdateTarget(int targetX, int targetY);  // обновить цель
    void SetActive(bool state) { active = state; }
    bool IsActive() { return active; }
};

#endif