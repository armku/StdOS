#include "Port.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[]={led1,led2,led3,led4};

void LedTask(void *param)
{
	static byte flag=1;
	OutputPort *leds = (OutputPort*)param;
	
	flag<<=1;
	flag&=0x0f;
	if(flag==0)
	{
		flag=1;
	}
	
	leds[0]=flag&0x01;
	leds[1]=flag&0x02;
	leds[2]=flag&0x04;
	leds[3]=flag&0x08;
	
    //    led2 = key0;
}

#define namee "StdOS"


void RCC_Configuration(void)//时钟初始化函数
{  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //设置A端口时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //设置串口1时钟使能
}

static void NVIC_Configuration(void)//中断优先级初始化函数
{
	NVIC_InitTypeDef   NVIC_InitStructure;//定义一个NVIC_InitTypeDef类型的结构体

	// rs232
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void RS232_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;//定义一个GPIO_InitTypeDef类型的结构体
	
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //定义PA9复用功能
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);  //定义PA10复用功能 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//选择要控制的GPIOA引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

void RS232_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

void BSP_Configuration(void)//硬件初始化函数
{	
	RCC_Configuration();
	RS232_GPIO();
	RS232_Configuration();
}
void Company_Information(void)//printf打印字符设计
{
	printf("******************************【众拳-独角兽STM32F072VB开发板】********************************\r\n"); 
    printf("　论坛网址: http://www.zxkjmcu.com\r\n"); 
    printf("　淘宝网店: http://zxkjmcu.taobao.com\r\n"); 
    printf("　关注微信公众平台微信号【zxkj-ly】\r\n");     
    printf("　STM32F0开发板技术交流QQ群【373645551】\r\n"); 
    printf("　哈尔滨卓恩科技开发有限公司\r\n"); 
    printf("*************************************************************************************************************\r\n"); 
}
int main()
{
	Sys.Name = (char*)namee;
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
	BSP_Configuration(); //调用硬件初始化函数
	NVIC_Configuration(); //调用中断优先级初始化函数
	Company_Information();
	Sys.AddTask(LedTask, &ledss, 0, 500, "LedTask");

    Sys.Start();
}
