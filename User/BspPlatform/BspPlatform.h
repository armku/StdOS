/*
init some hardware
*/
#ifndef _BSPPLATFORM_H
#define _BSPPLATFORM_H
#include "Core\Queue.h"
#ifdef __cplusplus
extern "C" {
#endif
	typedef enum
    {
        Rising	= 0x01,	// ������
        Falling	= 0x02,	// �½���
		Both	= 0x03	// �����½���
    }Trigger;
	void SetEXIT(int pinIndex, bool enable, Trigger trigger);

	void BspPlatformInit();
	void TimeTickInit();//ϵͳ�ö�ʱ����ʼ��
	
	int CurrentTick();
	uint32_t CurrentTicks1();
	
	void SerialPrintInit();//���Դ��ڳ�ʼ��
	void com3send();
	
	extern Queue	Txx1;
	extern Queue	Rxx1;

#ifdef __cplusplus
}
#endif
#endif // !_BSPPLATFORM_H
