#include "KEY.h"




static uint8_t KEY_Flag[KEY_COUNT];


uint8_t KEY_Check(uint8_t n,uint8_t flag)
{
    if(KEY_Flag[n]&flag)
    {
        if (flag!=KEY_HOLD)
        {
            KEY_Flag[n]&=~flag;
        }
        return 1;
    }
    return 0;
}


uint8_t KEY_State(uint8_t n)
{
    if(n==0)
    {
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==KEY_OK)
        {
            return KEY_OK;
        }
        else
        {
            return KEY_NO;
        }
    }
    else if(n==1)
    {
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==KEY_OK)
        {
            return KEY_OK;
        }
        else
        {
            return KEY_NO;
        }
    }
    else if(n==2)
    {
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)==KEY_OK)
        {
            return KEY_OK;
        }
        else
        {
            return KEY_NO;
        }
    }
    return KEY_NO;
}

void TIM_Tick(void)
{
    static uint8_t Count,i;
    static uint8_t CurrState[KEY_COUNT],PrevState[KEY_COUNT];
    static uint8_t S[KEY_COUNT];
    static uint16_t NUM[KEY_COUNT];
    for(i=0;i<KEY_COUNT;i++)
    {
        if (NUM[i]>0)
        {
            NUM[i]--;
        }
    }

    Count++;
    if(Count>=20)
    {
        Count=0;
        for(i=0;i<KEY_COUNT;i++)
        {
            PrevState[i]=CurrState[i];
            CurrState[i]=KEY_State(i);
            //一直按住
            if(CurrState[i]==KEY_OK)
            {
                KEY_Flag[i]|=KEY_HOLD;
            }
            else
            {
                KEY_Flag[i]&=~KEY_HOLD;
            }
            //按下瞬间
            if(CurrState[i]==KEY_OK&&PrevState[i]==KEY_NO)
            {
                KEY_Flag[i]|=KEY_DOWN;
            }
            //松开瞬间
            if(CurrState[i]==KEY_NO&&PrevState[i]==KEY_OK)
            {
                KEY_Flag[i]|=KEY_UP;
            }
            switch (S[i])
            {
            case 0:
                //判断按键是否按下
                if(CurrState[i]==KEY_OK)
                {
                    //按下S置1
                    S[i]=1;
                    //并开始记时间，如果2秒后按键还没有松开，按键长按
                    NUM[i]=KEY_TIME_LONG;
                }
                break;

            case 1:
                //按键松开
                if(CurrState[i]==KEY_NO)
                {
                    //S置2
                    S[i]=2;
                    //此时按键已经松开，不可能是长按，记时，如果在200ms内再次按下，就是双击
                    NUM[i]=KEY_TIME_DOUBLE;
                }
                //按键未松开
                else if (NUM[i]==0)
                {
                    //S置4
                    S[i]=4;
                    //计时判断是一直长按还是普通长按，如果在100ms后还没松开就是一直长按
                    NUM[i]=KEY_TIME_LONGLONG;
                }
                break;

            case 2:
                //按键按下，双击
                if(CurrState[i]==KEY_OK)
                {
                    //双击，S置3，在S=3中将S置0
                    KEY_Flag[i]|=KEY_DOUBLE;
                    S[i]=3;
                }
                //按键未按下，单击
                else if(NUM[i]==0)
                {
                    //单击，将S置0
                    KEY_Flag[i]|=KEY_SINGLE;
                    S[i]=0;
                }
                break;
            case 3:
                //双击，将S置0
                if(CurrState[i]==KEY_NO)
                {
                    S[i]=0;
                }
                break;
            case 4:
                //按键松开，长按
                if(CurrState[i]==KEY_NO)
                {
                    //长按
                    KEY_Flag[i]|=KEY_LONG;
                    S[i]=0;
                }
                //按键未松开，一直长按，
                else if (NUM[i]==0)
                {
                    //一直长按
                    KEY_Flag[i]|=KEY_LONG_LONG;
                }
                break;
            }
        }
    }
}

/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==&htim2)
    {
        TIM_Tick();
    }
}
*/
