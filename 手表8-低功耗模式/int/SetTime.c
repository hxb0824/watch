#include "Settings.h"


  static RTC_TimeTypeDef TIM;

void SetTime_UI(void)
{
    HAL_RTC_GetTime(&hrtc,&TIM,RTC_FORMAT_BIN);
    OLED_ShowImage(0,0,16,32,Exit);
    OLED_ShowChinese(0,17,"时");
    OLED_ShowChinese(0,33,"分");
    OLED_Printf(18,17,OLED_8X16,"%02d" ,TIM.Hours);
    OLED_Printf(18,33,OLED_8X16,"%02d",TIM.Minutes);

}




void SetTime_Init(void)
{
    SetTime_UI();
    OLED_ReverseArea(0,0,16,16);
    OLED_Update();
}


void SetTime_Loop(void)
{
    static uint8_t flag=1;

    SetTime_UI();

    if(KEY_Check(0,KEY_SINGLE))
    {
        flag--;
        if(flag<=0)flag=4;
    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
        flag++;
        if(flag>=4)flag=1;
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
         switch (flag)
         {
            //返回到时间和日期界面
            case 1: NextMode=2; break;
            //设置时
            case 2: NextMode=4; break;
            //设置分
            case 3: NextMode=5; break;
         }
        
    }
    

    switch (flag)
    {
    case 1:
        OLED_ReverseArea(0,0,16,16);
        OLED_Update();
        break;
    case 2:
        OLED_ReverseArea(0,17,16,16);
        OLED_Update();
        break;
    case 3:
        OLED_ReverseArea(0,33,16,16);
        OLED_Update();
        break;
    }

}


void SetTime_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}








//设置分
void SetMin_Init(void)
{
    SetTime_UI();
    OLED_ReverseArea(18,33,16,16);
    OLED_Update();
}


void SetMin_Loop(void)
{
    if(KEY_Check(0,KEY_SINGLE))
    {
        TIM.Minutes--;
        TIM.Seconds=0;
        HAL_RTC_SetTime(&hrtc,&TIM,RTC_FORMAT_BIN);
        SetTime_UI();
        OLED_ReverseArea(18,33,16,16);
        OLED_Update();
    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
        TIM.Minutes++;
        TIM.Seconds=0;
        HAL_RTC_SetTime(&hrtc,&TIM,RTC_FORMAT_BIN);
        SetTime_UI();
        OLED_ReverseArea(18,33,16,16);
        OLED_Update();
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
        NextMode=3;
    }
}


void SetMin_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}




//设置时
void SetHours_Init(void)
{
    SetTime_UI();
    OLED_ReverseArea(18,17,16,16);
    OLED_Update();
}


void SetHours_Loop(void)
{
    if(KEY_Check(0,KEY_SINGLE))
    {
        TIM.Hours--;
        TIM.Seconds=0;
        HAL_RTC_SetTime(&hrtc,&TIM,RTC_FORMAT_BIN);
        SetTime_UI();
        OLED_ReverseArea(18,17,16,16);
        OLED_Update();
    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
        TIM.Hours++;
        TIM.Seconds=0;
        HAL_RTC_SetTime(&hrtc,&TIM,RTC_FORMAT_BIN);
        SetTime_UI();
        OLED_ReverseArea(18,17,16,16);
        OLED_Update();
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
        NextMode=3;
    }
}


void SetHours_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}



