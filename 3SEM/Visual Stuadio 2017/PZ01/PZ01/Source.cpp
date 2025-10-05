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

#include "Header.h"
#include <iostream>

using namespace std;

// ������ ��� ����������� ���� ������� �������
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*           � � � � � � � � � �  � � � � � � �                 */
/****************************************************************/

/* =============== ����� Location =============== */

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

void Location::Show() { /* ������ �� ������, ���������� � ����������� */ }
void Location::Hide() { /* ������ �� ������, ���������� � ����������� */ }

/* =============== ����� Point =============== */

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

/* =============== ����� Circle =============== */

// ����������� ����� (������ Circle)
Circle::Circle(int InitX, int InitY, int InitRadius) :
    Point(InitX, InitY) {
    radius = InitRadius;
}

// �������������
Circle::~Circle() {}

// �������� ����
void Circle::Show() {
    visible = true;	// �������� ��������� �� ������

    // ������ ����� ��� ���������
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, Pen);

    // ������ ������� ����
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // ������� �����
    DeleteObject(Pen);
}

// �������� ����
void Circle::Hide() {
    visible = false;	// �������� ��������� �� �����

    // ������ ����� ��� ���������
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    SelectObject(hdc, Pen);

    // ������ ����� ����
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // ������� �����
    DeleteObject(Pen);
}

// ��������� ����
void Circle::Expand(int DeltaRad) {
    Hide();	// ������ ����

    // �������� ������
    radius = radius + DeltaRad;
    if (radius < 2) { radius = 2; } // �������� ��� Reduce

    Show();	// ���������� ����
}

// ����� ����
void Circle::Reduse(int DeltaRad) {
    Expand(-DeltaRad);
}

/* =============== ����� Screwdriver =============== */

Screwdriver::Screwdriver(int InitX, int InitY, int InitLength, int InitWidth)
    : Point(InitX, InitY)
{
    length = InitLength;
    width = InitWidth;
    visible = false;
}

Screwdriver::~Screwdriver() {}

void Screwdriver::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(139, 69, 19)); // ���������� ����
    HPEN ThinPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192)); // ���������� ����

    SelectObject(hdc, Pen);

    // ����� ��������
    Rectangle(hdc, x, y, x + width, y - length + 5);

    // ������������� �����
    SelectObject(hdc, ThinPen);
    Rectangle(hdc, x + 3, y, x + width / 2 + 3, y + length);

    // ����������
    POINT tip[3];
    tip[0].x = x + width / 4 + 3;
    tip[0].y = y + length;
    tip[1].x = x + 3;
    tip[1].y = y + length + 10;
    tip[2].x = x + width / 2 + 3;
    tip[2].y = y + length + 10;
    Polygon(hdc, tip, 3);

    DeleteObject(Pen);
    DeleteObject(ThinPen);
}

void Screwdriver::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    // ����� ��������
    Rectangle(hdc, x, y, x + width, y - length + 5);

    // ������������� �����
    SelectObject(hdc, Pen);
    Rectangle(hdc, x + 3, y, x + width / 2 + 3, y + length);

    // ����������
    POINT tip[3];
    tip[0].x = x + width / 4 + 3;
    tip[0].y = y + length;
    tip[1].x = x + 3;
    tip[1].y = y + length + 10;
    tip[2].x = x + width / 2 + 3;
    tip[2].y = y + length + 10;
    Polygon(hdc, tip, 3);

    DeleteObject(Pen);
}

/* =============== ������� ����� Flashlight =============== */

BaseFlashlight::BaseFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight) : Point(InitX, InitY)
{
    bodyWidth = InitBodyWidth;
    bodyHeight = InitBodyHeight;
    headWidth = InitHeadWidth;
    headHeight = InitHeadHeight;
    broken = false;
    damageLevel = 0;
}

BaseFlashlight::~BaseFlashlight() {}

bool BaseFlashlight::CheckWallCollision(int wallLeft, int wallTop, int wallRight, int wallBottom)
{
    // ��������� ������������ ������� �������� �� ������
    if (x < wallLeft || x + bodyWidth > wallRight ||
        y < wallTop || y + bodyHeight > wallBottom)
    {
        if (damageLevel < 3) damageLevel++;
        if (damageLevel >= 3) Break();
        return true;
    }
    return false;
}

bool BaseFlashlight::CheckScrewdriverCollision(Screwdriver* screwdriver)
{
    int screwX = screwdriver->GetX();
    int screwY = screwdriver->GetY();

    // ������� �������� ������������ - ����������� ���������������
    if (screwX >= x && screwX <= x + bodyWidth &&
        screwY >= y && screwY <= y + bodyHeight)
    {
        if (damageLevel > 0) damageLevel--;
        if (damageLevel == 0) Repair();
        return true;
    }
    return false;
}

/* =============== ������������� ������� =============== */

RectFlashlight::RectFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
}

RectFlashlight::~RectFlashlight() {}

void RectFlashlight::Show()
{
    visible = true;
    COLORREF color;

    if (broken)
        color = RGB(128, 128, 128); // ����� ���� ��� ��������� ��������
    else
        color = RGB(255, 0, 0);     // ������� ���� ��� ������

    HPEN Pen = CreatePen(PS_SOLID, 2, color);
    SelectObject(hdc, Pen);

    // ������ �������� (�������������)
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // ������� �������� (�������������)
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // ������ ��������
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // ������ ����������� � ����������� �� ������
    if (damageLevel > 0) {
        DrawCracks();
    }
    if (damageLevel >= 2) {
        DrawStains();
    }

    DeleteObject(Pen);
}

void RectFlashlight::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(Pen);
}

void RectFlashlight::DrawCracks()
{
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));
    SelectObject(hdc, crackPen);

    // ������ ������� � ����������� �� ������ �����������
    if (damageLevel >= 1) {
        // ������������ �������
        MoveToEx(hdc, x + bodyWidth / 2, y + 5, NULL);
        LineTo(hdc, x + bodyWidth / 2, y + bodyHeight - 5);
    }

    if (damageLevel >= 2) {
        // �������������� �������
        MoveToEx(hdc, x + 5, y + bodyHeight / 2, NULL);
        LineTo(hdc, x + bodyWidth - 5, y + bodyHeight / 2);

        // ������������ �������
        MoveToEx(hdc, x + 10, y + 10, NULL);
        LineTo(hdc, x + bodyWidth - 10, y + bodyHeight - 10);
    }

    if (damageLevel >= 3) {
        // �������������� ������� ��� ��������� ��������� ���������
        MoveToEx(hdc, x + bodyWidth - 10, y + 10, NULL);
        LineTo(hdc, x + 10, y + bodyHeight - 10);

        // ������ ������� �� �����
        for (int i = 0; i < 3; i++) {
            MoveToEx(hdc, x + 5 + i * 5, y + 5, NULL);
            LineTo(hdc, x + 5 + i * 5, y + 15);
        }
    }

    DeleteObject(crackPen);
}

void RectFlashlight::DrawStains()
{
    HPEN stainPen = CreatePen(PS_SOLID, 1, RGB(139, 69, 19));
    SelectObject(hdc, stainPen);

    // ������ ������ �����
    if (damageLevel >= 2) {
        // ����� � ����� ������� ����
        Ellipse(hdc, x + 5, y + 5, x + 20, y + 20);

        // ����� � ������ ������ ����
        Ellipse(hdc, x + bodyWidth - 20, y + bodyHeight - 20,
            x + bodyWidth - 5, y + bodyHeight - 5);
    }

    if (damageLevel >= 3) {
        // �������������� ����� ��� ��������� ��������� ���������
        Ellipse(hdc, x + bodyWidth - 25, y + 10, x + bodyWidth - 10, y + 25);
        Ellipse(hdc, x + 10, y + bodyHeight - 25, x + 25, y + bodyHeight - 10);
    }

    DeleteObject(stainPen);
}

/* =============== ������� ������� =============== */

RoundFlashlight::RoundFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
    int InitHeadWidth, int InitHeadHeight)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
}

RoundFlashlight::~RoundFlashlight() {}

void RoundFlashlight::Show()
{
    visible = true;
    COLORREF color;

    if (broken)
        color = RGB(128, 128, 128); // ����� ���� ��� ��������� ��������
    else
        color = RGB(0, 255, 0);     // ������� ���� ��� ������

    HPEN Pen = CreatePen(PS_SOLID, 2, color);
    SelectObject(hdc, Pen);

    // ������ �������� (����)
    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // ������� �������� (�����������)
    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;

    Polygon(hdc, triangle, 3);

    // ������ �������� (����)
    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    // ������ �����������
    if (damageLevel > 0) {
        DrawCracks();
    }
    if (damageLevel >= 3) {
        DrawBrokenPieces();
    }

    DeleteObject(Pen);
}

void RoundFlashlight::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    Ellipse(hdc, x, y, x + bodyWidth, y + bodyHeight);

    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;

    Polygon(hdc, triangle, 3);

    int buttonRadius = 8;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 20;
    Ellipse(hdc, buttonX - buttonRadius, buttonY - buttonRadius,
        buttonX + buttonRadius, buttonY + buttonRadius);

    DeleteObject(Pen);
}

void RoundFlashlight::DrawCracks()
{
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));
    SelectObject(hdc, crackPen);

    int centerX = x + bodyWidth / 2;
    int centerY = y + bodyHeight / 2;
    int radius = bodyWidth / 2;

    // ������ ���������� ������� �� ������
    if (damageLevel >= 1) {
        // ������� ������ ����
        MoveToEx(hdc, centerX, centerY - radius, NULL);
        LineTo(hdc, centerX, centerY + radius);
    }

    if (damageLevel >= 2) {
        // ������� ����� �������
        MoveToEx(hdc, centerX - radius, centerY, NULL);
        LineTo(hdc, centerX + radius, centerY);

        // ������������ �������
        double angle = 45 * 3.14159 / 180;
        MoveToEx(hdc, centerX - radius * cos(angle), centerY - radius * sin(angle), NULL);
        LineTo(hdc, centerX + radius * cos(angle), centerY + radius * sin(angle));
    }

    if (damageLevel >= 3) {
        // �������������� ������� ��� ��������� ��������� ���������
        double angle = 30 * 3.14159 / 180;
        MoveToEx(hdc, centerX - radius * cos(angle), centerY - radius * sin(angle), NULL);
        LineTo(hdc, centerX + radius * cos(angle), centerY + radius * sin(angle));

        angle = 60 * 3.14159 / 180;
        MoveToEx(hdc, centerX - radius * cos(angle), centerY - radius * sin(angle), NULL);
        LineTo(hdc, centerX + radius * cos(angle), centerY + radius * sin(angle));
    }

    DeleteObject(crackPen);
}

void RoundFlashlight::DrawBrokenPieces()
{
    HPEN piecePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    SelectObject(hdc, piecePen);

    int centerX = x + bodyWidth / 2;
    int centerY = y + bodyHeight / 2;
    int radius = bodyWidth / 2;

    // ������ ������������ ����� �� �����
    if (damageLevel >= 3) {
        // ������� ����� �����
        Arc(hdc, centerX - radius, centerY - radius, centerX, centerY,
            centerX - radius / 2, centerY - radius, centerX, centerY - radius / 2);

        // ������ ������ �����
        Arc(hdc, centerX, centerY, centerX + radius, centerY + radius,
            centerX, centerY + radius / 2, centerX + radius / 2, centerY + radius);

        // ������ �����, ������������, ��� ����� ��������
        MoveToEx(hdc, centerX - radius / 2, centerY - radius, NULL);
        LineTo(hdc, centerX - radius / 2, centerY - radius / 2);
        LineTo(hdc, centerX, centerY - radius / 2);

        MoveToEx(hdc, centerX + radius / 2, centerY + radius, NULL);
        LineTo(hdc, centerX + radius / 2, centerY + radius / 2);
        LineTo(hdc, centerX, centerY + radius / 2);
    }

    DeleteObject(piecePen);
}

// Flashlight::Flashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight, 
//                        int InitHeadWidth, int InitHeadHeight) 
//     : MyRectangle(InitX, InitY, InitBodyWidth, InitBodyHeight)
// {
//     headWidth = InitHeadWidth;
//     headHeight = InitHeadHeight;
// }

// void Flashlight::Show()
// {
//     visible = true;
//     // 1. ������� � ���������� ������ �������� (���������� ������� �����)
//     MyRectangle::Show();

//     // 2. ������� � ���������� ������� �������� (�������������)
//     int headX = x + (width - headWidth) / 2;
//     int headY = y - headHeight;
//     MyRectangle head(headX, headY, headWidth, headHeight);
//     head.Show();

//     // 3. ������� � ���������� ������ �������� (��������� �������������)
//     int buttonWidth = 15;
//     int buttonHeight = 20;
//     int buttonX = x + width - 25;
//     int buttonY = y + height - 30;
//     MyRectangle button(buttonX, buttonY, buttonWidth, buttonHeight);
//     button.Show();
// }

// void Flashlight::Hide()
// {
//     visible = false;
//     // 1. ������� � �������� ������ ��������
//     MyRectangle::Hide();

//     // 2. ������� � �������� ������� ��������
//     int headX = x + (width - headWidth) / 2;
//     int headY = y - headHeight;
//     MyRectangle head(headX, headY, headWidth, headHeight);
//     head.Hide();

//     // 3. ������� � �������� ������ ��������
//     int buttonX = x + width - 25;
//     int buttonY = y + height - 30;
//     int buttonWidth = 15;
//     int buttonHeight = 20;
//     MyRectangle button(buttonX, buttonY, buttonWidth, buttonHeight);
//     button.Hide();
// }