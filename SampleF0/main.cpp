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

void RCC_Configuration(void)//时钟初始化函数
{  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//设置A端口时钟使能
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);//设置B端口时钟使能
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);//设置C端口时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//设置SPI时钟使能
}
void W25QXX_GPIO(void)//flash控制管脚初始化函数
{    
	nss=1;//片选置高
}
void SPI1_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_0);  
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_0); 

	/*!< Configure SPI_FLASH_SPI pins: SCK */
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	/*!< Configure SPI_FLASH_SPI pins: MISO */
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*!< Configure SPI_FLASH_SPI pins: MOSI */
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void SPI1_Configation(void)//SPI通讯格式设置函数
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SPI1, ENABLE);
}

void BSP_Configuration(void)//硬件初始化函数
{	
	RCC_Configuration();//调用时钟初始化函数
	W25QXX_GPIO();//调用25Qxx控制管脚初始化函数
	SPI1_GPIO();//调用SPI标准通讯格式初始化函数
	SPI1_Configation();//调用SPI通讯格式设置函数
}


static void NVIC_Configuration(void)//中断优先级设置函数
{
	NVIC_InitTypeDef   NVIC_InitStructure;

	// rs232
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

uint8_t SPI1_SendByte(uint8_t byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI2 peripheral */
	SPI_SendData8(SPI1, byte);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_ReceiveData8(SPI1);
}
uint8_t W25QXX_ReadSR(void)//flash读入数据
{
    uint8_t byte=0;
    
    nss=0;
    SPI1_SendByte(W25X_ReadStatusReg);
    byte=SPI1_SendByte(0);
    nss=1;
    return byte;
}

void W25QXX_Write_Enable(void)//flash写数据启动函数
{    
    nss=0;
    SPI1_SendByte(W25X_WriteEnable);
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
    SPI1_SendByte(W25X_ChipErase);
    nss=1;

    W25QXX_Wait_Busy();
}

void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{    
    Dst_Addr*=4096;
    
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();

    nss=0;
    SPI1_SendByte(W25X_SectorErase);
    SPI1_SendByte((uint8_t)(Dst_Addr>>16));
    SPI1_SendByte((uint8_t)(Dst_Addr>>8));
    SPI1_SendByte((uint8_t)(Dst_Addr));
    nss=1;
    W25QXX_Wait_Busy();
}

void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint16_t i=0;
  
    W25QXX_Wait_Busy();
    
    nss=0;
    SPI1_SendByte(W25X_ReadData);
    SPI1_SendByte((uint8_t)(ReadAddr>>16));
    SPI1_SendByte((uint8_t)(ReadAddr>>8));
    SPI1_SendByte((uint8_t)(ReadAddr));
    
    for(i=0;i<NumByteToRead;i++)
    {
        pBuffer[i]=SPI1_SendByte(0);
    }
  
    nss=1;
}

void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)//写页
{
    uint16_t i=0;
  
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    
    nss=0;
    SPI1_SendByte(W25X_PageProgram);
    SPI1_SendByte((uint8_t)(WriteAddr>>16));
    SPI1_SendByte((uint8_t)(WriteAddr>>8));
    SPI1_SendByte((uint8_t)(WriteAddr));
    
    for(i=0;i<NumByteToWrite;i++)
    {
        SPI1_SendByte(pBuffer[i]);
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
	BSP_Configuration();//调用硬件初始化函数
	NVIC_Configuration();
	
	W25QXX_Demo();
	
}
