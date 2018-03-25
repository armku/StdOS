#ifndef _UTIL_H
#define _UTIL_H

    #include "Type.h"

    void bubbleSort(float *buf, uint32_t len); //设置浮点数
    void SetBufFloat(byte buf[], uint16_t pos, float da, byte type = 0); //设置浮点数 0:小端 1：大端
    float GetBufFloat(byte buf[], uint16_t pos,byte type=0);//设置浮点数 0:小端 1：大端
    uint16_t GetBufUshort(byte buf[], uint16_t pos,byte type=0);
    void SetBufUshort(byte buf[], uint16_t pos, uint16_t da,byte type=0); //从电阻获得温度
    float GetTFromPT100(float om);
#endif
