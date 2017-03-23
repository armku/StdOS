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
		uint sectorSize;//扇区大小
		uint flashSize;//Flash容量大小，单位K
		ushort readHalfWord(uint faddr); //读出半字  
        void writeNoCheck(uint addr, ushort *pBuffer, ushort len);	
		void write(uint addr, ushort *pBuffer, ushort len); //从指定地址开始写入指定长度的数据
        void read(uint addr, ushort *pBuffer, ushort len); //从指定地址开始读出指定长度的数据
};
