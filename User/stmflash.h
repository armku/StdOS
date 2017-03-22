#pragma once
#include "stdint.h"

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
        static void STMFLASH_Write_NoCheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
        static uint16_t STMFLASH_ReadHalfWord(uint32_t faddr); //读出半字  
        static void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len); //指定地址开始写入指定长度的数据
        static uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len); //指定地址开始读取指定长度数据
        static void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite); //从指定地址开始写入指定长度的数据
        static void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead); //从指定地址开始读出指定长度的数据
};
