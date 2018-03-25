#ifndef __Timer_H__
#define __Timer_H__

#include "Kernel\Sys.h"
#include "Device\Port.h"

// ��ʱ��
class Timer
{
protected:
	uint8_t	_index;		// �ڼ�����ʱ������0��ʼ
	Delegate<Timer&>	OnTick;	// ��this����

public:
	void*	_Timer;
	bool	Opened;	// �������ж���ر��Լ�

	uint16_t	Prescaler;	// Ԥ��Ƶ��ʵ��ֵ����ʱ�����һ��
	uint32_t	Period;		// ���ڡ�ʵ��ֵ����ʱ�����һ��

	Timer(TIMER index);

	virtual void Open();	// ��ʼ��ʱ��
	virtual void Close();	// ֹͣ��ʱ��
	virtual void Config();
	void SetFrequency(uint32_t frequency);	// ����Ƶ�ʣ��Զ�����Ԥ��Ƶ

	uint32_t GetCounter();
	void SetCounter(uint32_t cnt);		// ���ü�����ֵ

	void Register(const Delegate<Timer&>& dlg);
	virtual void OnInterrupt();

	static void ClockCmd(int idx, bool state);

private:
	static void OnHandler(uint16_t num, void* param);

protected:

public:
	static const uint8_t	TimerCount;	// ��ʱ������

	static Timer* Create(uint8_t index = 0xFF);	// ����ָ�������Ķ�ʱ�������������ֱ�ӷ��أ�Ĭ��0xFF��ʾ�������

private:
	void OnInit();
	void OnOpen();
	void OnClose();

	static const void* GetTimer(uint8_t idx);
};

#endif
