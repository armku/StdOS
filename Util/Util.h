#ifndef _UTIL_H
#define _UTIL_H

    #include "Type.h"

    void bubbleSort(float *buf, uint len); //���ø�����
    void SetBufFloat(byte buf[], ushort pos, float da, byte type = 0); //���ø����� 0:С�� 1�����
    float GetBufFloat(byte buf[], ushort pos,byte type=0);//���ø����� 0:С�� 1�����
    ushort GetBufUshort(byte buf[], ushort pos);
    void SetBufUshort(byte buf[], ushort pos, ushort da); //�ӵ������¶�
    float GetTFromPT100(float om);
#endif
