#pragma once

#include "Type.h"
#ifdef __cplusplus
    extern "C"
    {
    #endif 
    #define SYSTEM_SUPPORT_OS		0		//����ϵͳ�ļ����Ƿ�֧��UCOS

    //����Ϊ��ຯ��
    void WFI_SET(void); //ִ��WFIָ��
    void INTX_DISABLE(void); //�ر������ж�
    void INTX_ENABLE(void); //���������ж�
    void MSR_MSP(uint addr); //���ö�ջ��ַ
    #ifdef __cplusplus
    }
#endif
