#include "KEY.h"
#include "queue.h"
#include "timers.h"

// static uint8_t KEY_Flag[KEY_COUNT];

extern QueueHandle_t Queue_Key;
extern uint16_t Current_Key_Msg;

extern TimerHandle_t KeyTimer_Handle;

//给按键加上一个锁，检测到按键按下，必须让软件定时器走完20ms
volatile uint8_t key_timer_active = 0;

uint8_t KEY_Check(uint8_t n, uint8_t flag)
{
    /*     uint8_t res = 0; // 用变量存结果
        if (KEY_Flag[n] & flag)
        {
            if (flag != KEY_HOLD)
            {
                KEY_Flag[n] &= ~flag;
            }
            res = 1; // 记录结果，绝对不能在这里 return！
        }
        return res; // 最后统一返回 */

    uint16_t target_msg = (n << 8) | flag; // 构造目标消息

    if (Current_Key_Msg == target_msg)
    {
        Current_Key_Msg = 0; // 清除消息，确保下一次检测时不会重复
        return 1;            // 返回按键事件已发生
    }
    return 0; // 返回按键事件未发生
}

uint8_t KEY_State(uint8_t n)
{
    if (n == 0)
    {
        if (HAL_GPIO_ReadPin(KEY1_GPIO, KEY1_PIN) == KEY_OK)
        {
            return KEY_OK;
        }
        else
        {
            return KEY_NO;
        }
    }
    else if (n == 1)
    {
        if (HAL_GPIO_ReadPin(KEY2_GPIO, KEY2_PIN) == KEY_OK)
        {
            return KEY_OK;
        }
        else
        {
            return KEY_NO;
        }
    }
    else if (n == 2)
    {
        if (HAL_GPIO_ReadPin(KEY3_GPIO, KEY3_PIN) == KEY_OK)
        {
            return KEY_OK;
        }
        else
        {
            return KEY_NO;
        }
    }
    else if (n == 3)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == KEY_NO)
        {
            return KEY_OK;
        }
        else
        {
            return KEY_NO;
        }
    }
    return KEY_NO;
}

uint8_t c = 0;

void KeyTimer_Callback(TimerHandle_t xTimer)
{
    static uint8_t CurrState[KEY_COUNT] = {KEY_NO, KEY_NO, KEY_NO, KEY_NO};
    static uint8_t PrevState[KEY_COUNT] = {KEY_NO, KEY_NO, KEY_NO, KEY_NO};
    static uint8_t S[KEY_COUNT];
    static uint16_t NUM[KEY_COUNT];

    uint8_t all_keys_idle = 1; // 用于判断是不是所有按键都闲置了
                               /*   static uint8_t Count = 20;
                                 Count--; */
    for (uint8_t i = 0; i < KEY_COUNT; i++)
    {
        if (NUM[i] > 0)
        {
            NUM[i]--;
        }
    }
    /*  if (Count == 0)
     {
         Count = 20; */

    for (int i = 0; i < KEY_COUNT; i++)
    {
        PrevState[i] = CurrState[i];
        CurrState[i] = KEY_State(i);

        // 【关键】重置系统休眠倒计时，只要有动作就不关机
        if (CurrState[i] == KEY_OK && PrevState[i] == KEY_NO)
        {
            extern TimerHandle_t LowTimer_Handle;
            xTimerReset(LowTimer_Handle, 0);
        }

        uint16_t msg = 0;
        switch (S[i])
        {
        case 0:
            // 判断按键是否按下
            if (CurrState[i] == KEY_OK)
            {
                // 按下瞬间
                msg = i << 8 | KEY_DOWN;
                // 按下S置1
                S[i] = 1;
                // 并开始记时间，如果2秒后按键还没有松开，按键长按
                NUM[i] = KEY_TIME_LONG;
            }
            break;

        case 1:
            // 按键松开
            if (CurrState[i] == KEY_NO)
            {
                // S置2
                S[i] = 2;
                // 此时按键已经松开，不可能是长按，记时，如果在200ms内再次按下，就是双击
                NUM[i] = KEY_TIME_DOUBLE;
            }
            // 按键未松开
            else if (NUM[i] == 0)
            {
                // S置4
                S[i] = 4;
                // 计时判断是一直长按还是普通长按，如果在100ms后还没松开就是一直长按
                NUM[i] = KEY_TIME_LONGLONG;
            }
            break;

        case 2:
            // 按键按下，双击
            if (CurrState[i] == KEY_OK)
            {
                // 双击，S置3，在S=3中将S置0
                //  KEY_Flag[i] |= KEY_DOUBLE;
                msg = i << 8 | KEY_DOUBLE;
                S[i] = 3;
            }
            // 按键未按下，单击
            else if (NUM[i] == 0)
            {
                // 单击，将S置0
                // KEY_Flag[i] |= KEY_SINGLE;
                msg = i << 8 | KEY_SINGLE;
                S[i] = 0;
            }
            break;
        case 3:
            // 双击，将S置0
            if (CurrState[i] == KEY_NO)
            {
                S[i] = 0;
            }
            break;
        case 4:
            // 按键松开，长按
            if (CurrState[i] == KEY_NO)
            {
                // 长按
                // KEY_Flag[i] |= KEY_LONG;
                msg = i << 8 | KEY_LONG;
                S[i] = 0;
            }
            // 按键未松开，一直长按，
            else if (NUM[i] == 0)
            {
                // 一直长按
                //  KEY_Flag[i] |= KEY_LONG_LONG;
                msg = i << 8 | KEY_LONG_LONG;
                S[i] = 5;
            }
            break;
        case 5:
            // 按键松开，一直长按
            if (CurrState[i] == KEY_NO)
            {
                S[i] = 0;
            }
            break;
        }

        if (i != 3 && (msg & 0xFF) == KEY_DOUBLE)
        {
            msg = i << 8 | KEY_DOWN;
        }

        // 如果按键产生了事件，发送到队列,只发送按键按下信息
        if ((msg & 0xFF) == KEY_DOWN || (i == 3 && (msg & 0xFF) == KEY_DOUBLE))
        {
            xQueueSend(Queue_Key, &msg, 0);
        }

        // 检查这个按键是不是彻底空闲了（状态回到0，且没有未处理的动作）
        if (S[i] != 0 || CurrState[i] == KEY_OK)
        {
            all_keys_idle = 0; // 只要有一个按键还在操作中，就不空闲
        }
    }

    // 【最惊艳的一步：按需关闭轮询】
    // 如果所有按键都已经彻底松开，并且没有处于等待双击的状态
    if (all_keys_idle == 1)
    {
        // 停掉这个 20ms 的定时器！
        // CPU 再也不会每 20ms 被叫醒了，可以毫无顾忌地进入深度休眠！
        key_timer_active = 0;
        xTimerStop(xTimer, 0);
    }
    //}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 判断是不是我们的按键引脚
    if (GPIO_Pin == KEY1_PIN || GPIO_Pin == KEY2_PIN || GPIO_Pin == KEY3_PIN || GPIO_Pin == GPIO_PIN_0)
    {

        if (key_timer_active == 0)
        {
            key_timer_active = 1;
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;

            // 唤醒并启动软件定时器
            // 如果定时器已经在跑了，这句会自动帮我们 Reset 定时器，天然起到了“硬件防抖”的作用！
            xTimerStartFromISR(KeyTimer_Handle, &xHigherPriorityTaskWoken);

            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
