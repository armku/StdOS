#include "stdio.h"
#include "W25QXXX.h"
#include "spi.h"

typedef enum
{
    FAILED = 0, PASSED = !FAILED
} TestStatus;


/* 获取缓冲区的长度 */
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

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用stm32开发板\r\n";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

int flashtest(void)
{
    printf("\r\n这是一个8Mbyte串行flash(W25Q64)实验 \r\n");

    Spi spi(SPI_1, 9000000, false);	
    W25QXXX w25q64(&spi,PA4);
    /* 8M串行flash W25Q64初始化 */
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
        printf("检测到华邦串行flash W25Q64 !\r\n");

        /* Erase SPI FLASH Sector to write on */
        w25q64.SectorErase(FLASH_SectorToErase);

        /* 将发送缓冲区的数据写到flash中 */
        w25q64.BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
        w25q64.BufferWrite(Tx_Buffer, 252, BufferSize);
        printf("写入的数据为：%s \r\n", Tx_Buffer);

        /* 将刚刚写入的数据读出来放到接收缓冲区中 */
        w25q64.BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
        printf("读出的数据为：%s \r\n", Rx_Buffer);

        /* 检查写入的数据与读出的数据是否相等 */
        TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

        if (PASSED == TransferStatus1)
        {
            printf("8M串行flash(W25Q64)测试成功!\r\n");
        }
        else
        {
            printf("8M串行flash(W25Q64)测试失败!\r\n");
        }
    } // if (FlashID == sFLASH_ID)
    else
    {
        printf("获取不到 W25Q64 ID!\r\n");
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
            debug_printf("擦除0x%08x成功\r\n", addr);
        else
            debug_printf("擦除0x%08x失败\r\n", addr);

        byte Rx_Buffer[80];
        for (int i = 0; i < 9; i++)
        {
            sf.ErasePage(addr);
            memset(Rx_Buffer, 0, ArrayLength(Rx_Buffer));
            if (!sf.Write(addr, Tx_Buffer, size))
                debug_printf("写入0x%08X失败！\r\n", addr);
            if (!sf.ReadPage(addr, Rx_Buffer, size))
                debug_printf("读取0x%08X失败！\r\n", addr);
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
