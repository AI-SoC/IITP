#ifndef __GAUSS_JORDAN_H__
#define __GAUSS_JORDAN_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"

float gauss_jordan(float** coef_const, int num_var, float* result);

#endif