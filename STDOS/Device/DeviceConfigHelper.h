#ifndef _SERIALPORTCONFIG_H
#define _SERIALPORTCONFIG_H
#include <stdint.h>
#include "Platform\Pin.h"
#include "Port.h"
#include "Core\Queue.h"

#define USECOM1
#define USECOM2
#define USECOM3
#define USECOM4
#define USECOM5

#if defined USECOM1

#define COM1TXDMAFLAG  //����1DMA����

#if defined COM1TXDMAFLAG
#else
#define COM1SENDINTFLAG	//����1�жϷ���
#endif

#define COM1RXDMAFLAG  //����1DMA����
#define COM1RCVIDLEINTFLAG	//����1�����жϽ���
#endif

#if defined USECOM2
#define COM2TXDMAFLAG  //����2DMA����
#if defined COM2TXDMAFLAG
#else
#define COM2SENDINTFLAG	//����2�жϷ���
#endif

#define COM2RXDMAFLAG  //����2DMA����
#define COM2RCVIDLEINTFLAG	//����2�����жϽ���
#endif

#if defined USECOM3
#define COM3TXDMAFLAG  //����3DMA����
#if defined COM3TXDMAFLAG
#else
#define COM3SENDINTFLAG	//����3�жϷ���
#endif


#define COM3RXDMAFLAG  //����3DMA����
#define COM3RCVIDLEINTFLAG	//����3�����жϽ���
#endif

#if defined USECOM4
#define COM4TXDMAFLAG  //����4DMA����
#if defined COM4TXDMAFLAG
#else
#define COM4SENDINTFLAG	//����4�жϷ���
#endif


#define COM4RXDMAFLAG  //����4DMA����
#define COM4RCVIDLEINTFLAG	//����4�����жϽ���
#endif

#if defined USECOM5
#define COM5TXDMAFLAG  //����5DMA����
#if defined COM5TXDMAFLAG
#else
#define COM5SENDINTFLAG	//����5�жϷ���
#endif
#define COM5RXDMAFLAG  //����5DMA����
#define COM5RCVIDLEINTFLAG	//����5�����жϽ���
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
	static void com1send();
	static void com2send();
	static void com3send();
	static void com4send();
	static void com5send();
	static void com1send(Buffer& bs);
	static void com2send(Buffer& bs);
	static void com3send(Buffer& bs);
	static void com4send(Buffer& bs);
	static void com5send(Buffer& bs);
	static void comSend(COM com, Buffer bs);//��������
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
			
	static int RcvLastTimeCOM1;//����1����������ʱ��
	static int RcvLastTimeCOM2;//����2����������ʱ��
	static int RcvLastTimeCOM3;//����3����������ʱ��
	static int RcvLastTimeCOM4;//����4����������ʱ��
	static int RcvLastTimeCOM5;//����5����������ʱ��

	static int BUFLEN_TX1;	//����1���ͻ���������
	static int BUFLEN_RX1;	//����1���ջ���������
	static int BUFLEN_TX2;	//����2���ͻ���������
	static int BUFLEN_RX2;	//����2���ջ���������
	static int BUFLEN_TX3;	//����3���ͻ���������
	static int BUFLEN_RX3;	//����3���ջ���������
	static int BUFLEN_TX4;	//����4���ͻ���������
	static int BUFLEN_RX4;	//����4���ջ���������
	static int BUFLEN_TX5;	//����5���ͻ���������
	static int BUFLEN_RX5;	//����5���ջ���������

	static int FLAG_TX1EN;	//����1���Է���
	static int FLAG_TX2EN;	//����2���Է���
	static int FLAG_TX3EN;	//����3���Է���
	static int FLAG_TX4EN;	//����4���Է���
	static int FLAG_TX5EN;	//����5���Է���
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

	static void Com1RcvRoutin(void *param);//����1�����ж�
	static void Com2RcvRoutin(void *param);//����1�����ж�
	static void Com3RcvRoutin(void *param);//����1�����ж�
	static void Com4RcvRoutin(void *param);//����1�����ж�
	static void Com5RcvRoutin(void *param);//����1�����ж�
	//��ʱ��
};
#ifdef __cplusplus
extern "C" {
#endif	
#if defined USECOM1
	extern Queue	Txx1;
	extern Queue	Rxx1;
#endif // USECOM1

#if defined USECOM2
	extern Queue	Txx2;
	extern Queue	Rxx2;
#endif // USECOM2

#if defined USECOM3
	extern Queue	Txx3;
	extern Queue	Rxx3;
#endif // USECOM3

#if defined USECOM4
	extern Queue	Txx4;
	extern Queue	Rxx4;
#endif // USECOM4

#if defined USECOM5
	extern Queue	Txx5;
	extern Queue	Rxx5;
#endif // USECOM5

#ifdef __cplusplus
}
#endif

#endif // !_SERIALPORTCONFIG_H
