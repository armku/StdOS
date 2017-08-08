#include "Port.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\W25QXXX.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[] = 
{
    led1, led2, led3, led4
};

void LedTask(void *param)
{
    static byte flag = 1;
    OutputPort *leds = (OutputPort*)param;

    flag <<= 1;
    flag &= 0x0f;
    if (flag == 0)
    {
        flag = 1;
    }

    leds[0] = flag &0x01;
    leds[1] = flag &0x02;
    leds[2] = flag &0x04;
    leds[3] = flag &0x08;

    //    led2 = key0;
}

#define namee "StdOS"
//void AT24C02Test();
void w25q128test();
int main()
{
    Sys.Name = (char*)namee;
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
    //	AT24C02Test();	
    w25q128test();
    Sys.AddTask(LedTask, &ledss, 0, 500, "LedTask");

    Sys.Start();
}

//正点原子 探索者开发板
#if W25QXXXUSESPISOFT
    SpiSoft spi128;
#else 
    Spi spi128(Spi1);
#endif 
W25Q128 w25q128(&spi128);

//要写入到W25Q16的字符串数组
const byte TEXT_Buffer[] = 
{
    "Explorer STM32F4 SPI TEST"
};

void w25q128test()
{
    #if W25QXXXUSESPISOFT
        spi128.SetPin(PB3, PB4, PB5);
        spi128.CPOL = CPOL_High;
        spi128.CPHA = CPHA_2Edge;
    #endif 
    spi128.SetNss(PA8);
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
