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
		void SetSize(int size,int bytesperblock);//���ô�С��������С
        #ifdef DEBUG
            void TestFlash();
        #endif 
    private:
        ushort ReadHalfWord(uint addr);
        void Read(uint addr, ushort *pBuffer, ushort size);
        void Write(uint addr, ushort *pBuffer, ushort size);
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort size);
	
		int eraseSector(uint addr);//������������
        
        int Size;
        int BytesPerBlock;
};
