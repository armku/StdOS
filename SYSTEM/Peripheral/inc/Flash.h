#pragma once

#include "Type.h"
/*
单片机内部flash操作
 */
class Flash
{
    public:
		Flash();
        int Read(uint addr, void *pBuffer, int size);
		int ReadOpposite(uint addr, void *pBuffer, int size);//读取相对位置
        int Write(uint addr, void *pBuffer, int size);
		int WriteOpposite(uint addr, void *pBuffer, int size);//写相对位置
		void SetSectorSize(int bytesperblock,int size=-1);//设置扇区大小，容量大小，单位K
        #ifdef DEBUG
         static   void TestFlash();
        #endif 
    private:
        ushort ReadHalfWord(uint addr);
		union SectorBuffer
		{
			byte buf[2048];//字节缓冲区
			ushort buf16[1024];//缓冲区			
		}SectorBuffer;//最多2K
		
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort size);	
		bool sectorNeedErase(uint addr);//扇区是否需要擦除
		int eraseSector(uint addr);//擦除整个扇区	
		void WriteSector(uint addr, void *pBuffer);//写扇区
        
        int Size;//Flash大小
        int BytesPerBlock;//扇区字节大小
};
