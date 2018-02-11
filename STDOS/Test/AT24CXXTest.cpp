#include "..\Drivers\AT24CXX.h"
 DEBUGAT24C02Test

#ifdef _AT24CXX_TEST
    #define EE_SIZE				256			  /* 24xx02总容量 */    
    #ifdef STM32F0
        AT24CXX at2402(PB6, PB7, AT24C02);
    #elif defined STM32F1
        AT24CXX at2402(PB6, PB7, AT24C02);
    #elif defined STM32F4
        AT24CXX at2402(PI9, PI10, AT24C02);
    #endif 
    void AT24C02Test()
    {
        #if 0
            HardI2C SoftI2C 
            //初始化光强传感器BH750
            static SoftI2C iic;
            iic.SetPin(PB6, PB7);
            bh.IIC = &iic;
            bh.Init();

            bh.Read(); //这样得到数据 
        #endif 



        debug_printf("\r\n");

        ushort i;
        byte write_buf[EE_SIZE];
        byte read_buf[EE_SIZE];

        /*-----------------------------------------------------------------------------------*/
        if (at2402.CheckOk() == 0)
        {
            /* 没有检测到EEPROM */
            debug_printf("没有检测到串行EEPROM!\r\n");

            while (1)
                ;
            /* 停机 */
        }
        /*------------------------------------------------------------------------------------*/
        /* 填充测试缓冲区 */
        for (i = 0; i < EE_SIZE; i++)
        {
            write_buf[i] = i;
        }
        /*------------------------------------------------------------------------------------*/
        if (at2402.Write(0, write_buf, EE_SIZE) == 0)
        {
            debug_printf("写eeprom出错！\r\n");
            return ;
        }
        else
        {
            debug_printf("写eeprom成功！\r\n");
        }

        /*写完之后需要适当的延时再去读，不然会出错*/
        Sys.Sleep(10);
        /*-----------------------------------------------------------------------------------*/
        if (at2402.Read(0, read_buf, EE_SIZE) == 0)
        {
            debug_printf("读eeprom出错！\r\n");
            return ;
        }
        else
        {
            debug_printf("读eeprom成功，数据如下：\r\n");
        }
        /*-----------------------------------------------------------------------------------*/
        for (i = 0; i < EE_SIZE; i++)
        {
            if (read_buf[i] != write_buf[i])
            {
                debug_printf("0x%02X ", read_buf[i]);
                debug_printf("错误:EEPROM读出与写入的数据不一致");
                return ;
            }
            debug_printf(" %02X", read_buf[i]);

            if ((i &15) == 15)
            {
                debug_printf("\r\n");
            }
        }
        debug_printf("eeprom读写测试成功\r\n");
    }
#endif

