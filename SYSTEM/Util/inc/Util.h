#pragma once

#include "Type.h"

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    void bubbleSort(float *buf, uint32_t len);
    //设置浮点数
    void SetBufFloat(uint8_t buf[], uint16_t pos, float da);
    //设置浮点数
    float GetBufFloat(uint8_t buf[], uint16_t pos);
    //从电阻获得温度
    float GetTFromPT100(float om);

    #ifdef __cplusplus
    }
#endif
