#include "Flash.h"

#ifdef DEBUG1
    void STMFLASH::Test()
    {
        byte buftest1[3200];
        uint addr = STM32_FLASH_BASE + 1024 * 36+10;
        STMFLASH flash1;
        flash1.SetFlashSize(512);
        debug_printf("测试开始\r\n");
        for (int i = 0; i < 1200; i++)
        {
            buftest1[i] = i % 200;
        }

        int wid = flash1.Write(addr, buftest1, 3200);
        debug_printf("write ok\r\n");
        for (int i = 0; i < 3200; i++)
        {
            buftest1[i] = 0;
        }

        int rid = flash1.Read(addr, buftest1, 3200);
        debug_printf("read ok\r\n");
        for (int i = 0; i < 1200; i++)
        {
            if (buftest1[i] != (i % 200))
            {
                debug_printf("测试失败，数据错误：%d\r\n", buftest1[i]);
                return ;
            }
            //debug_printf("%d:%d\t", i, buftest1[i]);
        }

        debug_printf("测试成功\r\n");
    }
#endif

