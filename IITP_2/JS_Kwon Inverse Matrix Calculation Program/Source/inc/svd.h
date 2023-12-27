#ifndef __SVD_H__
#define __SVD_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gauss_jordan.h"
#include "matrix.h"

// #define DEBUG

float** genSingularValueMat(int row, int col, float** eigenMat);
float** genSingularValueMatPinv(float** singularValueMatrix, int row, int col);
float** genSingularVectorMat_V(float** A, float** eigValueMatrix, int col);

#endif