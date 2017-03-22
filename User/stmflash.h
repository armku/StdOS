#pragma once
#include "Type.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64 	 		//��ѡSTM32��FLASH������С(��λΪK)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ

class STMFLASH
{
    public:
        static void STMFLASH_Write_NoCheck(uint WriteAddr, ushort *pBuffer, ushort NumToWrite);
        static ushort STMFLASH_ReadHalfWord(uint faddr); //��������  
        static void STMFLASH_WriteLenByte(uint WriteAddr, uint DataToWrite, ushort Len); //ָ����ַ��ʼд��ָ�����ȵ�����
        static uint STMFLASH_ReadLenByte(uint ReadAddr, ushort Len); //ָ����ַ��ʼ��ȡָ����������
        static void STMFLASH_Write(uint WriteAddr, ushort *pBuffer, ushort NumToWrite); //��ָ����ַ��ʼд��ָ�����ȵ�����
        static void STMFLASH_Read(uint ReadAddr, ushort *pBuffer, ushort NumToRead); //��ָ����ַ��ʼ����ָ�����ȵ�����
	
};
