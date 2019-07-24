#ifndef __TIME_H__
#define __TIME_H__

#include "Kernel\Sys.h"

// ʱ����
// ʹ��˫����ʱ�ӣ�TIMxר���ں��뼶ϵͳ��ʱ��SysTick����΢�뼶�ӳ٣��뼶�Լ����ϲ���ȫ�������ۼ�
// �����ӿ��Ա���Ƶ��ʹ��΢��ʱ���������ͳ˳���
class TTime
{
private:
    static void OnHandler(ushort num, void* param);

public:
    uint	Seconds;		// ȫ��������ϵͳ���������������ۼ�
	UInt64	Milliseconds;	// ȫ�ֺ�������ϵͳ�������ܺ��루1000ms�����֣����ۼ�
    uint	BaseSeconds;	// ��׼������ϵͳ����ʱ�����1970���������ʱ����ڣ�����Seconds�õ���ǰʱ��Now()
    //byte	Ticks;			// ÿ΢���ʱ�ӵδ���
	byte	Index;			// ��ʱ��
#if ! (defined(STM32F0) || defined(GD32F150))
	byte	Div;			// ��Ƶϵ��������Ƶ64k���޷��ô���64M��Ƶ��оƬ����õ�1kʱ��
#endif

	Func OnInit;
	Func OnLoad;
	Func OnSave;
	typedef void (*FuncInt)(int);
	FuncInt OnSleep;

    TTime();

	void UseRTC();			// ʹ��RTC��������Initǰ����
	void Init();

    uint CurrentTicks() const;	// ��ǰ�δ�ʱ��
	UInt64 Current() const; 		// ��ǰ������
	void SetTime(UInt64 seconds);	// ����ʱ��

	void Sleep(int ms, bool* running = nullptr) const;
    void Delay(int us) const;	// ΢�뼶�ӳ�

	uint TicksToUs(uint ticks) const;
	uint UsToTicks(uint us) const;
};
extern TTime Time;
// ʱ���֡����ڳ�ʱ����
class TimeWheel
{
public:
	UInt64	Expire;		// ����ʱ�䣬����
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
	UInt64	Start;		// ��ʼʱ�䣬����
	uint	StartTicks;	// ��ʼ�δ�

	TimeCost();

	void Reset();
	int Elapsed() const;	// ��ȥ��ʱ�䣬΢��
	void Show(cstring format = nullptr) const;
};
void delayMicroseconds(uint us);//us��ʱ��100us���¾�ȷ
UInt64 millis();//ϵͳ������ĺ�����
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
