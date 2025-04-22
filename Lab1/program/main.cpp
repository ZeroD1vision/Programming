/***************************************************************************************
*                                   Курс Информатика                                   *
****************************************************************************************
* Project type  : Linux Console Application                                            *
* Project name  : Lab_1                                                                *
* File name     : Lab_1.cpp                                                            *
* Language      : CPP                                                                  *
* Programmers   : Шалаев Александр Максимович, Нарзиев Артемий Тимурович               *
* Modified By   :                                                                      *
* Created       : 02.03.2025                                                           *
* Last Revision : 02.03.2025                                                           *
* Comment       : Символьные данные. Вариант: 6                                        *
***************************************************************************************/

#include "lib.h"// Убрал либ/
#include <iostream>
#include <fstream>

using namespace std;

const char *FILE_NAME = "test.txt";//Убрал етстс/

const int MAX_LINE_LEN = 50; // максимальная длина строки входных данных

int main() {
    // ---- ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ ---- //
    char line[50]; // строка
    int i = 0;     // счётчик

    // ----- ЧТЕНИЕ ДАННЫХ ИЗ ФАЙЛА ----- //
    cout << "Reading values from file..." << endl;
    
    // объявление файловой переменной и открытие файла для чтения
    ifstream file(FILE_NAME);

    // проверка наличия файла
    if (!file.is_open()){
        cout << "File \"" << FILE_NAME << "\" dosen`t exist\n";
        file.close();
        getchar(); // Пауза
        return 0;
    } // if

    // проверка на наличие данных в файле
    if (file.eof()){
        cout << "File \"" << FILE_NAME << "\" is empty\n";
        file.close();
        getchar();
        return 1;
    } // if

    // запись данных в строку;
    file.getline(line, MAX_LINE_LEN, '\n');

    cout << "Original line              |\"" << line << "\"\n";

    space_deleter(&line[0]);

    cout << "Line after deleting spaces |\"" << line << "\"\n";

    swap_longest_words(line);

    cout << "Line after swapping letters in the longest words |\"" << line << "\"\n";

    return 0;
}