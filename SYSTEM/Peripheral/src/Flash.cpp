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
	this->BytesPerBlock=1024;
	this->Size=1024*16;
}
//设置大小及扇区大小
void Flash::SetSectorSize(int bytesperblock,int size)
{
	this->BytesPerBlock=bytesperblock;
	if(size<0)
	{
		this->Size=1024*1024;
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
	if(size<=0)
	{
		return 0;
	}
    for (int i = 0; i < size/2; i++)
    {
        ((ushort*)pBuffer)[i] = ReadHalfWord(addr); //读取2个字节.
        addr += 2; //偏移2个字节.	
    }
	if(size%2)
	{
		pBuffer[size-1]=this->ReadHalfWord(addr-1);
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
    }
#endif 
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址	





//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
ushort Flash::ReadHalfWord(uint addr)
{
    return *(volatile ushort*)addr;
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
#if STM32_FLASH_SIZE<256
    #define STM_SECTOR_SIZE 1024 //字节
#else 
    #define STM_SECTOR_SIZE	2048
#endif 
ushort STMFLASH_BUF[STM_SECTOR_SIZE / 2]; //最多是2K字节
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void Flash::Read(uint addr, ushort *pBuffer, ushort size)
{
    ushort i;
    for (i = 0; i < size; i++)
    {
        pBuffer[i] = ReadHalfWord(addr); //读取2个字节.
        addr += 2; //偏移2个字节.	
    }
	
}
//擦除整个扇区
int Flash::eraseSector(uint addr)
{
	int i=0;
	Read(addr, STMFLASH_BUF, STM_SECTOR_SIZE / 2); //读出整个扇区的内容
	return this->BytesPerBlock;
}
void Flash::Write(uint addr, ushort *pBuffer, ushort size)
{
    uint secpos; //扇区地址
    ushort secoff; //扇区内偏移地址(16位字计算)
    ushort secremain; //扇区内剩余地址(16位字计算)	   
    ushort i;
    uint offaddr; //去掉0X08000000后的地址
    if (addr < STM32_FLASH_BASE || (addr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
        return ;
    //非法地址
    FLASH_Unlock(); //解锁
    offaddr = addr - STM32_FLASH_BASE; //实际偏移地址.
    secpos = offaddr / STM_SECTOR_SIZE; //扇区地址  0~127 for STM32F103RBT6
    secoff = (offaddr % STM_SECTOR_SIZE) / 2; //在扇区内的偏移(2个字节为基本单位.)
    secremain = STM_SECTOR_SIZE / 2-secoff; //扇区剩余空间大小   
    if (size <= secremain)
        secremain = size;
    //不大于该扇区范围
    while (1)
    {
        Read(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2); //读出整个扇区的内容
        for (i = 0; i < secremain; i++)
        //校验数据
        {
            if (STMFLASH_BUF[secoff + i] != 0XFFFF)
                break;
            //需要擦除  	  
        }
        if (i < secremain)
        //需要擦除
        {
            FLASH_ErasePage(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE); //擦除这个扇区
            for (i = 0; i < secremain; i++)
            //复制
            {
                STMFLASH_BUF[i + secoff] = pBuffer[i];
            }
            Write_NoCheck(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2); //写入整个扇区  
        }
        else
		{
            Write_NoCheck(addr, pBuffer, secremain);
		}
        //写已经擦除了的,直接写入扇区剩余区间. 				   
        if (size == secremain)
            break;
        //写入结束了
        else
        //写入未结束
        {
            secpos++; //扇区地址增1
            secoff = 0; //偏移位置为0 	 
            pBuffer += secremain; //指针偏移
            addr += secremain; //写地址偏移	   
            size -= secremain; //字节(16位)数递减
            if (size > (STM_SECTOR_SIZE / 2))
                secremain = STM_SECTOR_SIZE / 2;
            //下一个扇区还是写不完
            else
                secremain = size;
            //下一个扇区可以写完了
        }
    };
    FLASH_Lock(); //上锁
}
