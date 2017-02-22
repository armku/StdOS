#pragma once

#include "Type.h"

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    void bubbleSort(float *buf, uint len); //���ø�����
    void SetBufFloat(byte buf[], ushort pos, float da); //���ø�����
    float GetBufFloat(byte buf[], ushort pos);
    ushort GetBufUshort(byte buf[], ushort pos);
    void SetBufUshort(byte buf[], ushort pos, ushort da); //�ӵ������¶�
    float GetTFromPT100(float om);

    #ifdef __cplusplus
    }
#endif
