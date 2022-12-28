#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include <CL/cl.hpp>

#define PAR     5
#define BASE    1024
#define GAIN    200

#define FS      360
#define SL      FS*275/1000     // 99
#define SR      FS*375/1000  // 135
#define PLENGTH 80
//#define DLENGTH SL+SR   // 234
#define DLENGTH 234

#define THR_UPD 0.9  
#define THR_SEP 0.7 

#define TS  15       // correction width

#define NN      20  // # of nodes for dynamic programming
#define N_BIT   32  // maximum distance of nodes

#define INF     99999

#ifndef max
#define max(a,b)    ( ((a)>(b)) ? (a) : (b) )
#endif

#ifndef min
#define min(a,b)    ( ((a)<(b)) ? (a) : (b) )
#endif

#define OCL_CHECK(error,call)                               \
    call;                                                   \
    if( error != CL_SUCCESS ) {                             \
        cout << __FILE__ << ":" << __LINE__;                \
        cout << " Error calling " << #call;                 \
        cout << ", error code is: " << error << endl;       \
        exit(EXIT_FAILURE);                                 \
    }
