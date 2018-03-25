#include "Flash.h"
//#include "_Core.h"
#include "Platform\stm32.h"

// ���ö�����   ע�⣺���������ʱ��������� Flash ����
bool Flash::ReadOutProtection(bool set)
{
	set=set;
	return false;
}
void OnInit()
{
}
// ������ ���ε�ַ��
bool Flash::EraseBlock(uint32_t address) const
{
	return false;
}
// д��
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
		int Read(uint32_t addr,void* pBuf,int len);//��ȡ
		int Write(uint32_t addr,void* pBuf,int len,bool saveold=true);//д��
	#ifdef DEBUG
		static void Test();
	#endif	
	private:
		uint32_t sectorSize;//������С
		uint32_t flashSize;//Flash������С����λK
		uint16_t readHalfWord(uint32_t faddr); //��������  
        void writeNoCheck(uint32_t addr, uint16_t *pBuffer, uint16_t len);	
		void write(uint32_t addr, uint16_t *pBuffer, uint16_t len); //��ָ����ַ��ʼд��ָ�����ȵ�����
		int writeSector(uint32_t addr);//д����������
        void read(uint32_t addr, uint16_t *pBuffer, uint16_t len); //��ָ����ַ��ʼ����ָ�����ȵ�����
		bool addrValid(uint32_t addr);//��ַ��ȷ���������޷�Χ��
		bool addrSectorFirst(uint32_t addr);//��ַ�Ƿ���������ʼ��ַ
};

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

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
        //��������Ʒ��Ϊ2k
        this->sectorSize = 2048;
    }
    else
    {
        //��С������ƷΪ1k
        this->sectorSize = 1024;
    }
}

//��ȡ
int STMFLASH::Read(uint32_t addr, void *pBuf, int len)
{
    if (len <= 0)
    {
        return 0;
    }
    if (!this->addrValid(addr))
    {
        //��ַ�Ƿ�
        return 0;
    }
    if (addr % 2)
    {
        //��ʼ��ַΪ����
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
        //û�ж���
        uint16_t tmp = this->readHalfWord(addr);
        ((uint8_t*)pBuf)[0] = tmp &0xff;
    }

    return len;
}

//д��
int STMFLASH::Write(uint32_t addr, void *pBuf, int len, bool protecold)
{
    if (len <= 0)
    {
        return 0;
    }
    if (!this->addrValid(addr))
    {
        //��ַ�Ƿ�
        return 0;
    }
    if (!this->addrValid(addr + len))
    {
        //���ݳ���
        return 0;
    }
    /*
    |--------|---------|----------|
     *******************
    1        2         3	
     */
    //��һ��
    uint32_t addr1;
    uint32_t len1;
    uint32_t sec1; //��һ��
    uint32_t sec1pos; //��ַ�ڵ�һ��λ��
    uint32_t writeSize; //д���С

    addr1 = addr;
    len1 = len;

    //��һ��	
    sec1 = (addr1 - STM32_FLASH_BASE) / this->sectorSize;
    sec1pos = (addr1 - STM32_FLASH_BASE) % this->sectorSize;
    writeSize = this->sectorSize - sec1pos;
    if (sec1pos != 0)
    {
        //��һ����Ҫд��
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
    //�ڶ���
    while (1)
    {
        if (len1 < this->sectorSize)
        {
            //С��һ���������˳�
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
    //������
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

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
uint16_t STMFLASH::readHalfWord(uint32_t faddr)
{
    return *(volatile uint16_t*)faddr;
}

//������д��
//addr:��ʼ��ַ
//pBuffer:����ָ��
//len:����(16λ)��   
void STMFLASH::writeNoCheck(uint32_t addr, uint16_t *pBuffer, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //��ַ����2.
    }
}

//��ַ��ȷ���������޷�Χ��
bool STMFLASH::addrValid(uint32_t addr)
{
    if (addr < STM32_FLASH_BASE)
    {
        //��ַ���޴���
        return false;
    }
    if (addr >= (STM32_FLASH_BASE + this->flashSize *1024))
    {
        //��ַ���޴���
        return false;
    }
    return true;
}

//��ַ�Ƿ���������ʼ��ַ
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

//д����������,��Ҫ��֤������ȷ���������ȷ��ֱ�ӷ���
int STMFLASH::writeSector(uint32_t addr)
{
    if (!this->addrSectorFirst(addr))
    {
        //����������ʼ��ַ���ߵ�ַ�Ƿ�ֱ�ӷ���
        return 0;
    }
    FLASH_Unlock(); //����	
	FLASH_ErasePage(addr); //�����������
	writeNoCheck(addr, Buff.buf16, this->sectorSize / 2); //д����������  
    FLASH_Lock(); //����
    return this->sectorSize;
}

//��ָ����ַ��ʼд��ָ�����ȵ�����
//addr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//len:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
void STMFLASH::write(uint32_t addr, uint16_t *pBuffer, uint16_t len)
{
    uint32_t secpos; //������ַ
    uint16_t secoff; //������ƫ�Ƶ�ַ(16λ�ּ���)
    uint16_t secremain; //������ʣ���ַ(16λ�ּ���)	   
    uint16_t i;
    uint32_t offaddr; //ȥ��0X08000000��ĵ�ַ
    if (addr < STM32_FLASH_BASE || (addr >= (STM32_FLASH_BASE + 1024 * this->flashSize)))
        return ;
    //�Ƿ���ַ
    FLASH_Unlock(); //����
    offaddr = addr - STM32_FLASH_BASE; //ʵ��ƫ�Ƶ�ַ.
    secpos = offaddr / this->sectorSize; //������ַ  0~127 for STM32F103RBT6
    secoff = (offaddr % this->sectorSize) / 2; //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain = this->sectorSize / 2-secoff; //����ʣ��ռ��С   
    if (len <= secremain)
        secremain = len;
    //�����ڸ�������Χ
    while (1)
    {
        this->read(secpos *this->sectorSize + STM32_FLASH_BASE, Buff.buf16, this->sectorSize / 2); //������������������
        for (i = 0; i < secremain; i++)
        //У������
        {
            if (Buff.buf16[secoff + i] != 0XFFFF)
                break;
            //��Ҫ����  	  
        }
        if (i < secremain)
        //��Ҫ����
        {
			FLASH_ErasePage(secpos *this->sectorSize + STM32_FLASH_BASE); //�����������
			for (i = 0; i < secremain; i++)
            //����
            {
                Buff.buf16[i + secoff] = pBuffer[i];
            }
            writeNoCheck(secpos *this->sectorSize + STM32_FLASH_BASE, Buff.buf16, this->sectorSize / 2); //д����������  
        }
        else
            writeNoCheck(addr, pBuffer, secremain);
        //д�Ѿ������˵�,ֱ��д������ʣ������. 				   
        if (len == secremain)
            break;
        //д�������
        else
        //д��δ����
        {
            secpos++; //������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0 	 
            pBuffer += secremain; //ָ��ƫ��
            addr += secremain; //д��ַƫ��	   
            len -= secremain; //�ֽ�(16λ)���ݼ�
            if (len > (this->sectorSize / 2))
                secremain = this->sectorSize / 2;
            //��һ����������д����
            else
                secremain = len;
            //��һ����������д����
        }
    };
    FLASH_Lock(); //����
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//addr:��ʼ��ַ
//pBuffer:����ָ��
//len:����(16λ)��
void STMFLASH::read(uint32_t addr, uint16_t *pBuffer, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        pBuffer[i] = readHalfWord(addr); //��ȡ2���ֽ�.
        addr += 2; //ƫ��2���ֽ�.	
    }
}
