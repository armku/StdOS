#include "SmartIRQ.h"
#include "Sys.h"
#ifdef __cplusplus
    extern "C"
    {
    #endif 
    //���������ж�
    void INTX_ENABLE(void);
    //�ر������ж�
    void INTX_DISABLE(void);
    #ifdef __cplusplus
    }
#endif 
SmartIRQ::SmartIRQ(bool enable)
{
    INTX_DISABLE();
}

SmartIRQ::~SmartIRQ()
{
    INTX_ENABLE();
}
