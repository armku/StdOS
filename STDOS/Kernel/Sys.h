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

#include "Type.h"
#include "TTime.h"
#include "Delegate.h"
#include "SString.h"
#include "..\Platform\Pin.h"

/* ���ڶ��� */
typedef enum
{
    AIN = 0x0,  //ģ������ 
    INPUT = 0x04,  //����
    INPUT_PD = 0x28,  //��������
    INPUT_PU = 0x48,  //����ʽ����
    OUTPUT_OD = 0x14,  //��©���
    OUTPUT_PP = 0x10,  //����ʽ���
    AF_OD = 0x1C,  //��©�������
    AF_PP = 0x18  //���츴�����
} PIN_MODE;
 
#define STM32F1    //ʹ��STM32F1ϵ�е�Ƭ��
/* ��� ------------------------------------------------------------------*/
#ifdef DEBUG

void assert_failed2(cstring msg, cstring file, unsigned int line);
#define assert(expr, msg) ((expr) ? (void)0 : assert_failed2(msg, (const char*)__FILE__, __LINE__))

#else

#define assert(expr, msg) ((void)0)

#endif
// ʹ�ú���ģ��ĺ궨��
#define using(module) Sys.module.Init = T##module##_Init;

#ifdef DEBUG
    #define debug_printf printf
#else 
    inline void debug_printf(const char *format, ...){}
#endif 

class TSys: public Object
{
    public:
        TSys(uint clock = 72000000, byte messagePort = COM1);
        void Init(); //��ʼ��
        void Start(); //����ϵͳ������ȣ��ú����ڲ�Ϊ��ѭ����
        void Stop();
        void StartInternal();
        virtual void Show(bool newLine = false)const;
        void ShowInfo(); //��ʾϵͳ��Ϣ
        UInt64 Ms(); //ϵͳ���������ĺ��������޷��ų�����8�ֽ�
        uint Seconds(); //ϵͳ����UTCʱ�䣬����4�ֽڣ�Unix��ʽ��1970����������������
        void Delay(uint us); //΢��ӳ٣������ڸ߾��������źſ���
        void Sleep(uint ms); //���뼶˯�ߣ�������ҵ��������ȴ�һ��ʱ��
        void Reboot(uint msDelay); //�첽������ϵͳ���ӳ�һ��������ִ�С�		

        uint AddTask(Action func, void *param, long dueTime, long period, const char *name = "δ��������");
        void SetTask(uint taskid,bool onoff,long delaytime=1); //�����������
        void RemoveTask(uint taskid); //ɾ������

        uint Clock; //ϵͳʱ��
        UInt64 StartTime; //ϵͳ����ʱ��
        ushort FlashSize;
        ushort RamSize;
        byte MessagePort; // ָ��printf����Ĵ���
        byte Inited; //�����ò���
        byte ID[12];
        uint CPUID;
        ushort RevID;
        ushort DevID;
        byte _Index; //δ֪
        String *CPUName;
		char * Name;//����
        ushort Crc16(const void *buf, uint len, ushort crc = 0); //crc ���� ���ʵ��
        uint Crc(const void *buf, uint len); //crc Ӳ��ʵ��
        uint Crc(const void *buf, uint len, uint crc);
        void ShowHex(byte *data, byte hexlength);

    private:
        void Initjs(); //����ram���ͺŵ�

};
extern TSys Sys; //ϵͳ����

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

//extern uint32_t __REV(uint32_t value);
//extern uint32_t __REV16(uint16_t value);

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
#if DEBUG
// ����ջ��
// ���뺯��ʱѹջ���������뿪ʱ�����������쳣ʱ��ȡ���̵߳����б�
class TraceStack
{
public:
	TraceStack(cstring name);
	~TraceStack();

	static void Show();
};

#define TS(name) TraceStack __ts(name)

#else

#define TS(name) ((void)0)

#endif

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
