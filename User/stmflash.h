#pragma once
#include "stdint.h"

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
        static void STMFLASH_Write_NoCheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
        static uint16_t STMFLASH_ReadHalfWord(uint32_t faddr); //��������  
        static void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len); //ָ����ַ��ʼд��ָ�����ȵ�����
        static uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len); //ָ����ַ��ʼ��ȡָ����������
        static void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite); //��ָ����ַ��ʼд��ָ�����ȵ�����
        static void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead); //��ָ����ַ��ʼ����ָ�����ȵ�����
};
