#ifndef _UTIL_H
#define _UTIL_H

    #include "Type.h"

    void bubbleSort(float *buf, uint32_t len); //���ø�����
    void SetBufFloat(uint8_t buf[], uint16_t pos, float da, uint8_t type = 0); //���ø����� 0:С�� 1�����
    float GetBufFloat(uint8_t buf[], uint16_t pos,uint8_t type=0);//���ø����� 0:С�� 1�����
    uint16_t GetBufUshort(uint8_t buf[], uint16_t pos,uint8_t type=0);
    void SetBufUshort(uint8_t buf[], uint16_t pos, uint16_t da,uint8_t type=0); //�ӵ������¶�
    float GetTFromPT100(float om);
#endif
