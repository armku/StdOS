#include "stm32f4xx.h" 
#include "Port.h"

//OutputPort led1(PF9);

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //使能GPIOF时钟

    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //LED0和LED1对应IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOF, &GPIO_InitStructure); //初始化GPIO

    GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10); //GPIOF9,F10设置高，灯灭
    GPIO_ResetBits(GPIOF, GPIO_Pin_9); //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
    GPIO_SetBits(GPIOF, GPIO_Pin_10); //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
    //delay_ms(500);  		   //延时300ms
    GPIO_SetBits(GPIOF, GPIO_Pin_9); //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
    GPIO_ResetBits(GPIOF, GPIO_Pin_10); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
    //delay_ms(500);   

}

int main(void)
{
    LED_Init(); //初始化LED端口
    while (1){

    }
}
