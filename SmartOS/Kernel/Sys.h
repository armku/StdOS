#ifndef _Sys_H_
#define _Sys_H_

#include "Core\Type.h"
#include "Core\List.h"
#include "Core\Delegate.h"
#include "Version.h"
#include "TTime.h"


#include <stdio.h>
#include <stddef.h>

#include "Core\Type.h"
#include "Core\Buffer.h"
#include "Core\Array.h"
#include "Core\ByteArray.h"
#include "Core\SString.h"
#include "Core\Stream.h"
#include "Core\DateTime.h"
#include "Core\Version.h"
#include "Core\List.h"
#include "Core\Dictionary.h"
#include "Core\Delegate.h"

/* ���Ŷ��� */
#include "Platform\Pin.h"



extern "C"
{
#ifdef DEBUG
	#define debug_printf SmartOS_printf	
#else
	#define debug_printf(format, ...)
#endif
}

#ifdef DEBUG
#define assert(expr, msg) ((expr) ? (void)0 : assert_failed2(msg, (const char*)__FILE__, __LINE__))

#else

#define assert(expr, msg) ((void)0)

#endif

// ϵͳ��
class TSys
{
public:
    uint	Clock;  	// ϵͳʱ��
    uint	CystalClock;// ����ʱ��
	uint	HeapSize;	// �Ѵ�С
	uint	StackSize;	// ջ��С

	byte	ID[12];		// оƬID��
    ushort	FlashSize;	// оƬFlash������
    //ushort	RAMSize;	// оƬRAM����
	Version OsVer;//ϵͳ�汾
	Version AppVer;//����汾

	TSys();

	void Init();     	// ��ʼ��ϵͳ
	void ShowInfo() const;
	
	uint	Seconds() const;	// ϵͳ���Ե�ǰʱ�䣬��

    void Sleep112233(int ms) const; // ���뼶�ӳ٣�����ϵͳ����
	
	// �ӳ��첽����
	void Reboot(int msDelay = 0) const;
	// ��ȫ���ж�
	void GlobalEnable();
	// �ر�ȫ���ж�
	void GlobalDisable();
		
private:
	void Reset() const;
	void OnInit();
	void OnShowInfo() const;

public:
	// �������񣬷��������š�dueTime�״ε���ʱ��ms��period���ȼ��ms��-1��ʾ������һ��
	uint AddTask(Action func, void* param, int dueTime = 0, int period = 0, cstring name = nullptr) const;
	template<typename T>
	uint AddTask(void(T::*func)(), T* target, int dueTime = 0, int period = 0, cstring name = nullptr)
	{
		return AddTask(*(Action*)&func, target, dueTime, period, name);
	}
	void RemoveTask(uint& taskid) const;
	// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
	bool SetTask(uint taskid, bool enable, int msNextTime = -1) const;
	// �ı���������
	bool SetTaskPeriod(uint taskid, int period) const;

	bool Started;
	void Start();	// ��ʼϵͳ��ѭ��
};

extern TSys Sys;		// ����һ��ȫ�ֵ�Sys����  ����main����֮ǰִ�й��캯����������������

#define STDOS_VERSION "0.8.2019.0408" //ϵͳ�汾��
extern char* AppVersion;//��Ҫ��������汾��ʵ�� �磺char *AppVersion = "0.1.2018.1114";

#endif //_Sys_H_

/*

v0.1.2018.0820  ʹ��C++11ʵ�֣�ʵ��F1 F4 F1��GPIO�ʹ��ڡ�IIC��SPI���жϹ���
v0.2.2018.		���ڷ��͡����ղ���DMA��ʽʵ�֡�
v0.3.2018       ��������ʵ�ַ�ʽ
v0.4.2018.1024  ���������Ż�������ʱ�ر�ȫ���жϣ�������ɿ�������ֹ�����쳣
v0.5.2018.1030  ϵͳͳ����Ϣ����ȥ������Ҫ����
v0.9.2019.0408	SPIsoft����
*/
