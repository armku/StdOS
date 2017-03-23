#pragma once
#include "Type.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#if STM32_FLASH_SIZE<256
    #define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
    #define STM_SECTOR_SIZE	2048
#endif 
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ

class STMFLASH
{
    public:        
        ushort ReadHalfWord(uint faddr); //��������  
        void WriteLenByte(uint addr, uint DataToWrite, ushort len); //ָ����ַ��ʼд��ָ�����ȵ�����
        uint ReadLenByte(uint addr, ushort len); //ָ����ַ��ʼ��ȡָ����������
        void Write(uint addr, ushort *pBuffer, ushort NumToWrite); //��ָ����ַ��ʼд��ָ�����ȵ�����
        void Read(uint addr, ushort *pBuffer, ushort NumToRead); //��ָ����ַ��ʼ����ָ�����ȵ�����
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		void Write_NoCheck(uint addr, ushort *pBuffer, ushort NumToWrite);		
};
