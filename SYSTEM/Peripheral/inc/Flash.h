#pragma once

#include "Type.h"
/*
单片机内部flash操作
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
