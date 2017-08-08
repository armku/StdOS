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

void BSP_Configuration(void)//硬件初始化函数
{	
	RS232_GPIO();
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
	Sys.AddTask(LedTask, &ledss, 0, 500, "LedTask");

    Sys.Start();
}
