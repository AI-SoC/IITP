/*
 * logo.h
 *
 *  Created on: 2022. 9. 27.
 *      Author: JSKWON
 */

#ifndef LOGO_H_
#define LOGO_H_

#include "Ifx_Types.h"

// AI-S20C Lab ASCII Logo
uint8 g_txData_start[423]= {
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 95, 95, 95, 95, 95, 32, 32, 32, 32, 32, 32, 95, 95, 95, 95, 95, 32, 95, 95, 95, 32, 32, 32, 32, 32, 32, 32, 32, 95,
95, 95, 95, 95, 32, 32, 32, 95, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 95, 95, 95, 95, 32, 32, 10, 32, 32, 32, 32, 32, 47, 92, 32, 32, 32,
124, 95, 32, 32, 32, 95, 124, 32, 32, 32, 32, 47, 32, 95, 95, 95, 95, 124, 95, 95, 32, 92, 32, 32, 32, 32, 32, 32, 47, 32, 95, 95, 95, 95, 124, 32, 124, 32, 124, 32,
32, 32, 32, 32, 32, 32, 32, 47, 92, 32, 32, 32, 124, 32, 32, 95, 32, 92, 32, 10, 32, 32, 32, 32, 47, 32, 32, 92, 32, 32, 32, 32, 124, 32, 124, 95, 95, 95, 95, 95,
124, 32, 40, 95, 95, 95, 32, 32, 32, 32, 41, 32, 124, 95, 95, 95, 32, 124, 32, 124, 32, 32, 32, 32, 32, 32, 124, 32, 124, 32, 32, 32, 32, 32, 32, 32, 47, 32, 32, 92,
32, 32, 124, 32, 124, 95, 41, 32, 124, 10, 32, 32, 32, 47, 32, 47, 92, 32, 92, 32, 32, 32, 124, 32, 124, 95, 95, 95, 95, 95, 95, 92, 95, 95, 95, 32, 92, 32, 32, 47,
32, 47, 47, 32, 95, 32, 92, 124, 32, 124, 32, 32, 32, 32, 32, 32, 124, 32, 124, 32, 32, 32, 32, 32, 32, 47, 32, 47, 92, 32, 92, 32, 124, 32, 32, 95, 32, 60, 32, 10,
32, 32, 47, 32, 95, 95, 95, 95, 32, 92, 32, 95, 124, 32, 124, 95, 32, 32, 32, 32, 32, 95, 95, 95, 95, 41, 32, 124, 47, 32, 47, 124, 32, 40, 95, 41, 32, 124, 32, 124,
95, 95, 95, 95, 32, 32, 124, 32, 124, 95, 95, 95, 95, 32, 47, 32, 95, 95, 95, 95, 32, 92, 124, 32, 124, 95, 41, 32, 124, 10, 32, 47, 95, 47, 32, 32, 32, 32, 92, 95,
92, 95, 95, 95, 95, 95, 124, 32, 32, 32, 124, 95, 95, 95, 95, 95, 47, 124, 95, 95, 95, 95, 92, 95, 95, 95, 47, 32, 92, 95, 95, 95, 95, 95, 124, 32, 124, 95, 95, 95,
95, 95, 95, 47, 95, 47, 32, 32, 32, 32, 92, 95, 92, 95, 95, 95, 95, 47, 32, 10, 10, 10, 10};

uint16 g_count_start = sizeof(g_txData_start);

#endif /* LOGO_H_ */