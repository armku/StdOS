/*
init some hardware
*/
#ifndef _BSPPLATFORM_H
#define _BSPPLATFORM_H
#include "Core\Queue.h"
#ifdef __cplusplus
extern "C" {
#endif
	void BspPlatformInit();
		
	void SerialPrintInit();//���Դ��ڳ�ʼ��
	void com3send();
	
	extern Queue	Txx1;
	extern Queue	Rxx1;

#ifdef __cplusplus
}
#endif
#endif // !_BSPPLATFORM_H
