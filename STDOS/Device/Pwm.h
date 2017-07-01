#ifndef __Pwm_H__
#define __Pwm_H__

#include "Timer.h"

class AlternatePort;

// �����ȵ���
class Pwm : public Timer
{
public:
	ushort	Pulse[4];	// ÿ��ͨ����ռ�ձ�
	bool	Polarity	= true;	// ���ԡ�Ĭ��true�ߵ�ƽ
	bool	IdleState	= true;	// ����״̬��
    //uint	Remap;		// ��ӳ�䡣0��ӳ�䣬����Ϊʵ��ӳ������
	AlternatePort*	Ports[4];
	byte	Enabled[4];	// ÿ��ͨ���Ƿ�����

	Pwm(TIMER index);		// index ��ʱ�����
	virtual ~Pwm();

	virtual void Open();
	virtual void Close();
	virtual void Config();

	void Flush();	// ˢ�����

	void SetPulse(int idx, ushort pulse);
	// ����ռ�ձȣ�0~255
	void SetDuty(int idx, byte duty);
	byte GetDuty(int idx);
};

// PWM�������
class PwmData : public Pwm
{
public:
	// ��������ռ�ձ�
	ushort* Pulses;		// �������
	byte	PulseCount;	// ��ȸ���
	byte	Channel;	// ��Ҫ����������ͨ������֧����������1��ͨ����Ĭ��0��ʾ��һ��ͨ��
	byte	PulseIndex;	// ������ʹ����������һ��λ�õ�����
	bool	Repeated;	// �Ƿ��ظ�

	PwmData(TIMER index);

	virtual void Config();

protected:
	virtual void OnInterrupt();
};

#endif










#pragma once 
#include "Type.h"
#include "Port.h"

class PWM:private AlternatePort
{
    public:
        PWM(Pin pin, uint frq = 10000, uint duty = 50,uint prescaler=0,bool voltlevellow=true);
        void Init();
		void SetOut(ushort pwmValue);//�������ֵ
		void SetOutPercent(float per);//�������������0-100		
    private:
        uint freq;//Ƶ��
        uint duty;//���ֵ
		uint prescal;//Ԥ��Ƶ
		uint voltLevelLow;//����ʱ������ֵС�ڱȽ�ֵʱΪ�͵�ƽ
};
