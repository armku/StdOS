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
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址	





//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
	ushort Flash::ReadHalfWord(uint faddr)
{
    return *(volatile ushort*)faddr;
}

#if STM32_FLASH_WREN	//如果使能了写   
    //不检查的写入
    //WriteAddr:起始地址
    //pBuffer:数据指针
    //NumToWrite:半字(16位)数   
    void Flash::Write_NoCheck(uint WriteAddr, ushort *pBuffer, ushort NumToWrite)
    {
        ushort i;
        for (i = 0; i < NumToWrite; i++)
        {
            FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
            WriteAddr += 2; //地址增加2.
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
    void Flash::STMFLASH_Read(uint ReadAddr, ushort *pBuffer, ushort NumToRead)
    {
        ushort i;
        for (i = 0; i < NumToRead; i++)
        {
            pBuffer[i] = ReadHalfWord(ReadAddr); //读取2个字节.
            ReadAddr += 2; //偏移2个字节.	
        }
    }
    void Flash::STMFLASH_Write(uint WriteAddr, ushort *pBuffer, ushort NumToWrite)
    {
        uint secpos; //扇区地址
        ushort secoff; //扇区内偏移地址(16位字计算)
        ushort secremain; //扇区内剩余地址(16位字计算)	   
        ushort i;
        uint offaddr; //去掉0X08000000后的地址
        if (WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
            return ;
        //非法地址
        FLASH_Unlock(); //解锁
        offaddr = WriteAddr - STM32_FLASH_BASE; //实际偏移地址.
        secpos = offaddr / STM_SECTOR_SIZE; //扇区地址  0~127 for STM32F103RBT6
        secoff = (offaddr % STM_SECTOR_SIZE) / 2; //在扇区内的偏移(2个字节为基本单位.)
        secremain = STM_SECTOR_SIZE / 2-secoff; //扇区剩余空间大小   
        if (NumToWrite <= secremain)
            secremain = NumToWrite;
        //不大于该扇区范围
        while (1)
        {
            STMFLASH_Read(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2); //读出整个扇区的内容
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
                Write_NoCheck(WriteAddr, pBuffer, secremain);
            //写已经擦除了的,直接写入扇区剩余区间. 				   
            if (NumToWrite == secremain)
                break;
            //写入结束了
            else
            //写入未结束
            {
                secpos++; //扇区地址增1
                secoff = 0; //偏移位置为0 	 
                pBuffer += secremain; //指针偏移
                WriteAddr += secremain; //写地址偏移	   
                NumToWrite -= secremain; //字节(16位)数递减
                if (NumToWrite > (STM_SECTOR_SIZE / 2))
                    secremain = STM_SECTOR_SIZE / 2;
                //下一个扇区还是写不完
                else
                    secremain = NumToWrite;
                //下一个扇区可以写完了
            }
        };
        FLASH_Lock(); //上锁
    }
#endif 



//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Flash::Test_Write(uint WriteAddr, ushort WriteData)
{
    STMFLASH_Write(WriteAddr, &WriteData, 1); //写入一个字 
}
