#include"GLOBAL.h"

/*

MODE1:首面UI
MODE2:设置时间和日期界面
MODE3；设置时间
MODE4：设置时
MODE5:设置分
MODE6:设置日期
MODE7:








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
    }
}

