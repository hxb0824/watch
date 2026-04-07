#include "FreeRTOS_Task.h"

// 任务函数声明
void main_Task(void *pvParameters);

// 任务配置
#define MAIN_TASK_STACK    256  // 增加到256，128对于复杂的UI逻辑可能不够
#define MAIN_TASK_PRIO     2
TaskHandle_t main_Task_Handle = NULL;

void FreeRTOS_Start(void)
{
    // 直接创建主任务
    BaseType_t xReturn = xTaskCreate(
        (TaskFunction_t )main_Task,     
        (const char* )"Main_Task",   // 任务名字
        (uint16_t       )MAIN_TASK_STACK, 
        (void* )NULL,          
        (UBaseType_t    )MAIN_TASK_PRIO, 
        (TaskHandle_t* )&main_Task_Handle // 传入句柄的地址
    );

    if (xReturn == pdPASS)
    {
        vTaskStartScheduler(); // 启动调度器
    }
}

void main_Task(void *pvParameters)
{
    // 1. 初始化当前模式 (只需执行一次)
    GLOBAL_init(CurrMode); 

    // 2. 进入死循环，这才是 FreeRTOS 任务的正确形态
    for(;;) 
    {
        // 建议先注释掉低功耗，调通逻辑后再开启
        // LOW_Loop();
        // switch_Loop();

        if (CurrMode == NextMode)
        {
            GLOBAL_Loop(CurrMode);
        }
        else
        {
            GLOBAL_Exit(CurrMode);
            GLOBAL_init(NextMode);
            CurrMode = NextMode;
        }

        // 这里的 20 是 Tick 数。如果你的系统频率是 1000Hz，这就是 20ms
        vTaskDelay(pdMS_TO_TICKS(20)); 
    }
}