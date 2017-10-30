#include "Sys.h"
#include "FSMC.h"
#include "stm32f4xx.h"

#define EXT_SRAM_ADDR  	((uint32_t)0x68000000)
#define EXT_SRAM_SIZE	(2 * 1024 * 1024)


/*
 *********************************************************************************************************
 *	�� �� ��: bsp_TestExtSRAM
 *	����˵��: ɨ������ⲿSRAM
 *	��    ��: ��
 *	�� �� ֵ: 0 ��ʾ����ͨ���� ����0��ʾ����Ԫ�ĸ�����
 *********************************************************************************************************
 */
byte bsp_TestExtSRAM()
{
    uint32_t i;
    uint32_t *pSRAM;
    byte *pBytes;
    uint32_t err;
    const byte ByteBuf[4] = 
    {
        0x55, 0xA5, 0x5A, 0xAA
    };

    /* дSRAM */
    pSRAM = (uint32_t*)EXT_SRAM_ADDR;
    for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
    {
        *pSRAM++ = i;
    }

    /* ��SRAM */
    err = 0;
    pSRAM = (uint32_t*)EXT_SRAM_ADDR;
    for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
    {
        if (*pSRAM++ != i)
        {
            err++;
        }
    }

    if (err > 0)
    {
        return (4 *err);
    }

    /* ��SRAM �������󷴲�д�� */
    pSRAM = (uint32_t*)EXT_SRAM_ADDR;
    for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
    {
        *pSRAM = ~ * pSRAM;
        pSRAM++;
    }

    /* �ٴαȽ�SRAM������ */
    err = 0;
    pSRAM = (uint32_t*)EXT_SRAM_ADDR;
    for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
    {
        if (*pSRAM++ != (~i))
        {
            err++;
        }
    }

    if (err > 0)
    {
        return (4 *err);
    }

    /* ���԰��ֽڷ�ʽ����, Ŀ������֤ FSMC_NBL0 �� FSMC_NBL1 ���� */
    pBytes = (byte*)EXT_SRAM_ADDR;
    for (i = 0; i < sizeof(ByteBuf); i++)
    {
        *pBytes++ = ByteBuf[i];
    }

    /* �Ƚ�SRAM������ */
    err = 0;
    pBytes = (byte*)EXT_SRAM_ADDR;
    for (i = 0; i < sizeof(ByteBuf); i++)
    {
        if (*pBytes++ != ByteBuf[i])
        {
            err++;
        }
    }
    if (err > 0)
    {
        return err;
    }
    return 0;
}
FSMC fsmc;
void SRamTest()
{
	fsmc.InitExtSRAM();
	byte ret=bsp_TestExtSRAM();
	printf("�ڴ���Դ�������%d \r\n",ret);
	
}
