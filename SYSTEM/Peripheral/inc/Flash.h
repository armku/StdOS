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
        #ifdef DEBUG
            void TestFlash();
        #endif 
    private:
        ushort ReadHalfWord(uint faddr);
        void Write_NoCheck(uint WriteAddr, ushort *pBuffer, ushort NumToWrite);
        void Read(uint ReadAddr, ushort *pBuffer, ushort NumToRead);
        void Write(uint WriteAddr, ushort *pBuffer, ushort NumToWrite);

        int Size;
        int BytesPerBlock;
};
