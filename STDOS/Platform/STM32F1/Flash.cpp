#include "Flash.h"
//#include "_Core.h"
#include "Platform\stm32.h"

// 设置读保护   注意：解除读保护时会擦除所有 Flash 内容
bool Flash::ReadOutProtection(bool set)
{
	set=set;
	return false;
}
void OnInit()
{
}
// 擦除块 （段地址）
bool Flash::EraseBlock(uint32_t address) const
{
	return false;
}
// 写块
bool Flash::WriteBlock(uint32_t address, const uint8_t* buf, int len, bool inc) const
{
	return false;
}
//////////////////////////////////////////////////////////////////////////////
class STMFLASH
{
    public:       
		STMFLASH();
		void SetFlashSize(uint32_t flashsize);
		int Read(uint32_t addr,void* pBuf,int len);//读取
		int Write(uint32_t addr,void* pBuf,int len,bool saveold=true);//写入
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		uint32_t sectorSize;//扇区大小
		uint32_t flashSize;//Flash容量大小，单位K
		uint16_t readHalfWord(uint32_t faddr); //读出半字  
        void writeNoCheck(uint32_t addr, uint16_t *pBuffer, uint16_t len);	
		void write(uint32_t addr, uint16_t *pBuffer, uint16_t len); //从指定地址开始写入指定长度的数据
		int writeSector(uint32_t addr);//写入整个扇区
        void read(uint32_t addr, uint16_t *pBuffer, uint16_t len); //从指定地址开始读出指定长度的数据
		bool addrValid(uint32_t addr);//地址正确，在上下限范围内
		bool addrSectorFirst(uint32_t addr);//地址是否是扇区起始地址
};

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

static union Buff
{
    uint8_t buf[2048];
    uint16_t buf16[1024];
} Buff;

STMFLASH::STMFLASH()
{
    this->flashSize = 64;
    this->sectorSize = 1024;
}

void STMFLASH::SetFlashSize(uint32_t flashsize)
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
int STMFLASH::Read(uint32_t addr, void *pBuf, int len)
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
        uint16_t tmp1 = this->readHalfWord(addr - 1);
        ((uint8_t*)pBuf)[0] = tmp1 &0xff;
        addr++;
        len--;
        pBuf = ((uint8_t*)pBuf) + 1;
    }
    while (len > 1)
    {
        ((uint16_t*)pBuf)[0] = this->readHalfWord(addr);
        addr++;
        addr++;
        len--;
        len--;
        pBuf = ((uint8_t*)pBuf) + 2;
    }
    if (len)
    {
        //没有对齐
        uint16_t tmp = this->readHalfWord(addr);
        ((uint8_t*)pBuf)[0] = tmp &0xff;
    }

    return len;
}

//写入
int STMFLASH::Write(uint32_t addr, void *pBuf, int len, bool protecold)
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
    uint32_t addr1;
    uint32_t len1;
    uint32_t sec1; //第一区
    uint32_t sec1pos; //地址在第一区位置
    uint32_t writeSize; //写入大小

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
            uint32_t addrt = sec1 * this->sectorSize + STM32_FLASH_BASE;
            for (int i = 0; i < this->sectorSize / 2; i++)
            {
                Buff.buf16[i] = this->readHalfWord(addrt + i * 2);
            }
        }
        for (int i = 0; i < writeSize; i++)
        {
            Buff.buf[sec1pos + i] = ((uint8_t*)pBuf)[i];
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
            Buff.buf[i] = ((uint8_t*)pBuf)[addr1 + i];
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
        Buff.buf[i] = ((uint8_t*)addr1)[i];
    }
    this->writeSector(addr1);
    return len;
}

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
uint16_t STMFLASH::readHalfWord(uint32_t faddr)
{
    return *(volatile uint16_t*)faddr;
}

//不检查的写入
//addr:起始地址
//pBuffer:数据指针
//len:半字(16位)数   
void STMFLASH::writeNoCheck(uint32_t addr, uint16_t *pBuffer, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //地址增加2.
    }
}

//地址正确，在上下限范围内
bool STMFLASH::addrValid(uint32_t addr)
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
bool STMFLASH::addrSectorFirst(uint32_t addr)
{
    if (!this->addrValid(addr))
    {
        return false;
    }
    uint32_t size = addr - STM32_FLASH_BASE;
    if (size % this->sectorSize)
    {
        return false;
    }
    return true;
}

//写入整个扇区,需要保证参数正确，如果不正确，直接返回
int STMFLASH::writeSector(uint32_t addr)
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
void STMFLASH::write(uint32_t addr, uint16_t *pBuffer, uint16_t len)
{
    uint32_t secpos; //扇区地址
    uint16_t secoff; //扇区内偏移地址(16位字计算)
    uint16_t secremain; //扇区内剩余地址(16位字计算)	   
    uint16_t i;
    uint32_t offaddr; //去掉0X08000000后的地址
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
void STMFLASH::read(uint32_t addr, uint16_t *pBuffer, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        pBuffer[i] = readHalfWord(addr); //读取2个字节.
        addr += 2; //偏移2个字节.	
    }
}
