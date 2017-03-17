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
	
		ushort ReadHalfWord(uint faddr);
		void Write_NoCheck(uint WriteAddr, ushort *pBuffer, ushort NumToWrite);
		void STMFLASH_Read(uint ReadAddr, ushort *pBuffer, ushort NumToRead);
		void STMFLASH_Write(uint WriteAddr, ushort *pBuffer, ushort NumToWrite);
		void Test_Write(uint WriteAddr, ushort WriteData);
		
        int Size;
        int BytesPerBlock;
};
#ifdef DEBUG
    void TestFlash();
#endif
