#pragma once

#ifdef DEBUG
    #include <stdio.h>
#endif 

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
