#include "Settings.h"

static RTC_TimeTypeDef TIM;
static RTC_DateTypeDef Date;

static uint8_t flag = 1;

void SetTime_UI(void)
{
   HAL_RTC_GetTime(&hrtc, &TIM, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);
    OLED_ShowImage(0, 0, 16, 32, Exit);
    OLED_ShowChinese(0, 17, "时");
    OLED_ShowChinese(0, 33, "分");
    OLED_Printf(18, 17, OLED_8X16, "%02d", TIM.Hours);
    OLED_Printf(18, 33, OLED_8X16, "%02d", TIM.Minutes);
}

void SetTime_Init(void)
{
    OLED_Clear();
    SetTime_UI();
    switch (flag)
    {
    case 1:
        OLED_ReverseArea(0, 0, 16, 16);
        break;
    case 2:
        OLED_ReverseArea(0, 17, 16, 16);
        break;
    case 3: 
        OLED_ReverseArea(0, 33, 16, 16);
        break;
    }
    OLED_Update();
}

void SetTime_Loop(void)
{

    SetTime_UI();

    if (KEY_Check(0, KEY_DOWN))
    {
        flag--;
        if (flag <= 0)
            flag = 4;
    }
    else if (KEY_Check(1, KEY_DOWN))
    {
        flag++;
        if (flag >= 4)
            flag = 1;
    }
    else if (KEY_Check(2, KEY_DOWN))
    {
        switch (flag)
        {
        // 返回到时间和日期界面
        case 1:
            NextMode = 2;
            break;
        // 设置时
        case 2:
            NextMode = 4;
            break;
        // 设置分
        case 3:
            NextMode = 5;
            break;
        }
    }

    switch (flag)
    {
    case 1:
        OLED_ReverseArea(0, 0, 16, 16);
        OLED_Update();
        break;
    case 2:
        OLED_ReverseArea(0, 17, 16, 16);
        OLED_Update();
        break;
    case 3:
        OLED_ReverseArea(0, 33, 16, 16);
        OLED_Update();
        break;
    }
}

void SetTime_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}




// 设置分
void SetMin_Init(void)
{
    SetTime_UI();
    OLED_ReverseArea(18, 33, 16, 16);
    OLED_Update();
}

void SetMin_Loop(void)
{
    if (KEY_Check(0, KEY_DOWN))
    {
       if (TIM.Minutes == 0) 
        {
            TIM.Minutes = 59;
            if (TIM.Hours == 0) { TIM.Hours = 23; }
            else { TIM.Hours--; }
        }
        else
        {
            TIM.Minutes--; 
        }
        TIM.Seconds = 0;
        HAL_RTC_SetTime(&hrtc, &TIM, RTC_FORMAT_BIN);
        SetTime_UI();
        OLED_ReverseArea(18, 33, 16, 16);
        OLED_Update();
    }
    else if (KEY_Check(1, KEY_DOWN))
    {
        
        if(TIM.Minutes>=59)
        {
            TIM.Minutes=0;
            TIM.Hours++;
            if(TIM.Hours>=24)
            {
                TIM.Hours=0;
            }
        }
        else
        {
            TIM.Minutes++;
        }
        TIM.Seconds = 0;
        HAL_RTC_SetTime(&hrtc, &TIM, RTC_FORMAT_BIN);
        SetTime_UI();
        OLED_ReverseArea(18, 33, 16, 16);
        OLED_Update();
    }
    else if (KEY_Check(2, KEY_DOWN))
    {
        NextMode = 3;
    }
}

void SetMin_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}

// 设置时
void SetHours_Init(void)
{
    SetTime_UI();
    OLED_ReverseArea(18, 17, 16, 16);
    OLED_Update();
}

void SetHours_Loop(void)
{
    if (KEY_Check(0, KEY_DOWN))
    {
        
        if(TIM.Hours==0)
        {
            TIM.Hours=23;
        }
        else
        {
            TIM.Hours--;
        }

        TIM.Seconds = 0;
        HAL_RTC_SetTime(&hrtc, &TIM, RTC_FORMAT_BIN);
        SetTime_UI();
        OLED_ReverseArea(18, 17, 16, 16);
        OLED_Update();
    }
    else if (KEY_Check(1, KEY_DOWN))
    {
     
        if(TIM.Hours>=23)
        {
            TIM.Hours=0;
        }
        else
        {
            TIM.Hours++;
        }
        
        TIM.Seconds = 0;
        HAL_RTC_SetTime(&hrtc, &TIM, RTC_FORMAT_BIN);
        SetTime_UI();
        OLED_ReverseArea(18, 17, 16, 16);
        OLED_Update();
    }
    else if (KEY_Check(2, KEY_DOWN))
    {
        NextMode = 3;
    }
}

void SetHours_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}
