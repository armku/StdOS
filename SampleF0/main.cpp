#include "Port.h"
#include "stm32f0xx.h"

void LED_GPIO(void);
void LED_Demo(void);
OutputPort led1(PC6, false);
OutputPort led2(PC7, false);
OutputPort led3(PC8, false);
OutputPort led4(PC9, false);
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

    LED_GPIO(); //Ӳ��������ʼ������
//    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");


    while (1)
    {
        LED_Demo(); //����LED��ˮ������
    }
    Sys.Start();
}

#define LED_1_PIN GPIO_Pin_6 //����GPIO_Pin_6����ΪLED_1_PIN���൱������������
#define LED_B_PIN GPIO_Pin_7 //����GPIO_Pin_7����ΪLED_B_PIN���൱������������
#define LED_G_PIN GPIO_Pin_8 //����GPIO_Pin_8����ΪLED_G_PIN���൱������������
#define LED_R_PIN GPIO_Pin_9 //����GPIO_Pin_9����ΪLED_R_PIN���൱������������
#define LED_PORT GPIOC  //����GPIOC�˿�ΪLED_PORT���൱������������

void LED_GPIO(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*ѡ��Ҫ���Ƶ�GPIOC����*/
    GPIO_InitStructure.GPIO_Pin = LED_1_PIN | LED_B_PIN | LED_G_PIN | LED_R_PIN;

    /*����Ҫ���Ƶ�GPIOC����Ϊ���ģʽ*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    /*��������ģʽΪ����*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    /*���ÿ⺯������ʼ��GPIOB*/
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    led1=1; //����Ϩ��
    led2=1; //����Ϩ��
    led3=1; //����Ϩ��
    led4=1; //����Ϩ��
}

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
    led1=0;; //����PC6���ţ�LED1���������(����)
    delay_ms(500); //
    led1=1; //����PC6���ţ�LED1���������(Ϩ��)
    delay_ms(500);

    led4=0; //����PC9���ţ�RGB���������(����ɫ��)
    delay_ms(500);
    led4=1; //����PC9���ţ�RGB���������(��ɫϨ��)
    delay_ms(500);

    led3=0; //����PC8���ţ�RGB���������(����ɫ��)
    delay_ms(500);
    led3=1; //����PC8���ţ�RGB���������(��ɫϨ��)
    delay_ms(500);

    led2=0; //����PC7���ţ�RGB���������(����ɫ��)
    delay_ms(500);
    led2=1; //����PC7���ţ�RGB���������(��ɫϨ��)
    delay_ms(500);
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
