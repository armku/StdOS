#pragma once
#include "Type.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K)
#if STM32_FLASH_SIZE<256
    #define STM_SECTOR_SIZE 1024 //字节
#else 
    #define STM_SECTOR_SIZE	2048
#endif 
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值

class STMFLASH
{
    public:        
        ushort ReadHalfWord(uint faddr); //读出半字  
        void WriteLenByte(uint addr, uint DataToWrite, ushort len); //指定地址开始写入指定长度的数据
        uint ReadLenByte(uint addr, ushort len); //指定地址开始读取指定长度数据
        void Write(uint addr, ushort *pBuffer, ushort NumToWrite); //从指定地址开始写入指定长度的数据
        void Read(uint addr, ushort *pBuffer, ushort NumToRead); //从指定地址开始读出指定长度的数据
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort NumToWrite);		
};
