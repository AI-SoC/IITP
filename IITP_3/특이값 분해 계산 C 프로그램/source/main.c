#include <stdio.h>
#include <stdlib.h>

#include "eigen.h"
#include "matrix.h"
#include "svd.h"
#include "gauss_jordan.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define DEBUG

int main(int argc, char *argv[])
{
    FILE *fin;
    int row, col;
    int i, j, Nrun;
    float t;
    double tol;

    /*--------------------------------------------------------------------------*/
    /*  Matrix declarations                                                     */
    /*  ONLY SUPPORT row >= col CASE !!!!                                       */
    /*--------------------------------------------------------------------------*/
    float** A;
    float** A_T;
    float** A_T_mul_A;
    float** A_mul_A_T;
    float** eigValueMatrix;

    float** leftSingularMatrix_U;   // U
    float** singularValueMatrix;    // S
    float** rightSingularMatrix_V;  // V


    /*--------------------------------------------------------------------------*/
    /*  Read the input symmetric matrix A                                       */
    /*--------------------------------------------------------------------------*/
    fin = fopen(argv[1], "r");
    fscanf(fin, "%d %d", &row, &col);
    A = genMatrix(row, col);
    A_T = genMatrix(col, row);
    eigValueMatrix = genMatrix(col, col);

    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++)
        {
            fscanf(fin, "%f", &t);
            A[i][j] = t;
        }

    /*--------------------------------------------------------------------------*/
    /* Matrix Calculation                                                       */
    /*--------------------------------------------------------------------------*/

    /*
    row X col
    A = [   ]
        [   ]
        [   ]
        [   ]
        [   ]
        [   ]
        */
    printf("A: \n");
    display(A, row, col);
    printf("\n\n");    

    /*
    row X col     col X row
    A = [   ]     A^T = [                   ]
        [   ]           [                   ]
        [   ]           [                   ]
        [   ]
        [   ]
        [   ]
        */    
    Transpose(A, A_T, row, col);
#ifdef DEBUG
    printf("Transposed A: \n");
    display(A_T, col, row);
    printf("\n\n");   
#endif    

    /* A^T * A */
    A_T_mul_A = MatrixMult(A_T, col, row, A, row, col);
#ifdef DEBUG    
    printf("A^T * A: \n");
    display(A_T_mul_A, col, col);
    printf("\n\n");    
#endif

    /* A * A^T */
    A_mul_A_T = MatrixMult(A, row, col, A_T, col, row);
#ifdef DEBUG    
    printf("A * A^T: \n");
    display(A_mul_A_T, col, col);
    printf("\n\n");    
#endif    

    /*--------------------------------------------------------------------------*/
    /* Eigenvalue calculation                                                   */
    /*--------------------------------------------------------------------------*/

    Nrun = 300;     // heuristic parameter (higher value --> higher accuracy of eigen values)
    tol = 1.0E-22;
    copyMatrix(A_T_mul_A, eigValueMatrix, col, col);
    Givens(col, eigValueMatrix, Nrun, tol);  // row, col don't care (eigenvalue always derive from square matrix)

    float* eigValueVector;
    eigValueVector = (float*)malloc(col * sizeof(float));

#ifdef DEBUG
    printf("Eigenvalues: \n");
    for (i = 0; i < col; i++) {
        printf("%.4f \n", eigValueMatrix[i][i]);
        eigValueVector[i] = eigValueMatrix[i][i];
    }
    printf("\n\n");
#endif

    // 내림차순 정렬
    for (i = 0; i < col; i++) {
        eigValueVector[i] = eigValueMatrix[i][i];
    }
    bubble_sort(eigValueVector, col);
    printf("Eigenvalues (descend sorted): \n");
    for (i = 0; i < col; i++) {
        printf("%.4f \n", eigValueVector[i]);
    }
    printf("\n\n");    


    /*--------------------------------------------------------------------------*/
    /* Singular value matrix (S)                                                */
    /*--------------------------------------------------------------------------*/

    /*
    singular value = sqrt(eigen value)
    if singular value == {a, b, c, d, e, f} --> number = col

    row X col
    S  =    [a     ]
            [ b    ]
            [  c   ]
            [   d  ]
            [    e ]
            [     f]
            [     0]
            [     0]
            ...
    */   
    singularValueMatrix = genSingularValueMat(row, col, eigValueVector);

// #ifdef DEBUG
    printf("Singular Value Matrix (S): \n");
    display(singularValueMatrix, row, col);
    printf("\n\n");  
// #endif

    /*--------------------------------------------------------------------------*/
    /* Singular vector matrix (V)                                               */
    /*--------------------------------------------------------------------------*/
    rightSingularMatrix_V = genSingularVectorMat_V(A_T_mul_A, eigValueVector, col);

// #ifdef DEBUG
    printf("Right Singular Vector Matrix (V): \n");
    display(rightSingularMatrix_V, col, col);
    printf("\n\n");  
// #endif


    /*--------------------------------------------------------------------------*/
    /* Clean Up                                                                 */
    /*--------------------------------------------------------------------------*/
    delMatrix(A, row);
    delMatrix(A_T, col);
    delMatrix(A_T_mul_A, col);
    delMatrix(A_mul_A_T, col);
    delMatrix(eigValueMatrix, col);
    delMatrix(singularValueMatrix, row);
    delMatrix(rightSingularMatrix_V, col);

    free(eigValueVector);

    fclose(fin);

    return 0;
}