#include "menu.h"



//pos是当前图像，target_pos是目标图像
 int8_t pos=0,target_pos=0;
//向左移是1,向右移是2
 uint8_t move_flag=0;
//开始移动为1，不移动为0
 uint8_t move_state=0;
 uint8_t x_pre=48;
 uint8_t Speed=4;


void menu_UI(void)
{
     OLED_Clear();
     OLED_ShowImage(42,10,44,44,Frame);
    if(pos>=1)
    {
        if(pos>=2)
        {
             OLED_ShowImage(x_pre-96,16,32,32,menu_Graph[pos-2]);
        }
        OLED_ShowImage(x_pre-48,16,32,32,menu_Graph[pos-1]);
    }
		
    OLED_ShowImage(x_pre,16,32,32,menu_Graph[pos]);
		
    if(pos<=2)
    {
        if(pos<=1)
        {
             OLED_ShowImage(x_pre+96,16,32,32,menu_Graph[pos+2]);
        }
         OLED_ShowImage(x_pre+48,16,32,32,menu_Graph[pos+1]);
    }
	
}




void move_start(void)
{
    //左移
    if(move_flag==1)
    {
        //如果移动前图像为0时，表示从0移到3
        //因为是特殊情况不加滚动效果更好
        if(pos<=0)
        {
            //更新现在图像为移动前的目标图像
            pos=MAX;
            //表示移动完成
            move_state=0;
            //恢复现在图像坐标
            x_pre=48;
                
        }

        else
        {
            //每次移动，移4像素
          x_pre+=Speed;
          //当现在的图像移到右边表示移动完成
             if(x_pre==96)
            {
                //移动结束
                move_state=0;
                pos--;
                x_pre=48;
            }
        }
    }
    //右移
    else if(move_flag==2)
    {

        if(pos>=MAX)
        {
            pos=0;
            move_state=0;
            x_pre=48;
        } 
        else
        {
             x_pre-=Speed;
            if(x_pre==0)
            {
                //移动结束
                move_state=0;
                pos++;
                x_pre=48;
            }
        }
    }
    menu_UI();
    OLED_Update();
}



void menu_Init(void)
{
    menu_UI();
    OLED_Update();

}



void menu_Loop(void)
{
    static int8_t flag=0;

    if (move_state == 1)
    {
        move_start();
        return; 
    }

    // 检测按键0（向左移动）
    if(KEY_Check(0,KEY_SINGLE))
    {
        
        flag--;
        //左移标志位
        move_flag=1;
        //准备移动标志
        move_state=1;
        if(flag<0)
        {
            flag=MAX;
        }
       
    }
    // 检测按键1（向右移动）
    else if (KEY_Check(1,KEY_SINGLE))
    {

        flag++;
        //右移标志
        move_flag=2;
        //开始移动
        move_state=1;
       
        if(flag>MAX)
        {
            flag=0;
        }  
    }

    // 检测按键2（确认选择）
    else if (KEY_Check(2,KEY_SINGLE))
    {
         switch (flag)
         {
            //返回到首界面
            case 0: NextMode=1; break;
            //秒表
            case 1: NextMode=11; break;
            //手电筒
            case 2: NextMode=12; break;
            //天气
            case 3: NextMode=13; break;
         }
    }

    if(move_state==1)
    {
        move_start();
    }

}





void menu_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}

