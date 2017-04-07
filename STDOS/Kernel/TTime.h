#pragma once

#include "Kernel\Sys.h"

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
		
	public:							
		UInt64 Ms() const;//ϵͳ���������ĺ�����		
		UInt64 mCurrent;//ϵͳ���������ĺ�����				
};
extern TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��
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
