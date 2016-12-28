#pragma once

#include "Type.h"
#include "Object.h"

/* ���ڶ��� */

typedef enum
{
	COM1=0,COM2,COM3,COM4,COM5,COM_NONE=0XFF
}MessagePort_T;//�˿ں�

class TSys:public Object
{
	public:
		TSys(uint clock =72000000,MessagePort_T messagePort=COM1);
		void Init();//��ʼ��
		void Start();//����ϵͳ������ȣ��ú����ڲ�Ϊ��ѭ����
		void Show(bool newLine=false) const;
		void ShowInfo();//��ʾϵͳ��Ϣ
		uint64_t Ms();//ϵͳ���������ĺ��������޷��ų�����8�ֽ�
		uint Seconds();//ϵͳ����UTCʱ�䣬����4�ֽڣ�Unix��ʽ��1970����������������
		void Delay(uint us);//΢��ӳ٣������ڸ߾��������źſ���
		void Sleep(uint ms);//���뼶˯�ߣ�������ҵ��������ȴ�һ��ʱ��
		void Reboot(uint msDelay);//�첽������ϵͳ���ӳ�һ��������ִ�С�
		uint AddTask(void(*callback)(void),void* para,uint delaycntms, uint intervalms,String name );
		uint AddTask(void(*callback)(void),void* para,uint delaycntms, uint intervalms);//��ʱ��
		void Remove(uint taskid);//ɾ������
	public:
		uint Clock;//ϵͳʱ��
		MessagePort_T MessagePort;//���Խӿ�
	public:
		uint64_t ms;//ϵͳ���������ĺ�����
		uint seconds;//ϵͳ����UTCʱ�䣬Unix��ʽ��1970����������������
		uint ID[3]; 
		uint FlashSize;
	
};
extern TSys Sys;//ϵͳ����
