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

    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
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
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
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

/* =============== ������������ ������� =============== */

BrokenFlashlight::BrokenFlashlight(int InitX, int InitY, int InitBodyWidth, int InitBodyHeight,
                                   int InitHeadWidth, int InitHeadHeight, int Type)
    : BaseFlashlight(InitX, InitY, InitBodyWidth, InitBodyHeight, InitHeadWidth, InitHeadHeight)
{
    broken = true;
    damageLevel = 3;
    flashlightType = Type;
}

BrokenFlashlight::~BrokenFlashlight() {}

void BrokenFlashlight::Show()
{
    visible = true;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
    HPEN crackPen = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

    SelectObject(hdc, Pen);

    // �������� ������
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // ������ ������� � ����������� �� ����
    if (flashlightType == 0) {
        DrawRectHead();  // ������������� �������
    }
    else {
        DrawRoundHead(); // �������/��������� �������
    }

    // �������
    // int headX = x + (bodyWidth - headWidth) / 2;
    // int headY = y - headHeight;
    // Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // ������
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // �������
    SelectObject(hdc, crackPen);
    MoveToEx(hdc, x + bodyWidth / 2, y + 5, NULL);
    LineTo(hdc, x + bodyWidth / 2, y + bodyHeight - 5);
    MoveToEx(hdc, x + 5, y + bodyHeight / 2, NULL);
    LineTo(hdc, x + bodyWidth - 5, y + bodyHeight / 2);
    MoveToEx(hdc, x + 10, y + 10, NULL);
    LineTo(hdc, x + bodyWidth - 10, y + bodyHeight - 10);

    DeleteObject(crackPen);
    DeleteObject(Pen);
}

void BrokenFlashlight::Hide()
{
    visible = false;
    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, Pen);

    // �������� ������
    Rectangle(hdc, x, y, x + bodyWidth, y + bodyHeight);

    // �������
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);

    // ������
    int buttonWidth = 10;
    int buttonHeight = 15;
    int buttonX = x + bodyWidth - 20;
    int buttonY = y + bodyHeight - 25;
    Rectangle(hdc, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    DeleteObject(Pen);
}

void BrokenFlashlight::DrawRectHead()
{
    // ������� ������������� ������� (��� ����)
    int headX = x + (bodyWidth - headWidth) / 2;
    int headY = y - headHeight;
    Rectangle(hdc, headX, headY, headX + headWidth, headY + headHeight);
}

void BrokenFlashlight::DrawRoundHead()
{
    // ���������/��������������� ������� ��� �������� ��������

    // ������� 1: 
    POINT triangle[3];
    triangle[0].x = x + bodyWidth / 2;
    triangle[0].y = y - headHeight;
    triangle[1].x = x + bodyWidth / 2 - headWidth / 2;
    triangle[1].y = y;
    triangle[2].x = x + bodyWidth / 2 + headWidth / 2;
    triangle[2].y = y;
    Polygon(hdc, triangle, 3);

    // ������� 2: ��������� ������
    // Ellipse(hdc, x + (bodyWidth - headWidth)/2, y - headHeight, 
    //          x + (bodyWidth + headWidth)/2, y);

    // ������� 3: ����������� ������ (��� � ������ ��������)
    // POINT triangle[3];
    // triangle[0].x = x + bodyWidth / 2;
    // triangle[0].y = y - headHeight;
    // triangle[1].x = x + bodyWidth / 2 - headWidth / 2 - 2;
    // triangle[1].y = y;
    // triangle[2].x = x + bodyWidth / 2 + headWidth / 2 + 2;
    // triangle[2].y = y - 3;
    // Polygon(hdc, triangle, 3);
}