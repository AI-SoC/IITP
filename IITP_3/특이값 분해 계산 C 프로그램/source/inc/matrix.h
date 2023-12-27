#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>

void Transpose(float** matrix, float** t_matrix, int row, int col);
float** MatrixMult(float** matrix_1, int row_1, int col_1, float** matrix_2, int row_2, int col_2);
void display(float** A, int row, int col);
float** genMatrix(int row, int col);
void copyMatrix(float** from, float** to, int row, int col);
void delMatrix(float** A, int row);
void MatrixDivConst(float** A, int row, int col, float c);
void bubble_sort(float arr[], int count);

#endif