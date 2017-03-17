#pragma once

#include "Type.h"
/*
��Ƭ���ڲ�flash����
 */
class Flash
{
    public:
        int Erase(uint addr, int size);
        int WriteBlock(uint addr, byte *pBuffer, int size);
        int Read(uint addr, byte *pBuffer, int size);
        int Write(uint addr, byte *pBuffer, int size);

        int Size;
        int BytesPerBlock;
};
#ifdef DEBUG
    void TestFlash();
#endif
