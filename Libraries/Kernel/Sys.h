#ifndef _Sys_H_
#define _Sys_H_

#include <stddef.h>
#include "Core\Type.h"
#include "Core\DateTime.h"
#include "Core\List.h"
#include "Core\Delegate.h"

/* ���Ŷ��� */
#include "Platform\Pin.h"

// ǿ������
#define _force_inline __attribute__( ( always_inline ) ) __INLINE

extern "C"
{
#ifdef DEBUG
	#define debug_printf StdPrintf	
#else
	#define debug_printf(format, ...)
#endif
}

#ifdef DEBUG
#define assert(expr, msg) ((expr) ? (void)0 : assert_failed2(msg, (const char*)__FILE__, __LINE__))

#else

#define assert(expr, msg) ((void)0)

#endif

//class SystemConfig;

// ϵͳ��
class TSys
{
public:
    uint32_t	Clock;  	// ϵͳʱ��
    //uint32_t	CystalClock;// ����ʱ��

	//cstring	Name;		// ϵͳ����
	//cstring	Company;	// ϵͳ����
	//uint16_t	Code;		// ��Ʒ����
	//uint16_t	Ver;		// ϵͳ�汾
    uint8_t	ID[12];		// оƬID��
    //uint16_t	DevID;		// MCU���롣�����豸�汾�������Ӱ汾
    //uint16_t	RevID;		// MCU���롣�����豸�汾�������Ӱ汾
    //uint32_t	CPUID;		// CPUID 
    uint16_t	FlashSize;	// оƬFlash������
    //uint16_t	RAMSize;	// оƬRAM����

	//const SystemConfig*	Config;	// ϵͳ����

    TSys();				// ���캯��

	void Init();     	// ��ʼ��ϵͳ
	void ShowInfo() const;
	
	uint64_t	Ms() const;		// ϵͳ������ĺ�����
	uint32_t	Seconds() const;	// ϵͳ���Ե�ǰʱ�䣬��

    void Sleep(int ms) const; // ���뼶�ӳ�
    void Delay(int us) const; // ΢�뼶�ӳ�
	
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
	uint32_t AddTask(Action func, void* param, int dueTime = 0, int period = 0, cstring name = nullptr) const;
	template<typename T>
	uint32_t AddTask(void(T::*func)(), T* target, int dueTime = 0, int period = 0, cstring name = nullptr)
	{
		return AddTask(*(Action*)&func, target, dueTime, period, name);
	}
	void RemoveTask(uint32_t& taskid) const;
	// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
	bool SetTask(uint32_t taskid, bool enable, int msNextTime = -1) const;
	// �ı���������
	bool SetTaskPeriod(uint32_t taskid, int period) const;

	bool Started;
	void Start();	// ��ʼϵͳ��ѭ��
};

extern TSys Sys;		// ����һ��ȫ�ֵ�Sys����  ����main����֮ǰִ�й��캯����������������

// ϵͳ����
//class SystemConfig
//{
//public:
//	// ����ϵͳ
//	uint32_t	Ver;		// ϵͳ�汾��Version��ʽ
//	char	Name[16];	// ϵͳ����
//
//	// Ӳ��
//	uint32_t	HardVer;	// Ӳ���汾
//
//	// Ӧ�����
//	uint16_t	Code;		// ��Ʒ����
//	uint32_t	AppVer;		// ��Ʒ�汾
//	char	Company[16];// ��˾
//	char	Product[16];// ��Ʒ����
//
//	char	DevID[16];	// �豸����
//	char	Server[32];	// �����������ú��ȳ��Գ���ǰ�ˣ��ٳ���ԭ������
//	char	Token[32];	// ���ʷ�����������
//
//	uint32_t	Expire;		// ��Ч�ڡ�1970������������
//	
//	uint16_t	Checksum;	// У��
//};

#define STDOS_VERSION "0.4.2018.1024" //�汾��

#endif //_Sys_H_

/*

v0.1.2018.0820  ʹ��C++11ʵ�֣�ʵ��F1 F4 F1��GPIO�ʹ��ڡ�IIC��SPI���жϹ���
v0.2.2018.		���ڷ��͡����ղ���DMA��ʽʵ�֡�
v0.3.2018       ��������ʵ�ַ�ʽ
v0.4.2018.1024  ���������Ż�������ʱ�ر�ȫ���жϣ�������ɿ�������ֹ�����쳣
*/
