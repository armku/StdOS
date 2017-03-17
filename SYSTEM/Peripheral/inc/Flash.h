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
        ushort ReadHalfWord(uint addr);
        void Read(uint addr, ushort *pBuffer, ushort size);
        void Write(uint addr, ushort *pBuffer, ushort size);
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort size);
        
        int Size;
        int BytesPerBlock;
};
