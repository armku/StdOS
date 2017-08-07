#include "Port.h"
#include "stm32f0xx.h"

void BSP_Configuration(void);
void LED_Demo(void);
//OutputPort led1(PC6, false);
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

    BSP_Configuration(); //硬件驱动初始化函数
//    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");


    while (1)
    {
        LED_Demo(); //调用LED流水灯例程
    }
    Sys.Start();
}

#define LED_1_PIN GPIO_Pin_6 //定义GPIO_Pin_6引脚为LED_1_PIN，相当于重新命名。
#define LED_B_PIN GPIO_Pin_7 //定义GPIO_Pin_7引脚为LED_B_PIN，相当于重新命名。
#define LED_G_PIN GPIO_Pin_8 //定义GPIO_Pin_8引脚为LED_G_PIN，相当于重新命名。
#define LED_R_PIN GPIO_Pin_9 //定义GPIO_Pin_9引脚为LED_R_PIN，相当于重新命名。
#define LED_PORT GPIOC  //定义GPIOC端口为LED_PORT，相当于重新命名。

//如果想用其他管脚控制LED,那么只需更改上面对应的端口与引脚编号即可

#define LED_1_L GPIO_ResetBits(LED_PORT,LED_1_PIN);    //定义LED_1_L,调用LED_1_L命令，PC6引脚输出低电平
#define LED_1_H GPIO_SetBits(LED_PORT,LED_1_PIN);      //定义LED_1_H,调用LED_1_H命令，PC6引脚输出低高平
#define LED_1_T GPIO_WriteBit(LED_PORT,LED_1_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_1_PIN)));//定义LED_1_T,调用LED_1_T命令，PC6引脚输出电平反转

#define LED_B_L GPIO_ResetBits(LED_PORT,LED_B_PIN);    //定义LED_B_L,调用LED_B_L命令，PC7引脚输出低电平
#define LED_B_H GPIO_SetBits(LED_PORT,LED_B_PIN);      //定义LED_B_H,调用LED_B_H命令，PC7引脚输出低高平
#define LED_B_T GPIO_WriteBit(LED_PORT,LED_B_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_B_PIN)));//定义LED_1_T,调用LED_1_T命令，PC7引脚输出电平反转

#define LED_G_L GPIO_ResetBits(LED_PORT,LED_G_PIN);    //定义LED_G_L,调用LED_G_L命令，PC8引脚输出低电平
#define LED_G_H GPIO_SetBits(LED_PORT,LED_G_PIN);      //定义LED_G_H,调用LED_G_H命令，PC8引脚输出低高平
#define LED_G_T GPIO_WriteBit(LED_PORT,LED_G_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_G_PIN)));//定义LED_1_T,调用LED_1_T命令，PC8引脚输出电平反转

#define LED_R_L GPIO_ResetBits(LED_PORT,LED_R_PIN);    //定义LED_R_L,调用LED_R_L命令，PC9引脚输出低电平
#define LED_R_H GPIO_SetBits(LED_PORT,LED_R_PIN);      //定义LED_R_H,调用LED_R_H命令，PC9引脚输出低高平
#define LED_R_T GPIO_WriteBit(LED_PORT,LED_R_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_R_PIN)));//定义LED_1_T,调用LED_1_T命令，PC9引脚输出电平反转



void RCC_Configuration(void) //时钟初始化函数
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
}

void LED_GPIO(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*选择要控制的GPIOC引脚*/
    GPIO_InitStructure.GPIO_Pin = LED_1_PIN | LED_B_PIN | LED_G_PIN | LED_R_PIN;

    /*设置要控制的GPIOC引脚为输出模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    /*设置引脚模式为上拉*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    /*调用库函数，初始化GPIOB*/
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    LED_1_H; //拉高熄灭
    LED_B_H; //拉高熄灭
    LED_G_H; //拉高熄灭
    LED_R_H; //拉高熄灭
}

/***********************************************************************
 *   函 数 名: BSP_Configuration
 *   功能说明: 初始化硬件设备。只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
 *   形    参：无
 *   返 回 值: 无
 ***********************************************************************/
void BSP_Configuration(void) //硬件驱动初始化函数
{
    RCC_Configuration(); //时钟初始化函数
    LED_GPIO(); //LED初始化函数
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
    LED_1_L; //拉低PC6引脚，LED1发光二极管(发光)
    delay_ms(500); //
    LED_1_H; //拉高PC6引脚，LED1发光二极管(熄灭)
    delay_ms(500);

//    LED_R_L; //拉低PC9引脚，RGB发光二极管(发红色光)
//    delay_ms(500);
//    LED_R_H; //拉高PC9引脚，RGB发光二极管(红色熄灭)
//    delay_ms(500);

//    LED_G_L; //拉低PC8引脚，RGB发光二极管(发绿色光)
//    delay_ms(500);
//    LED_G_H; //拉低PC8引脚，RGB发光二极管(绿色熄灭)
//    delay_ms(500);

//    LED_B_L; //拉低PC7引脚，RGB发光二极管(发蓝色光)
//    delay_ms(500);
//    LED_B_H; //拉高PC7引脚，RGB发光二极管(蓝色熄灭)
//    delay_ms(500);
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
