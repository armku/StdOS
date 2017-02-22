#pragma once

#include "Type.h"

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    void bubbleSort(float *buf, uint len); //设置浮点数
    void SetBufFloat(byte buf[], ushort pos, float da); //设置浮点数
    float GetBufFloat(byte buf[], ushort pos);
    ushort GetBufUshort(byte buf[], ushort pos);
    void SetBufUshort(byte buf[], ushort pos, ushort da); //从电阻获得温度
    float GetTFromPT100(float om);

    #ifdef __cplusplus
    }
#endif
