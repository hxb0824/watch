#include "Settings.h"

extern uint8_t CurrMode,NextMode;
static uint8_t flag=1;

#define Exit_x 0
#define Exit_y 0
#define TIM_x 2
#define TIM_y 17
#define date_x 0
#define date_y 33


void Settings_UI(void)
{
    OLED_ShowImage(Exit_x,Exit_y,16,16,Exit);
    OLED_ShowChinese(TIM_x,TIM_y,"时间");
    OLED_ShowChinese(date_x,date_y,"日期");
}


void Settings_Init(void)
{
    OLED_Clear();
    Settings_UI();
    switch (flag)
    {
    case 1:
        OLED_ReverseArea(Exit_x,Exit_y,16,16);
        break;
    case 2:
        OLED_ReverseArea(TIM_x,TIM_y,32,16);
        break;
    case 3:
        OLED_ReverseArea(date_x,date_y,32,16);
        break;
    }
    OLED_Update();
}

void Settings_Loop(void)
{
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
            //进入时间
            NextMode=3;
       }
       else if(flag==3)
       {
            //进入日历
            NextMode=6;
       }
    }


        switch (flag)
        {
        case 1:
             OLED_ReverseArea(Exit_x,Exit_y,16,16);
             OLED_Update();
             //last_flag=flag;
            break;
        case 2:
             OLED_ReverseArea(TIM_x,TIM_y,32,16);
             OLED_Update();
            // last_flag=flag;
            break;
        case 3:
             OLED_ReverseArea(date_x,date_y,32,16);
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
