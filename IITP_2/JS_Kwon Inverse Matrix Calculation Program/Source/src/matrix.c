#include "matrix.h"

void Transpose(float** matrix, float** t_matrix, int row, int col)
{

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            t_matrix[j][i] = matrix[i][j];
        }
    }

    return;
}

float** MatrixMult(float** matrix_1, int row_1, int col_1, float** matrix_2, int row_2, int col_2)
{
    int k;
    float** matrix_product = genMatrix(row_1, col_2);

    for (int i = 0; i < row_1; i++)
    {
        for (int j = 0; j < col_2; j++)
        { // not j<M
            matrix_product[i][j] = 0;
            for (k = 0; k < col_1; k++)
            {
                matrix_product[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }
        }
    }
    return matrix_product;
}

/*--------------------------------------------------------------------------*/
/* Display matrix                                                           */
/*--------------------------------------------------------------------------*/
void display(float** A, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            printf("%15f ", A[i][j]);
        printf("\n");
    }
}

float** genMatrix(int row, int col)
{
    float** mat;
    mat = (float **)malloc(row * col * sizeof(float*));
    if (mat == NULL) return (NULL);

    for (int i = 0; i < row; i++)
    {
        mat[i] = (float *)malloc(col * sizeof(float));
        if (mat[i] == NULL) return (NULL);
    }

    return mat;
}


/*--------------------------------------------------------------------------*/
/* Both matrix must have same row, col value                                */
/*--------------------------------------------------------------------------*/
void copyMatrix(float** from, float** to, int row, int col)
{
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            to[i][j] = from[i][j];
        }
    }
}

/*--------------------------------------------------------------------------*/
/* Free Matrix                                                              */
/* param @                                                                  */
/* row = target matrix num. of row                                          */
/*--------------------------------------------------------------------------*/
void delMatrix(float** A, int row)
{
    for(int i = 0; i < row; i++)
    {
        free(A[i]);
    }
    free(A);
}

void MatrixDivConst(float** A, int row, int col, float c)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            A[i][j] /= c;
    }
}