#ifndef __Power_H__
#define __Power_H__

#include "Kernel\Sys.h"

// ��Դ����ӿ�
class Power
{
public:
	// ���õ�Դ�����Ӻ���
	virtual void SetPower();
	// ��Դ�ȼ�����������͹���ģʽ��ʱ����
	virtual void ChangePower(int level);

	// �͹������
    static void Sleep(int msTime = 0);
	static void Stop(int msTime = 0);
	static void Standby(int msTime = 0);

	// ��ģ����ϵͳע��͹��ľ������ϵͳ����͹���ǰ����
	static void AddPower(Power* power);
	
	// ���ӵ�ϵͳʱ�ӣ�˯��ʱ����͹���
	static bool AttachTimeSleep();

private:
	static void OnSleep(int msTime);
	static void OnStop(int msTime);
	static void OnStandby(int msTime);
};

#endif
