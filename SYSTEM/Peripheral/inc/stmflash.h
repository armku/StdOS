#pragma once
#include "Type.h"

class STMFLASH
{
    public:       
		STMFLASH();
		void SetFlashSize(uint flashsize);        
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		uint sectorSize;//������С
		uint flashSize;//Flash������С����λK
		ushort readHalfWord(uint faddr); //��������  
        void writeNoCheck(uint addr, ushort *pBuffer, ushort len);	
		void write(uint addr, ushort *pBuffer, ushort len); //��ָ����ַ��ʼд��ָ�����ȵ�����
        void read(uint addr, ushort *pBuffer, ushort len); //��ָ����ַ��ʼ����ָ�����ȵ�����
};
