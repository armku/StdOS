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
    else
    {
        this->Size = size;
    }
}
//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ	
int Flash::Read(uint addr, void *pBuffer, int size)
{
    ushort tmp;
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
        tmp = this->ReadHalfWord(addr - 1);
        ((byte*)pBuffer)[size - 1] = tmp &0xff;
    }
    return size;
}
//��ȡ���λ��
int Flash::ReadOpposite(uint addr, void *pBuffer, int size)
{
	if(addr<this->Size*1024)
	{
		return this->Read(addr+STM32_FLASH_BASE,pBuffer,size);
	}
		return 0;
	
}
//д���λ��
int Flash::WriteOpposite(uint addr, void *pBuffer, int size)
{
	if(addr<this->Size*1024)
	{
		return this->Write(addr+STM32_FLASH_BASE,pBuffer,size);
	}
	
	return 0;
}



//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
ushort Flash::ReadHalfWord(uint addr)
{
    ushort ret = 0;
    if (addr % 2)
    {
        ret = *(volatile ushort*)addr;
    }
    return ret;
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

/*
--------|--------|--------|--------|  ����
 **************         ����
1        2          3     ���

 */
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
int Flash::Write(uint addr, void *pBuffer, int size)
{
    uint secpos; //������ַ
    ushort secoff; //������ƫ��	
    uint offaddr; ////ȥ��0X08000000��ĵ�ַ

    offaddr = addr - STM32_FLASH_BASE; //ʵ��ƫ�Ƶ�ַ
    secpos = offaddr / this->BytesPerBlock; //������ַ  0~127 for STM32F103RBT6
    secoff = (offaddr % this->BytesPerBlock); //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    int size1 = 0; //��һ�����ֽڴ�С
    int pos = 0;
    if (offaddr % this->BytesPerBlock == 0)
    {
        size1 = 0;
    }
    else
    {
        size1 = (secpos + 1) *this->BytesPerBlock + STM32_FLASH_BASE - addr;
    }
    if (size1 > this->BytesPerBlock)
    {
        //������ʱ��ֱ�ӷ��أ�������������˺�
        return  - 2;
    }


    //��ȡ
    FLASH_Unlock(); //����
    //�����һ����
    if (size1)
    {
        //����һ������Ҫ����ʱ
        this->Read(secpos *this->BytesPerBlock + STM32_FLASH_BASE, this->SectorBuffer.buf, this->BytesPerBlock);
        for (int i = 0; i < size1; i++)
        {
            this->SectorBuffer.buf[secoff + i] = ((byte*)pBuffer)[i];
        }
        this->eraseSector(secpos *this->BytesPerBlock + STM32_FLASH_BASE); //�����������
        this->WriteSector(secpos *this->BytesPerBlock + STM32_FLASH_BASE, this->SectorBuffer.buf16);
        size -= size1;
        addr += size1;
        pos += size1;
    }
    //����ڶ�����
    while (size >= this->BytesPerBlock)
    {
        this->eraseSector(addr);
        this->WriteSector(addr, (byte*)pBuffer + pos);
        size -= this->BytesPerBlock;
        pos += this->BytesPerBlock;
        addr += this->BytesPerBlock;
    }
    //�����������
    if (size > 0)
    {
        //������������Ҫ����ʱ
        //this->Read(addr, this->SectorBuffer.buf, this->BytesPerBlock);
        //this->eraseSector(addr);
        for (int i = 0; i < size; i++)
        {
            this->SectorBuffer.buf[i] = ((byte*)pBuffer)[pos + i];
        }
        //this->WriteSector(addr, this->SectorBuffer.buf);
		STMFLASH_Write(addr,this->SectorBuffer.buf16,this->BytesPerBlock/2);
    }
    FLASH_Lock(); //����
    return size;
}

//�����Ƿ���Ҫ����
bool Flash::sectorNeedErase(uint addr)
{
    Read(addr, this->SectorBuffer.buf16, this->BytesPerBlock / 2); //������������������
    for (int i = 0; i < this->BytesPerBlock / 2; i++)
    {
        if (this->SectorBuffer.buf16[i] != 0XFFFF)
            return true;
    }

    return false;
}

//������������
int Flash::eraseSector(uint addr)
{
    if (sectorNeedErase(addr))
    {
        FLASH_ErasePage(addr); //�����������		
    }
    return this->BytesPerBlock;
}

//д����
void Flash::WriteSector(uint addr, void *pBuffer)
{
    this->Write_NoCheck(addr, (ushort*)pBuffer, this->BytesPerBlock / 2);
}

#ifdef DEBUG

    void Flash::TestFlash()
    {

        debug_printf("\r\n\r\n");
        debug_printf("TestFlash Start......\r\n");

        uint addr = 0X08070000;

        Flash flash;
        flash.SetSectorSize(2048, 512);
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
			addr=1024*6;
            byte buftest1[120];
            debug_printf("���Կ�ʼ\r\n");
            for (int i = 0; i < 20; i++)
            {
                buftest1[i] = 1000+i;
            }
            debug_printf("-1 \r\n");
            flash.WriteOpposite(addr, buftest1, 20);
            debug_printf("0 \r\n");
            for (int i = 0; i < 20; i++)
            {
                buftest1[i] = 0;
            }
            debug_printf("1 \r\n");
            flash.ReadOpposite(addr, buftest1, 20);
            debug_printf("2 \r\n");
            for (int i = 0; i < 20; i++)
            {
                debug_printf("%d:%d\t", i, buftest1[i]);
            }
            debug_printf("�������\r\n");
        #endif 
    }
#endif
