#include "stdio.h"
#include "W25QXXX.h"
#include "spi.h"

typedef enum
{
    FAILED = 0, PASSED = !FAILED
} TestStatus;


/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer)-1)

#define FLASH_WriteAddress     0x00000
#define FLASH_ReadAddress      FLASH_WriteAddress
#define FLASH_SectorToErase    FLASH_WriteAddress
//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define sFLASH_ID              0XEF4017    //W25Q64

/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "��л��ѡ��stm32������\r\n";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

int flashtest(void)
{
    printf("\r\n����һ��8Mbyte����flash(W25Q64)ʵ�� \r\n");

    Spi spi(SPI_1, 9000000, false);	
    W25QXXX w25q64(&spi,PA4);
    /* 8M����flash W25Q64��ʼ�� */
    w25q64.Init();

    /* Get SPI Flash Device ID */
    DeviceID = w25q64.ReadDeviceID();
   
	Sys.Delay(100);

    /* Get SPI Flash ID */
    FlashID = w25q64.ReadID();

    printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

    /* Check the SPI Flash ID */
    if (FlashID == sFLASH_ID)
    /* #define  sFLASH_ID  0XEF4017 */
    {
        printf("��⵽�����flash W25Q64 !\r\n");

        /* Erase SPI FLASH Sector to write on */
        w25q64.SectorErase(FLASH_SectorToErase);

        /* �����ͻ�����������д��flash�� */
        w25q64.BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
        w25q64.BufferWrite(Tx_Buffer, 252, BufferSize);
        printf("д�������Ϊ��%s \r\n", Tx_Buffer);

        /* ���ո�д������ݶ������ŵ����ջ������� */
        w25q64.BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
        printf("����������Ϊ��%s \r\n", Rx_Buffer);

        /* ���д�������������������Ƿ���� */
        TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

        if (PASSED == TransferStatus1)
        {
            printf("8M����flash(W25Q64)���Գɹ�!\r\n");
        }
        else
        {
            printf("8M����flash(W25Q64)����ʧ��!\r\n");
        }
    } // if (FlashID == sFLASH_ID)
    else
    {
        printf("��ȡ���� W25Q64 ID!\r\n");
    }

    w25q64.PowerDown();
    return 0;
}

#if 0
    #include "AT45DB.h"

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
            debug_printf("����0x%08x�ɹ�\r\n", addr);
        else
            debug_printf("����0x%08xʧ��\r\n", addr);

        byte Rx_Buffer[80];
        for (int i = 0; i < 9; i++)
        {
            sf.ErasePage(addr);
            memset(Rx_Buffer, 0, ArrayLength(Rx_Buffer));
            if (!sf.Write(addr, Tx_Buffer, size))
                debug_printf("д��0x%08Xʧ�ܣ�\r\n", addr);
            if (!sf.ReadPage(addr, Rx_Buffer, size))
                debug_printf("��ȡ0x%08Xʧ�ܣ�\r\n", addr);
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

TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 !=  *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }
    return PASSED;
}
