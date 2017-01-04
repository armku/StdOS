#pragma once

#include "Type.h"
#include "Object.h"
#include "Task.h"
#include "TTime.h"

/* ���ڶ��� */

typedef enum
{
	COM1=0,COM2,COM3,COM4,COM5,COM_NONE=0XFF
}COM_Def;//�˿ں�
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
#ifndef NULL
	#define NULL 0
#endif
#define STM32F1    //ʹ��STM32F1ϵ�е�Ƭ��
/* ��� ------------------------------------------------------------------*/
//���Ŷ���
typedef enum
{
	PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
	PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
	PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
	PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
	PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
	PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
	P0 = 0XFFFF
}Pin;

// ʹ�ú���ģ��ĺ궨��
#define using(module) Sys.module.Init = T##module##_Init;

#ifdef DEBUG
    #define debug_printf printf
#else
    __inline void debug_printf( const char *format, ... ) {}
#endif

class TSys:public Object
{
	public:
		TSys(uint clock =72000000,COM_Def messagePort=COM1);
		void Init();//��ʼ��
		void Start();//����ϵͳ������ȣ��ú����ڲ�Ϊ��ѭ����
		virtual void Show(bool newLine=false) const;
		void ShowInfo();//��ʾϵͳ��Ϣ
		uint64_t Ms();//ϵͳ���������ĺ��������޷��ų�����8�ֽ�
		uint Seconds();//ϵͳ����UTCʱ�䣬����4�ֽڣ�Unix��ʽ��1970����������������
		void Delay(uint us);//΢��ӳ٣������ڸ߾��������źſ���
		void Sleep(uint ms);//���뼶˯�ߣ�������ҵ��������ȴ�һ��ʱ��
		void Reboot(uint msDelay);//�첽������ϵͳ���ӳ�һ��������ִ�С�		
	public:
		uint AddTask(void(*callback)(void),void* para, uint firstms, int periodms, const char *name = "δ��������");
		void SetTask(uint taskid,bool onoff,int delayms=0);//�����������
		void Remove(uint taskid);//ɾ������
		void TimeTick(); //���1ms����һ��
		void Routin(); //����  
	public:
		uint Clock;//ϵͳʱ��
		COM_Def MessagePort;//���Խӿ�
		byte Inited;//�����ò���
	public:
		byte ID[12];
		ushort FlashSize;
	private:
		Task task;	
	
};
extern TSys Sys;//ϵͳ����
extern TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��
