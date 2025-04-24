#include <iostream>

using namespace std;

int main(){
    const int N_MAX = 50;
    int i = 0, j = 0;
    char inputString[N_MAX] = {0};

    cin.getline(inputString, sizeof(inputString));
    
    while (inputString[i] != '\0') { // Пропуск проблелов
        while (inputString[i] == ' ') {
            inputString[j++] = inputString[i++];
        }

        int wordStart = j; // Запоминаем начало слова

        while (inputString[i] != ' ' && inputString[i] != '\0') {
            inputString[j++] = inputString[i++];
        }

        // Удаляем два последних символа, если слово длиннее 2 символов
        if (j - wordStart > 2) {
            j -= 2; // Уменьшаем индекс записи на 2
        } else {
            j = wordStart; // Если слово слишком короткое, просто сбрасываем j к началу слова
        }
    }


    inputString[j] = '\0';
    cout << inputString << endl;

    return 0;
}