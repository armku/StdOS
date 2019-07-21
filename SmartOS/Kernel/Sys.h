#ifndef _Sys_H_
#define _Sys_H_

#include <stdint.h>
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

// ǿ������
#define _force_inline __attribute__( ( always_inline ) ) __INLINE

extern "C"
{
#if defined(DEBUG)
	#define debug_printf SmartOS_printf
	extern int SmartOS_printf(const char *format, ...);
#else
	#define debug_printf(format, ...)
#endif
}

#ifdef DEBUG

void assert_failed2(cstring msg, cstring file, unsigned int line);
#define assert(expr, msg) ((expr) ? (void)0 : assert_failed2(msg, (const char*)__FILE__, __LINE__))

#else

#define assert(expr, msg) ((void)0)

#endif

// �ؼ��Դ���ŵ���ͷ
#if !defined(TINY) && defined(USEROOT)
	#define INROOT __attribute__((section(".InRoot")))
#else
	#define INROOT
#endif

#if defined(BOOT) || defined(APP)
struct HandlerRemap
{
	Func pUserHandler;
	void* Reserved1;
	void* Reserved2;
	void* Reserved3;
};
extern struct HandlerRemap StrBoot;
#endif

class SystemConfig;

#include "TTime.h"
// ϵͳ��
class TSys
{
public:
    byte	MessagePort;// ��Ϣ�ڣ�Ĭ��0��ʾUSART1

    uint	Clock;  	// ϵͳʱ��
    uint	CystalClock;// ����ʱ��

	cstring	Name;		// ϵͳ����
	cstring	Company;	// ϵͳ����
	cstring	Product;	// ��Ʒ����
	ushort	Code;		// ��Ʒ����
	uint	Ver;		// ϵͳ�汾
	uint	HardVer;	// Ӳ���汾
	//uint	AppVer;		// ��Ʒ�汾

	byte	ID[12];		// оƬID
    ushort	DevID;		// MCU���롣�����豸�汾�������Ӱ汾
    ushort	RevID;		// MCU���롣�����豸�汾�������Ӱ汾
    uint	CPUID;		// CPUID
    ushort	FlashSize;	// оƬFlash������
    ushort	RAMSize;	// оƬRAM����

	uint	StartTimes;	// ��������
	uint	HotStart;	// ����������

	const SystemConfig*	Config;	// ϵͳ����
TSys();				// ���캯��

	void InitClock();	// ��ʼ��ϵͳʱ��
    void Init();     	// ��ʼ��ϵͳ
	void ShowInfo() const;
	uint HeapBase() const;	// ����ʼ��ַ��ǰ���Ǿ�̬�����ڴ�
	uint StackTop() const;	// ջ���������ǳ�ʼ������������
	void SetStackTop(uint addr);

	UInt64	Ms() const;		// ϵͳ������ĺ�����
	uint	Seconds() const;	// ϵͳ���Ե�ǰʱ�䣬��

    void Sleep(int ms) const; // ���뼶�ӳ�
    void Delay(int us) const; // ΢�뼶�ӳ�
	typedef void (*FuncI32)(int param);
	FuncI32 OnSleep;

	bool CheckMemory() const;

	// �ӳ��첽����
	void Reboot(int msDelay = 0) const;

	// ϵͳ����
	void InitTrace(void* port) const;
	void Trace(int times = 1) const;

private:
	// ����ϵͳ
    void Reset() const;
	void OnInit();
	void OnShowInfo() const;
	void OnStart();

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





















	uint	HeapSize;	// �Ѵ�С
	uint	StackSize;	// ջ��С
	Version OsVer;//ϵͳ�汾
	Version AppVer;//����汾	
    void Sleep112233(int ms) const; // ���뼶�ӳ٣�����ϵͳ����	
	// ��ȫ���ж�
	void GlobalEnable();
	// �ر�ȫ���ж�
	void GlobalDisable();
};
class SmartIRQ
{
public:
	SmartIRQ(bool enable = false);
	~SmartIRQ();

private:
	uint _state;
};
// ����ջ��
// ���뺯��ʱѹջ���������뿪ʱ�����������쳣ʱ��ȡ���̵߳����б�
class TraceStack
{
public:
	TraceStack(cstring name);
	~TraceStack();

	static void Show();
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
