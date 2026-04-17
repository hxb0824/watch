#ifndef __WEATHER_H__
#define __WEATHER_H__

#include "main.h"
#include "usart.h"



void WI_FI_Init(void);

void Weather_Init(void);

void Weather_Loop(void);

void Weather_Exit(void);

void Parse_Time(void);

void Parse_Weather(void);

void City_Init(void);
void City_Loop(void);
void City_Exit(void);

#endif
