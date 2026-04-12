#include "stopwatch.h"
#include "tim.h"

static  uint16_t M=0,K=0;
uint8_t stopwatch_flag=0;


void StopWatch_UI(void)
{
   OLED_Printf(28,20,OLED_12x24,"%02d:%03d",M,K);
}

void StopWatch_Init(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
    StopWatch_UI();
    OLED_Update(); 
}




void StopWatch_Loop(void)
{
    OLED_Clear();
    //表示开始计时
     if(KEY_Check(0,KEY_DOWN))
    {
        if(stopwatch_flag==1)
        {
            stopwatch_flag=0;
        }
        else
        {
            stopwatch_flag=1;
        }
        
    }
    //表示停止计时并请零
    else if (KEY_Check(1,KEY_DOWN))
    {
        stopwatch_flag=0;
        M=0;
        K=0;
    }
    //退出秒表
    else if (KEY_Check(2,KEY_DOWN))
    {
        NextMode=10;
    }
    StopWatch_UI();
    OLED_Update();
}

void StopWatch_Exit(void)
{
    HAL_TIM_Base_Stop_IT(&htim2);
    OLED_Clear();
    K=0;
    M=0;
    stopwatch_flag=0;
}

void StopWatch_Tick(void)
{
    if(stopwatch_flag==1)
    {
        K++;
         if(K>=1000)
         {
            K=0;
            M++;
            if (M>=60)
             {
                 K=0;
                 M=0;
             }
        
        }
    }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
    {
      StopWatch_Tick();
    }
}




