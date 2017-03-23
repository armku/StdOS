#pragma once
#include "Type.h"

class STMFLASH
{
    public:       
		STMFLASH();
        ushort ReadHalfWord(uint faddr); //��������  
        void WriteLenByte(uint addr, uint DataToWrite, ushort len); //ָ����ַ��ʼд��ָ�����ȵ�����
        uint ReadLenByte(uint addr, ushort len); //ָ����ַ��ʼ��ȡָ����������
        void Write(uint addr, ushort *pBuffer, ushort len); //��ָ����ַ��ʼд��ָ�����ȵ�����
        void Read(uint addr, ushort *pBuffer, ushort len); //��ָ����ַ��ʼ����ָ�����ȵ�����
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort len);		
		uint sectorSize;//������С
		uint flashSize;//Flash������С����λK
};
