#pragma once
#include "Type.h"

class STMFLASH
{
    public:       
		STMFLASH();
		void SetFlashSize(uint flashsize);
        ushort ReadHalfWord(uint faddr); //读出半字  
        void Write(uint addr, ushort *pBuffer, ushort len); //从指定地址开始写入指定长度的数据
        void Read(uint addr, ushort *pBuffer, ushort len); //从指定地址开始读出指定长度的数据
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort len);		
		uint sectorSize;//扇区大小
		uint flashSize;//Flash容量大小，单位K
};
