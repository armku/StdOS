#ifndef __Timer_H__
#define __Timer_H__

#include "Kernel\Sys.h"
#include "Device\Port.h"
#include "stm32f10x.h"
// ��ʱ��
class Timer
{
	
protected:
	byte	_index;		// �ڼ�����ʱ������0��ʼ
	Delegate<Timer&>	OnTick;	// ��this����

	void SetHandler(bool set);
public:
	void*	_Timer;
	bool	Opened;	// �������ж���ر��Լ�

	ushort	Prescaler;	// Ԥ��Ƶ��ʵ��ֵ����ʱ�����һ��
	uint	Period;		// ���ڡ�ʵ��ֵ����ʱ�����һ��

	Timer(TIMER index);
	virtual ~Timer();

	virtual void Open();	// ��ʼ��ʱ��
	virtual void Close();	// ֹͣ��ʱ��
	virtual void Config();
	void SetFrequency(uint frequency);	// ����Ƶ�ʣ��Զ�����Ԥ��Ƶ

	uint GetCounter();
	void SetCounter(uint cnt);		// ���ü�����ֵ

	void Register(const Delegate<Timer&>& dlg);
	virtual void OnInterrupt();

	static void ClockCmd(int idx, bool state);

private:
	static void OnHandler(ushort num, void* param);

protected:

public:
	static const byte	TimerCount;	// ��ʱ������

	static Timer* Create(byte index = 0xFF);	// ����ָ�������Ķ�ʱ�������������ֱ�ӷ��أ�Ĭ��0xFF��ʾ�������

private:
	void OnInit();
	void OnOpen();
	void OnClose();

	static const void* GetTimer(byte idx);
//////////////////////����Ϊ��Ҫ���������
private:
        TIM_TypeDef *_port;    
    public:
        Timer(TIM_TypeDef *timer); 
        static Timer **Timers; // �Ѿ�ʵ�����Ķ�ʱ������
};

#endif
