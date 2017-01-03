#pragma once

#include "Type.h"
#include "Object.h"
#include "Task.h"

/* ���ڶ��� */

typedef enum
{
	COM1=0,COM2,COM3,COM4,COM5,COM_NONE=0XFF
}MessagePort_T;//�˿ں�

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
		TSys(uint clock =72000000,MessagePort_T messagePort=COM1);
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
		MessagePort_T MessagePort;//���Խӿ�
	public:
		byte ID[12];
		ushort FlashSize;
	private:
		Task task;	
	
};
extern TSys Sys;//ϵͳ����
