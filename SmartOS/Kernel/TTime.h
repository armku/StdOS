#ifndef __TIME_H__
#define __TIME_H__

#include "Kernel\Sys.h"

// ʱ����
// ʹ��˫����ʱ�ӣ�TIMxר���ں��뼶ϵͳ��ʱ��SysTick����΢�뼶�ӳ٣��뼶�Լ����ϲ���ȫ�������ۼ�
// �����ӿ��Ա���Ƶ��ʹ��΢��ʱ���������ͳ˳���
class TTime
{
public:
	volatile	uint	Seconds;		// ȫ��������ϵͳ���������������ۼ�
	volatile	UInt64	Milliseconds;	// ȫ�ֺ�������ϵͳ�������ܺ��루1000ms�����֣���
	volatile	uint	BaseSeconds;	// ��׼������ϵͳ����ʱ�����1970���������ʱ����ڣ�����Seconds�õ���ǰʱ��Now()
    //byte	Ticks;			// ÿ΢���ʱ�ӵδ���
	byte	Index;			// ��ʱ��
#if ! (defined(STM32F0) || defined(GD32F150))
	byte	Div;			// ��Ƶϵ��������Ƶ64k���޷��ô���64M��Ƶ��оƬ����õ�1kʱ��
#endif

    TTime();

	void Init();

    uint CurrentTicks() const;	// ��ǰ�δ�ʱ��

	uint TicksToUs(uint ticks) const;
	uint UsToTicks(uint us) const;
};

void delayMicroseconds(uint us);//us��ʱ��100us���¾�ȷ
void delay(UInt64 ms);//��ʱ����
UInt64 millis();//ϵͳ������ĺ�����
UInt64 micros();//���������ڵ�΢����

extern TTime Time;
// ʱ�俪֧�������δ���о�ȷ����
class TimeCost
{
public:
	UInt64	Start;		// ��ʼʱ�䣬����
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
