#include "../W25QXXX.h"

#define _W25QXX_TEST_CPP

#ifdef _W25QXX_TEST_CPP
    //正点原子 探索者开发板
    #if W25QXXXUSESPISOFT
        SpiSoft spi128;
    #else 
        Spi spi128(Spi1);
    #endif 
    W25Q128 w25q128(&spi128);
    #ifndef STM32F0
        Port nsspp;
    #endif 

    //要写入到W25Q16的字符串数组
    const uint8_t TEXT_Buffer[] = 
    {
        "Explorer STM32F4 SPI TEST"
    };

    void w25q128test()
    {
        #ifndef STM32F0
            nsspp.SetPin(PG7);
			nsspp.pinMode(GPIO_Out_PP);
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
        uint8_t datatemp[sizeof(TEXT_Buffer)];
        uint32_t FLASH_SIZE;
        w25q128.W25QXX_Init(); //W25QXX初始化
        debug_printf("\r\nSPI TEST\r\n");
        while (w25q128.ReadID() != W25QXXX128)
        //检测不到W25Q128
        {
            debug_printf("W25Q128 Check Failed!\r\n");
            return ;
        }
        debug_printf("W25Q128 Ready!\r\n");
        FLASH_SIZE = 16 * 1024 * 1024; //FLASH 大小为16字节
        debug_printf("\r\n");

        debug_printf("Start Write W25Q128....\r\n");
        w25q128.Write(FLASH_SIZE - 100, (uint8_t*)TEXT_Buffer, sizeof(TEXT_Buffer)); //从倒数第100个地址处开始,写入SIZE长度的数据
        debug_printf("W25Q128 Write Finished!\r\n"); //提示传送完成
        debug_printf("\r\n");

        debug_printf("Start Read W25Q128....\r\n");
        w25q128.Read(FLASH_SIZE - 100, datatemp, sizeof(TEXT_Buffer)); //从倒数第100个地址处开始,读出SIZE个字节
        debug_printf("The Data Readed Is:\r\n"); //提示传送完成
        debug_printf((const char*)datatemp);

        debug_printf("\r\n\r\n");
    }
#endif


