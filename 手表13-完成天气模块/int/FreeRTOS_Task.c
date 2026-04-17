#include "FreeRTOS_Task.h"
#include "Queue.h"
#include "semphr.h"
#include "tim.h"
#include "timers.h"

TaskHandle_t GUI_Task_Handle = NULL;
// TaskHandle_t Tick_Task_Handle = NULL;

void GUI_Task(void *pvParameters);
// void Tick_Task(void *pvParameters);

extern uint8_t move_state;     // 菜单滑动状态 (在 menu.c 中)
extern uint8_t stopwatch_flag; // 秒表运行状态 (在 stopwatch.c 中)

QueueHandle_t Queue_Key; // 用于按键事件的队列
uint16_t Current_Key_Msg = 0;
QueueHandle_t OLEDSemaphore = NULL; // 定义全局二值信号量

// 按键软件定时器的句柄
TimerHandle_t KeyTimer_Handle = NULL;
TimerHandle_t LowTimer_Handle = NULL;

// 按键软件定时器的回调函数
void KeyTimer_Callback(TimerHandle_t xTimer);
void LowTimer_Callback(TimerHandle_t xTimer);

void FreeRTOS_Start(void)
{

    // 创建消息队列
    Queue_Key = xQueueCreate(30, sizeof(uint16_t));

    // 【创建二值信号量】
    OLEDSemaphore = xSemaphoreCreateBinary();

    // 创建按键软件定时器
    KeyTimer_Handle = xTimerCreate("Key_Timer", pdMS_TO_TICKS(20), pdTRUE, (void *)1, KeyTimer_Callback);
    // 创建停止模式定时器
    LowTimer_Handle = xTimerCreate("Low_Timer", pdMS_TO_TICKS(60000), pdFALSE, (void *)2, LowTimer_Callback);

    xTaskCreate(GUI_Task, "GUI_Task", 256, NULL, 2, &GUI_Task_Handle);
    // xTaskCreate(Tick_Task, "Tick_Task", 256, NULL, 3, &Tick_Task_Handle);
    xTimerStart(LowTimer_Handle, 0);
    vTaskStartScheduler();
}

// ==========================================
// 任务1：前台界面任务
// ==========================================

uint32_t hour_sync_tick = 0;

void GUI_Task(void *pvParameters)
{
    // 1. 系统刚启动，先不进主循环，强制画一个开机联网界面！
    OLED_Clear();
    OLED_ShowChinese(32, 16, "正在连接");
    OLED_ShowString(40, 36, "Wi-Fi...", OLED_8X16);
    OLED_Update();



    
    // 2. 后台开始连网并对时
    WI_FI_Init();      // 连接路由器并配置 SNTP
    vTaskDelay(pdMS_TO_TICKS(3000));
    ESP8266_GetTime(); // 发指令获取时间
    Parse_Time();      // 解析并写入 RTC 时钟
    WiFi_PowerOff();   // 对时完成，立刻关闭 WiFi 模块省电！

    OLED_Clear();
    OLED_Update();

    // 3. 对时结束，正式进入首页 UI
    CurrMode = 1;
    NextMode = 1;
    GLOBAL_init(CurrMode);

    while (1)
    {
        TickType_t wait_time;

        if (move_state == 1)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
        }
        else
        {
            if (stopwatch_flag == 1)
                wait_time = pdMS_TO_TICKS(20);
            else
                wait_time = pdMS_TO_TICKS(1000);

            // 当队列没有按键消息时（超时）
            if (xQueueReceive(Queue_Key, &Current_Key_Msg, wait_time) != pdTRUE)
            {
                Current_Key_Msg = 0;

                // 【每小时静默对时逻辑】
                // 1000ms 超时一次，相当于过了 1 秒。3600 秒就是 1 小时。
                if (stopwatch_flag == 0 && CurrMode != 13) // 不在秒表和天气界面时才搞后台动作
                {
                    hour_sync_tick++;
                    if (hour_sync_tick >= 3600)
                    {
                        WI_FI_Init();       // 开启 WiFi
                        ESP8266_GetTime();  // 获取时间
                        Parse_Time();       // 更新 RTC
                        WiFi_PowerOff();    // 关闭 WiFi
                        hour_sync_tick = 0; // 重新计时
                    }
                }
            }
        }

        LOW_Loop();
        switch_Tick();
        switch_Loop();

        if (CurrMode == NextMode)
        {
            GLOBAL_Loop(CurrMode);
        }
        if (CurrMode != NextMode)
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
/* void Tick_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        LOW_Tick();
        switch_Tick();

        vTaskDelayUntil(&xLastWakeTime, 1);
    }
} */

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // 释放信号量，通知 OLED_Update 任务数据已发完
        xSemaphoreGiveFromISR(OLEDSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // 如果需要，进行任务切换
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // 发生错误时，也必须释放信号量！防止 GUI_Task 死等卡死
        xSemaphoreGiveFromISR(OLEDSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// void PRE_TICKLESS_SLEEP(uint32_t xExpectedIdleTime)
// {
//     /* 这里可以放进入休眠前的特殊处理，比如关闭特定外设 */
//     /* 目前可以先留空，但参数必须写对 */
// }

// void POST_TICKLESS_SLEEP(uint32_t xExpectedIdleTime)
// {
//     /* 这里可以放唤醒后的特殊处理 */
// }
