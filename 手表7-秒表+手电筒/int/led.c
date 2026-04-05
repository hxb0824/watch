#include "led.h"


static uint8_t led_flag=0;

void LED_UI(void)
{
    if(led_flag==0)
    {
        OLED_ShowImage(42,10,44,44,LED[0]);
    }
    else
    {
        OLED_ShowImage(42,10,44,44,LED[1]);
    }
}



void LED_init(void)
{
    LED_UI();
    OLED_Update();
}



void LED_Loop(void)
{

  if(KEY_Check(0,KEY_SINGLE))
    {
        if(led_flag==1)
        {
            led_flag=0;
          HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
        }
        else
        {
             led_flag=1;
          HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
        }
        
    }
  
    else if (KEY_Check(1,KEY_SINGLE))
    {

         if(led_flag==1)
        {
            led_flag=0;
          HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
        }
        else
        {
             led_flag=1;
          HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
        }
     
    }
   
    else if (KEY_Check(2,KEY_SINGLE))
    {
        NextMode=10;
    }

    LED_UI();
    OLED_Update();
}




void LED_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}

