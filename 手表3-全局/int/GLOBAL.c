#include"GLOBAL.h"

/*

MODE1:首面UI
MODE2:菜单界面
MODE3:设置时间和日期界面
MODE4；设置时间
MODE5：设置日期








*/


void GLOBAL_init(uint8_t n)
{
    switch (n)
    {
    case 1: Tim_Init(); break;
    case 2:Settings_Init();break;
    }
}


void GLOBAL_Loop(uint8_t n)
{
    switch (n)
    {
    case 1: Tim_Loop(); break;
    case 2:Settings_Loop();break;
    }
}


void GLOBAL_Exit(uint8_t n)
{
    switch (n)
    {
    case 1: Tim_Exit(); break;
    case 2:Settings_Exit();break;
    }
}