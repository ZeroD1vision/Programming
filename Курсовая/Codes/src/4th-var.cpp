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
const int MAX_PLANES = 100;
const int TIME_LEN = 6;
const int MODEL_LEN = 20;
const int BORT_LEN = 7;
const int AIRPORT_LEN = 4;

// СТРУКТУРА ДАННЫХ
struct Plane {
    char time[TIME_LEN];       // HH:MM
    int minutes;               // время в минутах
    char model[MODEL_LEN];     // модель самолета
    char bort[BORT_LEN];       // бортовой номер
    char airport[AIRPORT_LEN]; // аэродром
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

// Цель: Проверить обработку отсутствия файла
// const char *FILE_NAME = "tests/incorrect/test1.txt";  // Файл не существует

// Цель: Проверить неверный формат времени (25:70)
// const char *FILE_NAME = "tests/incorrect/test2.txt";  // 25:70,BOEING-777,E-1111,AP1

// Цель: Проверить неверный формат бортового номера
// const char *FILE_NAME = "tests/incorrect/test3.txt";  // 10:00,AIRBUS-A380,XYZ-12,AP2

// Цель: Проверить неверный код аэродрома
// const char *FILE_NAME = "tests/incorrect/test4.txt";  // 09:45,SUPERJET-100,F-3333,INV

// Цель: Проверить пустую модель
// const char *FILE_NAME = "tests/incorrect/test5.txt";  // 14:20,   ,G-4444,AP3

// Цель: Лишние данные
// const char *FILE_NAME = "tests/incorrect/test6.txt";    // Лишние данные

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


// Активный тест (для быстрой проверки)
// const char *FILE_NAME = "tests/correct/test1.txt";

/*******************************************************************************
*  ГЛАВНАЯ ФУНКЦИЯ 
******************************************************************************/
int main() {
    // Установка кодировки консоли для корректного отображения символов
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.UTF-8");
    Plane planes[MAX_PLANES];
    int count = 0;
    const char *airports[] = {"AP1", "AP2", "AP3"};
    
    int err = read_data(FILE_NAME, planes, &count);
    if (err == -1) {
        print_error(err, FILE_NAME, 0);
        return 1;
    }

    // Всегда обрабатываем данные, если count > 0
    for (int i = 0; i < 3; i++) {
        process_airport(planes, count, airports[i]);
    }
    
    return 0;
}

/*******************************************************************************
*  ИНИЦИАЛИЗАЦИЯ ФУНКЦИЙ
******************************************************************************/

void print_error(int err, const char* field, int line_num) {
    cout << "[31m";
    if (err == -1)      cout << "File \"" << field << "\" not found";
    else if (err == -2) cout << "Invalid time format in line " << line_num;
    else if (err == -3) cout << "Invalid bort number in line " << line_num;
    else if (err == -4) cout << "Invalid airport code in line " << line_num;
    else if (err == -5) cout << "Empty model in line " << line_num;
    else if (err == -6) cout << "Extra data in line " << line_num;
    cout << "[0m\n";
}

int read_data(const char *filename, Plane *planes, int *count) {
    ifstream file(filename);
    if (!file.is_open()) return -1;

    char line[100];
    int line_num = 0;
    bool has_errors = false;

    while (file.getline(line, 100) && *count < MAX_PLANES) {
        line_num++;
        Plane p = {};
        int pos = 0;
        bool is_valid = true;

        // ПАРСИНГ И ВАЛИДАЦИЯ ПОЛЕЙ
        for (int i = 0; i < 4; i++) {
            char* dest = i == 0 ? p.time : (i == 1 ? p.model : (i == 2 ? p.bort : p.airport));
            int max_len = i == 0 ? TIME_LEN-1 : (i == 1 ? MODEL_LEN-1 : (i == 2 ? BORT_LEN-1 : AIRPORT_LEN-1));

            int j = 0;
            while (line[pos] && line[pos] != ',' && j < max_len) {
                if (line[pos] != ' ') dest[j++] = line[pos];
                pos++;
            }
            dest[j] = '\0';
            while (line[pos] == ',' || line[pos] == ' ') pos++;
        }

        // ПРОВЕРКА НА ЛИШНИЕ ДАННЫЕ
        if (line[pos] != '\0') {
            print_error(-6, filename, line_num);
            has_errors = true;
            is_valid = false;
        }

        // НОРМАЛИЗАЦИЯ
        purify(p.time);
        purify(p.model);
        purify(p.bort);
        purify(p.airport);

        // ПОСЛЕДОВАТЕЛЬНЫЕ ПРОВЕРКИ
        if (p.model[0] == '\0' && is_valid) {
            print_error(-5, filename, line_num);
            is_valid = false;
        }
        if (is_time_valid(p.time, line_num) != 0 && is_valid) {
            print_error(-2, filename, line_num);
            is_valid = false;
        }
        if (is_bort_valid(p.bort, line_num) != 0 && is_valid) {
            print_error(-3, filename, line_num);
            is_valid = false;
        }
        if (is_airport_valid(p.airport, line_num) != 0 && is_valid) {
            print_error(-4, filename, line_num);
            is_valid = false;
        }

        if (!is_valid) {
            has_errors = true;
            continue;
        }

        // КОНВЕРТАЦИЯ ВРЕМЕНИ
        p.minutes = ((p.time[0]-'0')*10 + (p.time[1]-'0'))*60 +
                    ((p.time[3]-'0')*10 + (p.time[4]-'0'));
        
        planes[(*count)++] = p;
    }
    file.close();
    return has_errors ? -7 : 0; // Код -7: есть ошибки, но данные обработаны
}

void purify(char* field) {
    char* dst = field;
    for (; *field; field++) {
        if (*field != ' ' && *field != '\t') *dst++ = toupper(*field);
    }
    *dst = '\0';
}

int is_time_valid(const char *time, int line_num) {
    if (time[2] != ':' || time[5] != '\0') return -2;
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (time[i] < '0' || time[i] > '9') return -2;
    }
    int h = (time[0]-'0')*10 + (time[1]-'0');
    int m = (time[3]-'0')*10 + (time[4]-'0');
    return (h >= 0 && h < 24 && m >= 0 && m < 60) ? 0 : -2;
}

int is_bort_valid(const char *bort, int line_num) {
    if (bort[1] != '-' || bort[6] != '\0') return -3;
    if (bort[0] < 'A' || bort[0] > 'Z') return -3;
    for (int i = 2; i < 6; i++) {
        if (bort[i] < '0' || bort[i] > '9') return -3;
    }
    return 0;
}

int is_airport_valid(const char *airport, int line_num) {
    const char valid[3][4] = {"AP1", "AP2", "AP3"};
    for (int i = 0; i < 3; i++) {
        bool match = true;
        for (int j = 0; j < 3; j++) {
            if (airport[j] != valid[i][j]) match = false;
        }
        if (match) return 0;
    }
    return -4;
}

// ОСТАВШИЕСЯ ФУНКЦИИ БЕЗ ИЗМЕНЕНИЙ
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