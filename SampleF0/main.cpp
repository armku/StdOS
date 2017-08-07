#include "Port.h"
#include "stm32f0xx.h"

void BSP_Configuration(void);
void LED_Demo(void);
//OutputPort led1(PC6, false);
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

    BSP_Configuration(); //Ӳ��������ʼ������
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

//������������ܽſ���LED,��ôֻ����������Ӧ�Ķ˿������ű�ż���

#define LED_1_L GPIO_ResetBits(LED_PORT,LED_1_PIN);    //����LED_1_L,����LED_1_L���PC6��������͵�ƽ
#define LED_1_H GPIO_SetBits(LED_PORT,LED_1_PIN);      //����LED_1_H,����LED_1_H���PC6��������͸�ƽ
#define LED_1_T GPIO_WriteBit(LED_PORT,LED_1_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_1_PIN)));//����LED_1_T,����LED_1_T���PC6���������ƽ��ת

#define LED_B_L GPIO_ResetBits(LED_PORT,LED_B_PIN);    //����LED_B_L,����LED_B_L���PC7��������͵�ƽ
#define LED_B_H GPIO_SetBits(LED_PORT,LED_B_PIN);      //����LED_B_H,����LED_B_H���PC7��������͸�ƽ
#define LED_B_T GPIO_WriteBit(LED_PORT,LED_B_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_B_PIN)));//����LED_1_T,����LED_1_T���PC7���������ƽ��ת

#define LED_G_L GPIO_ResetBits(LED_PORT,LED_G_PIN);    //����LED_G_L,����LED_G_L���PC8��������͵�ƽ
#define LED_G_H GPIO_SetBits(LED_PORT,LED_G_PIN);      //����LED_G_H,����LED_G_H���PC8��������͸�ƽ
#define LED_G_T GPIO_WriteBit(LED_PORT,LED_G_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_G_PIN)));//����LED_1_T,����LED_1_T���PC8���������ƽ��ת

#define LED_R_L GPIO_ResetBits(LED_PORT,LED_R_PIN);    //����LED_R_L,����LED_R_L���PC9��������͵�ƽ
#define LED_R_H GPIO_SetBits(LED_PORT,LED_R_PIN);      //����LED_R_H,����LED_R_H���PC9��������͸�ƽ
#define LED_R_T GPIO_WriteBit(LED_PORT,LED_R_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_R_PIN)));//����LED_1_T,����LED_1_T���PC9���������ƽ��ת



void RCC_Configuration(void) //ʱ�ӳ�ʼ������
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
}

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

    LED_1_H; //����Ϩ��
    LED_B_H; //����Ϩ��
    LED_G_H; //����Ϩ��
    LED_R_H; //����Ϩ��
}

/***********************************************************************
 *   �� �� ��: BSP_Configuration
 *   ����˵��: ��ʼ��Ӳ���豸��ֻ��Ҫ����һ�Ρ��ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����
 *   ��    �Σ���
 *   �� �� ֵ: ��
 ***********************************************************************/
void BSP_Configuration(void) //Ӳ��������ʼ������
{
    RCC_Configuration(); //ʱ�ӳ�ʼ������
    LED_GPIO(); //LED��ʼ������
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
    LED_1_L; //����PC6���ţ�LED1���������(����)
    delay_ms(500); //
    LED_1_H; //����PC6���ţ�LED1���������(Ϩ��)
    delay_ms(500);

//    LED_R_L; //����PC9���ţ�RGB���������(����ɫ��)
//    delay_ms(500);
//    LED_R_H; //����PC9���ţ�RGB���������(��ɫϨ��)
//    delay_ms(500);

//    LED_G_L; //����PC8���ţ�RGB���������(����ɫ��)
//    delay_ms(500);
//    LED_G_H; //����PC8���ţ�RGB���������(��ɫϨ��)
//    delay_ms(500);

//    LED_B_L; //����PC7���ţ�RGB���������(����ɫ��)
//    delay_ms(500);
//    LED_B_H; //����PC7���ţ�RGB���������(��ɫϨ��)
//    delay_ms(500);
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
