#include "Port.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

void delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = 6000 * nms;
    SysTick->VAL = 0X00; //��ռ�����
    SysTick->CTRL = 0X01; //ʹ�ܣ����������޶����������ⲿʱ��Դ
    do
    {
        temp = SysTick->CTRL; //��ȡ��ǰ������ֵ
    }
    while ((temp &0x01) && (!(temp &(1 << 16)))); //�ȴ�ʱ�䵽��
    SysTick->CTRL = 0x00; //�رռ�����
    SysTick->VAL = 0X00; //��ռ�����
}

void LED_Demo(void)
{
    led1=1;; //����PC6���ţ�LED1���������(����)
    //Sys.Sleep(500);//delay_ms(500); //
    led1=0; //����PC6���ţ�LED1���������(Ϩ��)
    delay_ms(500);

    led4=1; //����PC9���ţ�RGB���������(����ɫ��)
    delay_ms(500);
    led4=0; //����PC9���ţ�RGB���������(��ɫϨ��)
    delay_ms(500);

    led3=1; //����PC8���ţ�RGB���������(����ɫ��)
    delay_ms(500);
    led3=0; //����PC8���ţ�RGB���������(��ɫϨ��)
    delay_ms(500);

    led2=1; //����PC7���ţ�RGB���������(����ɫ��)
    delay_ms(500);
    led2=0; //����PC7���ţ�RGB���������(��ɫϨ��)
    delay_ms(500);
}
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

//    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");


    while (1)
    {
        LED_Demo(); //����LED��ˮ������
    }
    Sys.Start();
}
