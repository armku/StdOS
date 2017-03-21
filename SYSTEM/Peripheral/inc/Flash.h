#pragma once

#include "Type.h"
/*
单片机内部flash操作
 */
class Flash
{
    public:
		Flash();
        int Erase(uint addr, int size);
        int WriteBlock(uint addr, byte *pBuffer, int size);
        int Read(uint addr, byte *pBuffer, int size);
        int Write(uint addr, byte *pBuffer, int size);
		void SetSectorSize(int bytesperblock,int size=-1);//设置扇区大小，容量大小，单位K
        #ifdef DEBUG
            void TestFlash();
        #endif 
    private:
        ushort ReadHalfWord(uint addr);
        void Read(uint addr, ushort *pBuffer, ushort size);
        void Write(uint addr, ushort *pBuffer, ushort size);
		
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort size);
	
		bool sectorNeedErase(uint addr);//扇区是否需要擦除
		int eraseSector(uint addr);//擦除整个扇区	
		void WriteSector(uint addr, ushort *pBuffer, ushort size);//写扇区
        
        int Size;
        int BytesPerBlock;
};
