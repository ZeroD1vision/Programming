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
    char field_name[30];
    char message[150];
};

/*******************************************************************************
*  ПРОТОТИПЫ ФУНКЦИЙ
******************************************************************************/
void print_error(const ErrorInfo& error);
int read_data(const char *filename, Plane *planes, int *count, ErrorInfo *errors, int *error_count);
void process_airport(Plane *planes, int count, const char *airport);
void purify(char* field);
void check_time_valid(const char *time, int line_num, ErrorInfo *errors, int *error_count);
void check_bort_valid(const char *bort, int line_num, ErrorInfo *errors, int *error_count);
void check_airport_valid(const char *airport, int line_num, ErrorInfo *errors, int *error_count);
void bubble_sort(int *indices, int size, Plane *data);
void print_table(int *indices, int size, Plane *data, const char *airport);

// Тестовый файл
const char *FILE_NAME = "tests/incorrect/test5.txt";

/*******************************************************************************
*  ГЛАВНАЯ ФУНКЦИЯ 
******************************************************************************/
int main() {
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

void print_error(const ErrorInfo& error) {
    cout << "[31m";
    cout << "Строка " << error.line_num << ", поле '" << error.field_name 
         << "': " << error.message;
    cout << "[0m\n";
}

int read_data(const char *filename, Plane *planes, int *count, ErrorInfo *errors, int *error_count) {
    ifstream file(filename);
    if (!file.is_open()) {
        strncpy(errors[*error_count].field_name, "Файл", sizeof(errors[*error_count].field_name) - 1);
        strncpy(errors[*error_count].message, "Файл не найден", sizeof(errors[*error_count].message) - 1);
        errors[*error_count].line_num = 0;
        (*error_count)++;
        return -1;
    }

    char line[100];
    int line_num = 0;
    bool has_errors = false;

    while (file.getline(line, 100) && *count < MAX_PLANES) {
        line_num++;
        
        // Эхо-печать обрабатываемой строки
        cout << "Обработка строки " << line_num << ": " << line << endl;
        
        Plane p = {};
        int pos = 0;
        bool is_valid = true;

        // Проверка на пустую строку
        bool is_empty = true;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != ' ' && line[i] != '\t') {
                is_empty = false;
                break;
            }
        }
        if (is_empty) {
            strncpy(errors[*error_count].field_name, "Строка", sizeof(errors[*error_count].field_name) - 1);
            strncpy(errors[*error_count].message, "Пустая строка", sizeof(errors[*error_count].message) - 1);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
            has_errors = true;
            continue;
        }

        // ПАРСИНГ ПОЛЕЙ
        for (int field_num = 0; field_num < 4 && is_valid; field_num++) {
            char* dest = NULL;
            int max_len = 0;
            char field_name[20];

            if (field_num == 0) {
                dest = p.time;
                max_len = TIME_LEN-1;
                strncpy(field_name, "Время", sizeof(field_name) - 1);
            } else if (field_num == 1) {
                dest = p.model;
                max_len = MODEL_LEN-1;
                strncpy(field_name, "Модель", sizeof(field_name) - 1);
            } else if (field_num == 2) {
                dest = p.bort;
                max_len = BORT_LEN-1;
                strncpy(field_name, "Бортовой номер", sizeof(field_name) - 1);
            } else {
                dest = p.airport;
                max_len = AIRPORT_LEN-1;
                strncpy(field_name, "Аэродром", sizeof(field_name) - 1);
            }

            while (line[pos] == ' ') pos++;

            int j = 0;
            while (line[pos] && line[pos] != ',' && j < max_len) {
                dest[j++] = line[pos++];
            }
            dest[j] = '\0';

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
            strncpy(errors[*error_count].field_name, "Модель", sizeof(errors[*error_count].field_name) - 1);
            strncpy(errors[*error_count].message, "Пустое поле", sizeof(errors[*error_count].message) - 1);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
            is_valid = false;
        }
        
        check_time_valid(p.time, line_num, errors, error_count);
        check_bort_valid(p.bort, line_num, errors, error_count);
        check_airport_valid(p.airport, line_num, errors, error_count);
        
        // Проверка на лишние данные
        while (line[pos] == ' ') pos++;
        if (line[pos] != '\0') {
            strncpy(errors[*error_count].field_name, "Строка", sizeof(errors[*error_count].field_name) - 1);
            strncpy(errors[*error_count].message, "Лишние данные в конце строки", sizeof(errors[*error_count].message) - 1);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
            is_valid = false;
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

void purify(char* field) {
    char* read_ptr = field;
    char* write_ptr = field;
    
    while (*read_ptr) {
        if (*read_ptr != ' ' && *read_ptr != '\t') {
            *write_ptr = toupper(*read_ptr);
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

void check_time_valid(const char *time, int line_num, ErrorInfo *errors, int *error_count) {
    if (strlen(time) != 5) {
        strcpy(errors[*error_count].field_name, "Время");
        strcpy(errors[*error_count].message, "Неправильная длина (должно быть 5 символов)");
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return;
    }
    
    if (time[2] != ':') {
        char msg[100];
        snprintf(msg, sizeof(msg), "Неправильный разделитель (должен быть ':', а получен '%c')", time[2]);
        strcpy(errors[*error_count].field_name, "Время");
        strcpy(errors[*error_count].message, msg);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
    
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (time[i] < '0' || time[i] > '9') {
            char msg[100];
            snprintf(msg, sizeof(msg), "Недопустимый символ '%c' в позиции %d", time[i], i);
            strcpy(errors[*error_count].field_name, "Время");
            strcpy(errors[*error_count].message, msg);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
        }
    }
    
    if (*error_count > 0) return;
    
    int hours = (time[0]-'0')*10 + (time[1]-'0');
    int minutes = (time[3]-'0')*10 + (time[4]-'0');
    
    if (hours < 0 || hours >= 24) {
        strcpy(errors[*error_count].field_name, "Время");
        strcpy(errors[*error_count].message, "Часы должны быть в диапазоне 00-23");
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
    
    if (minutes < 0 || minutes >= 60) {
        strcpy(errors[*error_count].field_name, "Время");
        strcpy(errors[*error_count].message, "Минуты должны быть в диапазоне 00-59");
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
}

void check_bort_valid(const char *bort, int line_num, ErrorInfo *errors, int *error_count) {
    if (strlen(bort) != 6) {
        strcpy(errors[*error_count].field_name, "Бортовой номер");
        strcpy(errors[*error_count].message, "Неправильная длина (должно быть 6 символов)");
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return;
    }
    
    if (bort[1] != '-') {
        char msg[100];
        snprintf(msg, sizeof(msg), "Неправильный разделитель (должен быть '-', а получен '%c')", bort[1]);
        strcpy(errors[*error_count].field_name, "Бортовой номер");
        strcpy(errors[*error_count].message, msg);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
    
    if (bort[0] < 'A' || bort[0] > 'Z') {
        char msg[100];
        snprintf(msg, sizeof(msg), "Первым символом должна быть буква A-Z (получено '%c')", bort[0]);
        strcpy(errors[*error_count].field_name, "Бортовой номер");
        strcpy(errors[*error_count].message, msg);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
    
    for (int i = 2; i < 6; i++) {
        if (bort[i] < '0' || bort[i] > '9') {
            char msg[100];
            snprintf(msg, sizeof(msg), "Недопустимый символ '%c' в позиции %d (ожидалась цифра)", bort[i], i);
            strcpy(errors[*error_count].field_name, "Бортовой номер");
            strcpy(errors[*error_count].message, msg);
            errors[*error_count].line_num = line_num;
            (*error_count)++;
        }
    }
}

void check_airport_valid(const char *airport, int line_num, ErrorInfo *errors, int *error_count) {
    if (strlen(airport) != 3) {
        strcpy(errors[*error_count].field_name, "Аэродром");
        strcpy(errors[*error_count].message, "Неправильная длина (должно быть 3 символа)");
        errors[*error_count].line_num = line_num;
        (*error_count)++;
        return;
    }
    
    if (airport[0] != 'A' || airport[1] != 'P') {
        strcpy(errors[*error_count].field_name, "Аэродром");
        strcpy(errors[*error_count].message, "Код должен начинаться с 'AP'");
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
    
    if (airport[2] < '1' || airport[2] > '3') {
        char msg[100];
        snprintf(msg, sizeof(msg), "Последний символ должен быть цифрой 1-3 (получено '%c')", airport[2]);
        strcpy(errors[*error_count].field_name, "Аэродром");
        strcpy(errors[*error_count].message, msg);
        errors[*error_count].line_num = line_num;
        (*error_count)++;
    }
}

void bubble_sort(int *indices, int size, Plane *data) {
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-i-1; j++) {
            if (data[indices[j]].minutes < data[indices[j+1]].minutes) {
                int temp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = temp;
            }
        }
    }
}

void print_table(int *indices, int size, Plane *data, const char *airport) {
    if (size == 0) {
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

void process_airport(Plane *planes, int count, const char *airport) {
    int indices[MAX_PLANES];
    int size = 0;
    
    for (int i = 0; i < count; i++) {
        bool match = true;
        for (int j = 0; j < 3; j++) {
            if (planes[i].airport[j] != airport[j]) match = false;
        }
        if (match) indices[size++] = i;
    }
    
    bubble_sort(indices, size, planes);
    print_table(indices, size, planes, airport);
}