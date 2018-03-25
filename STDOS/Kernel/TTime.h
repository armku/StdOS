#ifndef __TIME_H__
#define __TIME_H__

#include "Kernel\Sys.h"

// ʱ����
// ʹ��˫����ʱ�ӣ�TIMxר���ں��뼶ϵͳ��ʱ��SysTick����΢�뼶�ӳ٣��뼶�Լ����ϲ���ȫ�������ۼ�
// �����ӿ��Ա���Ƶ��ʹ��΢��ʱ���������ͳ˳���
class TTime
{
public:
    uint	Seconds;		// ȫ��������ϵͳ���������������ۼ�
	uint64_t	Milliseconds;	// ȫ�ֺ�������ϵͳ�������ܺ��루1000ms�����֣����ۼ� 1
    uint	BaseSeconds;	// ��׼������ϵͳ����ʱ�����1970���������ʱ����ڣ�����Seconds�õ���ǰʱ��Now()
    //byte	Ticks;			// ÿ΢���ʱ�ӵδ���
	byte	Index;			// ��ʱ��
#if ! (defined(STM32F0) || defined(GD32F150))
	byte	Div;			// ��Ƶϵ��������Ƶ64k���޷��ô���64M��Ƶ��оƬ����õ�1kʱ��
#endif

    TTime();

	void Init();

    uint CurrentTicks() const;	// ��ǰ�δ�ʱ��
	uint64_t Current() const; 		// ��ǰ������

	void Sleep(int ms, bool* running = nullptr) const;
    void Delay(int us) const;	// ΢�뼶�ӳ�	

	uint TicksToUs(uint ticks) const;
	uint UsToTicks(uint us) const;
	private:
		void DelayUs(int nus) const; //us��ʱ��100us���¾�ȷ
};

extern TTime Time; //extern const TTime Time

// ʱ���֡����ڳ�ʱ����
class TimeWheel
{
public:
	uint64_t	Expire;		// ����ʱ�䣬����
	ushort	Sleep;		// ˯��ʱ�䣬Ĭ��0����

	TimeWheel(uint ms);

	void Reset(uint ms);

	// �Ƿ��ѹ���
	bool Expired();
};

// ʱ�俪֧�������δ���о�ȷ����
class TimeCost
{
public:
	uint64_t	Start;		// ��ʼʱ�䣬����
	uint	StartTicks;	// ��ʼ�δ�

	TimeCost();

	void Reset();
	int Elapsed() const;	// ��ȥ��ʱ�䣬΢��
	void Show(cstring format = nullptr) const;
};

/*
������ʷ��

2015-10-05
ϵͳ��Ƶ��ʹ�õĺ���Current()���ڳ����������⣬ԭ���õδ�ʱ��8��Ƶ�����ɸ��δ�ŵõ�һ��΢�룬��Ȼ����һ���������㡣
���������ϵͳ�ȸ����ط���������Current�Ի�ȡ��ǰʱ�䣬�ر����жϺ����ڲ�ֱ�ӻ��ӵ��ã���ɼ����������ġ�
���ݻ���@405803243��ָ�㣬�޸�Ϊ˫��ʱ���ܹ���
ϵͳʱ�Ӳ��û�����ʱ���������Ƶ���ܷ���ճ�ÿ��������ʾ1���룬���׸���Current�������⡣
���⣬�δ�ʱ������΢�뼶�߾����ӳ٣�Ӧ����Ӳ�����������ȳ��ϡ�

*/

#endif
