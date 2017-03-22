#include "stmflash.h"
#include "stm32f10x_flash.h"
#include "Sys.h"

union Buff
{
    byte buf[2048];
    ushort buf16[1024];
} Buff;

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
ushort STMFLASH::ReadHalfWord(uint faddr)
{
    return *(volatile ushort*)faddr;
}

//������д��
//addr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH::Write_NoCheck(uint addr, ushort *pBuffer, ushort NumToWrite)
{
    ushort i;
    for (i = 0; i < NumToWrite; i++)
    {
        FLASH_ProgramHalfWord(addr, pBuffer[i]);
        addr += 2; //��ַ����2.
    }
}

//��ָ����ַ��ʼд��ָ�����ȵ�����
//addr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
void STMFLASH::Write(uint addr, ushort *pBuffer, ushort NumToWrite)
{
    uint secpos; //������ַ
    ushort secoff; //������ƫ�Ƶ�ַ(16λ�ּ���)
    ushort secremain; //������ʣ���ַ(16λ�ּ���)	   
    ushort i;
    uint offaddr; //ȥ��0X08000000��ĵ�ַ
    if (addr < STM32_FLASH_BASE || (addr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
        return ;
    //�Ƿ���ַ
    FLASH_Unlock(); //����
    offaddr = addr - STM32_FLASH_BASE; //ʵ��ƫ�Ƶ�ַ.
    secpos = offaddr / STM_SECTOR_SIZE; //������ַ  0~127 for STM32F103RBT6
    secoff = (offaddr % STM_SECTOR_SIZE) / 2; //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain = STM_SECTOR_SIZE / 2-secoff; //����ʣ��ռ��С   
    if (NumToWrite <= secremain)
        secremain = NumToWrite;
    //�����ڸ�������Χ
    while (1)
    {
        Read(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, Buff.buf16, STM_SECTOR_SIZE / 2); //������������������
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
            FLASH_ErasePage(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE); //�����������
            for (i = 0; i < secremain; i++)
            //����
            {
                Buff.buf16[i + secoff] = pBuffer[i];
            }
            Write_NoCheck(secpos *STM_SECTOR_SIZE + STM32_FLASH_BASE, Buff.buf16, STM_SECTOR_SIZE / 2); //д����������  
        }
        else
            Write_NoCheck(addr, pBuffer, secremain);
        //д�Ѿ������˵�,ֱ��д������ʣ������. 				   
        if (NumToWrite == secremain)
            break;
        //д�������
        else
        //д��δ����
        {
            secpos++; //������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0 	 
            pBuffer += secremain; //ָ��ƫ��
            addr += secremain; //д��ַƫ��	   
            NumToWrite -= secremain; //�ֽ�(16λ)���ݼ�
            if (NumToWrite > (STM_SECTOR_SIZE / 2))
                secremain = STM_SECTOR_SIZE / 2;
            //��һ����������д����
            else
                secremain = NumToWrite;
            //��һ����������д����
        }
    };
    FLASH_Lock(); //����
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//addr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH::Read(uint addr, ushort *pBuffer, ushort NumToRead)
{
    ushort i;
    for (i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = ReadHalfWord(addr); //��ȡ2���ֽ�.
        addr += 2; //ƫ��2���ֽ�.	
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

        debug_printf("���Կ�ʼ\r\n");
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

        debug_printf("�������\r\n");
    }
#endif
