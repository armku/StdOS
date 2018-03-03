#include "..\Drivers\AT24CXX.h"

//#define _AT24CXX_TEST_H
#ifdef _AT24CXX_TEST_H
#define EE_SIZE				256			  /* 24xx02������ */    
AT24CXX at2402(AT24C02);
void AT24C02Test()
{
#if 0
	HardI2C SoftI2C
		//��ʼ����ǿ������BH750
		static SoftI2C iic;
	iic.SetPin(PB6, PB7);
	bh.IIC = &iic;
	bh.Init();

	bh.Read(); //�����õ����� 
#endif 
#ifdef STM32F0
	at2402.SetPin(PB6, PB7);
#elif defined STM32F1
	at2402.SetPin(PB6, PB7);
#elif defined STM32F4
	at2402.SetPin(PI9, PI10);
#endif 

	debug_printf("\r\n");

	ushort i;
	byte write_buf[EE_SIZE];
	byte read_buf[EE_SIZE];

	/*-----------------------------------------------------------------------------------*/
	if (at2402.CheckOk() == 0)
	{
		/* û�м�⵽EEPROM */
		debug_printf("û�м�⵽����EEPROM!\r\n");

		while (1)
			;
		/* ͣ�� */
	}
	/*------------------------------------------------------------------------------------*/
	/* �����Ի����� */
	for (i = 0; i < EE_SIZE; i++)
	{
		write_buf[i] = i;
	}
	/*------------------------------------------------------------------------------------*/
	if (at2402.Write(0, write_buf, EE_SIZE) == 0)
	{
		debug_printf("дeeprom����\r\n");
		return;
	}
	else
	{
		debug_printf("дeeprom�ɹ���\r\n");
	}

	/*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
	Sys.Sleep(10);
	/*-----------------------------------------------------------------------------------*/
	if (at2402.Read(0, read_buf, EE_SIZE) == 0)
	{
		debug_printf("��eeprom����\r\n");
		return;
	}
	else
	{
		debug_printf("��eeprom�ɹ����������£�\r\n");
	}
	/*-----------------------------------------------------------------------------------*/
	for (i = 0; i < EE_SIZE; i++)
	{
		if (read_buf[i] != write_buf[i])
		{
			debug_printf("0x%02X ", read_buf[i]);
			debug_printf("����:EEPROM������д������ݲ�һ��");
			return;
		}
		debug_printf(" %02X", read_buf[i]);

		if ((i & 15) == 15)
		{
			debug_printf("\r\n");
		}
	}
	debug_printf("eeprom��д���Գɹ�\r\n");
}
#endif
