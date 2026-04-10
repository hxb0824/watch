#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"
//#include "tim.h"

#define KEY_OK 0
#define KEY_NO 1

#define KEY1_GPIO GPIOA
#define KEY2_GPIO GPIOA
#define KEY3_GPIO GPIOB
#define KEY1_PIN GPIO_PIN_8
#define KEY2_PIN GPIO_PIN_11
#define KEY3_PIN GPIO_PIN_3

 //按住不放
#define KEY_HOLD 0X01
//按下瞬间
#define KEY_DOWN 0X02
//松开瞬间
#define KEY_UP   0X04
//单击
#define KEY_SINGLE   0X08
//双击
#define KEY_DOUBLE   0X10
//长按
#define KEY_LONG     0X20
//一直长按
#define KEY_LONG_LONG   0X40

#define KEY_TIME_DOUBLE   200
#define KEY_TIME_LONG     1000
#define KEY_TIME_LONGLONG 100

#define KEY_COUNT 4

extern uint8_t c;

uint8_t KEY_Check(uint8_t n,uint8_t flag);
void TIM_Tick(void);

#endif
