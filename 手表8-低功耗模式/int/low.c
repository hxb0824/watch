#include "low.h"

volatile uint8_t low_flag = 0;
volatile uint8_t switch_flag = 0;


void LOW_Tick(void)
{
    static uint32_t NUM = 0;
    NUM++;

    /* 有按键活动，重置计时器 */
    if(NUM < 10000 && c == 1)
    {
        NUM = 0;
        c = 0;
    }

    /* 6秒无操作且非计时状态，准备进入停止模式 */
    if(NUM >= 10000 && c == 0 && stopwatch_flag == 0&&switch_flag==0)
    {
        NUM = 0; // 重置计时器
        low_flag = 1; // 告诉主循环：该睡觉了
    }
}

void LOW_Loop(void)
{
    // 检测是否需要进入停止模式
    if (low_flag == 1)
    {
        // 1. 清除标志位
        low_flag = 0; 

        OLED_Clear();
        OLED_Update();

        // 3. 进入停止模式
        // 注意：确保此时所有不需要的串口、定时器等外设都已关闭或挂起，以降低功耗
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
        SystemClock_Config(); 
    }
}




//待机模式
void switch_Tick(void)
{
    //没进入睡眠模式
    if (low_flag == 0)
    {   
        if(KEY_Check(3,KEY_LONG)==1)
        {
           switch_flag=1;
        
        }
    }
}

//待机模式
void switch_Loop(void)
{
    if(switch_flag==1)
    {
        switch_flag=0;
        OLED_Clear();
        OLED_Update();
        HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
        HAL_PWR_EnterSTANDBYMode();
    }
}
