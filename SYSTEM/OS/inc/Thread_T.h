#pragma once
#include <stdint.h>
class CThread
{
	public:
		CThread();
		~CThread();
	public:
		uint32_t TickCur;//��ǰ����ֵ	
		uint32_t intervalms;//�ص����
		bool canRun;//����ִ��
		uint32_t delaycnt;//��ʱʱ��		
	public:
		void (*callback)(void);//�ص�����
		void checkRun();//�ж��Ƿ�����
		void SetPara(void (*callback)(void),uint32_t intervalms,uint32_t delaycntms=0);//���ò���
};
