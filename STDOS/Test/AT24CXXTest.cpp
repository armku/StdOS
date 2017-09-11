#include "..\Drivers\AT24CXX.h"

#if 0
    #define EE_SIZE				256			  /* 24xx02������ */    
    #ifdef STM32F0
        AT24CXX at2402(PB6, PB7, AT24C02);
    #elif defined STM32F1
        AT24CXX at2402(PB6, PB7, AT24C02);
    #elif defined STM32F4
        AT24CXX at2402(PB8, PB9, AT24C02);
    #endif 
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



        printf("\r\n");

        ushort i;
        byte write_buf[EE_SIZE];
        byte read_buf[EE_SIZE];

        /*-----------------------------------------------------------------------------------*/
        if (at2402.CheckOk() == 0)
        {
            /* û�м�⵽EEPROM */
            printf("û�м�⵽����EEPROM!\r\n");

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
            printf("дeeprom����\r\n");
            return ;
        }
        else
        {
            printf("дeeprom�ɹ���\r\n");
        }

        /*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
        Sys.Sleep(10);
        /*-----------------------------------------------------------------------------------*/
        if (at2402.Read(0, read_buf, EE_SIZE) == 0)
        {
            printf("��eeprom����\r\n");
            return ;
        }
        else
        {
            printf("��eeprom�ɹ����������£�\r\n");
        }
        /*-----------------------------------------------------------------------------------*/
        for (i = 0; i < EE_SIZE; i++)
        {
            if (read_buf[i] != write_buf[i])
            {
                printf("0x%02X ", read_buf[i]);
                printf("����:EEPROM������д������ݲ�һ��");
                return ;
            }
            printf(" %02X", read_buf[i]);

            if ((i &15) == 15)
            {
                printf("\r\n");
            }
        }
        printf("eeprom��д���Գɹ�\r\n");
    }
#endif

