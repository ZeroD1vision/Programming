#pragma once
/******************************************************************************
 *                               ���� �����������                              *
 *******************************************************************************
 * Project type  : Windows Console Application                                 *
 * Project name  : Pt_1                                                        *
 * File name     : lib.h                                                       *
 * Language      : CPP                                                         *
 * Programmers   : ������� ������� ���������                                   *
 * Modified By   :                                                             *
 * Created       : 13.09.2025                                                  *
 * Last Revision : 26.09.2025                                                  *
 * Comment       : ���������� ��� ������ � �������� - �������                  *
 ******************************************************************************/

#ifndef LIB_H
#define LIB_H

#include <Windows.h>

 // �������� ���������� (����� ��� ���� �����)
extern HDC hdc;

// ����� ����� ����������
class Location
{
protected:
    // ����������
    int x;
    int y;

public:
    Location(int InitX, int InitY); // �����������
    ~Location();                    // ����������

    // ��������� ���������
    int GetX();
    int GetY();

    // ��������� ����� ���������
    virtual void Show();
    virtual void Hide();
    void SetX(int NewX);
    void SetY(int NewY);
    void MoveTo(int NewX, int NewY);
    void Drag(int Step);
};

// ����� ����� � ������������ - ����� ���������
class Point : public Location
{
protected:
    bool visible; // ���������

public:
    Point(int InitX, int InitY); // �����������
    ~Point();                    // ����������

    bool GetVisible();                // ��������� ��������� ���������
    void SetVisible(bool NewVisible); // ��������� ��������� ���������

    virtual void Show(); // "�������� �����"
    virtual void Hide(); // "��������" �����

    void MoveTo(int NewX, int NewY); // ����������� �����
    void Drag(int Step);
};

// ����� �����
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

// ����� ��������
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

// ������� ����� ��� ��������� � ����� �����������������
class BaseFlashlight : public Point
{
protected:
    int bodyWidth;
    int bodyHeight;
    int headWidth;
    int headHeight;
    bool broken; // ��������� ��������
    int damageLevel; // ������� ����������� (0-3)

public:
    BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
                   int InitHeadWidth, int InitHeadHeight);
    ~BaseFlashlight();

    bool IsBroken() { return broken; }
    void Break() { broken = true; }
    void Repair() { broken = false; damageLevel = 0; }
    void SetDamage(int level) { damageLevel = level; if (level >= 3) broken = true; }
    int GetDamage() { return damageLevel; }

    virtual void Show() = 0; // ������ ����������� �������
    virtual void Hide() = 0;

    // �������� ������������ �� ������
    bool CheckWallCollision(int wallLeft, int wallTop, int wallRight, int wallBottom);
    // �������� ������������ � ���������
    bool CheckScrewdriverCollision(Screwdriver* screwdriver);
};

// ������ ��� �������� - �������������
class RectFlashlight : public BaseFlashlight
{
public:
    RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~RectFlashlight();

    virtual void Show();
    virtual void Hide();

private:
    void DrawCracks(); // ������ ������� �� �������
    void DrawStains(); // ������ ����� �� �������
};

// ������ ��� �������� - �������
class RoundFlashlight : public BaseFlashlight
{
public:
    RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
                    int InitHeadWidth, int InitHeadHeight);
    ~RoundFlashlight();

    virtual void Show();
    virtual void Hide();
private:
    void DrawCracks(); // ������ ������� �� �������
    void DrawBrokenPieces(); // ������ ������������ �����
};


#endif

