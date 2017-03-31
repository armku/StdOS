#include "SmartIRQ.h"
#ifdef __cplusplus
    extern "C"
    {
    #endif 
    //开启所有中断
    void INTX_ENABLE(void);
    //关闭所有中断
    void INTX_DISABLE(void);
    #ifdef __cplusplus
    }
#endif 
SmartIRQ::SmartIRQ()
{
    INTX_DISABLE();
}

SmartIRQ::~SmartIRQ()
{
    INTX_ENABLE();
}
