#include "stm32f4xx.h" 
#include "Port.h"

OutputPort led1(PF9,false);
OutputPort led2(PF10,false);

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //GPIOF9,F10��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //LED0��LED1��ӦIO��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOF, &GPIO_InitStructure); //��ʼ��GPIO

}

int main(void)
{
    LED_Init(); //��ʼ��LED�˿�
	led1=0;
	led2=0;
	led1=1;
	//led2=1;
    while (1)
	{

    }
}
