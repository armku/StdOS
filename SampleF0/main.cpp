#include "Port.h"
#include "stm32f0xx.h"

void LED_GPIO(void);
void LED_Demo(void);
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);
void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
    //    led2 = key0;
}

#define namee "StdOS"

void main()
{
    TSys &sys = (TSys &)(Sys);
    sys.Name = (char*)namee;
    sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 

    LED_GPIO(); //硬件驱动初始化函数
//    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");


    while (1)
    {
        LED_Demo(); //调用LED流水灯例程
    }
    Sys.Start();
}

void LED_GPIO(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*选择要控制的GPIOC引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;

    /*设置要控制的GPIOC引脚为输出模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    /*设置引脚模式为上拉*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    /*调用库函数，初始化GPIOB*/
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    led1=0; //拉高熄灭
    led2=0; //拉高熄灭
    led3=0; //拉高熄灭
    led4=0; //拉高熄灭
}

void delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = 6000 * nms;
    SysTick->VAL = 0X00; //清空计数器
    SysTick->CTRL = 0X01; //使能，减到零是无动作，采用外部时钟源
    do
    {
        temp = SysTick->CTRL; //读取当前倒计数值
    }
    while ((temp &0x01) && (!(temp &(1 << 16)))); //等待时间到达
    SysTick->CTRL = 0x00; //关闭计数器
    SysTick->VAL = 0X00; //清空计数器
}

void LED_Demo(void)
{
    led1=1;; //拉低PC6引脚，LED1发光二极管(发光)
    delay_ms(500); //
    led1=0; //拉高PC6引脚，LED1发光二极管(熄灭)
    delay_ms(500);

    led4=1; //拉低PC9引脚，RGB发光二极管(发红色光)
    delay_ms(500);
    led4=0; //拉高PC9引脚，RGB发光二极管(红色熄灭)
    delay_ms(500);

    led3=1; //拉低PC8引脚，RGB发光二极管(发绿色光)
    delay_ms(500);
    led3=0; //拉低PC8引脚，RGB发光二极管(绿色熄灭)
    delay_ms(500);

    led2=1; //拉低PC7引脚，RGB发光二极管(发蓝色光)
    delay_ms(500);
    led2=0; //拉高PC7引脚，RGB发光二极管(蓝色熄灭)
    delay_ms(500);
}







#ifdef STM32F0TESTOLD


    #include <stdio.h>
    #include "SerialPort.h"
    #include "Drivers\AT24CXX.h"
    #include "Drivers\W25QXXX.h"
    #include "Drivers\Button.h"
    #include "SString.h"
    #include "string.h"
    #include "List.h"
    #include "Spi.h"
    #include "Flash.h"
    #include "Drivers\lcd_dr.h"
    #include "TInterrupt.h"


    const byte vers[] = "yyyy-MM-dd HH:mm:ss";
    #if 1
        OutputPort led1(PB0, false);
        OutputPort led2(PF7, false);
        OutputPort led3(PF8, false);
    #else 
        OutputPort led1(PD0, false);
        OutputPort led2(PD1, false);
        OutputPort led3(PD2, false);
        OutputPort ledLCD(PD12, false);
    #endif 

    void LedTask(void *param)
    {
        OutputPort *leds = (OutputPort*)param;
        *leds = ! * leds;
        //    led2 = key0;
    }

    #define namee "StdOS"
    void W25Q64Test();
    int main(void)
    {
        TSys &sys = (TSys &)(Sys);

        sys.Name = (char*)namee;
        byte aa = vers[0];
        aa = aa;

        sys.Init();
        #if DEBUG
            Sys.MessagePort = COM1;
            Sys.ShowInfo();
        #endif 
        W25Q64Test();
        Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

        Sys.Start();
    }
#endif
