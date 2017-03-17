#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
#include "Type.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ
 

ushort STMFLASH_ReadHalfWord(uint faddr);		  //��������  
void STMFLASH_WriteLenByte(uint WriteAddr,uint DataToWrite,ushort Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
uint STMFLASH_ReadLenByte(uint ReadAddr,ushort Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(uint WriteAddr,ushort *pBuffer,ushort NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(uint ReadAddr,ushort *pBuffer,ushort NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(uint WriteAddr,ushort WriteData);								   
#endif

















