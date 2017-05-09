#ifndef __Timer_H__
#define __Timer_H__

#include "Kernel\Sys.h"
#include "Device\Port.h"
#include "stm32f10x.h"
// ��ʱ��
class Timer
{
	#if 0
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
#endif
private:
        TIM_TypeDef *_port;
        byte _index; // �ڼ�����ʱ������0��ʼ
        volatile bool _started; // �������ж���ر��Լ�

        void ClockCmd(bool state);
    public:
        Timer(TIM_TypeDef *timer);
        ~Timer();

        ushort Prescaler; // Ԥ��Ƶ��ʵ��ֵ����ʱ�����һ��Ĭ��Ԥ���䵽1MHz
        uint Period; // ���ڡ�ʵ��ֵ����ʱ�����һ��Ĭ��1000������

        void Start(); // ��ʼ��ʱ��
        void Stop(); // ֹͣ��ʱ��
        //void SetScaler(uint scaler);        // ����Ԥ��ƵĿ�꣬����1MHz
        void SetFrequency(uint frequency); // ����Ƶ�ʣ��Զ�����Ԥ��Ƶ
        #if 0
            void Register(EventHandler handler, void *param = NULL);
        #endif 

    private:
        void OnInterrupt();
        static void OnHandler(ushort num, void *param);
        #if 0
            EventHandler _Handler;
        #endif 
        void *_Param;

    public:
        static Timer **Timers; // �Ѿ�ʵ�����Ķ�ʱ������
        static const byte TimerCount; // ��ʱ������

        static Timer *Create(byte index = 0xFF); // ����ָ�������Ķ�ʱ�������������ֱ�ӷ��أ�Ĭ��0xFF��ʾ�������
};

#endif
