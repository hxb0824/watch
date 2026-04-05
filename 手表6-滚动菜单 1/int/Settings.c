#include "Settings.h"

extern uint8_t CurrMode,NextMode;

void Settings_UI(void)
{
    OLED_ShowImage(0,0,16,16,Exit);
    OLED_ShowChinese(2,17,"时间");
    OLED_ShowChinese(0,33,"日期");
}


void Settings_Init(void)
{
    Settings_UI();
    OLED_ReverseArea(0,0,16,16);
    OLED_Update();
}

void Settings_Loop(void)
{
    static uint8_t flag=1;
    Settings_UI();

    if(KEY_Check(0,KEY_SINGLE))
    {
        flag--;
        if(flag<=0) flag=3;

    }
    else if (KEY_Check(1,KEY_SINGLE))
    {
         flag++;
         if(flag>=4) flag=1;
    }
    else if (KEY_Check(2,KEY_SINGLE))
    {
       if(flag==1)
       {
            ///返回首界面
            NextMode=1;
       }
       else if(flag==2)
       {
            //进入世界
            NextMode=3;
       }
       else if(flag=3)
       {
            //进入日历
            NextMode=6;
       }
    }


        switch (flag)
        {
        case 1:
             OLED_ReverseArea(0,0,16,16);
             OLED_Update();
             //last_flag=flag;
            break;
        case 2:
             OLED_ReverseArea(2,17,32,16);
             OLED_Update();
            // last_flag=flag;
            break;
        case 3:
             OLED_ReverseArea(0,33,32,16);
             OLED_Update();
            // last_flag=flag;
            break;
        } 
}

void Settings_Exit(void)
{
    OLED_Clear();
    OLED_Update();

}
