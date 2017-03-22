#pragma once
#include "Type.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值

class STMFLASH
{
    public:        
        static ushort STMFLASH_ReadHalfWord(uint faddr); //读出半字  
        static void STMFLASH_WriteLenByte(uint addr, uint DataToWrite, ushort Len); //指定地址开始写入指定长度的数据
        static uint STMFLASH_ReadLenByte(uint addr, ushort Len); //指定地址开始读取指定长度数据
        static void STMFLASH_Write(uint addr, ushort *pBuffer, ushort NumToWrite); //从指定地址开始写入指定长度的数据
        static void STMFLASH_Read(uint addr, ushort *pBuffer, ushort NumToRead); //从指定地址开始读出指定长度的数据
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		static void STMFLASH_Write_NoCheck(uint addr, ushort *pBuffer, ushort NumToWrite);
};
