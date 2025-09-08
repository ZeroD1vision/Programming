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
#include <cstring>
// Для локали
#include <locale>
#include <codecvt>
#include <windows.h>

using namespace std;

// КОНСТАНТЫ
const int MAX_PLANES = 100;
const int TIME_LEN = 6;
const int MODEL_LEN = 20;
const int BORT_LEN = 7;
const int AIRPORT_LEN = 4;
const int MAX_ERRORS = 1000; // Максимальное количество ошибок

// Ожидаемая длина полей
const int EXPECTED_TIME_LEN = 5;    // HH:MM
const int EXPECTED_BORT_LEN = 6;    // X-XXXX
const int EXPECTED_AIRPORT_LEN = 3; // APX
const int ALLOWED_FIELDS = 4;

// СТРУКТУРА ДАННЫХ
struct Plane {
    char time[TIME_LEN];
    int minutes;
    char model[MODEL_LEN];
    char bort[BORT_LEN];
    char airport[AIRPORT_LEN];
};

// Структура для хранения ошибок
struct ErrorInfo {
    int line_num;
    char field_name[50]; // Увеличен буфер для имени поля
    char message[250];   // Увеличен буфер для сообщения
};

/*******************************************************************************
*  ПРОТОТИПЫ ФУНКЦИЙ
******************************************************************************/
void print_error(const ErrorInfo& error);
int read_data(const char *filename, Plane *planes, int *count, ErrorInfo *errors, int *error_count);
void process_airport(Plane *planes, int count, const char *airport);
void purify(char* field);
int check_and_convert_time(const char *time, int line_num, ErrorInfo *errors, int *error_count);
void check_model_valid(const char *model, int line_num, ErrorInfo *errors, int *error_count);
void check_bort_valid(const char *bort, int line_num, ErrorInfo *errors, int *error_count);
void check_airport_valid(const char *airport, int line_num, ErrorInfo *errors, int *error_count);
void bubble_sort(int *indices, int size, Plane *data);
void print_table(int *indices, int size, Plane *data, const char *airport);

// Тестовый файл
const char *FILE_NAME = "tests/correct/test2.txt";

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

// Тест 8. Пустые строки
// const char *FILE_NAME = "tests/incorrect/test10.txt";    // Лишние данные


// ----- КОРРЕКТНЫЕ ----- //

// Цель: Абсолютно корректные данные
// const char *FILE_NAME = "tests/correct/test1.txt";    // 12:30,BOEING-747,A-1234,AP1

// Цель: Корректные данные с лишними пробелами
// const char *FILE_NAME = "tests/correct/test2.txt";    // 08:15,  AIRBUS-A320 , B-5678, AP2  

// Цель: Граничное время (23:59)
// const char *FILE_NAME = "tests/correct/test3.txt";    // 23:59,CONCORDE,C-9999,AP3

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

// Различные символы
// const char *FILE_NAME = "tests/incorrect/test9.txt";

/*******************************************************************************
*  ГЛАВНАЯ ФУНКЦИЯ 
******************************************************************************/
int main() 
{
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.UTF-8");
    
    Plane planes[MAX_PLANES];
    int count = 0;
    const char *airports[] = {"AP1", "AP2", "AP3"};
    ErrorInfo errors[MAX_ERRORS];
    int error_count = 0;
    
    // Эхо-печать имени файла
    cout << "Обрабатываем файл: " << FILE_NAME << "\n\n";
    
    int err = read_data(FILE_NAME, planes, &count, errors, &error_count);
    
    // Вывод всех ошибок
    if (error_count > 0) {
        cout << "\nНайдены ошибки:\n";
        for (int i = 0; i < error_count; i++) {
            print_error(errors[i]);
        }
    }
    
    if (err == -1) {
        return 1;
    }

    // Эхо-печать количества успешно обработанных записей
    cout << "\nУспешно обработано записей: " << count << "\n\n";
    
    for (int i = 0; i < 3; i++) {
        process_airport(planes, count, airports[i]);
    }
    
    return 0;
}

/*******************************************************************************
*  ИНИЦИАЛИЗАЦИЯ ФУНКЦИЙ
******************************************************************************/

void print_error(const ErrorInfo& error) 
{
    cout << "[31m";
    cout << "Строка " << error.line_num << ", поле '" << error.field_name 
         << "': " << error.message;
    cout << "[0m\n";
}

int read_data(const char *filename, Plane *planes, int *count, ErrorInfo *errors, int *error_count) 
{
    ifstream file(filename);
    if (!file.is_open()) 
    {
        strncpy(errors[*error_count].field_name, "Файл", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, "Файл не найден", sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = 0;
        (*error_count)++;
        return -1;
    }

    char line[100];
    int line_num = 0;
    bool has_errors = false;

    while (file.getline(line, 100) && *count < MAX_PLANES) 
    {
        line_num++;
        
        // Эхо-печать обрабатываемой строки
        cout << "Обработка строки " << line_num << ": " << line << endl;
        
        Plane p = {}; 
        int pos = 0;
        bool is_valid = true;
        bool is_empty = true;
        int comma_count = 0;

        // Проверка на пустую строку
        for (int i = 0; line[i] != '\0'; i++) 
        {
            if (line[i] != ' ' && line[i] != '\t') 
            {
                is_empty = false;
                break;
            }
        }
        if (is_empty) 
        {
            strncpy(errors[*error_count].field_name, "Строка", sizeof(errors[*error_count].field_name) - 1);
            strncpy(errors[*error_count].message, "Пустая строка", sizeof(errors[*error_count].message) - 1);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
            has_errors = true;
            continue;
        }

        // Проверка количества запятых
        for (int i = 0; line[i] != '\0'; i++) 
        {
            if (line[i] == ',') comma_count++;
        }
        if (comma_count < 3) 
        {
            strncpy(errors[*error_count].field_name, "Строка", sizeof(errors[*error_count].field_name) - 1);
            strncpy(errors[*error_count].message, "Неправильное построение строки (недостаточно запятых)", sizeof(errors[*error_count].message) - 1);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
            has_errors = true;
            continue;
        }

        // ПАРСИНГ ПОЛЕЙ
        for (int field_num = 0; field_num < ALLOWED_FIELDS; field_num++) 
        {
            char* dest = NULL;
            int max_len = 0;
            int j = 0;
            bool field_too_long = false;
            char temp_field[100] = {0}; // Временный буфер для хранения поля
            int temp_index = 0;

            if (field_num == 0) 
            {
                dest = p.time;
                max_len = TIME_LEN;
            } else if (field_num == 1) 
            {
                dest = p.model;
                max_len = MODEL_LEN;
            } else if (field_num == 2) 
            {
                dest = p.bort;
                max_len = BORT_LEN;
            } else 
            {
                dest = p.airport;
                max_len = AIRPORT_LEN;
            }

            while (line[pos] == ' ') pos++;

            // Читаем символы до запятой или конца строки во временный буфер
            while (line[pos] && line[pos] != ',' && temp_index < 99) 
            {
                temp_field[temp_index++] = line[pos++];
            }
            temp_field[temp_index] = '\0';

            // Очищаем поле от пробелов
            purify(temp_field);
            int purified_len = strlen(temp_field);

            // Проверяем длину очищенного поля
            if (purified_len > max_len - 1) 
            {
                field_too_long = true;
                is_valid = false;
            }

            // Копируем очищенное поле в целевую переменную
            strncpy(dest, temp_field, max_len - 1);
            dest[max_len - 1] = '\0';

            // Если поле было слишком длинным, добавляем ошибку
            if (field_too_long) 
            {
                const char* field_name = "";
                switch (field_num) 
                {
                    case 0: field_name = "Время"; break;
                    case 1: field_name = "Модель"; break;
                    case 2: field_name = "Бортовой номер"; break;
                    case 3: field_name = "Аэродром"; break;
                }
                char msg[250];
                snprintf(msg, sizeof(msg), "Поле слишком длинное (максимум %d символов, получено %d)", 
                         max_len - 1, purified_len);
                strncpy(errors[*error_count].field_name, field_name, sizeof(errors[*error_count].field_name) - 1);
                strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
                errors[*error_count].line_num = line_num;
                (*error_count)++;
            }

            if (line[pos] == ',') 
            {
                pos++;
            }
        }

        // Пропускаем пробелы после последнего поля
        while (line[pos] == ' ') pos++;

        // Запоминаем количество ошибок перед валидацией
        int prev_error_count = *error_count;

        // ПРОВЕРКА ВРЕМЕНИ (первой, чтобы ошибки шли в правильном порядке)
        p.minutes = check_and_convert_time(p.time, line_num, errors, error_count);
        if (p.minutes < 0) 
        {
            is_valid = false;
        }

        // ВАЛИДАЦИЯ ОСТАЛЬНЫХ ПОЛЕЙ
        check_model_valid(p.model, line_num, errors, error_count);
        check_bort_valid(p.bort, line_num, errors, error_count);
        check_airport_valid(p.airport, line_num, errors, error_count);
        
        // Проверка на лишние данные
        if (line[pos] != '\0') 
        {
            strncpy(errors[*error_count].field_name, "Строка", sizeof(errors[*error_count].field_name) - 1);
            strncpy(errors[*error_count].message, "Лишние данные в конце строки", sizeof(errors[*error_count].message) - 1);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
            is_valid = false;
        }

        // Если были ошибки валидации, не добавляем запись
        if (*error_count > prev_error_count) 
        {
            is_valid = false;
        }

        // Добавляем запись, если она валидна
        if (is_valid) 
        {
            planes[(*count)++] = p;
        } else {
            has_errors = true;
        }
    }
    file.close();
    return has_errors ? -7 : 0;
}

void purify(char* field) 
{
    char* read_ptr = field;
    char* write_ptr = field;
    
    while (*read_ptr) 
    {
        if (*read_ptr != ' ' && *read_ptr != '\t') {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

int check_and_convert_time(const char *time, int line_num, ErrorInfo *errors, int *error_count) 
{
    bool has_errors = false;
    // Проверка длины
    if (strlen(time) != EXPECTED_TIME_LEN) 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Неправильная длина (должно быть %d символов, получено %zu)", 
                 EXPECTED_TIME_LEN, strlen(time));
        strncpy(errors[*error_count].field_name, "Время", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return -1;
    }
    
    // Проверка разделителя
    if (time[2] != ':') 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Неправильный разделитель (должен быть ':', а получен '%c')", time[2]);
        strncpy(errors[*error_count].field_name, "Время", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        has_errors = true;
    }
    
    // Проверка цифровых символов
    for (int i = 0; i < EXPECTED_TIME_LEN; i++) 
    {
        if (i == 2) continue; // Пропускаем разделитель
        if (time[i] < '0' || time[i] > '9') 
        {
            char msg[250];
            snprintf(msg, sizeof(msg), "Недопустимый символ '%c' в позиции %d", time[i], i);
            strncpy(errors[*error_count].field_name, "Время", sizeof(errors[*error_count].field_name) - 1);
            strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
            has_errors = true;
        }
    }
    
    // Извлечение часов и минут
    int hours = (time[0]-'0')*10 + (time[1]-'0');
    int minutes = (time[3]-'0')*10 + (time[4]-'0');
    
    // Проверка диапазонов
    if (hours < 0 || hours >= 24) 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Часы должны быть в диапазоне 00-23 (получено %02d)", hours);
        strncpy(errors[*error_count].field_name, "Время", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        has_errors = true;
    }
    
    if (minutes < 0 || minutes >= 60) 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Минуты должны быть в диапазоне 00-59 (получено %02d)", minutes);
        strncpy(errors[*error_count].field_name, "Время", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        has_errors = true;
    }

    return has_errors ? -1 : hours * 60 + minutes;
}

void check_model_valid(const char *model, int line_num, ErrorInfo *errors, int *error_count) 
{
    if (strlen(model) == 0) 
    {
        strncpy(errors[*error_count].field_name, "Модель", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, "Пустое поле", sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return;
    }
    
    if (strlen(model) >= MODEL_LEN - 1) {
        char msg[250];
        snprintf(msg, sizeof(msg), "Слишком длинное название модели (максимум %d символов)", MODEL_LEN - 2);
        strncpy(errors[*error_count].field_name, "Модель", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
}

void check_bort_valid(const char *bort, int line_num, ErrorInfo *errors, int *error_count) 
{
    int len = strlen(bort);
    // Проверка на пустое поле
    if (len == 0) 
    {
        strncpy(errors[*error_count].field_name, "Бортовой номер", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, "Пустое поле", sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return;
    }
    
    if (len > EXPECTED_BORT_LEN) 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Слишком длинный бортовой номер (максимум %d символов, получено %d)", 
                 EXPECTED_BORT_LEN, len);
        strncpy(errors[*error_count].field_name, "Бортовой номер", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return;
    }
    
    if (len < EXPECTED_BORT_LEN) 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Слишком короткий бортовой номер (должно быть %d символов, получено %d)", 
                 EXPECTED_BORT_LEN, len);
        strncpy(errors[*error_count].field_name, "Бортовой номер", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return;
    }
    
    // Остальные проверки
    if (bort[1] != '-') 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Неправильный разделитель (должен быть '-', а получен '%c')", bort[1]);
        strncpy(errors[*error_count].field_name, "Бортовой номер", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
    
    if (bort[0] < 'A' || bort[0] > 'Z') 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Первым символом должна быть заглавная буква A-Z (получено '%c')", bort[0]);
        strncpy(errors[*error_count].field_name, "Бортовой номер", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
    
    for (int i = 2; i < EXPECTED_BORT_LEN; i++) 
    {
        if (bort[i] < '0' || bort[i] > '9') 
        {
            char msg[250];
            snprintf(msg, sizeof(msg), "Недопустимый символ '%c' в позиции %d (ожидалась цифра)", bort[i], i);
            strncpy(errors[*error_count].field_name, "Бортовой номер", sizeof(errors[*error_count].field_name) - 1);
            strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
        }
    }
}

void check_airport_valid(const char *airport, int line_num, ErrorInfo *errors, int *error_count) 
{
    if (strlen(airport) != EXPECTED_AIRPORT_LEN) 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Неправильная длина (должно быть %d символа, получено %zu)", 
                 EXPECTED_AIRPORT_LEN, strlen(airport));
        strncpy(errors[*error_count].field_name, "Аэродром", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return;
    }

    if (airport[0] != 'A' || airport[1] != 'P') 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Код должен начинаться с 'AP' (получено '%.2s')", airport);
        strncpy(errors[*error_count].field_name, "Аэродром", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
    
    if (airport[2] < '1' || airport[2] > '3') 
    {
        char msg[250];
        snprintf(msg, sizeof(msg), "Последний символ должен быть цифрой 1-3 (получено '%c')", airport[2]);
        strncpy(errors[*error_count].field_name, "Аэродром", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, msg, sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
}

void bubble_sort(int *indices, int size, Plane *data) 
{
    for (int i = 0; i < size-1; i++) 
    {
        for (int j = 0; j < size-i-1; j++) 
        {
            if (data[indices[j]].minutes < data[indices[j+1]].minutes) 
            {
                int temp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = temp;
            }
        }
    }
}

void print_table(int *indices, int size, Plane *data, const char *airport) 
{
    if (size == 0) 
    {
        cout << "Airport " << airport << ": no landings\n";
        return;
    }
    
    cout << "\nAirport " << airport << ":\n";
    cout << "┌────────────┬───────────────┬────────────────┬────────────────┐\n";
    cout << "│ Time       │ Model         │ Bort Number    │ Airport        │\n";
    cout << "├────────────┼───────────────┼────────────────┼────────────────┤\n";
    
    for (int i = 0; i < size; i++) {
        const Plane &p = data[indices[i]];
        cout << "│ " << left << setw(11) << p.time << "│ "
             << setw(14) << p.model << "│ "
             << setw(15) << p.bort << "│ "
             << setw(15) << p.airport << "│\n";
    }
    cout << "└────────────┴───────────────┴────────────────┴────────────────┘\n";
}

void process_airport(Plane *planes, int count, const char *airport) 
{
    int indices[MAX_PLANES];
    int size = 0;
    
    for (int i = 0; i < count; i++) 
    {
        if (strncmp(planes[i].airport, airport, EXPECTED_AIRPORT_LEN) == 0) 
        {
            indices[size++] = i;
        }
    }
    
    bubble_sort(indices, size, planes);
    print_table(indices, size, planes, airport);
}