#pragma once

#include "Type.h"
/*
��Ƭ���ڲ�flash����
 */
class Flash
{
    public:
		Flash();
        int Erase(uint addr, int size);
        int WriteBlock(uint addr, byte *pBuffer, int size);
        int Read(uint addr, byte *pBuffer, int size);
        int Write(uint addr, byte *pBuffer, int size);
		void SetSectorSize(int bytesperblock,int size=-1);//����������С��������С����λK
        #ifdef DEBUG
            void TestFlash();
        #endif 
    private:
        ushort ReadHalfWord(uint addr);
        void Read(uint addr, ushort *pBuffer, ushort size);
        void Write(uint addr, ushort *pBuffer, ushort size);
		
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort size);
	
		bool sectorNeedErase(uint addr);//�����Ƿ���Ҫ����
		int eraseSector(uint addr);//������������	
		void WriteSector(uint addr, ushort *pBuffer, ushort size);//д����
        
        int Size;
        int BytesPerBlock;
};
