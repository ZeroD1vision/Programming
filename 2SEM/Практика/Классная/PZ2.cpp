#include <iostream>

using namespace std;

int main(){
    // Удаление всего что между двумя символами
    int p1a = -1, p2a = -1, i = 0, j = 0;
    int len = 0;
    const int N_MAX = 50; 
    char str[N_MAX] = "Hello world dva!";
    cout << "Original string: " << str << endl;
    // Находим длину строки
    while (str[len] != '\0') len++;
    
    while((str[i] != '\0') && (str[i] != 'o')) {
        i++;
    }
    
    int p1a = i;
    int j = len - 1;
    
    while((str[j] != 'o') && (j >= 0)){
        j--;
    }
    
    int p2a = j;
    i = 1;
    
    /*
    // Первый способ
    while(str[p2a + i] != '\0'){
        str[p1a + i] = str[p2a + i];
        i++;
    }
    str[p1a + i] = '\0';
    */

    // Второй способ
    i = p2a + 1;
    j = p1a + 1;

    while(str[i] != '\0'){
        str[j] = str[i];
        j++;
        i++;
    }

    str[j] = '\0';

    cout << "First time: " << p1a << endl;
    cout << "Last time: " << p2a << endl;
    cout << str << endl;


    /* Меняем dva на pyat 
    Переносим конец на 3: 
    Идем с окнца строки до конца слова и сразу сдвигаем на 3 

    Дома написать
    */
    
    return 0;
}