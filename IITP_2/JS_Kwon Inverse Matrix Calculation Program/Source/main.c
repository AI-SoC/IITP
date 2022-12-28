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
    /*  MATRIX COL LIMIT = 4 !!!!                                               */
    /*--------------------------------------------------------------------------*/
    float** A;
    float** A_T;
    float** A_T_mul_A;
    float** singularValueMatrix;
    float** singularValueMatrix_pinv;
    float** singularVectorMatrix_V;
    float** eigValueMatrix;

    float** singularValueMatrix_pinv_T;
    float** singularVectorMatrix_V_T;

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

    /*--------------------------------------------------------------------------*/
    /* Eigenvalue calculation                                                   */
    /*--------------------------------------------------------------------------*/

    Nrun = 31;
    tol = 1.0E-22;
    copyMatrix(A_T_mul_A, eigValueMatrix, col, col);
    Givens(col, eigValueMatrix, Nrun, tol);  // row, col don't care (eigenvalue always derive from square matrix)

#ifdef DEBUG
    printf("Eigenvalues: \n");
    for (i = 0; i < col; i++)
        printf("%.4f ", eigValueMatrix[i][i]);
    printf("\n\n");
#endif

    /*--------------------------------------------------------------------------*/
    /* Singular value matrix (D)                                                */
    /*--------------------------------------------------------------------------*/

    /*
    singular value = {a, b, c, d, e, f} --> number = col

    row X col
    sigma = [a     ]
            [ b    ]
            [  c   ]
            [   d  ]
            [    e ]
            [     f]
            [     0]
            [     0]
            ...
    */   
    singularValueMatrix = genSingularValueMat(row, col, eigValueMatrix);

#ifdef DEBUG
    printf("Singular Value Matrix (D): \n");
    display(singularValueMatrix, row, col);
    printf("\n\n");  
#endif

    /*--------------------------------------------------------------------------*/
    /* Singular value matrix pseudo inverse (D+)                                */
    /*--------------------------------------------------------------------------*/
    singularValueMatrix_pinv = genSingularValueMatPinv(singularValueMatrix, row, col);

#ifdef DEBUG
    printf("Singular Value Matrix Pinv (D+): \n");
    display(singularValueMatrix_pinv, col, row);
    printf("\n\n");  
#endif


    /*--------------------------------------------------------------------------*/
    /* Singular vector matrix (V)                                               */
    /*--------------------------------------------------------------------------*/
    singularVectorMatrix_V = genSingularVectorMat_V(A_T_mul_A, eigValueMatrix, col);

#ifdef DEBUG
    printf("Singular Vector Matrix (V): \n");
    display(singularVectorMatrix_V, col, col);
    printf("\n\n");  
#endif

    /*--------------------------------------------------------------------------*/
    /* Get pseudo inverse matrix of A --> A+                                    */
    /*                                                                          */
    /* A          = U D V^T                                                     */
    /* A V D+     = U                                                           */
    /* (A V D+)^T = U^T                                                         */
    /*                                                                          */
    /* A+ = V D+ U^T                                                            */
    /*    = V D+ (A V D+)^T                                                     */
    /*    = V D+ (D+)^T V^T A^T                                                 */
    /*--------------------------------------------------------------------------*/
    singularValueMatrix_pinv_T = genMatrix(row, col);
    singularVectorMatrix_V_T = genMatrix(col, col);

    Transpose(singularValueMatrix_pinv, singularValueMatrix_pinv_T, col, row);
    Transpose(singularVectorMatrix_V, singularVectorMatrix_V_T, col, col);

#ifdef DEBUG
    printf("Singular Value Matrix Pinv Transpose (D+)^T: \n");
    display(singularValueMatrix_pinv_T, row, col);
    printf("\n\n");  
    printf("Singular Vector Matrix Transpose (V)^T: \n");
    display(singularVectorMatrix_V_T, col, col);
    printf("\n\n");  
#endif

    float** temp1;
    float** A_pinv;
    temp1 = MatrixMult(singularVectorMatrix_V, col, col, singularValueMatrix_pinv, col, row);
#ifdef DEBUG
    printf("V D+: \n");
    display(temp1, col, row);
    printf("\n\n");  
#endif

    A_pinv = MatrixMult(temp1, col, row, singularValueMatrix_pinv_T, row, col);
    delMatrix(temp1, col);
#ifdef DEBUG
    printf("V D+ (D+)^T: \n");
    display(A_pinv, col, col);
    printf("\n\n");  
#endif

    temp1 = MatrixMult(A_pinv, col, col, singularVectorMatrix_V_T, col, col);
    delMatrix(A_pinv, col);
#ifdef DEBUG
    printf("V D+ (D+)^T V^T: \n");
    display(temp1, col, col);
    printf("\n\n");  
#endif

    A_pinv = MatrixMult(temp1, col, col, A_T, col, row);
    delMatrix(temp1, col);

    printf("Pseudo Inverse (A+): \n");
    display(A_pinv, col, row);
    printf("\n\n");  

    /*--------------------------------------------------------------------------*/
    /* Clean Up                                                                 */
    /*--------------------------------------------------------------------------*/
    delMatrix(A, row);
    delMatrix(A_T, col);
    delMatrix(A_T_mul_A, col);
    delMatrix(singularValueMatrix, row);
    delMatrix(singularValueMatrix_pinv, col);
    delMatrix(singularVectorMatrix_V, col);
    delMatrix(eigValueMatrix, col);
    delMatrix(singularVectorMatrix_V_T, col);
    delMatrix(singularValueMatrix_pinv_T, row);
    delMatrix(A_pinv, col);

    fclose(fin);

    return 0;
}