#ifndef __Pwm_H__
#define __Pwm_H__

#include "Timer.h"

class AlternatePort;

// �����ȵ���
class Pwm : public Timer
{
public:
	uint16_t	Pulse[4];	// ÿ��ͨ����ռ�ձ�
	bool	Polarity	= true;	// ���ԡ�Ĭ��true�ߵ�ƽ
	bool	IdleState	= true;	// ����״̬��
    //uint32_t	Remap;		// ��ӳ�䡣0��ӳ�䣬����Ϊʵ��ӳ������
	AlternatePort*	Ports[4];
	uint8_t	Enabled[4];	// ÿ��ͨ���Ƿ�����

	Pwm(TIMER index);		// index ��ʱ�����
	virtual ~Pwm();

	virtual void Open();
	virtual void Close();
	virtual void Config();

	void Flush();	// ˢ�����

	void SetPulse(int idx, uint16_t pulse);
	// ����ռ�ձȣ�0~255
	void SetDuty(int idx, uint8_t duty);
	uint8_t GetDuty(int idx);
};

// PWM�������
class PwmData : public Pwm
{
public:
	// ��������ռ�ձ�
	uint16_t* Pulses;		// �������
	uint8_t	PulseCount;	// ��ȸ���
	uint8_t	Channel;	// ��Ҫ����������ͨ������֧����������1��ͨ����Ĭ��0��ʾ��һ��ͨ��
	uint8_t	PulseIndex;	// ������ʹ����������һ��λ�õ�����
	bool	Repeated;	// �Ƿ��ظ�

	PwmData(TIMER index);

	virtual void Config();

protected:
	virtual void OnInterrupt();
};

#endif
