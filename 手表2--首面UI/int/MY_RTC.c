#include"MY_RTC.h" 


  static RTC_TimeTypeDef TIM;
  static RTC_DateTypeDef Date;

void TIM_UI(void)
{

    HAL_RTC_GetTime(&hrtc,&TIM,RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc,&Date,RTC_FORMAT_BIN);
    OLED_Printf(19,0,OLED_8X16,"20%d-%02d-%02d",Date.Year,Date.Month,Date.Date);
    OLED_Printf(16,16,OLED_12x24,"%d:%d:%d",TIM.Hours,TIM.Minutes,TIM.Seconds);
    OLED_ShowChinese(95,48,"设置");
    OLED_ShowChinese(0,48,"菜单");
   // OLED_ReverseArea(95,48,127,63);
    OLED_Update();

}



void Tim_Init(void)
{

  TIM_UI();
  OLED_ReverseArea(0,48,32,63);
  OLED_Update();

}

void Tim_Loop(void)
{
    static uint8_t flag=1;
    static uint8_t last_seconds=0xFF;
    static uint8_t last_flag=0;
    
    HAL_RTC_GetTime(&hrtc,&TIM,RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc,&Date,RTC_FORMAT_BIN);
    
    // 只有秒数改变时才更新时间显示
    if(TIM.Seconds != last_seconds)
    {
        last_seconds = TIM.Seconds;
        OLED_Printf(19,0,OLED_8X16,"20%d-%02d-%02d",Date.Year,Date.Month,Date.Date);
        OLED_Printf(16,16,OLED_12x24,"%d:%d:%d",TIM.Hours,TIM.Minutes,TIM.Seconds);
        OLED_ShowChinese(95,48,"设置");
        OLED_ShowChinese(0,48,"菜单");
        // 更新时间后需要重绘菜单选择
        last_flag = 0;
    }
    
    if(KEY_Check(0,KEY_SINGLE))
    {
        flag--;
        if(flag<=0) flag=2;

    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
         flag++;
         if(flag>=3) flag=1;
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
       
    }
    
    // 只有flag改变或时间更新时才重绘菜单选择
    if(flag != last_flag)
    {
        last_flag = flag;
        switch (flag)
        {
        case 1:
             OLED_ReverseArea(0,48,32,63);
             OLED_Update();
            break;
        case 2:
             OLED_ReverseArea(95,48,127,63);
             OLED_Update();
            break;
        }
    }
    
}


void Tim_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}
