#pragma once
#include "Type.h"

class STMFLASH
{
    public:       
		STMFLASH();
		void SetFlashSize(uint flashsize);
        ushort ReadHalfWord(uint faddr); //��������  
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
