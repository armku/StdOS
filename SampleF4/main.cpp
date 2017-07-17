#include "SerialPort.h"

OutputPort led1(PF9,false);
OutputPort led2(PF10,false);
InputPort key0(PE4,true);
//InputPort key0(PE4,true);
InputPort key1(PE3,true);
InputPort key2(PE2,true);
InputPort keyup(PA0,true,InputPort::DOWN);

void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;	
    led2 = keyup;
}
void SerialTest(void *param)
{
	static int i=0;
	debug_printf("hello %4d\r\n",i++);
}
void test();
int main(void)
{
	TSys &sys = (TSys &)(Sys);
	sys.Init();
	#if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 	
	//W25Q64Test();
	//test();
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	Sys.AddTask(SerialTest,0,0,1000,"Ledtest");

    Sys.Start();
}
#include "stm32f4xx.h" 
void test()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP对应引脚PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
}
