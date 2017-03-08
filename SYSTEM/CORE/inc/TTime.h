#pragma once

#include "Type.h"

class TTime
{
	private:
		static void OnHandler(ushort num,void* param);
	public:
		uint seconds;//ȫ��������ϵͳ���������������ۼ�
		UInt64 Milliseconds;//ȫ�ֺ�������ϵͳ�������ܺ���(1000���������֣��ۼ�
		uint BaseSeconds;//��׼������ϵͳ����ʱ�����1970���������ʱ����ڣ�����Seconds�õ���ǰʱ��Now();
		byte Index;//��ʱ��
		#if !(defined(STM32F0)||()GD32F130)
		byte Div;//��Ƶϵ��������Ƶʱ�޷��ô���64M��Ƶ��оƬ����õ�1kʱ��
		#endif
		
		Func OnInit;
	Func OnLoad;
	Func OnSave;
	typedef void (*FuncInt)(int);
	Function OnSleep;
	
	TTime();
	public:
		
	
		//����Ϊ������
		UInt64 Ms() const;//ϵͳ���������ĺ�����
		UInt64 Current() const;//ϵͳ���������ĺ���
		uint Seconds();//ϵͳʱ��
		void SetTime(UInt64 seconds);//����ϵͳ��ǰʱ��,Unix��ʽ����
		void Sleep(uint us);//��ʱus		
					
		uint TicksToUs(uint ticks) const;
		uint UsToTicks(uint us) const;
	
		UInt64 mCurrent;//ϵͳ���������ĺ�����	
		UInt64 CurrentTicks;//�δ�������΢��̵ĳ��߾���ʱ��		
};
extern TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��
