#include "stm32f4xx.h" 
#include "Port.h"

OutputPort led1(PF9,false);
OutputPort led2(PF10,false);

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //LED0和LED1对应IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOF, &GPIO_InitStructure); //初始化GPIO

}

int main(void)
{
    LED_Init(); //初始化LED端口
	led1=0;
	led2=0;
	led1=1;
	//led2=1;
    while (1)
	{

    }
}
