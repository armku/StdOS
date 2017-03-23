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
		ushort ReadHalfWord(uint faddr); //��������  
        void Write_NoCheck(uint addr, ushort *pBuffer, ushort len);	
		void Write(uint addr, ushort *pBuffer, ushort len); //��ָ����ַ��ʼд��ָ�����ȵ�����
        void Read(uint addr, ushort *pBuffer, ushort len); //��ָ����ַ��ʼ����ָ�����ȵ�����
};
