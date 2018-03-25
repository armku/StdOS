#ifndef _Sys_H_
#define _Sys_H_

#include <stdint.h>
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
//#include "Core\Dictionary.h"
#include "Core\Delegate.h"

/* ���Ŷ��� */
#include "Platform\Pin.h"

// ǿ������
#define _force_inline __attribute__( ( always_inline ) ) __INLINE

extern "C"
{
#if defined(DEBUG)
	#define debug_printf StdPrintf	
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

class SystemConfig;

// ϵͳ��
class TSys
{
public:
    byte	MessagePort;// ��Ϣ�ڣ�Ĭ��0��ʾUSART1

    uint	Clock;  	// ϵͳʱ��
    uint	CystalClock;// ����ʱ��

	cstring	Name;		// ϵͳ���� 3
	cstring	Company;	// ϵͳ���� 4
	ushort	Code;		// ��Ʒ���� 5
	ushort	Ver;		// ϵͳ�汾 6
    byte	ID[12];		// оƬID��
    ushort	DevID;		// MCU���롣�����豸�汾�������Ӱ汾
    ushort	RevID;		// MCU���롣�����豸�汾�������Ӱ汾
    uint	CPUID;		// CPUID 
    ushort	FlashSize;	// оƬFlash������ 16
    ushort	RAMSize;	// оƬRAM���� 17

	const SystemConfig*	Config;	// ϵͳ���� 18

    TSys();				// ���캯��

	void Init();     	// ��ʼ��ϵͳ
	void ShowInfo() const;
	
	uint64_t	Ms() const;		// ϵͳ������ĺ�����
	uint	Seconds() const;	// ϵͳ���Ե�ǰʱ�䣬��

    void Sleep(int ms) const; // ���뼶�ӳ�
    void Delay(int us) const; // ΢�뼶�ӳ�
	
	// �ӳ��첽����
	void Reboot(int msDelay = 0) const;
		
private:
	// ����ϵͳ
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

// ϵͳ����
class SystemConfig
{
public:
	// ����ϵͳ
	uint	Ver;		// ϵͳ�汾��Version��ʽ
	char	Name[16];	// ϵͳ����

	// Ӳ��
	uint	HardVer;	// Ӳ���汾

	// Ӧ�����
	ushort	Code;		// ��Ʒ����
	uint	AppVer;		// ��Ʒ�汾
	char	Company[16];// ��˾
	char	Product[16];// ��Ʒ����

	char	DevID[16];	// �豸����
	char	Server[32];	// �����������ú��ȳ��Գ���ǰ�ˣ��ٳ���ԭ������
	char	Token[32];	// ���ʷ�����������

	uint	Expire;		// ��Ч�ڡ�1970������������
	
	ushort	Checksum;	// У��
};

void EnterCritical();
void ExitCritical();

uint _REV(uint value);
ushort _REV16(ushort value);

// ����IRQ����ʼ��ʱ���ݣ�����ʱ��ԭ
// SmartIRQ�൱�Ե�����ֱ�ӹر������жϣ���Ҳû�б��������Ը���ǰ��������MCU
class SmartIRQ
{
public:
	SmartIRQ(bool enable = false);
	~SmartIRQ();

private:
	uint _state;
};

// ������Ϣ�����Դ���
#ifndef __BUILD_DATE__
#define __BUILD_DATE__ 6289		// 2017-03-21
#endif
#ifndef __BUILD_TIME__
#define __BUILD_TIME__ 1490054400	// 2017-03-21 00:00:00
#endif
#ifndef __BUILD_SDATE__
#define __BUILD_SDATE__ "170321"
#endif
#ifndef __BUILD_STIME__
#define __BUILD_STIME__ "2017-03-21 00:00:00"
#endif
#ifndef __BUILD_USER__
#define __BUILD_USER__ "Computer_User"
#endif


#endif //_Sys_H_

/*
v3.2.2016.0517	�����������Ŀ¼Core��ƽ̨�޹أ�ϵͳ�޹�

v3.1.2015.1108	����ϵͳ���ô洢ģ�飬���ӵ�Դ����ģ��

v3.0.2015.0806	��ǿϵͳ��������֧�����������������

v2.8.2014.0927	���΢��ͨѶ�ܹ�����װ��ϢЭ�飬���ڼ�nRF24L01+����ͨ��
v2.7.2014.0919	֧����ռʽ���̵߳���
v2.6.2014.0823	ƽ̨�ļ��������ӹ�ϵͳ��ʼ������Ȩ
v2.5.2014.0819	���Ӷ�ջ������ģ�飬����new/deleteʵ�֣���Debug��Ч
v2.4.2014.0811	ʵ��ϵͳ��������ȣ�һ���Ա������ͨ����������С������4k
				ʵ����̫������Э��TinyIP��ARP/ICMP/TCP/UDP�������������7.5k
				���ӿ��Ź�����ʱ��ģ��
v2.3.2014.0806	ʹ��˫ջ�����ȶ��ԣ�����RTM�Ż����룬���ĺ���ǿ���������Զ���ջԽ����
v2.2.2014.0801	�������ű������ƣ����ⲻͬģ��ʹ����ͬ���ŵ��³�ͻ�����Է��ִ���
v2.1.2014.0728	�����жϹ���ģ�飬ȫ��ӹ��ж�������֧�ֶ�̬�޸��жϺ�����֧�ֶ��жϹ����жϺ�����F0������RAM��0x200000C0��ʼ
v2.0.2014.0725	ʹ��C++ȫ��ʵ��SmartOS��֧��ϵͳʱ�ӡ�IO��USART����־�����ԡ�Spi��NRF24L01��SPIFlash��CAN��Enc28j60��GD32��Ƶ

v1.3.2014.0624	����Spiģ���NRF24L01+ģ���֧��
v1.2.2014.0604	֧��GD32оƬ
v1.1.2014.0513	֧��F0/F1��GPIO�ʹ��ڹ���
v1.0.2014.0506	����Ƕ��ʽϵͳ���SmartOS��ʹ�ô�C����ʵ��
*/
