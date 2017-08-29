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
#include "Timer.h"
#include "TTime.h"
#include "Drivers\RX8025T.h"

class TTTTime
{
    public:
        uint Seconds; // 全局秒数，系统启动后总秒数。累加
        UInt64 Milliseconds; // 全局毫秒数，系统启动后总毫秒（1000ms整部分）。累加
        uint CurrentTicks; // 当前滴答时钟
        UInt64 Current; // 当前毫秒数
};
TTTTime now123;
void Test12(void *param)
{
    now123.Seconds = Time.Seconds;
    now123.Milliseconds = Time.Milliseconds;
    now123.Current = Time.Current();
    now123.CurrentTicks = Time.CurrentTicks();
}

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

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
    bs.Show(true);
    return 0;
}

uint time6cnt;
void TimerTask(void *param)
{
    static int i = 0;
    //printf("\r\n%d: cnt:%d", i++, time6cnt);
}

#define namee "StdOS"
void AT24C02Test();

RX8025T rx8025(PC1, PC2);
void ad71248Test();
DateTime now;

void Rx8025Refresh(void *param)
{
    RX8025T *rx = (RX8025T*)param;
    rx->LoadTime(now);
    now.Show();
    debug_printf("\r\n");
}

SerialPort sp2(COM2);
void com2test(void *param)
{
    sp2.SendData('A');
}

void com2tst();
int main(void)
{
    Sys.Name = (char*)namee;
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
    Sys.Init();
    if (now.Year < 2017)
    {
        now.Year = 2017;
        now.Month = 8;
        now.Day = 24;
        now.Hour = 11;
        now.Minute = 20;
        now.Second = 35;
        rx8025.SaveTime(now);
    }
    SerialPort::GetMessagePort()->Register(OnUsart1Read);
    //	AT24C02Test();    
    //ad71248Test();
    com2tst();

    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
    Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
    Sys.AddTask(Test12, 0, 600, 1000, "Test");
    Sys.AddTask(Rx8025Refresh, &rx8025, 100, 1000, "TimeUp");
    Sys.AddTask(com2test, 0, 1001, 1000, "comtest");

    Sys.Start();
}

void USARTx_Config(void);
void com2tst()
{
    sp2.Open();
//    USARTx_Config();
}

#include <stdarg.h>
#include "stm32f10x.h"

#define macUSART_BAUD_RATE                       115200

#define macUSARTx                                USART2
#define macUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define macUSART_CLK                             RCC_APB1Periph_USART2
#define macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define macUSART_GPIO_CLK                        (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)     
#define macUSART_TX_PORT                         GPIOA   
#define macUSART_TX_PIN                          GPIO_Pin_2
#define macUSART_RX_PORT                         GPIOA 
#define macUSART_RX_PIN                          GPIO_Pin_3
#define macUSART_IRQ                             USART2_IRQn
#define macUSART_INT_FUN                         USART1_IRQHandler



void Usart_SendStr_length(USART_TypeDef *pUSARTx, uint8_t *str, uint32_t strlen);
void Usart_SendString(USART_TypeDef *pUSARTx, uint8_t *str);


/// 配置USART2接收中断
static void NVIC_Configuration(void)
{
	return;
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = macUSART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：USARTx_Config
 * 描述  ：USART2 GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USARTx_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;

    /* config USART2 clock */
    //macUSART_APBxClock_FUN(macUSART_CLK, ENABLE);
//    macUSART_GPIO_APBxClock_FUN(macUSART_GPIO_CLK, ENABLE);

    /* USART2 GPIO config */
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = macUSART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);

    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);

    /* USART2 mode config */
//    USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//    USART_Init(macUSARTx, &USART_InitStructure);

//    /*	配置中断优先级 */
//    NVIC_Configuration();
//    /* 使能串口2接收中断 */
//    USART_ITConfig(macUSARTx, USART_IT_RXNE, ENABLE);

//    USART_Cmd(macUSARTx, ENABLE);
//    USART_ClearFlag(macUSARTx, USART_FLAG_TC);
}

/***************** 发送一个字符  **********************/
static void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
    /* 发送一个字节数据到USART1 */
    USART_SendData(pUSARTx, ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
        ;
}

/*****************  发送指定长度的字符串 **********************/
void Usart_SendStr_length(USART_TypeDef *pUSARTx, uint8_t *str, uint32_t strlen)
{
    unsigned int k = 0;
    do
    {
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    }
    while (k < strlen);
}


/*****************  发送字符串 **********************/
void Usart_SendString(USART_TypeDef *pUSARTx, uint8_t *str)
{
    unsigned int k = 0;
    do
    {
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    }
    while (*(str + k) != '\0');
}
