#include "stmflash.h"
#include "stm32f10x_flash.h"
#include "Sys.h"

union Buff
{
    byte buf[2048];
    ushort buf16[1024];
} Buff;

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
ushort STMFLASH::ReadHalfWord(uint faddr)
{
    return *(volatile ushort*)faddr;
}

//不检查的写入
//addr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH::Write_NoCheck(uint addr, ushort *pBuffer, ushort NumToWrite)
{
    ushort i;
    for (i = 0; i < NumToWrite; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //地址增加2.
    }
}

//从指定地址开始写入指定长度的数据
//addr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
void STMFLASH::Write(uint addr, ushort *pBuffer, ushort NumToWrite)
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
    if (NumToWrite <= secremain)
        secremain = NumToWrite;
    //不大于该扇区范围
    while (1)
    {
        Read(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, Buff.buf16, STM_SECTOR_SIZE / 2); //读出整个扇区的内容
        for (i = 0; i < secremain; i++)
        //校验数据
        {
            if (Buff.buf16[secoff + i] != 0XFFFF)
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
                Buff.buf16[i + secoff] = pBuffer[i];
            }
            Write_NoCheck(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, Buff.buf16, STM_SECTOR_SIZE / 2); //写入整个扇区  
        }
        else
            Write_NoCheck(addr, pBuffer, secremain);
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
            addr += secremain; //写地址偏移	   
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

//从指定地址开始读出指定长度的数据
//addr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH::Read(uint addr, ushort *pBuffer, ushort NumToRead)
{
    ushort i;
    for (i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = ReadHalfWord(addr); //读取2个字节.
        addr += 2; //偏移2个字节.	
    }
}

#ifdef DEBUG
    void STMFLASH::Test()
    {		
        ushort buftest1[120];
        uint addr = STM32_FLASH_BASE + 1024 * 30;
		STMFLASH flash1;

        debug_printf("\r\n\r\n");
        debug_printf("TestFlash Start......\r\n");

        debug_printf("测试开始\r\n");
        for (int i = 0; i < 20; i++)
        {
            buftest1[i] = 1000+i;
        }
        debug_printf("-1 \r\n");
        flash1.Write(addr, buftest1, 20);
        debug_printf("0 \r\n");
        for (int i = 0; i < 20; i++)
        {
            buftest1[i] = 0;
        }
        debug_printf("1 \r\n");
        flash1.Read(addr, buftest1, 20);
        debug_printf("2 \r\n");
        for (int i = 0; i < 20; i++)
        {
            debug_printf("%d:%d\t", i, buftest1[i]);
        }

        debug_printf("测试完成\r\n");
    }
#endif
