/***************************************************************************************
*                                   Курс Информатика                                   *
****************************************************************************************
* Project type  : Win64 Console Application                                            *
* Project name  : Lab_1                                                                *
* File name     : Lab_1.cpp                                                            *
* Language      : CPP                                                                  *
* Programmers   : Шалаев Александр Максимович                *
* Modified By   :                                                                      *
* Created       : 28.02.2025                                                           *
* Last Revision : 28.02.2025                                                           *
* Comment       : Вывод слов из предложения                                            *
***************************************************************************************/

#include <iostream>
using namespace std;

int main(){
    setlocale (LC_ALL, "RU");

    char sentense[] = "Hello world!    From 304  ";
    
    int bw, ew;
    int i = 0;

    cout << "TASK 1\n";

    // цикл до конца строки
    while (sentense[i] != '\0'){
        // цикл пропуска пробелов
        while (sentense[i] == ' ' and sentense[i] != '\0'){ i ++;}
        bw = i; // начало слова

        // цикл определения конца слова
        while (sentense[i] != ' ' and sentense[i] != '\0'){ i ++;}
        ew = i-1;  // конец слова

        // вывод слова
        while (bw <= ew){
            cout << sentense[bw];
            bw ++; 
        }
        cout << "\n";
    }

    cout << "TASK 2\n";
    bw = 0;
    ew = 0;
    i = 0;
    int bw_max = 0; // начало слова с большей длиной
    int max_len = 0; // макс длина

    // цикл до конца строки
    while (sentense[i] != '\0'){
        // цикл пропуска пробелов
        while (sentense[i] == ' ' and sentense[i] != '\0'){ i ++;}
        bw = i; // начало слова

        // цикл определения конца слова
        while (sentense[i] != ' ' and sentense[i] != '\0'){ i ++;}
        ew = i-1;  // конец слова

        // сравнение и перезапись длины слова
        if (ew - bw > max_len){
            max_len = ew - bw;
            bw_max = bw;
        }

        // вывод слова
        while (bw <= ew){
            cout << sentense[bw];
            bw ++; 
        }
        cout << "\n";
    }

    // вывод слова с макс длиной
    i = bw_max;
    cout << "Word with max len: ";
    while (sentense[i] != ' ' and sentense[i] != '\0'){
        cout << sentense[i];
        i ++;
    }
    cout << "\n";

    cout << "\nEND OF PROGRAMM\n";

    return 0;
}