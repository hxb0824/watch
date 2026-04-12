#include "MY_RTC.h"

static RTC_TimeTypeDef TIM;
static RTC_DateTypeDef Date;
static uint8_t flag = 1;
extern uint8_t CurrMode, NextMode;

void TIM_UI(void)
{

    HAL_RTC_GetTime(&hrtc, &TIM, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

    OLED_Printf(19, 0, OLED_8X16, "20%d-%02d-%02d", Date.Year, Date.Month, Date.Date);
    OLED_Printf(16, 16, OLED_12x24, "%02d:%02d:%02d", TIM.Hours, TIM.Minutes, TIM.Seconds);
    OLED_ShowChinese(95, 48, "设置");
    OLED_ShowChinese(0, 48, "菜单");
}

void Tim_Init(void)
{

    TIM_UI();

    if(flag==1)
    {
        OLED_ReverseArea(0, 48, 32, 16);
    }
    else if(flag==2)
    {
        OLED_ReverseArea(95, 48, 32, 16);
    }
    OLED_Update();
}

void Tim_Loop(void)
{

    OLED_Clear();
    TIM_UI();

    if (KEY_Check(0, KEY_DOWN))
    {
        flag--;
        if (flag <= 0)
            flag = 2;
    }
    else if (KEY_Check(1, KEY_DOWN))
    {
        flag++;
        if (flag >= 3)
            flag = 1;
    }
    else if (KEY_Check(2, KEY_DOWN))
    {
        if (flag == 1)
        {
            // 进入菜单
            NextMode = 10;
        }
        else
        {
            // 进入设置
            NextMode = 2;
        }
    }

   
        switch (flag)
        {
        case 1:
            OLED_ReverseArea(0, 48, 32, 16);
            OLED_Update();
            break;
        case 2:
            OLED_ReverseArea(95, 48, 32, 16);
            OLED_Update();
            break; 
        }

}

void Tim_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}
