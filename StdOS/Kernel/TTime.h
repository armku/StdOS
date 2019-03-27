#ifndef __TIME_H__
#define __TIME_H__

#include "Kernel\Sys.h"

// ʱ����
// ʹ��˫����ʱ�ӣ�TIMxר���ں��뼶ϵͳ��ʱ��SysTick����΢�뼶�ӳ٣��뼶�Լ����ϲ���ȫ�������ۼ�
// �����ӿ��Ա���Ƶ��ʹ��΢��ʱ���������ͳ˳���
class TTime
{
public:
    uint32_t	Seconds;		// ȫ��������ϵͳ���������������ۼ�
	uint64_t	Milliseconds;	// ȫ�ֺ�������ϵͳ�������ܺ��루1000ms�����֣���
    uint32_t	BaseSeconds;	// ��׼������ϵͳ����ʱ�����1970���������ʱ����ڣ�����Seconds�õ���ǰʱ��Now()
    //uint8_t	Ticks;			// ÿ΢���ʱ�ӵδ���
	uint8_t	Index;			// ��ʱ��
#if ! (defined(STM32F0) || defined(GD32F150))
	uint8_t	Div;			// ��Ƶϵ��������Ƶ64k���޷��ô���64M��Ƶ��оƬ����õ�1kʱ��
#endif

    TTime();

	void Init();

    uint32_t CurrentTicks() const;	// ��ǰ�δ�ʱ��
	uint64_t Current() const; 		// ��ǰ������

	uint32_t TicksToUs(uint32_t ticks) const;
	uint32_t UsToTicks(uint32_t us) const;
};

void delayMicroseconds(uint32_t us);//us��ʱ��100us���¾�ȷ
void delay(uint64_t ms);//��ʱ����
uint64_t millis();//���������ڵĺ�����
uint64_t micros();//���������ڵ�΢����

extern TTime Time;
// ʱ�俪֧�������δ���о�ȷ����
class TimeCost
{
public:
	uint64_t	Start;		// ��ʼʱ�䣬����
	uint32_t	StartTicks;	// ��ʼ�δ�

	TimeCost();

	void Reset();
	int Elapsed() const;	// ��ȥ��ʱ�䣬΢��
	void Show(cstring format = NULL) const;
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
