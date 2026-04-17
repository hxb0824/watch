#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include "main.h"

void StopWatch_Init(void);
void StopWatch_Loop(void);
void StopWatch_Exit(void);
void StopWatch_Tick(void);
void ESP8266_GetTime(void);

void WiFi_PowerOff(void);

extern uint8_t stopwatch_flag;

#endif
