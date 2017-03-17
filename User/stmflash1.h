#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
#include "Type.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值
 

ushort STMFLASH_ReadHalfWord(uint faddr);		  //读出半字  
void STMFLASH_WriteLenByte(uint WriteAddr,uint DataToWrite,ushort Len);	//指定地址开始写入指定长度的数据
uint STMFLASH_ReadLenByte(uint ReadAddr,ushort Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(uint WriteAddr,ushort *pBuffer,ushort NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(uint ReadAddr,ushort *pBuffer,ushort NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(uint WriteAddr,ushort WriteData);								   
#endif

















