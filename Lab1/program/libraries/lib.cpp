#include "lib.h"
#include <iostream>
#include <cstring>

// УДАЛЕНИЕ ЛИШНИХ ПРОБЕЛОВ В СТРОКЕ
void space_deleter(char* cell){
    char* _cell = cell; // указатель на 1-ый символ строки

    // цикл прохода по всей строке
    while (*_cell != '\0'){
        // цикл прохода пробелов
        while (*_cell == ' ' and *(_cell + 1) == ' '){
            // буфер для указателя
            char* i = _cell;
            // сдвиг символов на 1 назад
            while (*i != '\0'){
                *i = *(i + 1);
                i ++;
            }

            /*
            while (*A != '\0') {std::cout << *A; A++;}
            std::cout << "\n";
            A = B;*/
        }
        _cell ++;

        // цикл пропуска букв
        while (*_cell != ' ' and *_cell != '\0'){ _cell++;}
    }

    // если строка начинается с пробела, убираем её
    if (*cell = ' '){            // исправь ==
        while (*cell != '\0'){
            *cell = *(cell + 1);
            cell ++;
        }
    }
}

// СМЕНА ПЕРВЫХ И ПОСЛЕДНИХ БУКВ В САМЫХ ДЛИННЫХ СЛОВАХ СТРОКИ
void swap_longest_words(char *cell){
    
    // если строка пустая или меньше 2 символов, ничего не делаем
    if (cell == nullptr || strlen(cell) < 2) {
        return;
    }

    // инициализация
    char* _cell = cell; // указатель по символам строки
    int max_length = 0; // длина самого длинного слова
    char* longest_word_start = nullptr; // указатель на начало самого длинного слова
    int count = 0; // количество самых длинных слов
    char* longest_words[10]; // массив указателей на самые длинные слова
    
    
    // цикл прохода по всей строке
    while (*_cell!= '\0'){
        // цикл пропуска пробелов
        while (*_cell == ' ') {
            _cell++;
        }

        char* start = _cell;

        // цикл прохода букв
        while(*_cell != ' ' && *_cell != '\0'){
            _cell++;
        }

        int current_length = _cell - start;// длина текущего слова

        // если текущее слово длиннее предыдущего, обновляем массив
        if(current_length > max_length){
            max_length = current_length;
            count = 0; // сбрасываем счетчик
            longest_words[count++] = start; // добавляем новое самое длинное слово
        } else if (current_length == max_length) {
            longest_words[count++] = start; // добавляем еще одно самое длинное слово
        }
    }

    // меняем первую и последнюю буквы
    for(int i = 0; i < count; i++){
        if(max_length > 1){
            char* word_start = longest_words[i];
            char temp = *word_start;
            *word_start = *(word_start + max_length - 1);
            *(word_start + max_length - 1) = temp;
        }
    }
    /*    
    if(longest_word_start != nullptr && max_length > 1){
        char temp = *longest_word_start;
        *longest_word_start = *(_cell - 1);
        *(_cell - 1) = temp;
    }
    */
}