#include "Flash.h"
#include "Sys.h"
#ifdef TestFlsh
    #include "stdlib.h"
#endif 

int Flash::Erase(uint addr, int size)
{
    return 0;
}

int Flash::WriteBlock(uint addr, byte *pBuffer, int size)
{
    return 0;
}

int Flash::Read(uint addr, byte *pBuffer, int size)
{
    return 0;
}

int Flash::Write(uint addr, byte *pBuffer, int size)
{
    return 0;
}

#ifdef TestFlsh

    void TestFlash()
    {
        debug_printf("\r\n\r\n");
        debug_printf("TestFlash Start......\r\n");

        uint addr = 0x08004000;

        Flash flash;
        debug_printf("FlashSize: %d Bytes  BytesPerBlock: %d Bytes\r\n", flash.Size, flash.BytesPerBlock);
        flash.Erase(addr, 0x100);

        byte buf[] = "STM32F10x SPI Firmware Library Example: communication with an AT45DB SPI FLASH";
        uint size = ArrayLength(buf);
        flash.WriteBlock(addr, buf, size);

        byte *rx = (byte*)malloc(size);
        flash.Read(addr, rx, size);

        int n = 0;
        for (int i = 0; i < size; i++)
        {
            if (buf[i] != rx[i])
                n++;
        }
        debug_printf("diffent %d\r\n", n);

        // ¼¯³É²âÊÔ
        //flash.Erase(addr, 0x100);
        flash.Write(addr, buf, size);

        flash.Read(addr, rx, size);

        n = 0;
        for (int i = 0; i < size; i++)
        {
            if (buf[i] != rx[i])
                n++;
        }
        debug_printf("diffent %d\r\n", n);

        free(rx);

        debug_printf("\r\nTestFlash Finish!\r\n");
    }
#endif
