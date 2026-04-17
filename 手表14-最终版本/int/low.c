#include "low.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stopwatch.h" 

volatile uint8_t low_flag = 0;
volatile uint8_t switch_flag = 0;

// 60秒无操作闹钟到了，执行这个回调
void LowTimer_Callback(TimerHandle_t xTimer)
{
    // 【关键修复】：如果秒表正在运行，或者准备进双击待机了，绝对不能进入普通的 60s 休眠！
    if (stopwatch_flag == 0 && switch_flag == 0)
    {
        low_flag = 1;
    }
}

// 浅度休眠（STOP模式，保留 SRAM，外部中断可唤醒）
void LOW_Loop(void)
{
    if (low_flag == 1)
    {
        low_flag = 0; 

        // 睡前黑屏
        OLED_Clear();
        OLED_Update();

        // 进入停止模式，CPU 和绝大多数外设时钟停止
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
        
        // ================== 醒来后的分界线 ==================
        // 只要你按下按键，EXTI 硬件中断唤醒单片机，代码就会瞬间从这里继续往下跑
        
        // 1. 恢复被休眠打乱的系统时钟树（72MHz）
        extern void SystemClock_Config(void);
        SystemClock_Config();
        
        // 2. 【关键修复】：此时屏幕还是黑的！必须强制重新初始化当前界面，刷一次全屏UI！
        extern void GLOBAL_init(uint8_t mode);
        extern uint8_t CurrMode;
        GLOBAL_init(CurrMode);
    }
}

// 检查双击休眠标志
void switch_Tick(void)
{
    if (low_flag == 0)
    {   
        if(KEY_Check(3, KEY_DOUBLE) == 1) // 如果按键3被双击
        {
           switch_flag = 1;
        }
    }
}

// 深度休眠（STANDBY模式，SRAM 断电，只能靠 WakeUp 引脚或复位唤醒，极其省电）
void switch_Loop(void)
{
    if(switch_flag == 1)
    {
        switch_flag = 0;
        
        // 睡前黑屏
        OLED_Clear();
        OLED_Update();
        
        // 使能唤醒引脚 (通常是 PA0)
        HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
        
        // 进入待机模式（相当于关机，除了唤醒电路全部断电）
        // 醒来后相当于重新上电复位，代码会从 main() 重新开始跑，所以不需要写醒来后的代码
        HAL_PWR_EnterSTANDBYMode();
    }
}

