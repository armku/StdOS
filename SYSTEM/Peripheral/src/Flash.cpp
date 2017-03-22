#include "Flash.h"
#include "stm32f10x.h"
#include "Sys.h"
#include "stdlib.h"
#ifdef DEBUG
    #include "stdlib.h"
#endif 

/*
小容量是16~32K
中容量是64~128K
大容量是大于256K

大容量每页2k，中小容量每页1k
 */
Flash::Flash()
{
    this->BytesPerBlock = 1024;
    this->Size = 1024 * 16;
}

//设置大小及扇区大小
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
//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址	
int Flash::Read(uint addr, void *pBuffer, int size)
{
    ushort tmp;
    if (size <= 0)
    {
        return 0;
    }
    for (int i = 0; i < size / 2; i++)
    {
        ((ushort*)pBuffer)[i] = ReadHalfWord(addr); //读取2个字节.
        addr += 2; //偏移2个字节.	
    }
    if (size % 2)
    {
        tmp = this->ReadHalfWord(addr - 1);
        ((byte*)pBuffer)[size - 1] = tmp &0xff;
    }
    return size;
}
//读取相对位置
int Flash::ReadOpposite(uint addr, void *pBuffer, int size)
{
	if(addr<this->Size*1024)
	{
		return this->Read(addr+STM32_FLASH_BASE,pBuffer,size);
	}
		return 0;
	
}
//写相对位置
int Flash::WriteOpposite(uint addr, void *pBuffer, int size)
{
	if(addr<this->Size*1024)
	{
		return this->Write(addr+STM32_FLASH_BASE,pBuffer,size);
	}
	
	return 0;
}



//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
ushort Flash::ReadHalfWord(uint addr)
{
    ushort ret = 0;
    if (addr % 2)
    {
        ret = *(volatile ushort*)addr;
    }
    return ret;
}

//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void Flash::Write_NoCheck(uint addr, ushort *pBuffer, ushort size)
{
    ushort i;
    for (i = 0; i < size; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //地址增加2.
    }
}

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)

/*
--------|--------|--------|--------|  扇区
 **************         数据
1        2          3     编号

 */
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
int Flash::Write(uint addr, void *pBuffer, int size)
{
    uint secpos; //扇区地址
    ushort secoff; //扇区内偏移	
    uint offaddr; ////去掉0X08000000后的地址

    offaddr = addr - STM32_FLASH_BASE; //实际偏移地址
    secpos = offaddr / this->BytesPerBlock; //扇区地址  0~127 for STM32F103RBT6
    secoff = (offaddr % this->BytesPerBlock); //在扇区内的偏移(2个字节为基本单位.)
    int size1 = 0; //第一部分字节大小
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
        //错误发生时，直接返回，放置造成意外伤害
        return  - 2;
    }


    //读取
    FLASH_Unlock(); //解锁
    //处理第一部分
    if (size1)
    {
        //当第一部分需要处理时
        this->Read(secpos *this->BytesPerBlock + STM32_FLASH_BASE, this->SectorBuffer.buf, this->BytesPerBlock);
        for (int i = 0; i < size1; i++)
        {
            this->SectorBuffer.buf[secoff + i] = ((byte*)pBuffer)[i];
        }
        this->eraseSector(secpos *this->BytesPerBlock + STM32_FLASH_BASE); //擦除这个扇区
        this->WriteSector(secpos *this->BytesPerBlock + STM32_FLASH_BASE, this->SectorBuffer.buf16);
        size -= size1;
        addr += size1;
        pos += size1;
    }
    //处理第二部分
    while (size >= this->BytesPerBlock)
    {
        this->eraseSector(addr);
        this->WriteSector(addr, (byte*)pBuffer + pos);
        size -= this->BytesPerBlock;
        pos += this->BytesPerBlock;
        addr += this->BytesPerBlock;
    }
    //处理第三部分
    if (size > 0)
    {
        //当第三部分需要处理时
        //this->Read(addr, this->SectorBuffer.buf, this->BytesPerBlock);
        //this->eraseSector(addr);
        for (int i = 0; i < size; i++)
        {
            this->SectorBuffer.buf[i] = ((byte*)pBuffer)[pos + i];
        }
        //this->WriteSector(addr, this->SectorBuffer.buf);
		STMFLASH_Write(addr,this->SectorBuffer.buf16,this->BytesPerBlock/2);
    }
    FLASH_Lock(); //上锁
    return size;
}

//扇区是否需要擦除
bool Flash::sectorNeedErase(uint addr)
{
    Read(addr, this->SectorBuffer.buf16, this->BytesPerBlock / 2); //读出整个扇区的内容
    for (int i = 0; i < this->BytesPerBlock / 2; i++)
    {
        if (this->SectorBuffer.buf16[i] != 0XFFFF)
            return true;
    }

    return false;
}

//擦除整个扇区
int Flash::eraseSector(uint addr)
{
    if (sectorNeedErase(addr))
    {
        FLASH_ErasePage(addr); //擦除这个扇区		
    }
    return this->BytesPerBlock;
}

//写扇区
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

            // 集成测试
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
            debug_printf("测试开始\r\n");
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
            debug_printf("测试完成\r\n");
        #endif 
    }
#endif
