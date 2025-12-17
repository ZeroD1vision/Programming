/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Windows Console Application                                 *
 * Project name  : Pt_1                                                        *
 * File name     : main.cpp                                                    *
 * Language      : CPP                                                         *
 * Programmers   : Нарзиев Артемий Тимурович                                   *
 * Modified By   :                                                             *
 * Created       : 13.09.2025                                                  *
 * Last Revision : 24.10.2025                                                  *
 * Comment       : ЛР №3. Фигура: фонарик                                      *
 ******************************************************************************/

#include <windows.h>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
using namespace std;

#include "Header.h"

// Макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*Г Л О Б А Л Ь Н Ы Е  К О Н С Т А Н Т Ы  И  П Е Р Е М Е Н Н Ы Е*/
/****************************************************************/

HDC hdc;    // Объявим контекст устройства

// Глобальные переменные для игры
int BATTERY_COUNT = 3;                    // Количество батареек на поле
const int MAX_BATTERIES = 10;             // Максимальное количество батареек
const int BATTERY_SPAWN_TIME = 10;        // Секунд между появлением батареек
const int LIGHT_DURATION = 3;            // Длительность свечения от батарейки (сек)

/****************************************************************/
/*    Ф У Н К Ц И Я  П Р О В Е Р К И  С Т О Л К Н О В Е Н И Я   */
/****************************************************************/

bool IsCollision(Conflict& conflict, BaseFlashlight& flashlight)
{
    // Получаем координаты и размеры конфликтного объекта
    int conflictX = conflict.GetX();
    int conflictY = conflict.GetY();
    int conflictWidth = conflict.GetWidth();
    int conflictHeight = conflict.GetHeight();

    // Получаем координаты и размеры фонарика
    int flashlightX = flashlight.GetX();
    int flashlightY = flashlight.GetY();
    int bodyWidth = flashlight.GetBodyWidth();
    int bodyHeight = flashlight.GetBodyHeight();

    // Проверка столкновения по прямоугольникам
    bool bodyCollision = !(flashlightX + bodyWidth < conflictX ||
        flashlightX > conflictX + conflictWidth ||
        flashlightY + bodyHeight < conflictY ||
        flashlightY > conflictY + conflictHeight);

    return bodyCollision;
}

// Функция для отображения текста на экране
void DrawTextOnScreen(int x, int y, string text, COLORREF color = RGB(255, 255, 255))
{
    SetTextColor(hdc, color);
    SetBkColor(hdc, RGB(0, 0, 0));
    TextOutA(hdc, x, y, text.c_str(), text.length());
}

// Функция для получения случайного числа в диапазоне
int RandomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/

// ф-я для отработки нажатий стрелочек
bool HandleMovement(int& x, int& y, int step)
{
    int prev_x = x;
    int prev_y = y;

    // влево
    if (KEY_DOWN(VK_LEFT))
    {
        x = x - step;
    }
    // вправо
    if (KEY_DOWN(VK_RIGHT))
    {
        x = x + step;
    }
    // вверх
    if (KEY_DOWN(VK_UP))
    {
        y = y - step;
    }
    // вниз
    if (KEY_DOWN(VK_DOWN))
    {
        y = y + step;
    }

    return (prev_x != x) || (prev_y != y);
}

// Функция для получения времени в формате MM:SS
string FormatTime(int seconds)
{
    int minutes = seconds / 60;
    int secs = seconds % 60;

    stringstream ss;
    if (minutes < 10) ss << "0";
    ss << minutes << ":";
    if (secs < 10) ss << "0";
    ss << secs;

    return ss.str();
}

// Функция создания новой батарейки в случайном месте
Battery* CreateRandomBattery(int screenWidth, int screenHeight)
{
    int batteryWidth = 15, batteryHeight = 25;

    // Оставляем отступы от краёв
    int margin = 50;
    int x = RandomInt(margin, screenWidth - batteryWidth - margin);
    int y = RandomInt(margin, screenHeight - batteryHeight - margin);

    return new Battery(x, y, batteryWidth, batteryHeight);
}

int main()
{
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned int>(time(NULL)));

    //===============================================================
    //  Н А Ч А Л О  М О Л И Т В Ы
    HWND hwnd = GetConsoleWindow();
    if (hwnd == NULL)
    {
        cout << "Console Window Descriptor NOT FOUND !\n";
        return 1;
    }

    hdc = GetWindowDC(hwnd);
    if (hdc == 0)
    {
        cout << "Handle Device Context NOT FOUND !\n";
        return 2;
    }
    // К О Н Е Ц  М О Л И Т В Ы
    //===============================================================

    //===============================================================
    // Н А Ч А Л О  К О Н С Т А Н Т Ы

    const int CIRCLE = 1;
    const int FLASHLIGHT = 2;
    const int POINT = 3;
    const int SQUARE = 4;

    int STEP = 5;
    const int DELAY = 33;

    const int TRANSITIONS_COUNT = 12;
    const int NOT_TRANSITIONS_COUNT = 4;
    const int TOTAL_STATES = TRANSITIONS_COUNT + NOT_TRANSITIONS_COUNT;

    const int CONFLICT_COUNT = 7; // Убрали батарейки из общего массива
    const int GHOST_COUNT = 2;
    const int MOVING_COUNT = 3;
    const int FLASHLIGHT_COUNT = 6;

    // Игровые константы
    const int GAME_DURATION = 180; // 3 минуты в секундах
    const int RECORD_TIME = 90;    // рекорд - 1.5 минуты

    // Размеры экрана
    const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

    // Центр экрана
    const int CENTER_X = SCREEN_WIDTH / 2;
    const int CENTER_Y = SCREEN_HEIGHT / 2;

    //  К О Н Е Ц  К О Н С Т А Н Т Ы
    //===============================================================

    //===============================================================
    //  Н А Ч А Л О  П Е Р Е М Е Н Н Ы Е

    // --------------------//
    // ===== РАЗМЕРЫ ===== //
    // --------------------//

    int Radius = 20;                    // Круг
    int SquareSize = 40;                // Квадрат
    int BatteryWidth = 15, BatteryHeight = 25;  // Батарейка

    // Прямоугольный фонарик
    int RectBodyWidth = 30, RectBodyHeight = 90;
    int RectHeadWidth = 45, RectHeadHeight = 60;

    // Круглый фонарик
    int RoundBodyWidth = 35, RoundBodyHeight = 70;
    int RoundHeadWidth = 50, RoundHeadHeight = 40;

    // Другие объекты
    int ScrewLength = 50, ScrewWidth = 12;
    int StoneWidth = 50, StoneHeight = 40;
    int FlyWidth = 15, FlyHeight = 8;        // Муха
    int GhostWidth = 30, GhostHeight = 40;   // Призрак

    // -----------------------//
    // ===== КООРДИНАТЫ ===== //
    // -----------------------//

    // Фонарик в центре экрана
    int flashlight_x = CENTER_X - RectBodyWidth / 2;
    int flashlight_y = CENTER_Y - RectBodyHeight / 2;

    // Объекты ближе к центру для удобства
    int circle_x = CENTER_X - 200;
    int circle_y = CENTER_Y + 100;

    int square_x = CENTER_X + 150;
    int square_y = CENTER_Y + 100;

    int screw_x = CENTER_X - 50;
    int screw_y = CENTER_Y + 150;

    // Камни
    int stone1_x = CENTER_X - 250;
    int stone1_y = CENTER_Y;

    int stone2_x = CENTER_X + 200;
    int stone2_y = CENTER_Y;

    // Муха летает по центру
    int fly_x = CENTER_X;
    int fly_y = CENTER_Y;

    // Призраки начинают с разных сторон
    int ghost1_x = CENTER_X - 300;
    int ghost1_y = CENTER_Y;

    int ghost2_x = CENTER_X + 250;
    int ghost2_y = CENTER_Y;

    // Точка для управления
    int point_x = 50, point_y = 150;

    // -------------------------------//
    // ===== ОБЪЕКТЫ НА "СЦЕНЕ" ===== //
    // -------------------------------//

    // Фонарики
    RectFlashlight rectFlashlight(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    RoundFlashlight roundFlashlight(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    BrokenRectFlashlight brokenRect(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    BrokenRoundFlashlight brokenRound(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);
    LitRectFlashlight litRectFlashlight(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    LitRoundFlashlight litRoundFlashlight(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);

    // Движущиеся объекты
    Fly fly(fly_x, fly_y, FlyWidth, FlyHeight);
    Ghost ghost1(ghost1_x, ghost1_y, GhostWidth, GhostHeight);
    Ghost ghost2(ghost2_x, ghost2_y, GhostWidth, GhostHeight);

    // Указатель на текущий фонарик
    BaseFlashlight* currentFlashlight = &rectFlashlight;

    // Статические объекты
    Point point(point_x, point_y);
    Circle circle(circle_x, circle_y, Radius);
    Screwdriver screwdriver(screw_x, screw_y, ScrewWidth, ScrewLength);
    Stone stone1(stone1_x, stone1_y, StoneWidth, StoneHeight);
    Stone stone2(stone2_x, stone2_y, StoneWidth, StoneHeight);
    Square square(square_x, square_y, SquareSize, SquareSize);

    // Массив для динамических батареек
    Battery* batteries[MAX_BATTERIES] = { 0 };

    // Изначально создаем несколько батареек
    for (int i = 0; i < BATTERY_COUNT; i++)
    {
        batteries[i] = CreateRandomBattery(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    // Массивы для удобного управления
    BaseFlashlight* allFlashlights[FLASHLIGHT_COUNT] = {
        &rectFlashlight, &brokenRect, &litRectFlashlight,
        &roundFlashlight, &brokenRound, &litRoundFlashlight
    };

    // Массив всех конфликтных объектов (без батареек)
    Conflict* allConflicts[CONFLICT_COUNT] = {
        &ghost1,        // 0 - призрак 1
        &ghost2,        // 1 - призрак 2
        &fly,           // 2 - муха
        &stone1,        // 3 - камень 1
        &stone2,        // 4 - камень 2
        &screwdriver,   // 5 - отвертка
        &circle         // 6 - круг
    };

    // -------------------------------//
    // ===== СИСТЕМА ПЕРЕХОДОВ ===== //
    // -------------------------------//

    // УНИВЕРСАЛЬНЫЕ МАССИВЫ ПЕРЕХОДОВ
    BaseFlashlight* from_states[TOTAL_STATES] =
    {
        // ТРАНЗИТИВНЫЕ ПЕРЕХОДЫ - ПЕРВЫЕ 12
        &rectFlashlight,      // 0: обычный прямоугольный -> камень 1
        &brokenRect,          // 1: сломанный прямоугольный -> отвертка  
        &rectFlashlight,      // 2: обычный прямоугольный -> круг
        &rectFlashlight,      // 3: обычный прямоугольный -> квадрат

        &roundFlashlight,     // 4: обычный круглый -> камень 2
        &brokenRound,         // 5: сломанный круглый -> отвертка
        &roundFlashlight,     // 6: обычный круглый -> круг
        &roundFlashlight,     // 7: обычный круглый -> квадрат

        &rectFlashlight,      // 8: обычный прямоугольный -> муха
        &roundFlashlight,     // 9: обычный круглый -> муха

        // ПЕРЕХОДЫ ДЛЯ ПРИЗРАКОВ
        &rectFlashlight,      // 10: обычный прямоугольный -> призрак 1
        &roundFlashlight,     // 11: обычный круглый -> призрак 2

        // НЕТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &litRectFlashlight,   // 12: горящий прямоугольный -> обычный
        &litRoundFlashlight,  // 13: горящий круглый -> обычный
        &litRectFlashlight,   // 14: горящий прямоугольный -> обычный
        &litRoundFlashlight   // 15: горящий круглый -> обычный
    };

    Conflict* conflicts[TOTAL_STATES] =
    {
        // ТРАНЗИТИВНЫЕ ПЕРЕХОДЫ
        &stone1,       // 0: с камнем 1
        &screwdriver,  // 1: с отверткой
        &circle,       // 2: с кругом
        &square,       // 3: с квадратом

        &stone2,       // 4: с камнем 2
        &screwdriver,  // 5: с отверткой
        &circle,       // 6: с кругом
        &square,       // 7: с квадратом

        &fly,          // 8: с мухой
        &fly,          // 9: с мухой

        // ПЕРЕХОДЫ ДЛЯ ПРИЗРАКОВ
        &ghost1,       // 10: с призраком 1
        &ghost2,       // 11: с призраком 2

        // НЕТРАНЗИТИВНЫЕ ПЕРЕХОДЫ (используем любые объекты)
        &stone1,       // 12: для перехода
        &stone2,       // 13: для перехода
        &circle,       // 14: для перехода
        &square        // 15: для перехода
    };

    BaseFlashlight* to_states[TOTAL_STATES] =
    {
        &brokenRect,        // 0: -> сломанный прямоугольный
        &rectFlashlight,    // 1: -> прямоугольный
        &roundFlashlight,   // 2: -> круглый
        &roundFlashlight,   // 3: -> круглый

        &brokenRound,       // 4: -> сломанный круглый
        &roundFlashlight,   // 5: -> круглый
        &rectFlashlight,    // 6: -> прямоугольный
        &rectFlashlight,    // 7: -> прямоугольный

        &brokenRect,        // 8: -> сломанный прямоугольный
        &brokenRound,       // 9: -> сломанный круглый

        // ПРИЗРАКИ ЛОМАЮТ
        &brokenRect,        // 10: -> сломанный прямоугольный
        &brokenRound,       // 11: -> сломанный круглый

        &rectFlashlight,    // 12: -> обычный прямоугольный
        &roundFlashlight,   // 13: -> обычный круглый
        &rectFlashlight,    // 14: -> обычный прямоугольный
        &roundFlashlight    // 15: -> обычный круглый
    };

    // -------------------------- //
    // ===== ДЛЯ УПРАВЛЕНИЯ ===== //
    // -------------------------- //

    int fig_x, fig_y;
    int current_figure = FLASHLIGHT;
    bool collision;
    bool valid_transition;

    // Игровые переменные
    int gameTime = 0;
    int bestTime = 0;
    int batteriesCollected = 0;
    int activeBatteries = BATTERY_COUNT;
    bool gameActive = true;
    bool gameWon = false;
    bool recordBeaten = false;
    time_t lastTimeUpdate = time(NULL);
    time_t lastBatterySpawn = time(NULL);

    // Время свечения
    int lightTimeRemaining = 0;
    bool isLightOn = false;

    // Счётчики
    int activeGhosts = GHOST_COUNT;
    bool ghost1Active = true;
    bool ghost2Active = true;

    //  К О Н Е Ц  П Е Р Е М Е Н Н Ы Е
    //===============================================================

    // Очистка экрана
    system("cls");

    // Показываем стартовые фигуры
    square.Show(); // Квадрат отдельно
    for (int i = 0; i < CONFLICT_COUNT; i++)
    {
        allConflicts[i]->Show();
    }

    // Показываем батарейки
    for (int i = 0; i < BATTERY_COUNT; i++)
    {
        if (batteries[i] != NULL)
            batteries[i]->Show();
    }

    currentFlashlight->Show();

    // Выводим инструкции
    cout << "=================== FLASHLIGHT COLLECTOR ===================" << endl;
    cout << "Цель: Собирайте батарейки чтобы включать свет и выживать!" << endl;
    cout << "      Продержитесь 3 минуты чтобы выиграть!" << endl;
    cout << endl;
    cout << "Управление:" << endl;
    cout << "Стрелки - движение фонариком" << endl;
    cout << "ESC - выход из игры" << endl;
    cout << endl;
    cout << "Механика:" << endl;
    cout << "1. Собирайте батарейки (они появляются случайно)" << endl;
    cout << "2. Каждая батарейка дает " << LIGHT_DURATION << " секунд света" << endl;
    cout << "3. Свет защищает от призраков" << endl;
    cout << "4. Избегайте мухи и камней (они ломают фонарик)" << endl;
    cout << "5. Используйте отвертку для починки" << endl;
    cout << "6. Круг и квадрат меняют форму фонарика" << endl;
    cout << "============================================================" << endl;

    while (true)
    {
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        // Обновление времени
        time_t currentTime = time(NULL);
        if (currentTime - lastTimeUpdate >= 1)
        {
            gameTime++;
            lastTimeUpdate = currentTime;

            // Уменьшаем время свечения
            if (lightTimeRemaining > 0)
            {
                lightTimeRemaining--;
                if (lightTimeRemaining == 0)
                {
                    // Выключаем свет
                    if (currentFlashlight == &litRectFlashlight)
                    {
                        currentFlashlight->Hide();
                        currentFlashlight = &rectFlashlight;
                        currentFlashlight->Show();
                    }
                    else if (currentFlashlight == &litRoundFlashlight)
                    {
                        currentFlashlight->Hide();
                        currentFlashlight = &roundFlashlight;
                        currentFlashlight->Show();
                    }
                    isLightOn = false;
                    cout << "Свет погас! Найдите новую батарейку!" << endl;
                }
            }

            // Спавн новых батареек
            if (currentTime - lastBatterySpawn >= BATTERY_SPAWN_TIME && activeBatteries < MAX_BATTERIES)
            {
                // Находим свободный слот
                for (int i = 0; i < MAX_BATTERIES; i++)
                {
                    if (batteries[i] == NULL)
                    {
                        batteries[i] = CreateRandomBattery(SCREEN_WIDTH, SCREEN_HEIGHT);
                        batteries[i]->Show();
                        activeBatteries++;
                        lastBatterySpawn = currentTime;
                        cout << "Появилась новая батарейка!" << endl;
                        break;
                    }
                }
            }

            // Проверка рекорда
            if (gameTime == RECORD_TIME && !recordBeaten)
            {
                recordBeaten = true;
                cout << "ПОЗДРАВЛЯЕМ! Вы продержались 1.5 минуты!" << endl;
            }

            // Проверка победы
            if (gameTime >= GAME_DURATION)
            {
                gameWon = true;
                gameActive = false;
                cout << "УРА! ВЫ ВЫИГРАЛИ! Вы продержались 3 минуты!" << endl;
                cout << "Ваше время: " << FormatTime(gameTime) << endl;
                break;
            }
        }

        // Отображение игровой информации
        string timeStr = "Время: " + FormatTime(gameTime);
        string batteriesStr = "Батарейки: " + to_string(batteriesCollected);
        string lightStr = "Свет: " + to_string(lightTimeRemaining) + " сек";
        string activeBatStr = "На поле: " + to_string(activeBatteries);

        if (recordBeaten)
        {
            string recordStr = "РЕКОРД!";
            DrawTextOnScreen(10, 50, recordStr, RGB(255, 215, 0));
        }

        DrawTextOnScreen(10, 10, timeStr);
        DrawTextOnScreen(10, 30, batteriesStr);
        DrawTextOnScreen(10, 50, lightStr);
        DrawTextOnScreen(10, 70, activeBatStr);

        if (isLightOn)
        {
            string safetyStr = "В БЕЗОПАСНОСТИ";
            DrawTextOnScreen(SCREEN_WIDTH - 200, 10, safetyStr, RGB(0, 255, 0));
        }
        else
        {
            string dangerStr = "ОПАСНОСТЬ!";
            DrawTextOnScreen(SCREEN_WIDTH - 200, 10, dangerStr, RGB(255, 0, 0));
        }

        string remainingStr = "Цель: " + FormatTime(GAME_DURATION - gameTime);
        DrawTextOnScreen(CENTER_X - 100, 10, remainingStr, isLightOn ? RGB(0, 255, 0) : RGB(255, 255, 0));

        if (!gameActive) continue;

        // 1. ОБНОВЛЯЕМ ЦЕЛИ ДЛЯ ПРИЗРАКОВ
        ghost1.UpdateTarget(currentFlashlight->GetX(), currentFlashlight->GetY());
        ghost2.UpdateTarget(currentFlashlight->GetX(), currentFlashlight->GetY());

        // 2. ПРОВЕРЯЕМ СОСТОЯНИЕ СВЕТА
        isLightOn = (currentFlashlight == &litRectFlashlight ||
            currentFlashlight == &litRoundFlashlight);

        // Управляем активностью призраков
        ghost1.SetActive(!isLightOn && ghost1Active);
        ghost2.SetActive(!isLightOn && ghost2Active);

        // 3. ДВИЖЕНИЕ ДВИЖУЩИХСЯ ОБЪЕКТОВ
        for (int i = 0; i < MOVING_COUNT; i++)
        {
            allConflicts[i]->Move();

            // Проверяем столкновение
            if (IsCollision(*allConflicts[i], *currentFlashlight))
            {
                // Для призраков
                if (i < GHOST_COUNT)
                {
                    if (!isLightOn)
                    {
                        // Ломаем фонарик
                        if (currentFlashlight == &rectFlashlight ||
                            currentFlashlight == &litRectFlashlight)
                        {
                            currentFlashlight->Hide();
                            currentFlashlight = &brokenRect;
                            currentFlashlight->Show();
                            cout << "Призрак сломал фонарик!" << endl;
                        }
                        else if (currentFlashlight == &roundFlashlight ||
                            currentFlashlight == &litRoundFlashlight)
                        {
                            currentFlashlight->Hide();
                            currentFlashlight = &brokenRound;
                            currentFlashlight->Show();
                            cout << "Призрак сломал фонарик!" << endl;
                        }
                    }
                }
                // Для мухи
                else if (i == 2)
                {
                    if (currentFlashlight == &rectFlashlight ||
                        currentFlashlight == &litRectFlashlight)
                    {
                        currentFlashlight->Hide();
                        currentFlashlight = &brokenRect;
                        currentFlashlight->Show();
                        cout << "Муха врезалась в фонарик!" << endl;
                    }
                    else if (currentFlashlight == &roundFlashlight ||
                        currentFlashlight == &litRoundFlashlight)
                    {
                        currentFlashlight->Hide();
                        currentFlashlight = &brokenRound;
                        currentFlashlight->Show();
                        cout << "Муха врезалась в фонарик!" << endl;
                    }
                }
            }
        }

        // 4. УПРАВЛЕНИЕ ФОНАРИКОМ
        if (current_figure == FLASHLIGHT)
        {
            fig_x = currentFlashlight->GetX();
            fig_y = currentFlashlight->GetY();

            if (HandleMovement(fig_x, fig_y, STEP))
            {
                // Проверяем выход за границы экрана
                if (fig_x < 0) fig_x = 0;
                if (fig_y < 0) fig_y = 0;
                if (fig_x > SCREEN_WIDTH - RectBodyWidth) fig_x = SCREEN_WIDTH - RectBodyWidth;
                if (fig_y > SCREEN_HEIGHT - RectBodyHeight) fig_y = SCREEN_HEIGHT - RectBodyHeight;

                currentFlashlight->Hide();

                // Обновляем координаты всех фонариков
                for (int i = 0; i < FLASHLIGHT_COUNT; i++)
                {
                    allFlashlights[i]->SetX(fig_x);
                    allFlashlights[i]->SetY(fig_y);
                }

                flashlight_x = fig_x;
                flashlight_y = fig_y;

                // Проверяем переходы (без батареек)
                for (int i = 0; i < TOTAL_STATES; i++)
                {
                    // Пропускаем переходы для батареек
                    if (i == 2 || i == 6) continue; // Старые переходы с батарейками

                    collision = IsCollision(*conflicts[i], *currentFlashlight);

                    // Для горящих фонариков проверяем ОТСУТСТВИЕ света по времени
                    if (i >= TRANSITIONS_COUNT)
                    {
                        valid_transition = (lightTimeRemaining == 0);
                    }
                    else
                    {
                        valid_transition = collision;
                    }

                    if (currentFlashlight == from_states[i] && valid_transition)
                    {
                        currentFlashlight = to_states[i];
                        break;
                    }
                }
            }
        }

        // 5. ПРОВЕРКА СТОЛКНОВЕНИЙ С БАТАРЕЙКАМИ
        for (int i = 0; i < MAX_BATTERIES; i++)
        {
            if (batteries[i] != NULL && IsCollision(*batteries[i], *currentFlashlight))
            {
                // Собираем батарейку
                batteries[i]->Hide();
                delete batteries[i];
                batteries[i] = NULL;
                activeBatteries--;
                batteriesCollected++;

                // Включаем или продлеваем свет
                lightTimeRemaining += LIGHT_DURATION;

                if (!isLightOn)
                {
                    if (currentFlashlight == &rectFlashlight || currentFlashlight == &brokenRect)
                    {
                        currentFlashlight->Hide();
                        currentFlashlight = &litRectFlashlight;
                        currentFlashlight->Show();
                    }
                    else if (currentFlashlight == &roundFlashlight || currentFlashlight == &brokenRound)
                    {
                        currentFlashlight->Hide();
                        currentFlashlight = &litRoundFlashlight;
                        currentFlashlight->Show();
                    }
                    isLightOn = true;
                }

                cout << "Батарейка собрана! Свет горит еще " << lightTimeRemaining << " секунд!" << endl;

                // При свете призраки отступают
                if (isLightOn)
                {
                    ghost1.SetActive(false);
                    ghost2.SetActive(false);
                }
            }
        }

        // 6. ПРОВЕРКА СТОЛКНОВЕНИЙ С КАМНЯМИ И ДРУГИМИ ОБЪЕКТАМИ
        for (int i = 3; i < CONFLICT_COUNT; i++)
        {
            if (IsCollision(*allConflicts[i], *currentFlashlight))
            {
                // Камни ломают фонарик
                if (i == 3 || i == 4) // Камни
                {
                    if (currentFlashlight == &rectFlashlight || currentFlashlight == &litRectFlashlight)
                    {
                        currentFlashlight->Hide();
                        currentFlashlight = &brokenRect;
                        currentFlashlight->Show();
                        cout << "Наткнулись на камень!" << endl;
                    }
                    else if (currentFlashlight == &roundFlashlight || currentFlashlight == &litRoundFlashlight)
                    {
                        currentFlashlight->Hide();
                        currentFlashlight = &brokenRound;
                        currentFlashlight->Show();
                        cout << "Наткнулись на камень!" << endl;
                    }
                }
            }
        }

        // 7. ПЕРЕРИСОВКА
        // Очищаем старые позиции движущихся объектов
        for (int i = 0; i < MOVING_COUNT; i++)
        {
            allConflicts[i]->Hide();
        }

        // Показываем все статические объекты
        for (int i = MOVING_COUNT; i < CONFLICT_COUNT; i++)
        {
            allConflicts[i]->Show();
        }
        square.Show();

        // Показываем батарейки
        for (int i = 0; i < MAX_BATTERIES; i++)
        {
            if (batteries[i] != NULL)
                batteries[i]->Show();
        }

        // Показываем движущиеся объекты
        for (int i = 0; i < MOVING_COUNT; i++)
        {
            allConflicts[i]->Show();
        }

        // Показываем фонарик
        currentFlashlight->Show();

        // Небольшая задержка
        Sleep(DELAY);
    }

    //===============================================================
    // ИГРОВАЯ СТАТИСТИКА

    // Очистка памяти
    for (int i = 0; i < MAX_BATTERIES; i++)
    {
        if (batteries[i] != NULL)
        {
            delete batteries[i];
        }
    }

    cout << endl;
    cout << "Нажмите любую клавишу для выхода...";
    cin.get();

    ReleaseDC(hwnd, hdc);
    return 0;
}