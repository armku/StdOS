#pragma once
#include "Type.h"
class CThread
{
	public:
		CThread();
		~CThread();
	public:
		uint TickCur;//��ǰ����ֵ	
		uint intervalms;//�ص����
		bool canRun;//����ִ��
		uint delaycnt;//��ʱʱ��		
	public:
		void (*callback)(void);//�ص�����
		void checkRun();//�ж��Ƿ�����
		void SetPara(void (*callback)(void),uint intervalms,uint delaycntms=0);//���ò���
};
