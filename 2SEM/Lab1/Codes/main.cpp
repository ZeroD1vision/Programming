/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Linux Console Application                                   *
 * Project name  : Lab_2                                                       *
 * File name     : main.cpp                                                    *
 * Language      : CPP                                                         *
 * Programmers   : Шалаев Александр Максимович, Нарзиев Артемий Тимурович      *
 * Modified By   :                                                             *
 * Created       : 02.04.2025                                                  *
 * Last Revision : 10.04.2025                                                  *
 * Comment       : Двумерные массивы. Вариант: 6                               *
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include "lib/lib.h"

using namespace std;

// ТЕСТЫ
//  ----- НЕКОРРЕКТНЫЕ ----- //

// цель теста: проверить работоспособность программы при отсутствии исходных файлов.
// const char *FILE_NAME_A = "tests/incorrect/A/1.txt";
// const char *FILE_NAME_B = "tests/incorrect/B/1.txt";

// цель теста: проверить работоспособность программы при отсутствии данных в файлах.
// const char *FILE_NAME_A = "tests/incorrect/A/2.txt";
// const char *FILE_NAME_B = "tests/incorrect/B/2.txt";

// цель теста: проверить работоспособность программы при отсутствии файла матрицы А
//             и наличии файла матрицы В, в котором содержится только буква.
// const char *FILE_NAME_A = "tests/incorrect/A/3.txt";
// const char *FILE_NAME_B = "tests/incorrect/B/3.txt";

// цель теста: проверить работоспособность программы при отсутствии файла матрицы B
//             и наличии файла матрицы А, в котором стоит отрицательный элемент матрицы
// const char *FILE_NAME_A = "tests/incorrect/A/4.txt";
// const char *FILE_NAME_B = "tests/incorrect/B/4.txt";

// цель теста: проверить работоспособность программы при отсутствии данных в файле
//             матрицы А и наличии файла матрицы В, в котором стоит размер матрицы
//             превышает максимальный(10)
// const char *FILE_NAME_A = "tests/incorrect/A/5.txt";
// const char *FILE_NAME_B = "tests/incorrect/B/5.txt";

// цель теста: проверить работоспособность программы при нехватке данных для матрицы А
//             и наличии буквы вместо одного из элементов при корректном размере в матрице В
// const char *FILE_NAME_A = "tests/incorrect/A/6.txt";
// const char *FILE_NAME_B = "tests/incorrect/B/6.txt";

// цель теста: проверить работоспособность программы при отсутствии данных для матрицы B
//             при её корректном размере и наличии двух минусов перед одним из элементов
//             при корректном размере в матрице A
// const char *FILE_NAME_A = "tests/incorrect/A/7.txt";
// const char *FILE_NAME_B = "tests/incorrect/B/7.txt";

// ----- КОРРЕКТНЫЕ ----- //

// цель теста:  проверить работоспособность программы при абсолютно корректных данных
//              для матриц А и В
// const char *FILE_NAME_A = "tests/correct/A/1.txt";
// const char *FILE_NAME_B = "tests/correct/B/1.txt";

// цель теста:  проверить работоспособность программы при наличии избыточных корректных
//              данных для матрицы А и при наличии избыточных некорректных данных для матрицы В
// const char *FILE_NAME_A = "tests/correct/A/2.txt";
// const char *FILE_NAME_B = "tests/correct/B/2.txt";

// цель теста:  проверить работоспособность программы при отсутствии файла с матрицей А
//              и при отсутствии нечётных элементов в матрице В
// const char *FILE_NAME_A = "tests/correct/A/3.txt";
// const char *FILE_NAME_B = "tests/correct/B/3.txt";

// цель теста:  проверить работоспособность программы при отсутствии отрицательных элементов
//              над главной диагональю матрицы А (причём матрица содержит отрицательные элементы)
//              и отсутствием данных в файле матрицы В
// const char *FILE_NAME_A = "tests/correct/A/4.txt";
// const char *FILE_NAME_B = "tests/correct/B/4.txt";

// цель теста:  проверить работоспособность программы отрицательном размере матрицы А
//              и при размере матрицы В равном 0
// const char *FILE_NAME_A = "tests/correct/A/5.txt";
// const char *FILE_NAME_B = "tests/correct/B/5.txt";

// цель теста:  проверить работоспособность программы при отсутствии отрицательных элементов
//              и нечетных элементов над главной диагональю в матрице А и наличием слова
//              вместо размера в файле матрицы В
const char *FILE_NAME_A = "tests/correct/A/6.txt";
const char *FILE_NAME_B = "tests/correct/B/6.txt";

// цель теста:  проверить работоспособность программы при наличии и корректных и некорректных
//              избыточных данных в матрице А и наличием буквы вместо одного из элементов матрицы В
// const char *FILE_NAME_A = "tests/correct/A/7.txt";
// const char *FILE_NAME_B = "tests/correct/B/7.txt";

// ф-я вывода ошибки
void print_error(int err, const char *FILE_NAME, int N)
{
    cout << "[31m"; // активируем вывод красного тектса
    // если файл не существует
    if (err == -1)
    {
        cout << "File \"" << FILE_NAME << "\" dosen`t exist\n";
    }
    // если файл пустой
    else if (err == -2)
    {
        cout << "File \"" << FILE_NAME << "\" is empty\n";
    }
    // если произошла ошибка при чтении размера матрицы из файла
    else if (err == -3)
    {
        cout << "Failed reading size of matrix in file \""
             << FILE_NAME << "\" (there can be a letter)\n";
    }
    // если считанный размер матрицы < 0
    else if (err == -4)
    {
        cout << "Size of matrix in file \""
             << FILE_NAME << "\" < 0\n";
    }
    // если считанный размер матрицы больше зарезервированного
    else if (err == -5)
    {
        cout << "Size of matrix in file \""
             << FILE_NAME << "\" is bigger then we expected\n";
    }
    // если произошла ошибка при считывании элементов матрицы
    else if (err == -6)
    {
        cout << "Failed reading matrix in file \""
             << FILE_NAME << "\" (there can be a letter)\n";
    }
    // если в файле есть лишняя информация
    else if (err == -7)
    {
        cout << "There is too much information in file \""
             << FILE_NAME << "\".\nSome of it won`t be used\n";
    }
    // если в файле не достаточно информации
    else if (err > 0)
    {
        cout << "Size of matrix in file \"" << FILE_NAME
             << "\" = " << N << ".\nIt must include " << N * N
             << " values, but there only " << err - 1 << "\n";
    }
    cout << "[0m\n"; // выключаем вывод красного текста
}

int main()
{
    // ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    const int N_MAX = 10; // максимальный размер матрицы
    int N_A = 0;          // реальный размер матрицы
    int N_B = 0;
    int matrix_A[N_MAX][N_MAX] = {0}; // матрица, в которой будет храниться считываемая
    int matrix_B[N_MAX][N_MAX] = {0};
    int err;   // код ошибки или буфер для результата
    int i = 0; // счётчики
    int j = 0;

    // СЧИТЫВАНИЕ И ВЫВОД МАТРИЦЫ A
    err = reading_matrix(FILE_NAME_A, matrix_A, &N_A);
    // обработка ошибки
    if (err != 0)
    {
        print_error(err, FILE_NAME_A, N_A);
    }
    // если всё в порядке выводим матрицу
    if (err == 0 || err == -7)
    {
        cout << "For matrix_A " << N_A << "x" << N_A << " in file \"" << FILE_NAME_A << "\":\n";
        cout << "[33m"; // активируем вывод жёлтого текста
        for (i = 0; i < N_A; i++)
        { // выводим матрицу
            cout << "\t";
            for (j = 0; j < N_A; j++)
            {
                cout << matrix_A[i][j] << " ";
            } // for j
            cout << "\n";
        } // for i
        cout << "[0m - "; // выключаем вывод жёлтого текста

        // вычисляем произведение по заданию
        err = multiplication_neg_el(matrix_A, N_A);
        if (err == 0)
        {
            cout << "There is no negative elevents over main diagonal";
        }
        else
        {
            cout << "Result of multiplication negative elevents over main diagonal = [33m" << err;
        }
        cout << "[0m\n - ";

        // вычисляем мин. нечётный элемент
        err = min_odd_el(matrix_A, N_A);
        if (err == 0)
        {
            cout << "There is no odd elevents in this matrix_A";
        }
        else
        {
            cout << "Min odd element = [33m" << err;
        }
        cout << "[0m\n";
    } // if (err == 0 || err == -7)

    cout << "\n";

    // СЧИТЫВАНИЕ И ВЫВОД МАТРИЦЫ B
    err = reading_matrix(FILE_NAME_B, matrix_B, &N_B);
    // обработка ошибки
    if (err != 0)
    {
        print_error(err, FILE_NAME_B, N_B);
    }
    // если всё в порядке выводим матрицу
    if (err == 0 || err == -7)
    {
        cout << "For matrix_B " << N_B << "x" << N_B << " in file \"" << FILE_NAME_B << "\":\n";
        cout << "[33m"; // активируем вывод жёлтого текста
        for (i = 0; i < N_B; i++)
        { // выводим матрицу
            cout << "\t";
            for (j = 0; j < N_B; j++)
            {
                cout << matrix_B[i][j] << " ";
            } // for j
            cout << "\n";
        } // for i
        cout << "[0m - "; // выключаем вывод жёлтого текста

        // вычисляем произведение по заданию
        err = multiplication_neg_el(matrix_B, N_B);
        if (err == 0)
        {
            cout << "There is no negative elevents over main diagonal";
        }
        else
        {
            cout << "Result of multiplication negative elevents over main diagonal = [33m" << err;
        }
        cout << "[0m\n - ";

        // вычисляем мин. нечётный элемент
        err = min_odd_el(matrix_B, N_B);
        if (err == 0)
        {
            cout << "There is no odd elevents in this matrix_B";
        }
        else
        {
            cout << "Min odd element = [33m" << err;
        }
        cout << "[0m\n";
    } // if (err == 0 || err == -7)

    cout << "\n\n";
    return 0;
}
/******************************* КОНЕЦ main.cpp ******************************/