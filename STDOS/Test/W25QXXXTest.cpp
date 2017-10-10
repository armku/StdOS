#include "Drivers\W25QXXX.h"


#ifdef DEBUG
    //正点原子 探索者开发板
    #if W25QXXXUSESPISOFT
        SpiSoft spi128;
    #else 
        Spi spi128(Spi1);
    #endif 
    W25Q128 w25q128(&spi128);
    #ifndef STM32F0
        OutputPort nsspp;
    #endif 

    //要写入到W25Q16的字符串数组
    const byte TEXT_Buffer[] = 
    {
        "Explorer STM32F4 SPI TEST"
    };

    void w25q128test()
    {
        #ifndef STM32F0
            nsspp.Invert = false;
            nsspp.OpenDrain = false;
            nsspp.Set(PG7);
            nsspp = 1; //PG7输出1,防止NRF干扰SPI FLASH的通信 
        #endif 

        #if W25QXXXUSESPISOFT
            spi128.SetPin(PB3, PB4, PB5);
            spi128.CPOL = CPOL_High;
            spi128.CPHA = CPHA_2Edge;
        #endif 
        #ifndef STM32F0
            spi128.SetNss(PB14);
        #else 
            spi128.SetNss(PA8);
        #endif 
        byte datatemp[sizeof(TEXT_Buffer)];
        uint FLASH_SIZE;
        w25q128.W25QXX_Init(); //W25QXX初始化
        printf("\r\nSPI TEST\r\n");
        while (w25q128.ReadID() != W25QXXX128)
        //检测不到W25Q128
        {
            printf("W25Q128 Check Failed!\r\n");
            return ;
        }
        printf("W25Q128 Ready!\r\n");
        FLASH_SIZE = 16 * 1024 * 1024; //FLASH 大小为16字节
        printf("\r\n");

        printf("Start Write W25Q128....\r\n");
        w25q128.Write(FLASH_SIZE - 100, (byte*)TEXT_Buffer, sizeof(TEXT_Buffer)); //从倒数第100个地址处开始,写入SIZE长度的数据
        printf("W25Q128 Write Finished!\r\n"); //提示传送完成
        printf("\r\n");

        printf("Start Read W25Q128....\r\n");
        w25q128.Read(FLASH_SIZE - 100, datatemp, sizeof(TEXT_Buffer)); //从倒数第100个地址处开始,读出SIZE个字节
        printf("The Data Readed Is:\r\n"); //提示传送完成
        printf((const char*)datatemp);

        printf("\r\n\r\n");
    }
#endif


