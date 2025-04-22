#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
// Для локали на русском
#include <locale>
#include <codecvt>
#include <windows.h>

using namespace std;

// Структура для хранения информации о самолете
struct Plane {
    string time;         // Время посадки
    int minutes;        // Время в минутах
    string model;       // Модель летательного аппарата
    string bortNumber;  // Бортовой номер
    string airport;     // Аэродром посадки
};

const int MAX_PLANES = 100; // Максимальное количество самолетов
Plane planes[MAX_PLANES];    // Массив для хранения самолетов
int planeCount = 0;          // Счетчик самолетов

// Функция сортировки пузырьком для сортировки индексов самолетов по времени посадки
void bubbleSort(int indices[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) { // Исправлено: было 'i++'
            if (planes[indices[j]].minutes < planes[indices[j + 1]].minutes) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
}

int main() {
    // Установка кодировки консоли для корректного отображения символов
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.UTF-8");

    // Открытие файла с данными о посадках и проверка на ошибки
    ifstream file("landings.txt");
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла!";
        return 1; // Завершение программы при ошибке открытия файла
    }

    // Чтение данных из файла в массив
    string line;
    while (getline(file, line) && planeCount < MAX_PLANES) {
        // Обработка строки (без использования stringstream)
        // Реализация разбиения строки на части через поиск запятых
        size_t pos1 = line.find(','); // Найти первую запятую
        size_t pos2 = line.find(',', pos1 + 1); // Найти вторую запятую
        size_t pos3 = line.find(',', pos2 + 1); // Найти третью запятую

        // Проверка на корректность формата строки
        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) {
            cerr << "Некорректная строка: " << line << endl; // Вывод ошибки
            continue; // Переход к следующей строке
        }

        // Заполнение структуры данными из строки
        planes[planeCount].time = line.substr(0, pos1); // Время
        planes[planeCount].model = line.substr(pos1 + 2, pos2 - pos1 - 2); // Модель (с учетом пробела после запятой)
        planes[planeCount].bortNumber = line.substr(pos2 + 2, pos3 - pos2 - 2); // Бортовой номер (с учетом пробела после запятой)
        planes[planeCount].airport = line.substr(pos3 + 2); // Аэродром (с учетом пробела после запятой)

        // Конвертация времени в минуты
        planes[planeCount].minutes = 
            ((planes[planeCount].time[0] - '0') * 10 + (planes[planeCount].time[1] - '0')) * 60 +
            ((planes[planeCount].time[3] - '0') * 10 + (planes[planeCount].time[4] - '0'));
        
        planeCount++; // Увеличение счетчика самолетов
    }
    file.close(); // Закрытие файла после чтения

    // Обработка аэродромов
    const string airports[] = {"АП1", "АП2", "АП3"}; // Список аэродромов
    const size_t airportsCount = sizeof(airports) / sizeof(airports[0]); // Вычисляем количество аэропортов
    
    // Цикл по каждому аэродрому
    for (size_t j = 0; j < airportsCount; j++) {
        const string& ap = airports[j]; // Получаем текущий аэропорт
        int indices[MAX_PLANES];
        int count = 0;
        
        // Сбор индексов
        for (int i = 0; i < planeCount; i++) {
            if (planes[i].airport == ap) {
                indices[count++] = i;
            }
        }
        
        // Сортировка и вывод
        if (count == 0) {
            cout << "Аэродром " << ap << ": посадок нет\n";
            continue;
        }
        
        bubbleSort(indices, count);
        // Вывод таблицы
        cout << "Аэродром " << ap << ":\n"; // Вывод на русском
        cout << "Время   | Модель      | Бортовой | Аэродром\n"; // Вывод на русском
        cout << "------------------------------------------\n";
        for (int i = 0; i < count; i++) {
            const Plane& p = planes[indices[i]];
            cout << p.time << " | " << p.model 
                 << "\t| " << p.bortNumber << "\t| " << p.airport << endl;
        }
        cout << endl;
    }
    
    return 0;
}
 
