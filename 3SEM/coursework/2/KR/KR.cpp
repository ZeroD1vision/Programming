/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Windows Console Application                                 *
 * Project name  : coursework                                                  *
 * File name     : main.cpp                                                    *
 * Language      : CPP                                                         *
 * Programmers   : Нарзиев Артемий Тимурович                                   *
 * Modified By   :                                                             *
 * Created       : 11.12.2025                                                  *
 * Last Revision : 20.12.2025                                                  *
 * Comment       : Курсовая (2 вариант). Фигура: фонарик                       *
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
const int BATTERY_SPAWN_TIME = 5;        // Секунд между появлением батареек
const int LIGHT_DURATION = 5;            // Длительность свечения от батарейки (сек)
// Размеры экрана
const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

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

    const int FLASHLIGHT = 2;

    int STEP = 5;
    const int DELAY = 30;

    // Количество проверок столкновений со статическими объектами
    // Камень, отвертка, круг, квадрат для обоих типов фонариков
    const int STATIC_TRANSITIONS_COUNT = 10;

    // Количество нетранзитивных переходов (конец света)
    const int NON_TRANSITIONS_COUNT = 0;

    // Количество призраков (легко изменить)
    const int GHOST_COUNT = 2;

    // Количество переходов с призраками (для каждого типа фонарика)
    const int GHOST_TRANSITIONS_COUNT = GHOST_COUNT * 2;

    // Количество переходов с мухой
    const int FLY_TRANSITIONS_COUNT = 4;

    // Общее количество переходов
    const int TOTAL_TRANSITIONS = STATIC_TRANSITIONS_COUNT + 
                                  NON_TRANSITIONS_COUNT + 
                                  FLY_TRANSITIONS_COUNT + 
                                  GHOST_TRANSITIONS_COUNT;

    // Количество статических конфликтных объектов
    const int STATIC_CONFLICTS_COUNT = 4; // Камни, отвертка, круг

    // Все движущиеся объекты
    const int TOTAL_MOVING = GHOST_COUNT + 1; // Призраки + муха

    // Разные типы фонариков
    const int FLASHLIGHT_COUNT = 8;

    // Размеры экрана
    const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

    // Индексы начала разных типов переходов
    const int STATIC_START_INDEX = 0;
    const int NON_TRANS_START_INDEX = STATIC_TRANSITIONS_COUNT; // 8
    const int FLY_START_INDEX = STATIC_TRANSITIONS_COUNT; // 12
    const int GHOST_START_INDEX = FLY_START_INDEX + FLY_TRANSITIONS_COUNT; // 14

    // Новые константы для массивов движущихся объектов
    const int MOVING_TRANSITIONS_COUNT = FLY_TRANSITIONS_COUNT + GHOST_TRANSITIONS_COUNT; // 6

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

    int Radius = 20;
    int SquareSize = 40;
    int BatteryWidth = 15, BatteryHeight = 25;

    // Прямоугольный фонарик
    int RectBodyWidth = 30, RectBodyHeight = 90;
    int RectHeadWidth = 45, RectHeadHeight = 60;

    // Круглый фонарик
    int RoundBodyWidth = 35, RoundBodyHeight = 70;
    int RoundHeadWidth = 50, RoundHeadHeight = 40;

    // Другие объекты
    int ScrewLength = 50, ScrewWidth = 12;
    int StoneWidth = 50, StoneHeight = 40;
    int FlyWidth = 15, FlyHeight = 8;
    int GhostWidth = 30, GhostHeight = 40;

    // -----------------------//
    // ===== КООРДИНАТЫ ===== //
    // -----------------------//

    int flashlight_x = CENTER_X - RectBodyWidth / 2;
    int flashlight_y = CENTER_Y - RectBodyHeight / 2;

    int circle_x = CENTER_X - 200;
    int circle_y = CENTER_Y + 100;

    int square_x = CENTER_X + 150;
    int square_y = CENTER_Y + 100;

    int screw_x = CENTER_X - 50;
    int screw_y = CENTER_Y + 150;

    int stone1_x = CENTER_X - 250;
    int stone1_y = CENTER_Y;

    int stone2_x = CENTER_X + 200;
    int stone2_y = CENTER_Y;

    int fly_x = CENTER_X;
    int fly_y = CENTER_Y;

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
    // Светящиеся с таймером
    LitRectFlashlightWithTimer litRectWithTimer(flashlight_x, flashlight_y, RectBodyWidth, RectBodyHeight, RectHeadWidth, RectHeadHeight);
    LitRoundFlashlightWithTimer litRoundWithTimer(flashlight_x, flashlight_y, RoundBodyWidth, RoundBodyHeight, RoundHeadWidth, RoundHeadHeight);

    // Все фонарики в одном массиве
    BaseFlashlight* allFlashlights[FLASHLIGHT_COUNT] = {
        &rectFlashlight, &brokenRect,
        &litRectFlashlight, &litRectWithTimer, // Оба светящихся прямоугольных
        &roundFlashlight, &brokenRound,
        &litRoundFlashlight, &litRoundWithTimer  // Оба светящихся круглых
    };

    // Текущий фонарик
    BaseFlashlight* currentFlashlight = &rectFlashlight;

    // Статические объекты
    Circle circle(circle_x, circle_y, Radius);
    Screwdriver screwdriver(screw_x, screw_y, ScrewWidth, ScrewLength);
    Stone stone1(stone1_x, stone1_y, StoneWidth, StoneHeight);
    Stone stone2(stone2_x, stone2_y, StoneWidth, StoneHeight);
    Square square(square_x, square_y, SquareSize, SquareSize);

    // Массив статических конфликтных объектов
    Conflict* staticConflicts[STATIC_CONFLICTS_COUNT] = {
        &stone1, &stone2, &screwdriver, &circle
    };

    // Муха
    Fly fly(fly_x, fly_y, FlyWidth, FlyHeight);

    // Призраки
    Ghost* ghosts[GHOST_COUNT];
    bool ghostActive[GHOST_COUNT];
    int ghostRespawnTimers[GHOST_COUNT];

    // Случайные позиции для призраков
    for (int i = 0; i < GHOST_COUNT; i++) 
    {
        int x = RandomInt(100, SCREEN_WIDTH - GhostWidth - 100);
        int y = RandomInt(100, SCREEN_HEIGHT - GhostHeight - 100);
        ghosts[i] = new Ghost(x, y, GhostWidth, GhostHeight);
        ghostActive[i] = true;
        ghostRespawnTimers[i] = 0;
    }

    // Все движущиеся объекты
    Conflict* movingConflicts[TOTAL_MOVING];
    movingConflicts[0] = &fly; // Муха первая
    for (int i = 0; i < GHOST_COUNT; i++) 
    {
        movingConflicts[i + 1] = ghosts[i]; // Призраки после
    }

    // Батарейки - создаем недалеко от начальной позиции фонарика
    Battery* batteries[MAX_BATTERIES] = { 0 };
    for (int i = 0; i < BATTERY_COUNT; i++)
    {
        batteries[i] = CreateBatteryNearFlashlight(flashlight_x, flashlight_y, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    // -------------------------------//
    // ===== СИСТЕМА ПЕРЕХОДОВ ===== //
    // -------------------------------//

    // Массив с состояниями фонариков из которых переходим
    BaseFlashlight* from_states[STATIC_TRANSITIONS_COUNT] = {
        // ============== СТАТИЧЕСКИЕ ПЕРЕХОДЫ (0-7) ============== //
        // Прямоугольный фонарик + камни
        &rectFlashlight,    // 0: прямоугольный + камень 1 -> сломанный
        &brokenRect,        // 1: сломанный прямоугольный + отвертка -> целый
        &rectFlashlight,    // 2: прямоугольный + камень 2 -> сломанный

        // Круглый фонарик + камни
        &roundFlashlight,   // 3: круглый + камень 1 -> сломанный
        &brokenRound,       // 4: сломанный круглый + отвертка -> целый
        &roundFlashlight,   // 5: круглый + камень 2 -> сломанный

        // Смена типа
        &rectFlashlight,    // 6: прямоугольный + круг -> круглый
        &roundFlashlight,   // 7: круглый + квадрат -> прямоугольный

        // ============== СВЕТЯЩИЕСЯ ПЕРЕХОДЫ ============== //
        &litRectWithTimer,  // 8: горящий прямоугольный -> обычный
        &litRectWithTimer,  // 9: горящий прямоугольный -> обычный
    };

    // Массив с конфликтными объектами
    Conflict* conflicts[STATIC_TRANSITIONS_COUNT] = {
        // ============== СТАТИЧЕСКИЕ ОБЪЕКТЫ (0-7) ============== //
        &stone1,        // 0: прямоугольный + камень 1 -> сломанный
        &screwdriver,   // 1: сломанный прямоугольный + отвертка -> целый
        &stone2,        // 2: прямоугольный + камень 2 -> сломанный

        &stone1,        // 3: круглый + камень 1 -> сломанный
        &screwdriver,   // 4: сломанный круглый + отвертка -> целый
        &stone2,        // 5: круглый + камень 2 -> сломанный

        &circle,        // 6: прямоугольный + круг -> круглый
        &square,        // 7: круглый + квадрат -> прямоугольный

        // ============== СВЕТЯЩИЕСЯ ПЕРЕХОДЫ ============== //
        &stone1,        // 8: для проверки конца света
        &stone2,        // 9: для проверки конца света
    };

    // Массив с состояниями фонариков в которые переходим
    BaseFlashlight* to_states[STATIC_TRANSITIONS_COUNT] = {
        // ============== СТАТИЧЕСКИЕ ПЕРЕХОДЫ (0-7) ============== //
        &brokenRect,        // 0: прямоугольный -> сломанный (камень 1)
        &rectFlashlight,    // 1: сломанный -> целый (отвертка)
        &brokenRect,        // 2: прямоугольный -> сломанный (камень 2)

        &brokenRound,       // 3: круглый -> сломанный (камень 1)
        &roundFlashlight,   // 4: сломанный -> целый (отвертка)
        &brokenRound,       // 5: круглый -> сломанный (камень 2)

        &roundFlashlight,   // 6: прямоугольный -> круглый (круг)
        &rectFlashlight,    // 7: круглый -> прямоугольный (квадрат)

        // ============== СВЕТЯЩИЕСЯ ПЕРЕХОДЫ ============== //
        &brokenRect,    // 8: горящий прямоугольный -> обычный
        &brokenRect,    // 9: горящий прямоугольный -> обычный
    };

    // Отдельные массивы для движущихся объектов
    BaseFlashlight* moving_from_states[MOVING_TRANSITIONS_COUNT] = {
        // Муха для обычных фонариков (индексы 0-1)
        &rectFlashlight,    // 0: обычный прямоугольный -> муха
        &roundFlashlight,   // 1: обычный круглый -> муха

        // Муха для светящихся фонариков (индексы 2-5)
        &litRectWithTimer,  // 2: горящий прямоугольный -> муха
        &litRoundWithTimer, // 4: горящий круглый -> муха

        // Призрак 0 (индексы 6-7)
        &rectFlashlight,    // 6: обычный прямоугольный -> призрак 0
        &roundFlashlight,   // 7: обычный круглый -> призрак 0

        // Призрак 1 (индексы 8-9)
        &rectFlashlight,    // 8: обычный прямоугольный -> призрак 1
        &roundFlashlight    // 9: обычный круглый -> призрак 1
    };

    Conflict* moving_conflicts[MOVING_TRANSITIONS_COUNT] = {
        // Муха для обычных (0-1)
        &fly,          // 0: прямоугольный + муха
        &fly,          // 1: круглый + муха

        // Муха для светящихся (2-5)
        &fly,          // 2: горящий прямоугольный + муха
        &fly,          // 4: горящий круглый + муха

        // Призрак 0 (6-7)
        ghosts[0],     // 6: прямоугольный + призрак 0
        ghosts[0],     // 7: круглый + призрак 0

        // Призрак 1 (8-9)
        ghosts[1],     // 8: прямоугольный + призрак 1
        ghosts[1]      // 9: круглый + призрак 1
    };

    BaseFlashlight* moving_to_states[MOVING_TRANSITIONS_COUNT] = {
        // Муха для обычных (0-1)
        &brokenRect,   // 0: прямоугольный -> сломанный
        &brokenRound,  // 1: круглый -> сломанный

        // Муха для светящихся (2-5) 
        &brokenRect,   // 2: горящий прямоугольный -> сломанный прямоугольный
        &brokenRound,  // 4: горящий круглый -> сломанный круглый

        // Призраки (6-9)
        &brokenRect,   // 6: прямоугольный -> сломанный
        &brokenRound,  // 7: круглый -> сломанный
        &brokenRect,   // 8: прямоугольный -> сломанный
        &brokenRound   // 9: круглый -> сломанный
    };

    // -------------------------- //
    // ===== ДЛЯ УПРАВЛЕНИЯ ===== //
    // -------------------------- //

    int fig_x, fig_y;
    int current_figure = FLASHLIGHT;
    
    // Игровые переменные
    int lightTimeRemaining = 0;
    bool isLightOn = false;
    int batteriesCollected = 0;
    int activeBatteries = BATTERY_COUNT;
    int batterySpawnTimer = 0;

    // Таймер для обновления
    int frameCounter = 0;

    string currentFlashlightState;

    //  К О Н Е Ц  П Е Р Е М Е Н Н Ы Е
    //===============================================================

    // Очистка экрана
    system("cls");

    /************************************************************/
    /*        О С Н О В Н О Й  Ц И К Л  О Б Р А Б О Т К И       */
    /************************************************************/
    while (true)
    {
        /************************************************************/
        /*     И Н Ф О Р М А Ц И О Н Н А Я   П А Н Е Л Ь           */
        /************************************************************/

        // Проверяем, светится ли фонарик (просто сравниваем указатели)
        bool isLit = (currentFlashlight == &litRectWithTimer ||
                      currentFlashlight == &litRoundWithTimer);

        // Рисуем информационную панель ПЕРЕД отрисовкой объектов
        DrawInfoPanel(hdc, SCREEN_WIDTH, SCREEN_HEIGHT,
            isLit,
            currentFlashlight->GetTimeRemaining(),
            batteriesCollected, currentFlashlightState);

        Sleep(DELAY);
        if (KEY_DOWN(VK_ESCAPE)) { break; }

        /************************************************************/
        /*     О Б Н О В Л Е Н И Е  С О С Т О Я Н И Й               */
        /************************************************************/

        currentFlashlightState = GetFlashlightState(currentFlashlight);

        // ПРИЗРАК АКТИВЕН ТОЛЬКО ЕСЛИ:
            // 1. Таймер возрождения = 0
            // 2. Свет выключен (не litRectWithTimer или litRoundWithTimer)

        // Обновляем ВСЕХ призраков
        for (int i = 0; i < GHOST_COUNT; i++)
        {
            ghosts[i]->UpdateTarget(currentFlashlight->GetX(), currentFlashlight->GetY());
            ghostActive[i] = (ghostRespawnTimers[i] == 0) && !(currentFlashlight == &litRoundWithTimer) && !(currentFlashlight == &litRectWithTimer);
            ghosts[i]->SetActive(ghostActive[i]);
        }

        /************************************************************/
        /*     Д В И Ж Е Н И Е  В С Е Х  О Б Ъ Е К Т О В            */
        /************************************************************/

        // Двигаем ВСЕ движущиеся объекты одним циклом
        for (int i = 0; i < TOTAL_MOVING; i++)
            movingConflicts[i]->Move();

        /************************************************************/
        /*   П Р О В Е Р К А  С Т О Л К Н О В Е Н И Й               */
        /*          С  Д В И Ж У Щ И М И  О Б Ъ Е К Т А М И         */
        /************************************************************/
        
        // Проверяем ВСЕ переходы с движущимися объектами одним циклом
        // Используем отдельный массив для движущихся объектов
        for (int i = 0; i < MOVING_TRANSITIONS_COUNT; i++)
        {
            if (IsCollision(*moving_conflicts[i], *currentFlashlight) &&
                currentFlashlight == moving_from_states[i])
            {
                // Проверяем, является ли это столкновением с призраком
                // (первые 2 элемента - муха, остальные - призраки)
                bool isGhostCollision = (i >= FLY_TRANSITIONS_COUNT); // Индексы 0-1: муха, 2-5: призраки

                if (isGhostCollision)
                {
                    // Для призраков: проверяем активность и свет
                    // Вычисляем индекс призрака: 0 для i=2,3; 1 для i=4,5
                    int ghostIdx = (i - FLY_TRANSITIONS_COUNT) / 2;
                    if (!ghostActive[ghostIdx])
                    {
                        continue; // Пропускаем неактивных призраков или когда свет включен
                    }

                    // Деактивируем призрака при столкновении
                    ghostActive[ghostIdx] = false;
                    ghostRespawnTimers[ghostIdx] = 100;
                }

                // Если ломаем светящийся фонарик - сбрасываем таймер
                int remainingTime = currentFlashlight->GetTimeRemaining();
                currentFlashlight->AddTime(-remainingTime); // Обнуляем таймер

                // Выполняем переход
                currentFlashlight->Hide();
                currentFlashlight = moving_to_states[i];
                currentFlashlight->Show();
            }
        }


        /************************************************************/
        /*     У П Р А В Л Е Н И Е  Ф О Н А Р И К О М               */
        /************************************************************/

        if (current_figure == FLASHLIGHT)
        {
            fig_x = currentFlashlight->GetX();
            fig_y = currentFlashlight->GetY();

            if (HandleMovement(fig_x, fig_y, STEP))
            {
                /************************************************************/
                /*     П О Л Н А Я  П Е Р Е Р И С О В К А  К А Д Р А        */
                /************************************************************/

                // 1. Прячем ВСЕ объекты перед обновлением
                for (int i = 0; i < TOTAL_MOVING; i++) movingConflicts[i]->Hide();
                currentFlashlight->Hide();

                // Обновляем ВСЕ фонарики
                for (int i = 0; i < FLASHLIGHT_COUNT; i++)
                {
                    allFlashlights[i]->SetX(fig_x);
                    allFlashlights[i]->SetY(fig_y);
                }

                /*********************************************************************/
                /*   П Р О В Е Р К А  С Т А Т И Ч Е С К И Х  С Т О Л К Н О В Е Н И Й */
                /*********************************************************************/

                // Все статические столкновения одним циклом (индексы 0-7)
                for (int i = STATIC_START_INDEX; i < NON_TRANS_START_INDEX; i++)
                {
                    bool collision = IsCollision(*conflicts[i], *currentFlashlight);

                    if (currentFlashlight == from_states[i] && collision)
                    {
                        int remainingTime = currentFlashlight->GetTimeRemaining();
                        currentFlashlight->AddTime(-remainingTime); // Обнуляем таймер
                        currentFlashlight = to_states[i];
                    }
                }


                /************************************************************/
                /*   П Р О В Е Р К А  С Б О Р А  Б А Т А Р Е Е К            */
                /************************************************************/

                // Все батарейки одним циклом
                for (int i = 0; i < MAX_BATTERIES; i++)
                {
                    if (batteries[i] != NULL && IsCollision(*batteries[i], *currentFlashlight))
                    {
                        delete batteries[i];
                        batteries[i] = NULL;
                        batteriesCollected++;

                        // Если фонарик не светится - включаем его
                        if (currentFlashlight != &litRectWithTimer &&
                            currentFlashlight != &litRoundWithTimer)
                        {
                            currentFlashlight->Hide();

                            // Простая проверка типа
                            if (currentFlashlight == &rectFlashlight)
                                currentFlashlight = &litRectWithTimer;
                            else if (currentFlashlight == &roundFlashlight)
                                currentFlashlight = &litRoundWithTimer;

                            currentFlashlight->Show();
                        }

                        // добавляем время независимо от типа фонарика
                        currentFlashlight->AddTime(LIGHT_DURATION);
                    }
                }
            }
        }

        /************************************************************/
        /*     О Б Н О В Л Е Н И Е  Т А Й М Е Р О В                */
        /************************************************************/
        
        /* НАБРОСОК ОТ ВИТАЛИЯ ЕВГЕНЬЕВИЧА
        //litRoundWithTimer
        bool res = currentFlashlight->DecreaseTimer();
        if (res == false)
        {
            currentFlashlight = &rectFlashlight;//?????
        }*/

        frameCounter++;
        
        // КОРОТКАЯ ВЕРСИЯ С ООП
        // Обновляем таймер раз в секунду (примерно 30 кадров)
        if (frameCounter >= 30)
        {
            // Уменьшаем таймер у любого фонарика (работает только у светящихся с таймером)
            if (currentFlashlight->DecreaseTimer())
            {
                // Время вышло - переключаем на обычный фонарик
                currentFlashlight->Hide();

                // Простая проверка круглый или квадратный
                if (currentFlashlight == &litRectWithTimer)
                    currentFlashlight = &rectFlashlight;
                else if (currentFlashlight == &litRoundWithTimer)
                    currentFlashlight = &roundFlashlight;

                currentFlashlight->Show();
            }
            frameCounter = 0;
        }

        /* ДЛИННАЯ ВЕРСИЯ БЕЗ ООП
        if (frameCounter >= 33) // Каждую секунду
        {
            // Таймер света
            if (isLightOn && lightTimeRemaining > 0)
            {
                lightTimeRemaining--;
                if (lightTimeRemaining <= 0)
                {
                    // Нетранзитивные переходы
                    for (int i = NON_TRANS_START_INDEX; i < FLY_START_INDEX && !transitionMade; i++)
                    {
                        if (currentFlashlight == from_states[i])
                        {
                            currentFlashlight->Hide();
                            currentFlashlight = to_states[i];
                            currentFlashlight->Show();
                            isLightOn = false;
                            break;
                        }
                    }
                }
            }
        */
        
        /************************************************************/
        /*     Г Е Н Е Р А Ц И Я  Б А Т А Р Е Е К                  */
        /************************************************************/

        // Генерация новых батареек каждые 5 секунд
        batterySpawnTimer++;

        if (batterySpawnTimer >= (BATTERY_SPAWN_TIME * 30)) // 5 секунд * 30 кадров
        {
            batterySpawnTimer = 0;

            // Ищем свободный слот для батарейки
            for (int i = 0; i < MAX_BATTERIES; i++)
            {
                if (batteries[i] == NULL)
                {
                    // Создаем батарейку недалеко от текущей позиции фонарика
                    batteries[i] = CreateBatteryNearFlashlight(
                        currentFlashlight->GetX(),
                        currentFlashlight->GetY(),
                        SCREEN_WIDTH, SCREEN_HEIGHT
                    );
                    break;
                }
            }
        }
        
        /************************************************************/
        /*     П О Л Н А Я  О Т Р И С О В К А  К А Д Р А            */
        /************************************************************/
        
        // Рисуем ВСЕ объекты
        
        // 1. Статические объекты
        for (int i = 0; i < STATIC_CONFLICTS_COUNT; i++) staticConflicts[i]->Show();
        square.Show();
        
        // 2. Батарейки
        for (int i = 0; i < MAX_BATTERIES; i++) 
            if (batteries[i] != NULL) batteries[i]->Show();
        
        // 3. Движущиеся объекты
        for (int i = 0; i < TOTAL_MOVING; i++) movingConflicts[i]->Show();
        
        // 4. Фонарик (ПОСЛЕДНИМ - поверх всего)
        currentFlashlight->Show();
    }

    //===============================================================
    // ИГРОВАЯ СТАТИСТИКА

    // Освобождаем память
    for (int i = 0; i < MAX_BATTERIES; i++) 
        if (batteries[i] != NULL) delete batteries[i];
    
    for (int i = 0; i < GHOST_COUNT; i++) 
        delete ghosts[i];

    ReleaseDC(hwnd, hdc);
    return 0;
}