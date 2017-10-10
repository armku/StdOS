#include "Drivers\W25QXXX.h"


#ifdef DEBUG
    //����ԭ�� ̽���߿�����
    #if W25QXXXUSESPISOFT
        SpiSoft spi128;
    #else 
        Spi spi128(Spi1);
    #endif 
    W25Q128 w25q128(&spi128);
    #ifndef STM32F0
        OutputPort nsspp;
    #endif 

    //Ҫд�뵽W25Q16���ַ�������
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
            nsspp = 1; //PG7���1,��ֹNRF����SPI FLASH��ͨ�� 
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
        w25q128.W25QXX_Init(); //W25QXX��ʼ��
        printf("\r\nSPI TEST\r\n");
        while (w25q128.ReadID() != W25QXXX128)
        //��ⲻ��W25Q128
        {
            printf("W25Q128 Check Failed!\r\n");
            return ;
        }
        printf("W25Q128 Ready!\r\n");
        FLASH_SIZE = 16 * 1024 * 1024; //FLASH ��СΪ16�ֽ�
        printf("\r\n");

        printf("Start Write W25Q128....\r\n");
        w25q128.Write(FLASH_SIZE - 100, (byte*)TEXT_Buffer, sizeof(TEXT_Buffer)); //�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
        printf("W25Q128 Write Finished!\r\n"); //��ʾ�������
        printf("\r\n");

        printf("Start Read W25Q128....\r\n");
        w25q128.Read(FLASH_SIZE - 100, datatemp, sizeof(TEXT_Buffer)); //�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
        printf("The Data Readed Is:\r\n"); //��ʾ�������
        printf((const char*)datatemp);

        printf("\r\n\r\n");
    }
#endif


