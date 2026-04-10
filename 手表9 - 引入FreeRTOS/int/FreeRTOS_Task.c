#include "FreeRTOS_Task.h"
#include "Queue.h"

TaskHandle_t GUI_Task_Handle = NULL;
TaskHandle_t Tick_Task_Handle = NULL;

void GUI_Task(void *pvParameters);
void Tick_Task(void *pvParameters);



extern uint8_t move_state;      // 菜单滑动状态 (在 menu.c 中)
extern uint8_t stopwatch_flag;  // 秒表运行状态 (在 stopwatch.c 中)

QueueHandle_t Queue_Key; // 用于按键事件的队列
uint16_t Current_Key_Msg=0;

void FreeRTOS_Start(void)
{

    Queue_Key = xQueueCreate(20, sizeof(uint16_t)); // 创建一个长度为20的队列，每个元素大小为uint16_t

    // 后台任务优先级应低于前台任务
    xTaskCreate(GUI_Task, "GUI_Task", 1024, NULL, 2, &GUI_Task_Handle);
    xTaskCreate(Tick_Task, "Tick_Task", 256, NULL, 3, &Tick_Task_Handle);
    vTaskStartScheduler();
}

// ==========================================
// 任务1：前台界面任务 
// ==========================================
void GUI_Task(void *pvParameters)
{


    GLOBAL_init(CurrMode); 

    while (1)
    {

        TickType_t wait_time; 
        
        if (move_state == 1 || stopwatch_flag == 1) 
        {
            wait_time = pdMS_TO_TICKS(20);
        } 
        else 
        {
            wait_time = pdMS_TO_TICKS(1000);
        }

        if (xQueueReceive(Queue_Key, &Current_Key_Msg, wait_time) != pdTRUE)
        {
            Current_Key_Msg=0; // 超时未收到消息，清零
        }


         // 1. 先检测是否需要休眠或关机
        LOW_Loop();
        switch_Loop();


        // 3. 醒来后执行画面刷新
        if (CurrMode == NextMode)
        {
            GLOBAL_Loop(CurrMode); 
        }
        if (CurrMode!=NextMode)
        {
            GLOBAL_Exit(CurrMode); 
            GLOBAL_init(NextMode); 
            CurrMode = NextMode;   
        }
    }
}

// ==========================================
// 任务2：后台计时任务 (事件大总管)
// ==========================================
void Tick_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount(); 
    while (1)
    {
        TIM_Tick();        // 扫描按键
        StopWatch_Tick();  
        LOW_Tick();        
        switch_Tick();     

        vTaskDelayUntil(&xLastWakeTime, 1); 
    }
}



