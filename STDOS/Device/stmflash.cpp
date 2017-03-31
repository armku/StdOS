#include "stmflash.h"
#include "stm32f10x_flash.h"
#include "Sys.h"

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

static union Buff
{
    byte buf[2048];
    ushort buf16[1024];
} Buff;

STMFLASH::STMFLASH()
{
    this->flashSize = 64;
    this->sectorSize = 1024;
}

void STMFLASH::SetFlashSize(uint flashsize)
{
    this->flashSize = flashsize;
    if (this->flashSize > 256)
    {
        //大容量产品能为2k
        this->sectorSize = 2048;
    }
    else
    {
        //中小容量产品为1k
        this->sectorSize = 1024;
    }
}

//读取
int STMFLASH::Read(uint addr, void *pBuf, int len)
{
    if (len <= 0)
    {
        return 0;
    }
    if (!this->addrValid(addr))
    {
        //地址非法
        return 0;
    }
    if (addr % 2)
    {
        //起始地址为奇数
        ushort tmp1 = this->readHalfWord(addr - 1);
        ((byte*)pBuf)[0] = tmp1 &0xff;
        addr++;
        len--;
        pBuf = ((byte*)pBuf) + 1;
    }
    while (len > 1)
    {
        ((ushort*)pBuf)[0] = this->readHalfWord(addr);
        addr++;
        addr++;
        len--;
        len--;
        pBuf = ((byte*)pBuf) + 2;
    }
    if (len)
    {
        //没有对齐
        ushort tmp = this->readHalfWord(addr);
        ((byte*)pBuf)[0] = tmp &0xff;
    }

    return len;
}

//写入
int STMFLASH::Write(uint addr, void *pBuf, int len, bool protecold)
{
    if (len <= 0)
    {
        return 0;
    }
    if (!this->addrValid(addr))
    {
        //地址非法
        return 0;
    }
    if (!this->addrValid(addr + len))
    {
        //数据超限
        return 0;
    }
    /*
    |--------|---------|----------|
     *******************
    1        2         3	
     */
    //第一区
    uint addr1;
    uint len1;
    uint sec1; //第一区
    uint sec1pos; //地址在第一区位置
    uint writeSize; //写入大小

    addr1 = addr;
    len1 = len;

    //第一区	
    sec1 = (addr1 - STM32_FLASH_BASE) / this->sectorSize;
    sec1pos = (addr1 - STM32_FLASH_BASE) % this->sectorSize;
    writeSize = this->sectorSize - sec1pos;
    if (sec1pos != 0)
    {
        //第一区需要写入
        if (writeSize > len1)
        {
            writeSize = len1;
        }
        if (protecold)
        {
            uint addrt = sec1 * this->sectorSize + STM32_FLASH_BASE;
            for (int i = 0; i < this->sectorSize / 2; i++)
            {
                Buff.buf16[i] = this->readHalfWord(addrt + i * 2);
            }
        }
        for (int i = 0; i < writeSize; i++)
        {
            Buff.buf[sec1pos + i] = ((byte*)pBuf)[i];
        }


        this->writeSector(sec1 *this->sectorSize + STM32_FLASH_BASE);
        len1 -= writeSize;
        addr1 += writeSize;
        return 0;
    }
    //第二区
    while (1)
    {
        if (len1 < this->sectorSize)
        {
            //小于一个扇区，退出
            break;
        }
        for (int i = 0; i < this->flashSize; i++)
        {
            Buff.buf[i] = ((byte*)pBuf)[addr1 + i];
        }
        this->writeSector(addr1);
        addr1 += this->flashSize;
        len1 -= this->flashSize;
    }
    //第三区
    if (protecold)
    {
        this->Read(addr1, Buff.buf, this->sectorSize);
    }
    for (int i = 0; i < len1; i++)
    {
        Buff.buf[i] = ((byte*)addr1)[i];
    }
    this->writeSector(addr1);
    return len;
}

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
ushort STMFLASH::readHalfWord(uint faddr)
{
    return *(volatile ushort*)faddr;
}

//不检查的写入
//addr:起始地址
//pBuffer:数据指针
//len:半字(16位)数   
void STMFLASH::writeNoCheck(uint addr, ushort *pBuffer, ushort len)
{
    ushort i;
    for (i = 0; i < len; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //地址增加2.
    }
}

//地址正确，在上下限范围内
bool STMFLASH::addrValid(uint addr)
{
    if (addr < STM32_FLASH_BASE)
    {
        //地址下限错误
        return false;
    }
    if (addr >= (STM32_FLASH_BASE + this->flashSize *1024))
    {
        //地址上限错误
        return false;
    }
    return true;
}

//地址是否是扇区起始地址
bool STMFLASH::addrSectorFirst(uint addr)
{
    if (!this->addrValid(addr))
    {
        return false;
    }
    uint size = addr - STM32_FLASH_BASE;
    if (size % this->sectorSize)
    {
        return false;
    }
    return true;
}

//写入整个扇区,需要保证参数正确，如果不正确，直接返回
int STMFLASH::writeSector(uint addr)
{
    if (!this->addrSectorFirst(addr))
    {
        //不是扇区起始地址或者地址非法直接返回
        return 0;
    }
    FLASH_Unlock(); //解锁	
    FLASH_ErasePage(addr); //擦除这个扇区
    writeNoCheck(addr, Buff.buf16, this->sectorSize / 2); //写入整个扇区  
    FLASH_Lock(); //上锁
    return this->sectorSize;
}

//从指定地址开始写入指定长度的数据
//addr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//len:半字(16位)数(就是要写入的16位数据的个数.)
void STMFLASH::write(uint addr, ushort *pBuffer, ushort len)
{
    uint secpos; //扇区地址
    ushort secoff; //扇区内偏移地址(16位字计算)
    ushort secremain; //扇区内剩余地址(16位字计算)	   
    ushort i;
    uint offaddr; //去掉0X08000000后的地址
    if (addr < STM32_FLASH_BASE || (addr >= (STM32_FLASH_BASE + 1024 * this->flashSize)))
        return ;
    //非法地址
    FLASH_Unlock(); //解锁
    offaddr = addr - STM32_FLASH_BASE; //实际偏移地址.
    secpos = offaddr / this->sectorSize; //扇区地址  0~127 for STM32F103RBT6
    secoff = (offaddr % this->sectorSize) / 2; //在扇区内的偏移(2个字节为基本单位.)
    secremain = this->sectorSize / 2-secoff; //扇区剩余空间大小   
    if (len <= secremain)
        secremain = len;
    //不大于该扇区范围
    while (1)
    {
        this->read(secpos *this->sectorSize + STM32_FLASH_BASE, Buff.buf16, this->sectorSize / 2); //读出整个扇区的内容
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
            FLASH_ErasePage(secpos *this->sectorSize + STM32_FLASH_BASE); //擦除这个扇区
            for (i = 0; i < secremain; i++)
            //复制
            {
                Buff.buf16[i + secoff] = pBuffer[i];
            }
            writeNoCheck(secpos *this->sectorSize + STM32_FLASH_BASE, Buff.buf16, this->sectorSize / 2); //写入整个扇区  
        }
        else
            writeNoCheck(addr, pBuffer, secremain);
        //写已经擦除了的,直接写入扇区剩余区间. 				   
        if (len == secremain)
            break;
        //写入结束了
        else
        //写入未结束
        {
            secpos++; //扇区地址增1
            secoff = 0; //偏移位置为0 	 
            pBuffer += secremain; //指针偏移
            addr += secremain; //写地址偏移	   
            len -= secremain; //字节(16位)数递减
            if (len > (this->sectorSize / 2))
                secremain = this->sectorSize / 2;
            //下一个扇区还是写不完
            else
                secremain = len;
            //下一个扇区可以写完了
        }
    };
    FLASH_Lock(); //上锁
}

//从指定地址开始读出指定长度的数据
//addr:起始地址
//pBuffer:数据指针
//len:半字(16位)数
void STMFLASH::read(uint addr, ushort *pBuffer, ushort len)
{
    ushort i;
    for (i = 0; i < len; i++)
    {
        pBuffer[i] = readHalfWord(addr); //读取2个字节.
        addr += 2; //偏移2个字节.	
    }
}

#ifdef DEBUG
    void STMFLASH::Test()
    {
        byte buftest1[3200];
        uint addr = STM32_FLASH_BASE + 1024 * 36+10;
        STMFLASH flash1;
        flash1.SetFlashSize(512);
        debug_printf("测试开始\r\n");
        for (int i = 0; i < 1200; i++)
        {
            buftest1[i] = i % 200;
        }

        int wid = flash1.Write(addr, buftest1, 3200);
        debug_printf("write ok\r\n");
        for (int i = 0; i < 3200; i++)
        {
            buftest1[i] = 0;
        }

        int rid = flash1.Read(addr, buftest1, 3200);
        debug_printf("read ok\r\n");
        for (int i = 0; i < 1200; i++)
        {
            if (buftest1[i] != (i % 200))
            {
                debug_printf("测试失败，数据错误：%d\r\n", buftest1[i]);
                return ;
            }
            //debug_printf("%d:%d\t", i, buftest1[i]);
        }

        debug_printf("测试成功\r\n");
    }
#endif
