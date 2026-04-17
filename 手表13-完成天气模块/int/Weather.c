#include "Weather.h"
#include "stdlib.h"

// 【全剧终！只留这一个绝对安全的1024字节大缓存，其他全删！】
uint8_t UART_Rx_Buffer[1024];
uint16_t UART_Rx_Len;

typedef struct
{
    char utf8_code[15];
    char name[10];
} WeatherDict;

static const WeatherDict my_weather_dict[] = {
    {{0xE6, 0x99, 0xB4, 0x00}, "晴"},
    {{0xE5, 0xA4, 0x9A, 0xE4, 0xBA, 0x91, 0x00}, "多云"},
    {{0xE9, 0x98, 0xB4, 0x00}, "阴"},
    {{0xE9, 0x98, 0xB5, 0xE9, 0x9B, 0xA8, 0x00}, "阵雨"},
    {{0xE9, 0x9B, 0xB7, 0xE9, 0x98, 0xB5, 0xE9, 0x9B, 0xA8, 0x00}, "雷阵雨"},
    {{0xE5, 0xB0, 0x8F, 0xE9, 0x9B, 0xA8, 0x00}, "小雨"},
    {{0xE4, 0xB8, 0xAD, 0xE9, 0x9B, 0xA8, 0x00}, "中雨"},
    {{0xE5, 0xA4, 0xA7, 0xE9, 0x9B, 0xA8, 0x00}, "大雨"},
    {{0xE6, 0x9A, 0xB4, 0xE9, 0x9B, 0xA8, 0x00}, "暴雨"},
    {{0xE9, 0x9B, 0xBE, 0x00}, "雾"},
    {{0xE9, 0x9C, 0xBE, 0x00}, "霾"},
    {{0xE6, 0xB2, 0x99, 0xE5, 0xB0, 0x98, 0xE6, 0x9A, 0xB4, 0x00}, "沙尘暴"},
    {{0xE5, 0xA4, 0xA7, 0xE9, 0xA3, 0x8E, 0x00}, "大风"}};

typedef struct
{
    char utf8_code[15];
    char name[10];
    uint8_t *AT;
} City;

static const City my_City[] = {
    {{0xE8, 0xB4, 0xB5, 0xE9, 0x98, 0xB3, 0x00}, "贵阳", "GET /v3/weather/now.json?key=SW8vHSfMLZWr8g66k&location=guiyang&language=zh-Hans&unit=c HTTP/1.1\r\nHost: api.seniverse.com\r\n\r\n"},
    {{0xE5, 0x8C, 0x97, 0xE4, 0xBA, 0xAC, 0x00}, "北京", "GET /v3/weather/now.json?key=SW8vHSfMLZWr8g66k&location=beijing&language=zh-Hans&unit=c HTTP/1.1\r\nHost: api.seniverse.com\r\n\r\n"},
    {{0xE9, 0x81, 0xB5, 0xE4, 0xB9, 0x89, 0x00}, "遵义", "GET /v3/weather/now.json?key=SW8vHSfMLZWr8g66k&location=zunyi&language=zh-Hans&unit=c HTTP/1.1\r\nHost: api.seniverse.com\r\n\r\n"}};

void ESP8266_ReadDate(uint8_t *Buff, uint16_t *UART_Rx_Len)
{
    HAL_UARTEx_ReceiveToIdle(&huart1, Buff, 1024, UART_Rx_Len, 1000);
}

void ESP8266_SendAt(const uint8_t *At, uint16_t At_Len, uint8_t *Buff, uint16_t *UART_Rx_Len)
{
    memset(Buff, 0, 1024); // 清空缓存防干扰

    HAL_UART_Transmit(&huart1, At, At_Len, 100);

    if (strstr((char *)At, "+++") != NULL || strstr((char *)At, "GET") != NULL)
    {
        HAL_UARTEx_ReceiveToIdle(&huart1, Buff, 1024, UART_Rx_Len, 2000);
    }
    else
    {
        uint16_t TIMEOUT = 0;
        do
        {
            HAL_UARTEx_ReceiveToIdle(&huart1, Buff, 1024, UART_Rx_Len, 1000);
            TIMEOUT++;
            if (TIMEOUT >= 10)
                break;
        } while (strstr((char *)Buff, "OK") == NULL && strstr((char *)Buff, "ERROR") == NULL && strstr((char *)Buff, "ready") == NULL);
    }
    HAL_Delay(100);
}

void WI_FI_Init(void)
{
    // ESP8266_SendAt("AT\r\n", strlen("AT\r\n"), UART_Rx_Buffer, &UART_Rx_Len);
    // ESP8266_SendAt("AT+RST\r\n", strlen("AT+RST\r\n"), UART_Rx_Buffer, &UART_Rx_Len);
    // HAL_Delay(1000);

    uint8_t *AT = "AT+CWMODE=1\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    AT = "AT+CWJAP=\"HUAWEI-E106JI\",\"kx111111\"\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    AT = "AT+CIPSNTPCFG=1,8,\"ntp.aliyun.com\"\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);
}

char weather_text[40];
char weather_temp[10];
char weather_city[20];
char current_time[20];

uint8_t city_i = 0;
uint8_t Weather_i = 0;

// 【关键修复：必须把解析函数放在上面，供下面调用】
void Parse_Weather(void)
{
    char *p;
    uint8_t i;

    p = strstr((char *)UART_Rx_Buffer, "\"name\":\"");
    if (p != NULL)
    {
        sscanf(p + 8, "%19[^\"]", weather_city);
        for (i = 0; i < sizeof(my_City) / sizeof(my_City[0]); i++)
        {
            if (strstr(weather_city, my_City[i].utf8_code) != NULL)
            {
                city_i = i;
                break;
            }
        }
    }

    p = strstr((char *)UART_Rx_Buffer, "\"text\":\"");
    if (p != NULL)
    {
        sscanf(p + 8, "%39[^\"]", weather_text);
        uint8_t dict_size = sizeof(my_weather_dict) / sizeof(my_weather_dict[0]);
        for (i = 0; i < dict_size; i++)
        {
            if (strstr(weather_text, my_weather_dict[i].utf8_code) != NULL)
            {
                Weather_i = i;
                break;
            }
        }
    }

    p = strstr((char *)UART_Rx_Buffer, "\"temperature\":\"");
    if (p != NULL)
        sscanf(p + 15, "%9[^\"]", weather_temp);
}

void Parse_Time(void)
{
    char *p;

    p = strstr((char *)UART_Rx_Buffer, "+CIPSNTPTIME:");
    if (p != NULL)
    {
        // 【核心防御装甲】：如果返回的时间里包含 "1970"，说明 ESP8266 在糊弄我们！
        // 此时阿里云的时间还没发过来，绝不能写入！直接 return 退出。
        // 这样就能完美保留 STM32 RTC 硬件在复位前原本精准的时间！
        if (strstr((char *)UART_Rx_Buffer, "1970") != NULL)
        {
            return;
        }

        strncpy(current_time, p + 24, 8);
        current_time[8] = '\0';

        int hour, minute, second;
        if (sscanf(current_time, "%d:%d:%d", &hour, &minute, &second) == 3)
        {
            RTC_TimeTypeDef sTime = {0};
            sTime.Hours = (uint8_t)hour;
            sTime.Minutes = (uint8_t)minute;
            sTime.Seconds = (uint8_t)second;
            HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        }
    }
}

static uint8_t L = 0;
void ESP8266_GetWeather(void)
{
    uint8_t *AT = "AT+CIPMUX=0\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    AT = "AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    AT = "AT+CIPMODE=1\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    AT = "AT+CIPSEND\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    AT = (uint8_t *)my_City[L].AT;
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    // 【核心策略：拿到 JSON 数据后，趁着缓存没被清空，立刻在这里解析！】
    Parse_Weather();

    AT = "+++";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    AT = "AT+CIPCLOSE\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);
}

void ESP8266_GetTime(void)
{
    uint8_t *AT = "AT+CIPSNTPTIME?\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);

    // 【核心策略：拿到时间后，立刻解析并写入硬件 RTC！】
    Parse_Time();
}

void WiFi_PowerOff(void)
{
    uint8_t *AT = "AT+CWMODE=0\r\n";
    ESP8266_SendAt(AT, strlen((char *)AT), UART_Rx_Buffer, &UART_Rx_Len);
}

void Weather_Init(void)
{
    OLED_Clear();
    OLED_ShowChinese(16, 24, "正在获取天气");
    OLED_Update();

    WI_FI_Init();
    ESP8266_GetWeather();
    // 这里就不需要 Parse_Weather() 了，因为里面已经自动解析好了
    WiFi_PowerOff();
}

static uint8_t flag = 1;

void Weather_Loop(void)
{

    OLED_Clear();
    OLED_ShowChinese(0, 0, "城市");
    OLED_ShowChinese(0, 20, "温度");
    OLED_ShowChinese(0, 40, "天气");
    OLED_ShowImage(90, 0, 16, 16, JT);
    OLED_ShowImage(70, 0, 16, 16, Exit);

    OLED_ShowChinese(35, 0, (char *)my_City[city_i].name);
    OLED_ShowChinese(35, 40, (char *)my_weather_dict[Weather_i].name);
    OLED_ShowString(35, 20, weather_temp, OLED_8X16);
    OLED_ShowImage(52, 20, 16, 16, temp1);

    if (KEY_Check(0, KEY_DOWN))
    {
        flag--;
        if (flag <= 0)
            flag = 2;
    }
    else if (KEY_Check(1, KEY_DOWN))
    {
        flag++;
        if (flag >= 3)
            flag = 1;
    }
    else if (KEY_Check(2, KEY_DOWN))
    {
        if (flag == 1)
        {
            // 进入菜单
            NextMode = 10;
        }
        else if (flag == 2)
        {

            NextMode = 14;
        }
    }

    switch (flag)
    {
    case 1:
        OLED_ReverseArea(70, 0, 16, 16);
        break;
    case 2:
        OLED_ReverseArea(90, 0, 16, 16);
        break;
    }

    OLED_Update();
}

void Weather_Exit(void)
{
    WiFi_PowerOff();

    if (flag == 1)
    {
        OLED_Clear();
        OLED_Update();
    }
}

static uint8_t City_Flag = 1;

void City_Init(void)
{

}

void City_Loop(void)
{
    OLED_Clear();
    OLED_ShowChinese(0, 0, "城市");
    OLED_ShowChinese(0, 20, "温度");
    OLED_ShowChinese(0, 40, "天气");
    OLED_ShowImage(90, 0, 16, 16, JT);
    OLED_ShowImage(70, 0, 16, 16, Exit);

    OLED_ShowChinese(35, 0, (char *)my_City[city_i].name);
    OLED_ShowChinese(35, 40, (char *)my_weather_dict[Weather_i].name);
    OLED_ShowString(35, 20, weather_temp, OLED_8X16);
    OLED_ShowImage(52, 20, 16, 16, temp1);
    OLED_ReverseArea(90, 0, 16, 16);
    OLED_ShowChinese(90, 0, "贵阳");
    OLED_ShowChinese(90, 20, "北京");
    OLED_ShowChinese(90, 40, "遵义");

    if (KEY_Check(0, KEY_DOWN))
    {
        City_Flag--;
        if (City_Flag <= 0)
            City_Flag = 3;
    }
    else if (KEY_Check(1, KEY_DOWN))
    {
        City_Flag++;
        if (City_Flag >= 4)
            City_Flag = 1;
    }
    else if (KEY_Check(2, KEY_DOWN))
    {
        NextMode = 13;
    }

    switch (City_Flag)
    {
    case 1:

        L = 0;
        OLED_ReverseArea(90, 0, 32, 16);
        break;

    case 2:
        L = 1;
        OLED_ReverseArea(90, 20, 32, 16);
        break;
    case 3:
        L = 2;
        OLED_ReverseArea(90, 40, 32, 16);
        break;
    }
    OLED_Update();
}

void City_Exit(void)
{
    OLED_Clear();
    OLED_Update();
}
