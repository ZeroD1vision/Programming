#include <iostream>
using namespace std;

int main() {
    char str[] = "apple banana orange berry avocado";
    char target = 'a';
    int i = 0;

    cout << "Words starting with '" << target << "':" << endl;

    while (str[i] != '\0') {
        // Пропуск пробелов
        while (str[i] == ' ') i++;
        if (str[i] == '\0') break;

        int start = i;  // Начало слова
        
        // Поиск конца слова
        while (str[i] != ' ' && str[i] != '\0') i++;
        int end = i - 1;

        // Проверка первой буквы
        if (str[start] == target) {
            for (int j = start; j <= end; j++) {
                cout << str[j];
            }
            cout << endl;
        }
    }

    return 0;
}