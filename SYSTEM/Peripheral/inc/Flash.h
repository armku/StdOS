#pragma once

#include "Type.h"
/*
��Ƭ���ڲ�flash����
 */
class Flash
{
    public:
		Flash();
        int Read(uint addr, void *pBuffer, int size);
        int Write(uint addr, void *pBuffer, int size);
		void SetSectorSize(int bytesperblock,int size=-1);//����������С��������С����λK
        #ifdef DEBUG
            void TestFlash();
        #endif 
    private:
        ushort ReadHalfWord(uint addr);
		union SectorBuffer
		{
			byte buf[2048];//�ֽڻ�����
			ushort buf16[1024];//������			
		}SectorBuffer;//���2K
		
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort size);	
		bool sectorNeedErase(uint addr);//�����Ƿ���Ҫ����
		int eraseSector(uint addr);//������������	
		void WriteSector(uint addr, void *pBuffer);//д����
        
        int Size;//Flash��С
        int BytesPerBlock;//������С
};
