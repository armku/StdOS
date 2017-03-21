#include "Flash.h"
#include "stm32f10x.h"
#include "Sys.h"
#include "stdlib.h"
#ifdef DEBUG
    #include "stdlib.h"
#endif 

/*
С������16~32K
��������64~128K
�������Ǵ���256K

������ÿҳ2k����С����ÿҳ1k
 */
Flash::Flash()
{
    this->BytesPerBlock = 1024;
    this->Size = 1024 * 16;
}

//���ô�С��������С
void Flash::SetSectorSize(int bytesperblock, int size)
{
    this->BytesPerBlock = bytesperblock;
    if (size < 0)
    {
        this->Size = 1024;
    }
}

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
    if (size <= 0)
    {
        return 0;
    }
    for (int i = 0; i < size / 2; i++)
    {
        ((ushort*)pBuffer)[i] = ReadHalfWord(addr); //��ȡ2���ֽ�.
        addr += 2; //ƫ��2���ֽ�.	
    }
    if (size % 2)
    {
        pBuffer[size - 1] = this->ReadHalfWord(addr - 1);
    }
    return size;
}

int Flash::Write(uint addr, byte *pBuffer, int size)
{
    return 0;
}

#ifdef DEBUG

    void Flash::TestFlash()
    {
        
            debug_printf("\r\n\r\n");
            debug_printf("TestFlash Start......\r\n");

            uint addr = 0x08010000;

            Flash flash;
			flash.SetSectorSize(2048,512);
            debug_printf("FlashSize: %d kBytes  BytesPerBlock: %d kBytes\r\n", flash.Size, flash.BytesPerBlock);
		#if 0
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
        #else            
            ushort buftest1[20];
			debug_printf("���Կ�ʼ\r\n");
            for (int i = 0; i < 20; i++)
            {
                buftest1[i] = 1000+i;
            }
			debug_printf("-1\r\n");
            flash.Write(addr, buftest1, 20);
			debug_printf("0\r\n");
            for (int i = 0; i < 20; i++)
            {
                buftest1[i] = 0;
            }
			debug_printf("1\r\n");
            flash.Read(addr, buftest1, 20);
			debug_printf("2\r\n");
            for (int i = 0; i < 20; i++)
            {
                debug_printf("%d:%d\r\n", i, buftest1[i]);
            }
            debug_printf("�������\r\n");
        #endif 
    }
#endif 
//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ	

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
ushort Flash::ReadHalfWord(uint addr)
{
    return *(volatile ushort*)addr;
}

//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void Flash::Write_NoCheck(uint addr, ushort *pBuffer, ushort size)
{
    ushort i;
    for (i = 0; i < size; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //��ַ����2.
    }
}

//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
ushort STMFLASH_BUF1[1024]; //�����2K�ֽ�
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void Flash::Read(uint addr, ushort *pBuffer, ushort size)
{
    ushort i;
    for (i = 0; i < size; i++)
    {
        pBuffer[i] = ReadHalfWord(addr); //��ȡ2���ֽ�.
        addr += 2; //ƫ��2���ֽ�.	
    }

}



void Flash::Write(uint addr, ushort *pBuffer, ushort size)
{
    uint secpos; //������ַ
    ushort secoff; //������ƫ�Ƶ�ַ(16λ�ּ���)
    ushort secremain; //������ʣ���ַ(16λ�ּ���)	   
    ushort i;
    uint offaddr; //ȥ��0X08000000��ĵ�ַ
    if (addr < STM32_FLASH_BASE || (addr >= (STM32_FLASH_BASE + 1024 * this->Size)))
        return ;
    //�Ƿ���ַ
    FLASH_Unlock(); //����
    offaddr = addr - STM32_FLASH_BASE; //ʵ��ƫ�Ƶ�ַ.
    secpos = offaddr / this->BytesPerBlock; //������ַ  0~127 for STM32F103RBT6
    secoff = (offaddr % this->BytesPerBlock) / 2; //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain = this->BytesPerBlock / 2-secoff; //����ʣ��ռ��С   
    if (size <= secremain)
        secremain = size;
    //�����ڸ�������Χ
    while (1)
    {
        Read(secpos *this->BytesPerBlock + STM32_FLASH_BASE, STMFLASH_BUF1, this->BytesPerBlock / 2); //������������������
        for (i = 0; i < secremain; i++)
        //У������
        {
            if (STMFLASH_BUF1[secoff + i] != 0XFFFF)
                break;
            //��Ҫ����  	  
        }
        if (i < secremain)
        //��Ҫ����
        {
            FLASH_ErasePage(secpos *this->BytesPerBlock + STM32_FLASH_BASE); //�����������
            for (i = 0; i < secremain; i++)
            //����
            {
                STMFLASH_BUF1[i + secoff] = pBuffer[i];
            }
			this->WriteSector(secpos *this->BytesPerBlock + STM32_FLASH_BASE, STMFLASH_BUF1, this->BytesPerBlock / 2); //д����������  
        }
        else
        {
           this->WriteSector(addr, pBuffer, secremain);
        }
        //д�Ѿ������˵�,ֱ��д������ʣ������. 				   
        if (size == secremain)
            break;
        //д�������
        else
        //д��δ����
        {
            secpos++; //������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0 	 
            pBuffer += secremain; //ָ��ƫ��
            addr += secremain; //д��ַƫ��	   
            size -= secremain; //�ֽ�(16λ)���ݼ�
            if (size > (this->BytesPerBlock / 2))
                secremain = this->BytesPerBlock / 2;
            //��һ����������д����
            else
                secremain = size;
            //��һ����������д����
        }
    };
    FLASH_Lock(); //����
}
//������������
int Flash::eraseSector(uint addr)
{
    int i = 0;
    Read(addr, STMFLASH_BUF1, this->BytesPerBlock / 2); //������������������
    for (i = 0; i < this->BytesPerBlock / 2; i++)
    {
        if (STMFLASH_BUF1[i] != 0XFFFF)
            break;
    }
    if (i < this->BytesPerBlock / 2)
    {
        //��Ҫ����
        FLASH_ErasePage(addr); //�����������		
    }
    return this->BytesPerBlock;
}
//д����
void Flash::WriteSector(uint addr, ushort *pBuffer, ushort size)
{
  this->Write_NoCheck(addr, pBuffer, size);
}
