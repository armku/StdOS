#include "Drivers\AT45DB.h"

#ifdef DEBUG1
	const byte Tx_Buffer[] = "STM32F10x SPI Firmware Library Example: communication with an AT45DB SPI FLASH";

	void TestAT45DB()
	{
		Spi spi(SPI_2, 9000000, true);
		AT45DB sf(&spi);
		debug_printf("AT45DB ID=0x%08X PageSize=%d\r\n", sf.ID, sf.PageSize);
		int size = ArrayLength(Tx_Buffer);
		debug_printf("DataSize=%d\r\n", size);

		uint addr = 0x00000;
		if (sf.ErasePage(addr))
			debug_printf("²Á³ý0x%08x³É¹¦\r\n", addr);
		else
			debug_printf("²Á³ý0x%08xÊ§°Ü\r\n", addr);

		byte Rx_Buffer[80];
		for (int i = 0; i < 9; i++)
		{
			sf.ErasePage(addr);
			memset(Rx_Buffer, 0, ArrayLength(Rx_Buffer));
			if (!sf.Write(addr, Tx_Buffer, size))
				debug_printf("Ð´Èë0x%08XÊ§°Ü£¡\r\n", addr);
			if (!sf.ReadPage(addr, Rx_Buffer, size))
				debug_printf("¶ÁÈ¡0x%08XÊ§°Ü£¡\r\n", addr);
			memset(Rx_Buffer, 0, ArrayLength(Rx_Buffer));
			addr += size;
		}

		for (int i = 0; i < size; i++)
		{
			if (Rx_Buffer != Tx_Buffer)
				debug_printf("Error %d ", i);
		}
		debug_printf("\r\nFinish!\r\n");
	}
#endif
