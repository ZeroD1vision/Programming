/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Windows Console Application                                 *
 * Project name  : Pt_1                                                        *
 * File name     : main.cpp                                                    *
 * Language      : CPP                                                         *
 * Programmers   : Шалаев Александр Максимович                                 *
 * Modified By   :                                                             *
 * Created       : 13.09.2025                                                  *
 * Last Revision : 13.09.2025                                                  *
 * Comment       : Фигура:  фотоаппарат                                        *
 ******************************************************************************/

#include <iostream>
#include <Windows.h> // windows API

using namespace std;

// макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*Г Л О Б А Л Ь Н Ы Е  К О Н С Т А Н Т Ы  И  П Е Р Е М Е Н Н Ы Е*/
/****************************************************************/

HDC hdc; // Объявим контекст устройства
         // Контекст устройства это структура, содержащая
         // описание видеокарты на вашем компьютере
         // и всех необходимых графических элементов

/****************************************************************/
/*                Р А Б О Т А  С  К Л А С С А М И               */
/****************************************************************/

/* =============== ИНИЦИАЛИЗАЦИЯ КЛАССОВ =============== */

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
private:
    bool visible; // видимость

public:
    Point(int InitX, int InitY); // конструктор
    ~Point();                    // деструктор

    bool GetVisible();                // Получение состояния видимости
    void SetVisible(bool NewVisible); // Установка состояния видимости

    void Show(); // "Показать точку"
    void Hide(); // "Спрятать" точку

    void MoveTo(int NewX, int NewY); // Переместить точку
};

/* ========== РЕАЛИЗАЦИЯ МЕТОДОВ КЛАССОВ ========== */

/* ----- Класс Location ----- */

// конструктор класса Location
Location::Location(int InitX, int InitY)
{
    // Устанавливаем заданные координаты
    x = InitX;
    y = InitY;
}

// деструктор класса Location
Location::~Location() {}

// получение координат класса Location
int Location::GetX() { return x; }
int Location::GetY() { return y; }

// установка координат класса  Location
void Location::SetX(int NewX) { x = NewX; }
void Location::SetY(int NewY) { y = NewY; }

/* ----- Класс Point ----- */

// конструктор класса Point, от части наследованный у класса Location
Point::Point(int InitX, int InitY) : Location(InitX, InitY)
{
    visible = false; // Видимость по умолчинаю фальш
}

// деструктор класса Point
Point::~Point() {}

// показать точку (класса Point)
void Point::Show()
{
    visible = true; // Значение видимости на правду

    // Рисуем красную точку
    SetPixel(hdc, x, y, RGB(255, 0, 0));
    SetPixel(hdc, x, y + 1, RGB(255, 0, 0));
    SetPixel(hdc, x + 1, y, RGB(255, 0, 0));
    SetPixel(hdc, x + 1, y + 1, RGB(255, 0, 0));
}

// спрятать точку (класса Point)
void Point::Hide()
{
    visible = false; // Значение видимости на фальш

    // Рисуем синюю точку
    SetPixel(hdc, x, y, RGB(0, 0, 255));
    SetPixel(hdc, x, y + 1, RGB(0, 0, 255));
    SetPixel(hdc, x + 1, y, RGB(0, 0, 255));
    SetPixel(hdc, x + 1, y + 1, RGB(0, 0, 255));
}

// перемещение точки (класса Point)
void Point::MoveTo(int NewX, int NewY)
{
    Hide(); // Спрятали точку

    // Переместили на новые координаты
    x = NewX;
    y = NewY;

    Show(); // Показали точку
}

// получение видимости точки (класса Point)
bool Point::GetVisible() { return visible; }

// изменение видимости точки (класса Point)
void Point::SetVisible(bool NewVisible) { visible = NewVisible; }

/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/
int main()
{

    //===============================================================
    // Н А Ч А Л О  П Е Р Е М Е Н Н Ы Е  Д Л Я  П Р И М Е Р А

    // объявление и инициализация переменных
    // для классов LOCATION и POINT
    int x0 = 100; // стартовые координаты ТОЧКИ
    int y0 = 100;

    // для класса CIRCLE
    int Radius0 = 50;  // начальное значение радиуса КРУГА
    int DeltaRad = 30; // шаг изменения радиуса КРУГА

    // для класса RING
    int RingW0 = 20;     // начальное значение толщины КОЛЬЦА
    int DeltaWide0 = 10; // шаг изменения толщины КОЛЬЦА

    //	К О Н Е Ц  П Е Р Е М Е Н Н Ы Е  Д Л Я  П Р И М Е Р А
    //===============================================================
    // получим дескриптор консольного окна
    HWND hwnd = GetConsoleWindow();

    // если дескриптор НЕ существует
    if (hwnd == NULL)
    {
        cout << "Console Window Descriptor NOT FOUND !\n";
        return 1; // ERROR
    } // if

    // дескриптор существует

    // получим контекст устройства для консольного окна
    hdc = GetWindowDC(hwnd);

    // если контекст НЕ существует
    if (hdc == 0)
    {
        cout << "Handle Device Context NOT FOUND !\n";
        return 2; // ERROR
    } // if

    // контекст существует - можем работать
    //===============================================================
    //	М Е С Т О    В С Т А В К И - Р А Б О Т А  С  К Л А С С О М

    SetPixel(hdc, 0, 0, RGB(255, 0, 0));

    Point point(x0, y0);
    point.Show();
    int i;

    // рисование точек
    /*
    for (i = 0; i < 6; i++) {
        point.MoveTo(x0 + 50, y0 + 35 * i);

        cout << i << "Press ENTER" << endl;
        getchar();
    }*/

    /*
    // диагональная линия
    while (1)
    {
        for (i = 0; i < 50; i++)
        {
            point.MoveTo(x0 + 100 + i, y0 + 100 + i);
            Sleep(50);
        }//for i

    }//while */

    // рисование круга
    /*
    point.SetVisible(true);

    HPEN PenR = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // создание пера

    SelectObject(hdc, PenR); // выбор пера

    int x = x0 + 10;
    int y = y0 + 10;
    int R = 50;
    Ellipse(hdc, x - R, y - R, x + R, y + R);

    DeleteObject(PenR);
    */

    /* ----- Фотоаппарат -----*/

    point.SetVisible(true);

    HPEN PenR = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // создание пера

    SelectObject(hdc, PenR); // выбор пера

    // корпус
    Rectangle(hdc, x0, y0, x0 + 500, y0 + 350);
    Rectangle(hdc, x0, y0 + 100, x0 + 500, y0 + 250);

    // объектив
    int circle_radius = 140;
    Ellipse(hdc, x0 + 150 - circle_radius, y0 + 175 - circle_radius, x0 + 150 + circle_radius, y0 + 175 + circle_radius);
    circle_radius -= 25;
    Ellipse(hdc, x0 + 150 - circle_radius, y0 + 175 - circle_radius, x0 + 150 + circle_radius, y0 + 175 + circle_radius);

    // кнопка
    Rectangle(hdc, x0 + 400, y0 - 20, x0 + 450, y0);

    getchar();
    point.Show();

    //	К О Н Е Ц     В С Т А В К И
    //===============================================================
    ReleaseDC(hwnd, hdc);

    return 0;

} // end main()

/****************  End Of ExOOP_ШАБЛОН.СPP File ****************/
