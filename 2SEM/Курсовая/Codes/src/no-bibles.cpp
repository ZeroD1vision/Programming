#include <iostream>
#include <fstream>
#include <iomanip>
// Для локали на русском
#include <locale>
#include <codecvt>
#include <windows.h>

using namespace std;

const int MAX_PLANES = 100;
const int TIME_LEN = 6;
const int MODEL_LEN = 20;
const int BORT_LEN = 7;
const int AIRPORT_LEN = 4;

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

void purify(char* field);
int read_data(const char *filename, Plane *planes, int *count);
void process_airport(Plane *planes, int count, const char *airport);
bool is_time_valid(const char *time);
bool is_bort_valid(const char *bort);
bool is_airport_valid(const char *airport);
void bubble_sort(int *indices, int size, Plane *data);
void print_table(int *indices, int size, Plane *data, const char *airport);

/*******************************************************************************
*  ГЛАВНАЯ ФУНКЦИЯ 
******************************************************************************/
int main() {
    // Установка кодировки консоли для корректного отображения символов
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.UTF-8");

    // ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    Plane planes[MAX_PLANES];
    int count = 0;
    const char *airports[] = {"AP1", "AP2", "AP3"};
    
    // ЧТЕНИЕ ДАННЫХ
    int err = read_data("landings.txt", planes, &count);
    if (err != 0) {
        cerr << "Error opening file!";
        return 1;
    }//if
    
    // ОБРАБОТКА АЭРОДРОМОВ
    for (int i = 0; i < 3; i++) {
        process_airport(planes, count, airports[i]);
    }//for i
    
    return 0;
}//main


/*******************************************************************************
*  ИНИЦИАЛИЗАЦИЯ ФУНКЦИЙ
******************************************************************************/

/******************************************************************************
* @brief Чтение данных из файла
* @param filename - имя файла
* @param planes - массив структур
* @param count - количество записей
* @return - Код ошибки (0 - успех)
******************************************************************************/
int read_data(const char *filename, Plane *planes, int *count) {
    // ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    ifstream file(filename);
    char line[100];
    *count = 0;
    int i = 0;
    int j = 0;
    int pos = 0;
    char buffer[20];

    // ОБРАБОТКА ОШИБОК
    if (!file.is_open()) return -1;

    // ЧТЕНИЕ СТРОК
    while (file.getline(line, 100) && *count < MAX_PLANES) {
        Plane p;
        pos = 0;
        
        // СЧИТЫВАНИЕ ВРЕМЕНИ
        for (i = 0; line[pos] != ',' && i < TIME_LEN-1; pos++) {
            if (line[pos] != ' ') p.time[i++] = line[pos];
        }
        p.time[i] = '\0';
        while (line[pos] == ',' || line[pos] == ' ') pos++;

        // СЧИТЫВАНИЕ МОДЕЛИ
        for (i = 0; line[pos] != ',' && i < MODEL_LEN-1; pos++) {
            if (line[pos] != ' ') p.model[i++] = line[pos];
        }
        p.model[i] = '\0';
        while (line[pos] == ',' || line[pos] == ' ') pos++;

        // СЧИТЫВАНИЕ БОРТОВОГО НОМЕРА
        for (i = 0; line[pos] != ',' && i < BORT_LEN-1; pos++) {
            if (line[pos] != ' ') p.bort[i++] = line[pos];
        }
        p.bort[i] = '\0';
        while (line[pos] == ',' || line[pos] == ' ') pos++;

        // СЧИТЫВАНИЕ АЭРОДРОМА
        for (i = 0; line[pos] && i < AIRPORT_LEN-1; pos++) {
            if (line[pos] != ' ') p.airport[i++] = line[pos];
        }
        p.airport[i] = '\0';

        // НОРМАЛИЗАЦИЯ ПОЛЕЙ
        purify(p.time);    // форматирование времени
        purify(p.bort);    // форматирование бортового номера
        purify(p.airport); // форматирование аэродрома
        purify(p.model);   // форматирование модели

        // ПРОВЕРКА НА ПУСТУЮ МОДЕЛЬ
        if (strlen(p.model) == 0) {
            cout << "Skipped: invalid model [" << line << "]\n";
            continue; // пропуск записи
        }//if

        // ПРОВЕРКА ВАЛИДНОСТИ ВРЕМЕНИ
        if (!is_time_valid(p.time)) {
            cout << "Skipped: invalid time [" << p.time << "]\n";
            continue; // пропуск записи
        }//if

        // ПРОВЕРКА ФОРМАТА БОРТОВОГО НОМЕРА
        if (!is_bort_valid(p.bort)) {
            cout << "Skipped: invalid bort [" << p.bort << "]\n";
            continue; // пропуск записи
        }//if

        // ПРОВЕРКА КОДА АЭРОДРОМА
        if (!is_airport_valid(p.airport)) {
            cout << "Skipped: invalid airport [" << p.airport << "]\n";
            continue; // пропуск записи
        }//if

        // КОНВЕРТАЦИЯ ВРЕМЕНИ
        p.minutes = ((p.time[0]-'0')*10 + (p.time[1]-'0'))*60 +
                    ((p.time[3]-'0')*10 + (p.time[4]-'0'));

        planes[(*count)++] = p;
    }//while

    file.close();
    return 0;
}//read_data

/******************************************************************************
* @brief Удаление пробелов и приведение к верхнему регистру
* 
* Функция обрабатывает строку, удаляя пробелы и табуляции,
* а также преобразует символы к верхнему регистру
* 
* @param field  указатель на обрабатываемую строку
******************************************************************************/
void purify(char* field) {
    char* dst = field;
    for (char* src = field; *src; ++src) {
        if (*src != ' ' && *src != '\t') {
            *dst++ = toupper(*src);
        }
    }//for
    *dst = '\0';
}//purify

/******************************************************************************
* @brief Проверка корректности времени
* @param time - строка времени
* @return true если валидно
******************************************************************************/
bool is_time_valid(const char *time) {
    // ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    int i = 0;
    if (time[2] != ':') return false;
    for (i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (time[i] < '0' || time[i] > '9') return false;
    }//for
    int hours = (time[0]-'0')*10 + (time[1]-'0');
    int mins = (time[3]-'0')*10 + (time[4]-'0');
    return (hours >= 0 && hours <=23) && (mins >=0 && mins <=59);
}//is_time_valid

/******************************************************************************
* @brief Проверка бортового номера
* @param bort - строка номера
* @return true если валидно
******************************************************************************/
bool is_bort_valid(const char *bort) {
    // ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    int i = 0;
    if (bort[1] != '-') return false;
    if (bort[0] < 'A' || bort[0] > 'Z') return false;
    for (i = 2; i < 6; i++) {
        if (bort[i] < '0' || bort[i] > '9') return false;
    }//for
    return true;
}//is_bort_valid

/******************************************************************************
* @brief Проверка аэродрома
* @param airport - код аэродрома
* @return true если валидно
******************************************************************************/
bool is_airport_valid(const char *airport) {
    const char *valid[] = {"AP1", "AP2", "AP3"};
    for (int i = 0; i < 3; i++) {
        bool match = true;
        for (int j = 0; j < 3; j++) {
            if (airport[j] != valid[i][j]) match = false;
        }//for j
        if (match) return true;
    }//for i
    return false;
}//is_airport_valid

/******************************************************************************
* @brief Сортировка индексов методом пузырька
* @param indices - массив индексов
* @param size - размер массива
* @param data - массив данных
******************************************************************************/
void bubble_sort(int *indices, int size, Plane *data) {
    // ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    int i = 0;
    int j = 0;
    int temp = 0;
    for (i = 0; i < size-1; i++) {
        for (j = 0; j < size-i-1; j++) {
            if (data[indices[j]].minutes < data[indices[j+1]].minutes) {
                temp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = temp;
            }//if
        }//for j
    }//for i
}//bubble_sort

/******************************************************************************
* @brief Вывод таблицы
* @param indices - отсортированные индексы
* @param size - количество записей
* @param data - массив данных
* @param airport - код аэродрома
******************************************************************************/
void print_table(int *indices, int size, Plane *data, const char *airport) {
    // ЗАГОЛОВОК
    cout << "\nAirport " << airport << ":\n";
    cout << "┌────────────┬───────────────┬────────────────┬────────────────┐\n";
    cout << "│ Time       │ Model         │ Bort Number    │ Airport        │\n";
    cout << "├────────────┼───────────────┼────────────────┼────────────────┤\n";
    
    // ДАННЫЕ
    for (int i = 0; i < size; i++) {
        const Plane &p = data[indices[i]];
        cout << "│ " << left << setw(11) << p.time << "│ "
             << setw(14) << p.model << "│ "
             << setw(15) << p.bort << "│ "
             << setw(15) << p.airport << "│\n";
    }//for
    
    cout << "└────────────┴───────────────┴────────────────┴────────────────┘\n\n";
}//print_table

/******************************************************************************
* @brief Обработка данных для аэродрома
* @param planes - массив данных
* @param count - количество записей
* @param airport - код аэродрома
******************************************************************************/
void process_airport(Plane *planes, int count, const char *airport) {
    // ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    int indices[MAX_PLANES];
    int size = 0;
    int i = 0;
    
    // СОБИРАЕМ ИНДЕКСЫ
    for (i = 0; i < count; i++) {
        bool match = true;
        for (int j = 0; j < 3; j++) {
            if (planes[i].airport[j] != airport[j]) match = false;
        }//for j
        if (match) indices[size++] = i;
    }//for i
    
    // СОРТИРОВКА И ВЫВОД
    if (size == 0) {
        cout << "Airport " << airport << ": no landings\n";
        return;
    }//if
    
    bubble_sort(indices, size, planes);
    print_table(indices, size, planes, airport);
}//process_airport