#include "SetDate.h"


  static RTC_DateTypeDef Date;


void SetDate_UI(void)
{
    HAL_RTC_GetDate(&hrtc,&Date,RTC_FORMAT_BIN);
    OLED_ShowImage(0,0,16,32,Exit);
    OLED_ShowChinese(0,16,"年");
    OLED_ShowChinese(0,32,"月");
    OLED_ShowChinese(0,48,"日");
    OLED_Printf(17,16,OLED_8X16,"20%02d" ,Date.Year);
    OLED_Printf(17,32,OLED_8X16,"%02d",Date.Month);
    OLED_Printf(17,48,OLED_8X16,"%02d",Date.Date);

}




void SetDate_Init(void)
{
    SetDate_UI();
    OLED_ReverseArea(0,0,16,16);
    OLED_Update();
}


void SetDate_Loop(void)
{
    static uint8_t flag=1;

    SetDate_UI();

    if(KEY_Check(0,KEY_SINGLE))
    {
        flag--;
        if(flag<=0)flag=5;
    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
        flag++;
        if(flag>=5)flag=1;
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
         switch (flag)
         {
            //返回到时间和日期界面
            case 1: NextMode=2; break;
            //设置年
            case 2: NextMode=7; break;
            //设置月
            case 3: NextMode=8; break;
            //设置日
            case 4:NextMode= 9;break;
         }
        
    }
    
    switch (flag)
    {
    case 1:
        OLED_ReverseArea(0,0,16,16);
        OLED_Update();
        break;
    case 2:
        OLED_ReverseArea(0,16,16,16);
        OLED_Update();
        break;
    case 3:
        OLED_ReverseArea(0,33,16,16);
        OLED_Update();
        break;
    case 4:
        OLED_ReverseArea(0,48,16,16);
        OLED_Update();
        break;
    }

}

void SetDate_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}




//设置年
void SetYear_Init(void)
{
    SetDate_UI();
    OLED_ReverseArea(17,16,32,16);
    OLED_Update();
}


void SetYear_Loop(void)
{
    if(KEY_Check(0,KEY_SINGLE))
    {
        Date.Year--;
        HAL_RTC_SetDate(&hrtc,&Date,RTC_FORMAT_BIN);
        SetDate_UI();
        OLED_ReverseArea(17,16,32,16);
        OLED_Update();
    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
        Date.Year++;
        HAL_RTC_SetDate(&hrtc,&Date,RTC_FORMAT_BIN);
        SetDate_UI();
        OLED_ReverseArea(17,16,32,16);
        OLED_Update();
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
        NextMode=6;
    }
}


void SetYear_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}













//设置月
void SetDA_Init(void)
{
    SetDate_UI();
    OLED_ReverseArea(17,48,16,16);
    OLED_Update();
}


void SetDA_Loop(void)
{
    if(KEY_Check(0,KEY_SINGLE))
    {
        Date.Date--;
        HAL_RTC_SetDate(&hrtc,&Date,RTC_FORMAT_BIN);
        SetDate_UI();
        OLED_ReverseArea(17,48,16,16);
        OLED_Update();
    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
        Date.Date++;
        HAL_RTC_SetDate(&hrtc,&Date,RTC_FORMAT_BIN);
        SetDate_UI();
        OLED_ReverseArea(17,48,16,16);
        OLED_Update();
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
        NextMode=6;
    }
}


void SetDA_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}











//设置月
void SetMonth_Init(void)
{
    SetDate_UI();
    OLED_ReverseArea(17,32,16,16);
    OLED_Update();
}


void SetMonth_Loop(void)
{
    if(KEY_Check(0,KEY_SINGLE))
    {
        Date.Month--;
        HAL_RTC_SetDate(&hrtc,&Date,RTC_FORMAT_BIN);
        SetDate_UI();
        OLED_ReverseArea(17,32,16,16);
        OLED_Update();
    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
        Date.Month++;
        HAL_RTC_SetDate(&hrtc,&Date,RTC_FORMAT_BIN);
        SetDate_UI();
        OLED_ReverseArea(17,32,16,16);
        OLED_Update();
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
        NextMode=6;
    }
}


void SetMonth_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}
