//#include <stdio.h>
//#include "SerialPort.h"
//#include "Drivers\AT24CXX.h"
//#include "Drivers\W25QXXX.h"
//#include "Drivers\Button.h"
//#include "SString.h"
//#include "string.h"
//#include "List.h"
//#include "Spi.h"
//#include "Flash.h"
//#include "Drivers\lcd_dr.h"
//#include "TInterrupt.h"


//const byte vers[] = "yyyy-MM-dd HH:mm:ss";
//#if 1
//    OutputPort led1(PB0,false);
//    OutputPort led2(PF7,false);
//    OutputPort led3(PF8,false);
//#else 
//    OutputPort led1(PD0,false);
//    OutputPort led2(PD1,false);
//    OutputPort led3(PD2,false);
//	OutputPort ledLCD(PD12,false);
//#endif 

//void LedTask(void *param)
//{
//    OutputPort *leds = (OutputPort*)param;
//    *leds = ! * leds;
//    //    led2 = key0;
//}

//#define namee "StdOS"
//void W25Q64Test();
#include "stm32f4xx.h" 
void LED_Init(void);
int main(void)
{
//    TSys &sys = (TSys &)(Sys);

//    sys.Name = (char*)namee;
//    byte aa = vers[0];
//    aa = aa;

//    sys.Init();
//    #if DEBUG
//        Sys.MessagePort = COM1;
//        Sys.ShowInfo();
//    #endif 	
//	W25Q64Test();
//	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

//    Sys.Start();
	LED_Init();		        //初始化LED端口
	GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
	GPIO_SetBits(GPIOF,GPIO_Pin_10);   //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
	//delay_ms(500);  		   //延时300ms
	GPIO_SetBits(GPIOF,GPIO_Pin_9);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
	GPIO_ResetBits(GPIOF,GPIO_Pin_10); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	//delay_ms(500);   
	while(1)
	{
		    
	}
}
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 

#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10设置高，灯灭

}
