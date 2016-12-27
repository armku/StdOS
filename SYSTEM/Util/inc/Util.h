#pragma once

#include "Type.h"

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    void bubbleSort(float *buf, uint32_t len);
    //���ø�����
    void SetBufFloat(uint8_t buf[], uint16_t pos, float da);
    //���ø�����
    float GetBufFloat(uint8_t buf[], uint16_t pos);
    //�ӵ������¶�
    float GetTFromPT100(float om);

    #ifdef __cplusplus
    }
#endif
