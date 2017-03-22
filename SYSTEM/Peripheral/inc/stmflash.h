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
        static ushort ReadHalfWord(uint faddr); //��������  
        static void WriteLenByte(uint addr, uint DataToWrite, ushort Len); //ָ����ַ��ʼд��ָ�����ȵ�����
        static uint ReadLenByte(uint addr, ushort Len); //ָ����ַ��ʼ��ȡָ����������
        static void Write(uint addr, ushort *pBuffer, ushort NumToWrite); //��ָ����ַ��ʼд��ָ�����ȵ�����
        static void Read(uint addr, ushort *pBuffer, ushort NumToRead); //��ָ����ַ��ʼ����ָ�����ȵ�����
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		static void Write_NoCheck(uint addr, ushort *pBuffer, ushort NumToWrite);
		static union Buffer
		{
			byte buf[2048];
			ushort buf16[1024];
		}Buffer;
};
