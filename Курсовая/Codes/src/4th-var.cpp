/******************************************************************************
*                               Курс Информатика                              *
*******************************************************************************
* Project type  : Linux Console Application                                   *
* Project name  : Lab_2                                                       *
* File name     : main.cpp                                                    *
* Language      : CPP                                                         *
* Programmers   : Шалаев Александр Максимович, Нарзиев Артемий Тимурович      *
* Modified By   :                                                             *
* Created       : 02.04.2025                                                  *
* Last Revision : 10.04.2025                                                  *
* Comment       : Обработка данных о посадках. Вариант: 6                     *
******************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
// Для локали на русском
#include <locale>
#include <codecvt>
#include <windows.h>

using namespace std;

// КОНСТАНТЫ
const int MAX_PLANES = 100;    // Максимальное количество записей о самолетах
const int TIME_LEN = 6;        // Длина строки времени (HH:MM + нуль-терминатор)
const int MODEL_LEN = 20;      // Максимальная длина модели самолета
const int BORT_LEN = 7;        // Длина бортового номера (X-XXXX + нуль-терминатор)
const int AIRPORT_LEN = 4;     // Длина кода аэродрома (APX + нуль-терминатор)

// СТРУКТУРА ДАННЫХ
struct Plane {
    char time[TIME_LEN];       // Время посадки в формате HH:MM
    int minutes;               // Время в минутах для сортировки
    char model[MODEL_LEN];     // Модель самолета
    char bort[BORT_LEN];       // Бортовой номер
    char airport[AIRPORT_LEN]; // Код аэродрома
};

/*******************************************************************************
*  ПРОТОТИПЫ ФУНКЦИЙ
******************************************************************************/
void print_error(int err, const char* field, int line_num);
int read_data(const char *filename, Plane *planes, int *count);
void process_airport(Plane *planes, int count, const char *airport);
void purify(char* field);
int is_time_valid(const char *time, int line_num);
int is_bort_valid(const char *bort, int line_num);
int is_airport_valid(const char *airport, int line_num);
void bubble_sort(int *indices, int size, Plane *data);
void print_table(int *indices, int size, Plane *data, const char *airport);

//ТЕСТЫ
// ----- НЕКОРРЕКТНЫЕ ----- //

// Тест 1. Ошибка формата времени
// const char *FILE_NAME = "tests/incorrect/test2.txt";

// Тест 2. Ошибка бортового номера  
// const char *FILE_NAME = "tests/incorrect/test3.txt";

// Тест 3. Ошибка аэродрома
// const char *FILE_NAME = "tests/incorrect/test4.txt";

// Тест 4. Пустая модель
// const char *FILE_NAME = "tests/incorrect/test5.txt";

// Тест 5. Лишние данные
// const char *FILE_NAME = "tests/incorrect/test6.txt";    // Лишние данные

// Тест 6. Смешанные ошибки
// const char *FILE_NAME = "tests/incorrect/test7.txt";    // Лишние данные

// Тест 7. Граничные значения времени
// const char *FILE_NAME = "tests/incorrect/test8.txt";    // Лишние данные

// Тест 8. Различные символы
// const char *FILE_NAME = "tests/incorrect/test9.txt";    // Лишние данные

// Тест 9. Пустые строки
// const char *FILE_NAME = "tests/incorrect/test10.txt";    // Лишние данные


// ----- КОРРЕКТНЫЕ ----- //

// Цель: Абсолютно корректные данные
// const char *FILE_NAME = "tests/correct/test1.txt";    // 12:30,BOEING-747,A-1234,AP1

// Цель: Корректные данные с лишними пробелами
// const char *FILE_NAME = "tests/correct/test2.txt";    // 08:15,  AIRBUS-A320 , B-5678, AP2  

// Цель: Граничное время (23:59)
const char *FILE_NAME = "tests/correct/test3.txt";    // 23:59,CONCORDE,C-9999,AP3

// Цель: Минимально допустимые значения
// const char *FILE_NAME = "tests/correct/test4.txt";    // 00:00,MINI-JET,A-0000,AP1

// Цель: Нет посадок на все аэродромы
// const char *FILE_NAME = "tests/correct/test6.txt";    
/* 
Airport AP1: no landings
Airport AP2: no landings
Airport AP3: no landings
*/

// Цель: Нет посадок на 1-ый аэродром
// const char *FILE_NAME = "tests/correct/test7.txt";    // Airport AP1: no landings

// Цель: Нет посадок на 2-ой аэродром
// const char *FILE_NAME = "tests/correct/test8.txt";    // Airport AP2: no landings

// Цель: Нет посадок на 3-ий аэродром
// const char *FILE_NAME = "tests/correct/test9.txt";    // Airport AP3: no landings

// Цель: Нормализация значений с дефисами
// const char *FILE_NAME = "tests/correct/test10.txt";

// Цель: Повторяющееся время посадки
// const char *FILE_NAME = "tests/correct/test11.txt"; //Цель: Проверить сортировку при одинаковом времени.

// Цель: Частично корректные входные данные 
// const char *FILE_NAME = "tests/correct/test12.txt"; //Цель: Проверить обработку файла с одной корректной записью среди ошибок.

// Цель: Время с ведущими нулями
// const char *FILE_NAME = "tests/correct/test13.txt"; //Цель: Проверить обработку времени вида 00:05 и 05:09.

// Активный тест (для быстрой проверки)
// const char *FILE_NAME = "tests/correct/test1.txt";

/*******************************************************************************
*  ГЛАВНАЯ ФУНКЦИЯ 
******************************************************************************/
int main() {
    // Установка кодировки консоли для корректного отображения символов
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.UTF-8");
    
    // Основные переменные программы
    Plane planes[MAX_PLANES];  // Массив для хранения данных о самолетах
    int count = 0;             // Счетчик успешно загруженных записей
    const char *airports[] = {"AP1", "AP2", "AP3"}; // Список аэродромов для обработки
    
    // Чтение данных из файла
    int err = read_data(FILE_NAME, planes, &count);
    if (err == -1) {
        print_error(err, FILE_NAME, 0);
        return 1;
    }

    // Обработка данных для каждого аэродрома
    for (int i = 0; i < 3; i++) {
        process_airport(planes, count, airports[i]);
    }
    
    return 0;
}

/*******************************************************************************
*  ИНИЦИАЛИЗАЦИЯ ФУНКЦИЙ
******************************************************************************/

/**
 * Выводит сообщение об ошибке с указанием типа и местоположения.
 * @param err Код ошибки (от -1 до -6).
 * @param field Название файла или проблемное поле.
 * @param line_num Номер строки с ошибкой.
 * @note Цвет сообщения: красный (использует ANSI-коды).
 */
void print_error(int err, const char* field, int line_num) {
    cout << "[31m";
    if (err == -1)      cout << "File \"" << field << "\" not found";
    else if (err == -2) cout << "Invalid time format in line " << line_num << " (skipped)";
    else if (err == -3) cout << "Invalid bort number in line " << line_num << " (skipped)";
    else if (err == -4) cout << "Invalid airport code in line " << line_num << " (skipped)";
    else if (err == -5) cout << "Empty model in line " << line_num << " (skipped)";
    else if (err == -6) cout << "Extra data in line " << line_num << " (skipped)";
    else if (err == -8) cout << "Empty line " << line_num << " (skipped)";
    cout << "[0m\n";
}

/**
 * Читает данные о посадках из файла, парсит и валидирует их.
 * @param filename Имя файла с данными.
 * @param planes Массив структур Plane для заполнения.
 * @param count Указатель на количество успешно считанных записей.
 * @return 0 при успехе, -1 если файл не найден, -7 при наличии ошибок в данных.
 * @note Формат строки: "HH:MM,Модель,Бортовой_номер,Аэродром".
 */
int read_data(const char *filename, Plane *planes, int *count) {
    // Открытие файла
    ifstream file(filename);
    if (!file.is_open()) return -1;

    // Переменные для обработки файла
    char line[100];         // Буфер для чтения строки
    int line_num = 0;       // Счетчик строк
    bool has_errors = false; // Флаг наличия ошибок

    while (file.getline(line, 100) && *count < MAX_PLANES) {
        line_num++;
        Plane p = {};       // Временная структура для хранения данных
        int pos = 0;       // Позиция в строке
        bool is_valid = true; // Флаг валидности строки

        // Проверка на пустую строку
        bool is_empty = true;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != ' ' && line[i] != '\t') {
                is_empty = false;
                break;
            }
        }
        if (is_empty) {
            print_error(-8, filename, line_num);
            has_errors = true;
            continue;
        }

        // ПАРСИНГ ПОЛЕЙ
        for (int field_num = 0; field_num < 4 && is_valid; field_num++) {
            // Определение целевого поля и его максимальной длины
            char* dest = NULL;
            int max_len = 0;
            int j = 0;

            if (field_num == 0) {
                dest = p.time;
                max_len = TIME_LEN-1;
            } else if (field_num == 1) {
                dest = p.model;
                max_len = MODEL_LEN-1;
            } else if (field_num == 2) {
                dest = p.bort;
                max_len = BORT_LEN-1;
            } else {
                dest = p.airport;
                max_len = AIRPORT_LEN-1;
            }

            // Пропуск пробелов перед полем
            while (line[pos] == ' ') pos++;

            // Чтение поля до запятой или конца строки
            j = 0;
            while (line[pos] && line[pos] != ',' && j < max_len) {
                dest[j++] = line[pos++];
            }
            dest[j] = '\0';

            // Пропуск запятой и пробелов после поля
            if (line[pos] == ',') {
                pos++;
                while (line[pos] == ' ') pos++;
            }
        }

        // НОРМАЛИЗАЦИЯ
        purify(p.time);
        purify(p.model);
        purify(p.bort);
        purify(p.airport);

        // ВАЛИДАЦИЯ ПОЛЕЙ
        if (p.model[0] == '\0') {
            print_error(-5, filename, line_num);
            is_valid = false;
        }
        else if (is_time_valid(p.time, line_num) != 0) {
            print_error(-2, filename, line_num);
            is_valid = false;
        }
        else if (is_bort_valid(p.bort, line_num) != 0) {
            print_error(-3, filename, line_num);
            is_valid = false;
        }
        else if (is_airport_valid(p.airport, line_num) != 0) {
            print_error(-4, filename, line_num);
            is_valid = false;
        }
        else {
            // Проверка на лишние данные только если все поля валидны
            while (line[pos] == ' ') pos++;
            if (line[pos] != '\0') {
                print_error(-6, filename, line_num);
                is_valid = false;
            }
        }

        if (!is_valid) {
            has_errors = true;
            continue;
        }

        // КОНВЕРТАЦИЯ ВРЕМЕНИ И СОХРАНЕНИЕ
        p.minutes = ((p.time[0]-'0')*10 + (p.time[1]-'0'))*60 +
                    ((p.time[3]-'0')*10 + (p.time[4]-'0'));
        planes[(*count)++] = p;
    }
    file.close();
    return has_errors ? -7 : 0;
}

/**
 * Нормализует поле: удаляет пробелы и переводит символы в верхний регистр.
 * @param field Указатель на строку для обработки.
 * @note Изменяет исходную строку.
 */
void purify(char* field) {
    // Указатели для чтения и записи
    char* read_ptr = field;
    char* write_ptr = field;
    
    // Обработка каждого символа
    while (*read_ptr) {
        if (*read_ptr != ' ' && *read_ptr != '\t') {
            *write_ptr = toupper(*read_ptr);
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

/**
 * Проверяет корректность формата времени (HH:MM).
 * @param time Строка времени.
 * @param line_num Номер строки (для вывода ошибки).
 * @return 0 при корректном формате, -2 при ошибке.
 */
int is_time_valid(const char *time, int line_num) {
    // Проверка формата
    if (time[2] != ':' || time[5] != '\0') return -2;
    
    // Проверка цифр
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (time[i] < '0' || time[i] > '9') return -2;
    }
    // Проверка диапазонов часов и минут
    int hours = (time[0]-'0')*10 + (time[1]-'0');
    int minutes = (time[3]-'0')*10 + (time[4]-'0');
    
    return (hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60) ? 0 : -2;
}

/**
 * Проверяет формат бортового номера (X-XXXX, где X — буква/цифра).
 * @param bort Бортовой номер.
 * @param line_num Номер строки (для вывода ошибки).
 * @return 0 при корректном формате, -3 при ошибке.
 */
int is_bort_valid(const char *bort, int line_num) {
    // Проверка длины и формата
    if (bort[1] != '-' || bort[6] != '\0') return -3;
    
    // Проверка первой буквы
    if (bort[0] < 'A' || bort[0] > 'Z') return -3;
    
    // Проверка цифр
    for (int i = 2; i < 6; i++) {
        if (bort[i] < '0' || bort[i] > '9') return -3;
    }
    
    return 0;
}

/**
 * Проверяет код аэродрома (допустимые значения: AP1, AP2, AP3).
 * @param airport Код аэродрома.
 * @param line_num Номер строки (для вывода ошибки).
 * @return 0 при корректном коде, -4 при ошибке.
 */
int is_airport_valid(const char *airport, int line_num) {
    // Объявление всех переменных в начале
    int len = 0; // Длина поля аэропоорта
    int i, j; // Счетчики
    bool match; // Флаг для добавления самолетов в соответств. аэропорт
    const char valid[3][4] = {"AP1", "AP2", "AP3"};

    // Проверка длины
    while (airport[len] != '\0' && len < 4) len++;
    if (len != 3) return -4;
    
    // Проверка допустимых значений
    for (i = 0; i < 3; i++) {
        match = true;
        for (j = 0; j < 3; j++) {
            if (airport[j] != valid[i][j]) match = false;
        }
        if (match) return 0;
    }
    return -4;
}

/**
 * Сортирует индексы записей по времени посадки (от поздних к ранним).
 * @param indices Массив индексов для сортировки.
 * @param size Размер массива.
 * @param data Массив структур Plane с данными.
 */
void bubble_sort(int *indices, int size, Plane *data) {
    int temp; // Временная переменная для обмена

    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-i-1; j++) {
            if (data[indices[j]].minutes < data[indices[j+1]].minutes) {
                temp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = temp;
            }
        }
    }
}

/**
 * Форматирует и выводит таблицу с данными для указанного аэродрома.
 * @param indices Массив индексов записей.
 * @param size Количество записей.
 * @param data Массив структур Plane.
 * @param airport Код аэродрома.
 * @note Если записей нет, выводит "no landings".
 */
void print_table(int *indices, int size, Plane *data, const char *airport) {
    // Проверка на отсутствие данных
    if (size == 0) {
        cout << "Airport " << airport << ": no landings\n";
        return;
    }

    // Вывод заголовка таблицы
    cout << "\nAirport " << airport << ":\n";
    cout << "┌────────────┬───────────────┬────────────────┬────────────────┐\n";
    cout << "│ Time       │ Model         │ Bort Number    │ Airport        │\n";
    cout << "├────────────┼───────────────┼────────────────┼────────────────┤\n";
    
    // Вывод данных
    for (int i = 0; i < size; i++) {
        const Plane &p = data[indices[i]];
        cout << "│ " << left << setw(11) << p.time << "│ "
             << setw(14) << p.model << "│ "
             << setw(15) << p.bort << "│ "
             << setw(15) << p.airport << "│\n";
    }
    // Вывод нижней границы таблицы
    cout << "└────────────┴───────────────┴────────────────┴────────────────┘\n";
}

/**
 * Обрабатывает данные для конкретного аэродрома: фильтрация, сортировка, вывод.
 * @param planes Массив структур Plane.
 * @param count Общее количество записей.
 * @param airport Код аэродрома.
 */
void process_airport(Plane *planes, int count, const char *airport) {
    // Массив для хранения индексов подходящих записей
    int indices[MAX_PLANES];
    int size = 0; // Количество подходящих записей
    bool match; // Флаг для добавления самолетов в соответств. аэропорт

    // Фильтрация записей по аэродрому
    for (int i = 0; i < count; i++) {
        match = true;
        for (int j = 0; j < 3; j++) {
            if (planes[i].airport[j] != airport[j]) match = false;
        }
        if (match) indices[size++] = i;
    }

    // Сортировка и вывод
    bubble_sort(indices, size, planes);
    print_table(indices, size, planes, airport);
}