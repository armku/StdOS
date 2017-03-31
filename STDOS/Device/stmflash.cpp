#include "stmflash.h"
#include "stm32f10x_flash.h"
#include "Sys.h"

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

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
int STMFLASH::Read(uint addr, void *pBuf, int len)
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
        //û�ж���
        ushort tmp = this->readHalfWord(addr);
        ((byte*)pBuf)[0] = tmp &0xff;
    }

    return len;
}

//д��
int STMFLASH::Write(uint addr, void *pBuf, int len, bool protecold)
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
    uint addr1;
    uint len1;
    uint sec1; //��һ��
    uint sec1pos; //��ַ�ڵ�һ��λ��
    uint writeSize; //д���С

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
            Buff.buf[i] = ((byte*)pBuf)[addr1 + i];
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
        Buff.buf[i] = ((byte*)addr1)[i];
    }
    this->writeSector(addr1);
    return len;
}

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
ushort STMFLASH::readHalfWord(uint faddr)
{
    return *(volatile ushort*)faddr;
}

//������д��
//addr:��ʼ��ַ
//pBuffer:����ָ��
//len:����(16λ)��   
void STMFLASH::writeNoCheck(uint addr, ushort *pBuffer, ushort len)
{
    ushort i;
    for (i = 0; i < len; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //��ַ����2.
    }
}

//��ַ��ȷ���������޷�Χ��
bool STMFLASH::addrValid(uint addr)
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

//д����������,��Ҫ��֤������ȷ���������ȷ��ֱ�ӷ���
int STMFLASH::writeSector(uint addr)
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
void STMFLASH::write(uint addr, ushort *pBuffer, ushort len)
{
    uint secpos; //������ַ
    ushort secoff; //������ƫ�Ƶ�ַ(16λ�ּ���)
    ushort secremain; //������ʣ���ַ(16λ�ּ���)	   
    ushort i;
    uint offaddr; //ȥ��0X08000000��ĵ�ַ
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
void STMFLASH::read(uint addr, ushort *pBuffer, ushort len)
{
    ushort i;
    for (i = 0; i < len; i++)
    {
        pBuffer[i] = readHalfWord(addr); //��ȡ2���ֽ�.
        addr += 2; //ƫ��2���ֽ�.	
    }
}

#ifdef DEBUG
    void STMFLASH::Test()
    {
        byte buftest1[3200];
        uint addr = STM32_FLASH_BASE + 1024 * 36+10;
        STMFLASH flash1;
        flash1.SetFlashSize(512);
        debug_printf("���Կ�ʼ\r\n");
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
                debug_printf("����ʧ�ܣ����ݴ���%d\r\n", buftest1[i]);
                return ;
            }
            //debug_printf("%d:%d\t", i, buftest1[i]);
        }

        debug_printf("���Գɹ�\r\n");
    }
#endif
