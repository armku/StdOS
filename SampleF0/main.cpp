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

Spi spi(Spi1);
OutputPort nss(PA8, false,true);

//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

uint8_t W25QXX_ReadSR(void)//flash读入数据
{
    uint8_t byte=0;
    
    nss=0;
    spi.Write(W25X_ReadStatusReg);
    byte=spi.Write(0);
    nss=1;
    return byte;
}

void W25QXX_Write_Enable(void)//flash写数据启动函数
{    
    nss=0;
    spi.Write(W25X_WriteEnable);
    nss=1;
}

void W25QXX_Wait_Busy(void)//flash忙等待
{    
    while((W25QXX_ReadSR()&0x01)==0x01);
}

void W25QXX_Erase_Chip(void)
{    
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();

    nss=0;
    spi.Write(W25X_ChipErase);
    nss=1;

    W25QXX_Wait_Busy();
}

void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{    
    Dst_Addr*=4096;
    
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();

    nss=0;
    spi.Write(W25X_SectorErase);
    spi.Write((uint8_t)(Dst_Addr>>16));
    spi.Write((uint8_t)(Dst_Addr>>8));
    spi.Write((uint8_t)(Dst_Addr));
    nss=1;
    W25QXX_Wait_Busy();
}

void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint16_t i=0;
  
    W25QXX_Wait_Busy();
    
    nss=0;
    spi.Write(W25X_ReadData);
    spi.Write((uint8_t)(ReadAddr>>16));
    spi.Write((uint8_t)(ReadAddr>>8));
    spi.Write((uint8_t)(ReadAddr));
    
    for(i=0;i<NumByteToRead;i++)
    {
        pBuffer[i]=spi.Write(0);
    }
  
    nss=1;
}

void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)//写页
{
    uint16_t i=0;
  
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    
    nss=0;
    spi.Write(W25X_PageProgram);
    spi.Write((uint8_t)(WriteAddr>>16));
    spi.Write((uint8_t)(WriteAddr>>8));
    spi.Write((uint8_t)(WriteAddr));
    
    for(i=0;i<NumByteToWrite;i++)
    {
        spi.Write(pBuffer[i]);
    }
    nss=1;
    W25QXX_Wait_Busy();
}

void W25QXX_Demo(void)//例程
{
    uint8_t pBuffer1[]="1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uint8_t pBuffer2[]="000000000000000000000000000000000000";
    
    W25QXX_Erase_Sector(0);
    
	printf("\r\n写入数据:%s\r\n",pBuffer1);
    W25QXX_Write_Page(pBuffer1,0,36);
    Sys.Sleep(20);
	
    W25QXX_Read(pBuffer2,0,36);
	printf("读出数据:%s\r\n",pBuffer2);    
}
void w25q128test()
{
	nss=1;//片选置高
	
	W25QXX_Demo();
	
}
