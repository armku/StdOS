#ifndef _SERIALPORTCONFIG_H
#define _SERIALPORTCONFIG_H

//#define USE_GPS
//#ifdef USE_GPS
//#include "GPS.h"
//#endif

//~~~~~~~~~~~~~~~USART Congfigure~~~~~~~~~~~//
//#define USE_USART              //---USART-without-DMA--//
//#define USE_USART1             //---USART1-without-DMA-//
//#define USE_USART2            //---USART2-without-DMA-//
//#define USE_USART3            //---USART3-without-DMA-//

//#define USE_USART_DMA          //---USART-use-DMA--//
//#define USE_USART1_DMA         //---USART1-use-DMA--//
//#define USE_USART2_DMA         //---USART2-use-DMA--//
//#define USE_USART3_DMA         //---USART3-use-DMA--//

//~~~~~~~~~~~~~~~~I2C Congfigure~~~~~~~~~~~~//
//#define USE_I2C                //----IIC----//
//#define USE_I2C1             	 //----IIC1---//
//#define USE_I2C2               //----IIC2---//

//~~~~~~~~~~~~~~~Timer Congfigure~~~~~~~~~~~//
//#define USE_TIMER              //---Timer---//
//#define USE_TIMER1              //---Timer1--//
//#define USE_TIMER2             //---Timer2--//
//#define USE_TIMER3            //---Timer3--//
//#define USE_TIMER4            //---Timer4--//

//~~~~~~~~~~~~~~Capture Congfigure~~~~~~~~~~//
//#define USE_CAPTURE_TIM         //--Capture- With Timer-//
//#define USE_CAPTURE_TIM1       //--ICP-TIM1-//
//#define USE_CAPTURE_TIM2       //--ICP-TIM2-//
//#define USE_CAPTURE_TIM3       //--ICP-TIM3-//
//#define USE_CAPTURE_TIM4       //--ICP-TIM4-//

//#define USE_CAPTURE_EXIT        //--Capture-With EXIT-//
//#define USE_CAPTURE_EXIT0       //--ICP-EXIT0//
//#define USE_CAPTURE_EXIT1       //--ICP-EXIT1//
//#define USE_CAPTURE_EXIT2       //--ICP-EXIT2//
//#define USE_CAPTURE_EXIT3       //--ICP-EXIT3//
//#define USE_CAPTURE_EXIT4       //--ICP-EXIT4//
//#define USE_CAPTURE_EXIT5       //--ICP-EXIT5//
//#define USE_CAPTURE_EXIT6       //--ICP-EXIT6//
//#define USE_CAPTURE_EXIT7       //--ICP-EXIT7//
//#define USE_CAPTURE_EXIT8       //--ICP-EXIT8//
//#define USE_CAPTURE_EXIT9       //--ICP-EXIT9//
//#define USE_CAPTURE_EXIT10      //--ICP-EXIT10//
//#define USE_CAPTURE_EXIT11      //--ICP-EXIT11//
//#define USE_CAPTURE_EXIT12      //--ICP-EXIT12//
//#define USE_CAPTURE_EXIT13      //--ICP-EXIT13//
//#define USE_CAPTURE_EXIT14      //--ICP-EXIT14//
//#define USE_CAPTURE_EXIT15      //--ICP-EXIT15//


//~~~~~~~~~~~~~~~~EXTI Congfigure~~~~~~~~~~~//
//#define USE_EXTI0            //---EXTI0---//
//#define USE_EXTI1            //---EXTI1---//
//#define USE_EXTI2            //---EXTI2---//
//#define USE_EXTI3            //---EXTI3---//
//#define USE_EXTI4            //---EXTI4---//
//#define USE_EXTI5            //---EXTI5---//
//#define USE_EXTI6            //---EXTI6---//
//#define USE_EXTI7            //---EXTI7---//
//#define USE_EXTI8            //---EXTI8---//
//#define USE_EXTI9            //---EXTI9---//
//#define USE_EXTI10           //---EXTI10--//
//#define USE_EXTI11           //---EXTI11--//
//#define USE_EXTI12           //---EXTI12--//
//#define USE_EXTI13           //---EXTI13--//
//#define USE_EXTI14           //---EXTI14--//
//#define USE_EXTI15           //---EXTI15--//

//~~~~~~~~~~~~~Ultrasonic Congfigure~~~~~~~~~//
//#define USE_ULTRASONIC         //-Ultrasonic-//
//#define USE_ULTRASONIC_EXTI0   //-Ultr-EXTI0-//
//#define USE_ULTRASONIC_EXTI1   //-Ultr-EXTI1-//
//#define USE_ULTRASONIC_EXTI2   //-Ultr-EXTI2-//
//#define USE_ULTRASONIC_EXTI3   //-Ultr-EXTI3-//
//#define USE_ULTRASONIC_EXTI4   //-Ultr-EXTI4-//
//#define USE_ULTRASONIC_EXTI5   //-Ultr-EXTI5-//
//#define USE_ULTRASONIC_EXTI6   //-Ultr-EXTI6-//
//#define USE_ULTRASONIC_EXTI7   //-Ultr-EXTI7-//
//#define USE_ULTRASONIC_EXTI8   //-Ultr-EXTI8-//
//#define USE_ULTRASONIC_EXTI9   //-Ultr-EXTI9-//
//#define USE_ULTRASONIC_EXTI10  //-Ultr-EXTI10-//
//#define USE_ULTRASONIC_EXTI11  //-Ultr-EXTI11-//
//#define USE_ULTRASONIC_EXTI12  //-Ultr-EXTI12-//
//#define USE_ULTRASONIC_EXTI13  //-Ultr-EXTI13-//
//#define USE_ULTRASONIC_EXTI14  //-Ultr-EXTI14-//
//#define USE_ULTRASONIC_EXTI15  //-Ultr-EXTI15-//


#ifdef USE_USART
#include "USART.h"
#endif
#ifdef USE_USART_DMA
#include "USART.h"
#endif

#ifdef USE_I2C
#include "I2C.h"
#endif

#ifdef USE_CAPTURE_TIM
#include "InputCapture_TIM.h"
#endif

#ifdef USE_CAPTURE_EXIT
#include "InputCapture_EXIT.h"
#endif

#ifdef USE_TIMER
#include "Timer.h"
#endif

#ifdef USE_ULTRASONIC
#include "Ultrasonic.h"
#endif





#include <stdint.h>
#include "Platform\Pin.h"
#include "Port.h"
#include "Core\Queue.h"

//#define USECOM1
//#define USECOM2
//#define USECOM3
//#define USECOM4
//#define USECOM5

#if defined USECOM1
#define USECOM1RXD	//���Խ���
#define USECOM1TXD	//���Է���
#endif

#if defined USECOM2
#define USECOM2RXD	//���Խ���
#define USECOM2TXD	//���Է���
#endif

#if defined USECOM3
#define USECOM3RXD	//���Խ���
#define USECOM3TXD	//���Է���
#endif

#if defined USECOM4
#define USECOM4RXD	//���Խ���
#define USECOM4TXD	//���Է���
#endif

#if defined USECOM5
#define USECOM5RXD	//���Խ���
#define USECOM5TXD	//���Է���
#endif

#if defined USECOM1TXD
//#define COM1TXDMAFLAG  //DMA����
#if (!defined COM1TXDMAFLAG)
//#define COM1SENDINTFLAG	//�жϷ���
#endif
#endif

#if defined USECOM1RXD
//#define COM1RXDMAFLAG  //DMA����
//#define COM1RCVIDLEINTFLAG	//�����жϽ���
#endif

#if defined USECOM2TXD
//#define COM2TXDMAFLAG  //DMA����
#if (!defined COM2TXDMAFLAG)
#define COM2SENDINTFLAG	//�жϷ���
#endif
#endif

#if defined USECOM2RXD
//#define COM2RXDMAFLAG  //DMA����
#define COM2RCVIDLEINTFLAG	//�����жϽ���
#endif

#if defined USECOM3TXD
//#define COM3TXDMAFLAG  //DMA����
#if (!defined COM3TXDMAFLAG)
//#define COM3SENDINTFLAG	//�жϷ���
#endif
#endif

#if defined USECOM3RXD
//#define COM3RXDMAFLAG  //DMA���� �쳣
//#define COM3RCVIDLEINTFLAG	//�����жϽ���
#endif


#if defined USECOM4TXD
//#define COM4TXDMAFLAG  //DMA����
#if (!defined COM4TXDMAFLAG)
//#define COM4SENDINTFLAG	//�жϷ���
#endif
#endif

#if defined USECOM4RXD
//#define COM4RXDMAFLAG  //DMA����
//#define COM4RCVIDLEINTFLAG	//�����жϽ���
#endif

#if defined USECOM5TXD
//#define COM5TXDMAFLAG  //DMA����
#if (!defined COM5TXDMAFLAG)
//#define COM5SENDINTFLAG	//�жϷ���
#endif
#endif
#if defined USECOM5RXD
//#define COM5RXDMAFLAG  //DMA����
//#define COM5RCVIDLEINTFLAG	//�����жϽ���
#endif

typedef enum
{
	Rising = 0x01,	// ������
	Falling = 0x02,	// �½���
	Both = 0x03	// �����½���
}Trigger;

class DeviceConfigCenter
{
public:
	//�ж�
	static void SetEXIT(int pinIndex, bool enable, Trigger trigger = Both);
	static void InputPort_OpenEXTI(Pin pin, Trigger trigger = Both);

	static Func PExit0;
	static Func PExit1;
	static Func PExit2;
	static Func PExit3;
	static Func PExit4;
	static Func PExit5;
	static Func PExit6;
	static Func PExit7;
	static Func PExit8;
	static Func PExit9;
	static Func PExit10;
	static Func PExit11;
	static Func PExit12;
	static Func PExit13;
	static Func PExit14;
	static Func PExit15;

	//����	
	static void ConfigCom(COM com, int baudRate);
	static void ComChgBaudRate(COM com, int baudRate);//�ı䲨����
	static void com1send(void* buf, int len);
	static void com2send(void* buf, int len);
	static void com3send(void* buf, int len);
	static void com4send(void* buf, int len);
	static void com5send(void* buf, int len);
	static void comSend(COM com, void* buf, int len);//��������
	static OutputPort *pCOM1Rx485;
	static OutputPort *pCOM2Rx485;
	static OutputPort *pCOM3Rx485;
	static OutputPort *pCOM4Rx485;
	static OutputPort *pCOM5Rx485;

	static Func PRcvCOM1;
	static Func PRcvCOM2;
	static Func PRcvCOM3;
	static Func PRcvCOM4;
	static Func PRcvCOM5;

	static int RcvLastTimeCOM1;//����������ʱ��
	static int RcvLastTimeCOM2;//����������ʱ��
	static int RcvLastTimeCOM3;//����������ʱ��
	static int RcvLastTimeCOM4;//����������ʱ��
	static int RcvLastTimeCOM5;//����������ʱ��

	static int RcvCom1PackInterval;//��������֡ʱ����
	static int RcvCom2PackInterval;//��������֡ʱ����
	static int RcvCom3PackInterval;//��������֡ʱ����
	static int RcvCom4PackInterval;//��������֡ʱ����
	static int RcvCom5PackInterval;//��������֡ʱ����

	static int BUFLEN_TX1;	//���ͻ���������
	static int BUFLEN_RX1;	//���ջ���������
	static int BUFLEN_TX2;	//���ͻ���������
	static int BUFLEN_RX2;	//���ջ���������
	static int BUFLEN_TX3;	//���ͻ���������
	static int BUFLEN_RX3;	//���ջ���������
	static int BUFLEN_TX4;	//���ͻ���������
	static int BUFLEN_RX4;	//���ջ���������
	static int BUFLEN_TX5;	//���ͻ���������
	static int BUFLEN_RX5;	//���ջ���������

	static int FLAG_TX1EN;	//���Է���
	static int FLAG_TX2EN;	//���Է���
	static int FLAG_TX3EN;	//���Է���
	static int FLAG_TX4EN;	//���Է���
	static int FLAG_TX5EN;	//���Է���
	//��ʱ��
	static void TimeTickInit();//ϵͳ�ö�ʱ����ʼ��
	static void TimerConfig(TIMER tim, int interval, int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);//��ʱ������
	static void Timer0Config(int interval);//��ʱ������
	static void Timer1Config(int interval);//��ʱ������
	static void Timer2Config(int interval);//��ʱ������
	static void Timer3Config(int interval);//��ʱ������
	static void Timer4Config(int interval);//��ʱ������
	static void Timer5Config(int interval);//��ʱ������
	static void Timer6Config(int interval);//��ʱ������
	static void Timer7Config(int interval);//��ʱ������
	static void Timer8Config(int interval);//��ʱ������
	static void TimerConfigNvic(TIMER tim, int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer0ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer1ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer2ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer3ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer4ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer5ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer6ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer7ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer8ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);

	static Func PTim2Update;
	static Func PTim3Update;
	static Func PTim4Update;
	static Func PTim5Update;
	static Func PTim6Update;
	static Func PTim7Update;
	static Func PTim8Update;
	//ϵͳ
	static int CurrentTick();
	static uint32_t CurrentTicks1();

private:
	//�ж�

	//����
	static void configCOM1(int baudRate);
	static void configCOM2(int baudRate);
	static void configCOM3(int baudRate);
	static void configCOM4(int baudRate);
	static void configCOM5(int baudRate);
	static void Com1ChgBaudRate(int baudRate);
	static void Com2ChgBaudRate(int baudRate);
	static void Com3ChgBaudRate(int baudRate);
	static void Com4ChgBaudRate(int baudRate);
	static void Com5ChgBaudRate(int baudRate);
	static void SerialPort_GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap = false);

	//��ʱ��
};
#ifdef __cplusplus
extern "C" {
#endif	
#if defined USECOM1TXD
	extern Queue	Txx1;
#endif
#if defined USECOM1RXD
	extern Queue	Rxx1;
#endif 

#if defined USECOM2TXD
	extern Queue	Txx2;
#endif
#if defined USECOM2RXD
	extern Queue	Rxx2;
#endif

#if defined USECOM3TXD
	extern Queue	Txx3;
#endif
#if defined USECOM3RXD
	extern Queue	Rxx3;
#endif

#if defined USECOM4TXD
	extern Queue	Txx4;
#endif
#if defined USECOM4RXD
	extern Queue	Rxx4;
#endif

#if defined USECOM5TXD
	extern Queue	Txx5;
#endif
#if defined USECOM5RXD
	extern Queue	Rxx5;
#endif

#ifdef __cplusplus
}
#endif

#endif // !_SERIALPORTCONFIG_H
