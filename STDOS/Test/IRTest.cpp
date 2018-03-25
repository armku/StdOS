//#include "App\IR.h"
#include "Kernel\Sys.h"

//#define _IR_TEST_H
#ifdef _IR_TEST_H
/* ����
Pwm * IRFREQ()
{
	// Pwm ���ų�ʼ��
	AlternatePort* irpin = new AlternatePort(PA8);
	irpin->Set(PA8);
#if defined(STM32F0) || defined(STM32F4)
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_2);
#endif
	Pwm * IRFreq = new Pwm(0x00);		// ����
	//  38KHz ռ�ձ� 50%
//	IRFreq->SetFrequency(38000);	// ���ز�׼ȷ 103ve��ʵ�����ֻ��25KHz
#if defined(STM32F0)
	IRFreq->Prescaler = 0x04;	// ok
	IRFreq->Period	= 0x13c;
#elif defined(STM32F1)
	IRFreq->Prescaler = 0x06;	// ok
	IRFreq->Period	= 0x13c;
#else
	IRFreq->Prescaler = 0x0D;	// �����д���
	IRFreq->Period	= 0x13c;
#endif
	IRFreq->Pulse[0]=IRFreq->Period/2;		// ����
//	IRFreq->Start();
	return IRFreq;
}*/

//����C
//PB1			HIRPWM  (TIME3 CH4)
Pwm* IRFREQ()
{
	new AlternatePort(PB1);

	auto IRFreq	= new Pwm(Timer3);
	//  38KHz ռ�ձ� 50%
//	IRFreq->SetFrequency(38000);	// ���ز�׼ȷ 103ve��ʵ�����ֻ��25KHz
	IRFreq->Prescaler = 0x06;
	IRFreq->Period	= 0x13c;
	IRFreq->Pulse[3]	= IRFreq->Period/2;
	IRFreq->Enabled[3]	= true;
//	IRFreq->Start();
	return IRFreq;
}

void IRTest()
{
	auto HIRPOWER	= new OutputPort(PE11,false,true);
	*HIRPOWER = false;

	/*auto ir	= new IR(IRFREQ(), PE10, PE15);
	uint8_t Recbuff[512];
	for(int i =0;i<sizeof(Recbuff);i++)Recbuff[i]=0x00;
	int length=0;
	length = ir->Receive(Recbuff);

	Sys.Sleep(1000);
	while(true)
	{
		ir->Send(Recbuff,length);
		Sys.Sleep(1000);
	}*/
}
#endif
