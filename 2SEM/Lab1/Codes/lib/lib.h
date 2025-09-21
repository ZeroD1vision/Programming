/******************************************************************************
 *                               Курс Информатика                              *
 *******************************************************************************
 * Project type  : Linux Console Application                                   *
 * Project name  : Lab_2                                                       *
 * File name     : lib.h                                                       *
 * Language      : CPP                                                         *
 * Programmers   : Шалаев Александр Максимович, Нарзиев Артемий Тимурович      *
 * Modified By   :                                                             *
 * Created       : 02.04.2025                                                  *
 * Last Revision : 10.04.2025                                                  *
 * Comment       : Двумерные массивы. Вариант: 6                               *
 ******************************************************************************/
#ifndef LIB_H
#define LIB_H

#include <fstream>

// ######################## ДЛЯ СТАТИЧЕСКИХ МАССИВОВ ####################### //

const int N_MAX = 10; // максимальный размер матрицы

// ф-я считывания матрицы из файла
int reading_matrix(const char *FILE_NAME, int matrix[][N_MAX], int *N);
// ф-я вычисляющая произведение отрицательных эл. над главной диагональю
int multiplication_neg_el(int matrix[][N_MAX], int N);
// ф-я для нахождения минимума среди нечётных эл. матрицы
int min_odd_el(int matrix[][N_MAX], int N);

// ####################### ДЛЯ ДИНАМИЧЕСКИХ МАССИВОВ ####################### //

// ф-я считывания размера матрицы из файла
int reading_size_of_matrix(const char *FILE_NAME);
// ф-я считывания динамической матрицы из файла
int reading_dinamic_matrix(const char *FILE_NAME, int **matrix, int N);
// ф-я вычисляющая произведение отрицательных эл. над главной диагональю
int dinamic_multiplication_neg_el(int **matrix, int N);
// ф-я для нахождения минимума среди нечётных эл. матрицы
int dinamic_min_odd_el(int **matrix, int N);

#endif // LIB_H
       /******************************** КОНЕЦ lib.h ********************************/