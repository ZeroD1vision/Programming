#include <iostream>

using namespace std;

int main() {
    const int N_MAX = 50;
    char inputString[N_MAX] = {0};

    // Ввод строки
    cin.getline(inputString, sizeof(inputString));

    int i = 0; // Индекс чтения
    int j = 0; // Индекс записи

    while (inputString[i] != '\0') {
        // Пропускаем пробелы
        while (inputString[i] == ' ') {
            i++;
        }

        if (inputString[i] == '\0') break; // Если достигнут конец строки, выходим

        int wordStart = i; // Запоминаем начало слова

        // Ищем конец слова
        while (inputString[i] != ' ' && inputString[i] != '\0') {
            i++;
        }
        int wordEnd = i - 1; // Индекс последнего символа слова

        // Проверяем длину слова
        int wordLength = wordEnd - wordStart + 1; // Длина слова

        // Удаляем два последних символа, если слово длиннее 2 символов
        int copyLength = (wordLength > 2) ? (wordLength - 2) : wordLength;
        for (int k = 0; k < copyLength; k++) {
            inputString[j++] = inputString[wordStart + k];
        }

        // Добавляем пробел, если не достигнут конец строки
        if (inputString[i] != '\0') {
            inputString[j++] = ' ';
        }
    }

    // Завершаем строку нулевым символом
    inputString[j] = '\0';

    // Выводим результат
    cout << inputString << endl;

    return 0;
}
