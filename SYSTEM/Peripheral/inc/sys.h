#pragma once

#include "Type.h"
#ifdef __cplusplus
    extern "C"
    {
    #endif 
    #define SYSTEM_SUPPORT_OS		0		//定义系统文件夹是否支持UCOS

    //以下为汇编函数
    void WFI_SET(void); //执行WFI指令
    void INTX_DISABLE(void); //关闭所有中断
    void INTX_ENABLE(void); //开启所有中断
    void MSR_MSP(uint addr); //设置堆栈地址
    #ifdef __cplusplus
    }
#endif
