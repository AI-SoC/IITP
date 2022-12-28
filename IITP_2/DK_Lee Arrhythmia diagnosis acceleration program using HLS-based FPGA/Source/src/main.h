#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

//#define DEBUG
//#define ONE

#define BASE    1024
#define GAIN    200

#define Fs      360
#define Sl      Fs*275/1000  // 99
#define Sr      Fs*375/1000  // 135
#define PLength 80
#define Fl      1
#define Fh      25

#define PI      3.1415927

#define THR_UPD 0.9 
#define THR_SEP 0.7

#define TS      7   // peak correction

#define N       20  // # of nodes for dynamic programming
#define N_BIT   32  // maximum distance of nodes

#define INF     99999

#ifndef max
#define max(a,b)    ( ((a)>(b)) ? (a) : (b) )
#endif

#ifndef min
#define min(a,b)    ( ((a)<(b)) ? (a) : (b) )
#endif

