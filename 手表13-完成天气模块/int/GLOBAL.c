#include"GLOBAL.h"

/*

MODE1:首面UI
MODE2:设置时间和日期界面
MODE3；设置时间
MODE4：设置时
MODE5:设置分
MODE6:设置日期
MODE7:设置年
MODE8：设置月
MODE9：设置日
MODE10：菜单
MODE11:秒表
MODE12:手电筒
MODE13:天气
MODE14:设置城市
*/


void GLOBAL_init(uint8_t n)
{
    switch (n)
    {
    case 1: Tim_Init(); break;
    case 2:Settings_Init();break;
    case 3:SetTime_Init();break;
    case 4:SetHours_Init();break;
    case 5:SetMin_Init();break;
    case 6:SetDate_Init();break;
    case 7:SetYear_Init();break;
    case 8:SetMonth_Init();break;
    case 9:SetDA_Init();break;
    case 10:menu_Init();break;
    case 11:StopWatch_Init();break;
    case 12:LED_init();break;
    case 13:Weather_Init();break;
    case 14:City_Init();break;

    }
}


void GLOBAL_Loop(uint8_t n)
{
    switch (n)
    {
    case 1: Tim_Loop(); break;
    case 2:Settings_Loop();break;
    case 3:SetTime_Loop();break;
    case 4:SetHours_Loop();break;
    case 5:SetMin_Loop();break;
    case 6:SetDate_Loop();break;
    case 7:SetYear_Loop();break;
    case 8:SetMonth_Loop();break;
    case 9:SetDA_Loop();break;
    case 10:menu_Loop();break;
    case 11:StopWatch_Loop();break;
    case 12:LED_Loop();break;
    case 13:Weather_Loop();break;
    case 14:City_Loop();break;
    }
}


void GLOBAL_Exit(uint8_t n)
{
    switch (n)
    {
    case 1: Tim_Exit(); break;
    case 2:Settings_Exit();break;
    case 3:SetTime_Exit();break;
    case 4:SetHours_Exit();break;
    case 5:SetMin_Exit();break;
    case 6:SetDate_Exit();break;
    case 7:SetYear_Exit();break;
    case 8:SetMonth_Exit();break;
    case 9:SetDA_Exit();break;
    case 10:menu_Exit();break;
    case 11:StopWatch_Exit();break;
    case 12:LED_Exit();break;
    case 13:Weather_Exit();break;
    case 14:City_Exit();break;
    }
}

