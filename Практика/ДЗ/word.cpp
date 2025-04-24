#include <iostream>
// Для локали на русском
#include <locale>
#include <codecvt>
#include <windows.h>

using namespace std;

int main() {
    // Установка кодировки консоли для корректного отображения символов
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.UTF-8");

    char str[50] = "Hello world dva! Bozhe, chem ya zanyat?";
    cout << "Было: " << str << endl;

    // 1. Находим где кончается строка
    int end = 0;
    while (str[end] != '\0') {
        end++;
    }

    // 2. Ищем слово "dva"
    int i = 0;
    while (i < end - 2) {
        if (str[i] == 'd' && str[i+1] == 'v' && str[i+2] == 'a') {
            // 3. Сдвигаем хвост строки на 1 символ (dva → pyat)
            int j = end + 1; // Новый конец строки
            while (j >= i + 3) {
                str[j] = str[j - 1];
                j--;
            }

            // 4. Вставляем "pyat"
            str[i]   = 'p';
            str[i+1] = 'y';
            str[i+2] = 'a';
            str[i+3] = 't';
            break;
        }
        i++;
    }

    cout << "Стало: " << str << endl;
    return 0;
}
/* Меняем dva на pyat 
Переносим конец на 3: 
Идем с окнца строки до конца слова и сразу сдвигаем на 3 */