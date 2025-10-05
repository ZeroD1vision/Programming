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

#include <windows.h>

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
class Flashlight : public Point
{
protected:
    int bodyWidth; // ������� �����
    int bodyHeight; // ������ �����
    int headWidth; // ������ �������
    int headHeight; // ������ �������
    bool visible; // ���������

public:
    Flashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
        int InitHeadWidth, int InitHeadHeight);
    ~Flashlight();

    virtual void Show();
    virtual void Hide();
};

#endif


