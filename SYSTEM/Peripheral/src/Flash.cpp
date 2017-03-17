#include "Flash.h"
#include "stm32f10x.h"
#include "Sys.h"
#include "stdlib.h"
#ifdef TestFlsh
    #include "stdlib.h"
#endif 

int Flash::Erase(uint addr, int size)
{
    return 0;
}

int Flash::WriteBlock(uint addr, byte *pBuffer, int size)
{
    return 0;
}

int Flash::Read(uint addr, byte *pBuffer, int size)
{
    return 0;
}

int Flash::Write(uint addr, byte *pBuffer, int size)
{
    return 0;
}

#ifdef DEBUG

    void TestFlash()
    {
        debug_printf("\r\n\r\n");
        debug_printf("TestFlash Start......\r\n");

        uint addr = 0x08004000;

        Flash flash;
        debug_printf("FlashSize: %d Bytes  BytesPerBlock: %d Bytes\r\n", flash.Size, flash.BytesPerBlock);
        flash.Erase(addr, 0x100);

        byte buf[] = "STM32F10x SPI Firmware Library Example: communication with an AT45DB SPI FLASH";
        uint size = ArrayLength(buf);
        flash.WriteBlock(addr, buf, size);

        byte *rx = (byte*)malloc(size);
        flash.Read(addr, rx, size);

        int n = 0;
        for (int i = 0; i < size; i++)
        {
            if (buf[i] != rx[i])
                n++;
        }
        debug_printf("diffent %d\r\n", n);

        // ���ɲ���
        //flash.Erase(addr, 0x100);
        flash.Write(addr, buf, size);

        flash.Read(addr, rx, size);

        n = 0;
        for (int i = 0; i < size; i++)
        {
            if (buf[i] != rx[i])
                n++;
        }
        debug_printf("diffent %d\r\n", n);

        free(rx);

        debug_printf("\r\nTestFlash Finish!\r\n");
    }
#endif 
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ	





//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
	ushort Flash::ReadHalfWord(uint faddr)
{
    return *(volatile ushort*)faddr;
}

#if STM32_FLASH_WREN	//���ʹ����д   
    //������д��
    //WriteAddr:��ʼ��ַ
    //pBuffer:����ָ��
    //NumToWrite:����(16λ)��   
    void Flash::Write_NoCheck(uint WriteAddr, ushort *pBuffer, ushort NumToWrite)
    {
        ushort i;
        for (i = 0; i < NumToWrite; i++)
        {
            FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
            WriteAddr += 2; //��ַ����2.
        }
    }
    //��ָ����ַ��ʼд��ָ�����ȵ�����
    //WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
    //pBuffer:����ָ��
    //NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
    #if STM32_FLASH_SIZE<256
        #define STM_SECTOR_SIZE 1024 //�ֽ�
    #else 
        #define STM_SECTOR_SIZE	2048
    #endif 
    ushort STMFLASH_BUF[STM_SECTOR_SIZE / 2]; //�����2K�ֽ�
    //��ָ����ַ��ʼ����ָ�����ȵ�����
    //ReadAddr:��ʼ��ַ
    //pBuffer:����ָ��
    //NumToWrite:����(16λ)��
    void Flash::STMFLASH_Read(uint ReadAddr, ushort *pBuffer, ushort NumToRead)
    {
        ushort i;
        for (i = 0; i < NumToRead; i++)
        {
            pBuffer[i] = ReadHalfWord(ReadAddr); //��ȡ2���ֽ�.
            ReadAddr += 2; //ƫ��2���ֽ�.	
        }
    }
    void Flash::STMFLASH_Write(uint WriteAddr, ushort *pBuffer, ushort NumToWrite)
    {
        uint secpos; //������ַ
        ushort secoff; //������ƫ�Ƶ�ַ(16λ�ּ���)
        ushort secremain; //������ʣ���ַ(16λ�ּ���)	   
        ushort i;
        uint offaddr; //ȥ��0X08000000��ĵ�ַ
        if (WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
            return ;
        //�Ƿ���ַ
        FLASH_Unlock(); //����
        offaddr = WriteAddr - STM32_FLASH_BASE; //ʵ��ƫ�Ƶ�ַ.
        secpos = offaddr / STM_SECTOR_SIZE; //������ַ  0~127 for STM32F103RBT6
        secoff = (offaddr % STM_SECTOR_SIZE) / 2; //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
        secremain = STM_SECTOR_SIZE / 2-secoff; //����ʣ��ռ��С   
        if (NumToWrite <= secremain)
            secremain = NumToWrite;
        //�����ڸ�������Χ
        while (1)
        {
            STMFLASH_Read(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2); //������������������
            for (i = 0; i < secremain; i++)
            //У������
            {
                if (STMFLASH_BUF[secoff + i] != 0XFFFF)
                    break;
                //��Ҫ����  	  
            }
            if (i < secremain)
            //��Ҫ����
            {
                FLASH_ErasePage(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE); //�����������
                for (i = 0; i < secremain; i++)
                //����
                {
                    STMFLASH_BUF[i + secoff] = pBuffer[i];
                }
                Write_NoCheck(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2); //д����������  
            }
            else
                Write_NoCheck(WriteAddr, pBuffer, secremain);
            //д�Ѿ������˵�,ֱ��д������ʣ������. 				   
            if (NumToWrite == secremain)
                break;
            //д�������
            else
            //д��δ����
            {
                secpos++; //������ַ��1
                secoff = 0; //ƫ��λ��Ϊ0 	 
                pBuffer += secremain; //ָ��ƫ��
                WriteAddr += secremain; //д��ַƫ��	   
                NumToWrite -= secremain; //�ֽ�(16λ)���ݼ�
                if (NumToWrite > (STM_SECTOR_SIZE / 2))
                    secremain = STM_SECTOR_SIZE / 2;
                //��һ����������д����
                else
                    secremain = NumToWrite;
                //��һ����������д����
            }
        };
        FLASH_Lock(); //����
    }
#endif 



//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Flash::Test_Write(uint WriteAddr, ushort WriteData)
{
    STMFLASH_Write(WriteAddr, &WriteData, 1); //д��һ���� 
}
