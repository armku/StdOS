#ifndef _UTIL_H
#define _UTIL_H

    #include "Type.h"

    void bubbleSort(float *buf, uint32_t len); //���ø�����
    void SetBufFloat(byte buf[], uint16_t pos, float da, byte type = 0); //���ø����� 0:С�� 1�����
    float GetBufFloat(byte buf[], uint16_t pos,byte type=0);//���ø����� 0:С�� 1�����
    uint16_t GetBufUshort(byte buf[], uint16_t pos,byte type=0);
    void SetBufUshort(byte buf[], uint16_t pos, uint16_t da,byte type=0); //�ӵ������¶�
    float GetTFromPT100(float om);
#endif
