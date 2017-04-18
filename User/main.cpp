#include <stdio.h>
#include "SerialPort.h"
#include "AT24CXX.h"
#include "Button.h"
#include "Port.h"
#include "WatchDog.h"
#include "SString.h"
#include "string.h"
#include "DateTime.h"
#include "List.h"
#include "Pwm.h"
#include "Modbus1.h"
#include "Spi.h"
#include "RTC.h"
#include "Flash.h"

typedef enum
{
    KEY_UP = 1,  // ģ������ 
    KEY_DOWN = 1 << 1,  // ����
    KEY_LEFT = 1 << 2,  // ��������
    KEY_RIGHT = 1 << 3,  // ����ʽ����
    KEY_OK = 1 << 4,  // ��©���
    KEY_MENU = 1 << 5,  // ����ʽ���
    KEY_NULL = 0
} KEY_MODE;

OutputPort led1(PB0, true);
OutputPort led2(PF7, true);
OutputPort led(PF8,true);

//���� PC13 PA0
//InputPort exti(PC13); //PA1 PB3     PA0 PC13
//InputPort exti1(PA0);
//void OnKeyPress(InputPort* pin, bool down, void *param)
//{
//    //led1.Write(onoff);
//    led2 = !led2;
////    printf("�ж����ţ�P%c%d ֵ%d \r\n", _PIN_NAME(pin), down);
//}

/*
ISO-V2:PB5����485����
ISO-V3:PC2����485����
 */
//ModbusSlave ModbusSlave;
//static uint OnUsartRead(ITransport *transport, Buffer &bs, void *para)
//{
//    ModbusSlave.Process(bs, para);
//    return 0;
//}
//static uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para)
//{
//   bs.Show(true);
//    return 0;
//}

void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
}

#define namee "StdOS"
void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
#define ON  0
#define OFF 1

/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_7);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_7)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_8);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_8)
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����GPIOB��GPIOF������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIOF����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;

		/*���ÿ⺯������ʼ��GPIOF7*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);
		
		/*ѡ��Ҫ���Ƶ�GPIOF����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

		/*���ÿ⺯������ʼ��GPIOF7*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);			  

		/* �ر�����led��	*/
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOF, GPIO_Pin_7|GPIO_Pin_8);	 
}
int main(void)
{
    TSys &sys = (TSys &)(Sys);
    //    #if 0
    //        sys.Codec = codec;
    //	#endif

    sys.Name = (char*)namee;
    //Rtc��ȡʱ��
    //    HardRtc *Rtc = HardRtc::Instance();
    //    Rtc->LowPower = false;
    //    Rtc->External = false;
    //    Rtc->Init();
    //    Rtc->Start(false, false);
    sys.Init();
    //    #if DEBUG
    //        Sys.MessagePort = COM1;
    //        Sys.ShowInfo();

    //        WatchDog::Start(20000, 10000);
    //    #else 
    //        WatchDog::Start();
    //    #endif 
    //    #if 0
    //        //flash ���һ����Ϊ������
    //        Config::Current = &Config::CreateFlash();
    //    #endif 

    //    exti.Register(OnKeyPress);
    //    exti1.Register(OnKeyPress);
    //	SerialPort::GetMessagePort()->Register(OnUsart1Read);

    // ��ʼ��Ϊ���
    

    //Sys.AddTask(LedTask, &led, 0, 500, "LedTask");

    //    Sys.Start();
	LED_GPIO_Config();
    led = 0;
        led1 = 0;
        led2 = 0;
	while (true)
    {
        LED1( ON );			  // ��
		Delay(0x0FFFFF);
		LED1( OFF );		  // ��

		LED2( ON );			  // ��
		Delay(0x0FFFFF);
		LED2( OFF );		  // ��

		LED3( ON );			  // ��
		Delay(0x0FFFFF);
		LED3( OFF );		  // ��	   
    }
}
