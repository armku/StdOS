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
        static ushort STMFLASH_ReadHalfWord(uint faddr); //��������  
        static void STMFLASH_WriteLenByte(uint addr, uint DataToWrite, ushort len); //ָ����ַ��ʼд��ָ�����ȵ�����
        static uint STMFLASH_ReadLenByte(uint addr, ushort len); //ָ����ַ��ʼ��ȡָ����������
        static void STMFLASH_Write(uint addr, ushort *pBuffer, ushort len); //��ָ����ַ��ʼд��ָ�����ȵ�����
        static void STMFLASH_Read(uint addr, ushort *pBuffer, ushort len); //��ָ����ַ��ʼ����ָ�����ȵ�����
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		static void STMFLASH_Write_NoCheck(uint addr, ushort *pBuffer, ushort len);
};
