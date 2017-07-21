#include "W25QXXX.h"

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable			0x06 
#define W25X_WriteDisable		    0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			    0x03 
#define W25X_FastReadData		    0x0B 
#define W25X_FastReadDual		    0x3B 
#define W25X_PageProgram		    0x02 
#define W25X_BlockErase			    0xD8 
#define W25X_SectorErase		    0x20 
#define W25X_ChipErase			    0xC7 
#define W25X_PowerDown			    0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  	0x01  /* Write In Progress (WIP) flag */

// 设置操作地址
void W25Q64::SetAddr(uint addr){

}

// 读取编号
uint W25Q64::ReadID()
{
    uint Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* Select the FLASH: Chip Select low */
    this->_spi->Start();
    /* Send "RDID " instruction */
    this->_spi->Write(W25X_JedecDeviceID);
    Temp0 = this->_spi->Write(0xFF);
    Temp1 = this->_spi->Write(0xFF);
    Temp2 = this->_spi->Write(0xFF);
    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

W25Q64::W25Q64(Spi *spi)
{
    this->_spi = spi;
    this->PageSize = 256;
}

W25Q64::~W25Q64(){

}
// 读取一页
bool W25Q64::ReadPage(uint addr, byte *buf, uint count)
{
    return true;
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_ReadID
 * Description    : Reads FLASH identification.
 * Input          : None
 * Output         : None
 * Return         : FLASH identification
 *******************************************************************************/
uint W25Q64::ReadDeviceID(void)
{
    uint Temp = 0;

    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "RDID " instruction */
    this->_spi->Write(W25X_DeviceID);
    this->_spi->Write(0XFF);
    this->_spi->Write(0XFF);
    this->_spi->Write(0XFF);

    /* Read a byte from the FLASH */
    Temp = this->_spi->Write(0XFF);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();

    this->ID = this->ReadID();
    return Temp;
}

// 擦除扇区
bool W25Q64::Erase(uint SectorAddr)
{
    /* Send write enable instruction */
    WriteEnable();
    this->WaitForEnd();
    /* Sector Erase */
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();
    /* Send Sector Erase instruction */
    this->_spi->Write(W25X_SectorErase);
    /* Send SectorAddr high nibble address byte */
    this->_spi->Write((SectorAddr &0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    this->_spi->Write((SectorAddr &0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    this->_spi->Write(SectorAddr &0xFF);
    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
    /* Wait the end of Flash writing */
    this->WaitForEnd();
    return true;
}

/*******************************************************************************
 * Function Name  : ErasePage
 * Description    : Erases the specified FLASH sector.
 * Input          : SectorAddr: address of the sector to erase.
 * Output         : None
 * Return         : None
 *******************************************************************************/
// 擦除页
bool W25Q64::ErasePage(uint pageAddr)
{
    return this->Erase(pageAddr);
}

/*******************************************************************************
 * Function Name  : WaitForEnd
 * Description    : Polls the status of the Write In Progress (WIP) flag in the
 *                  FLASH's status  register  and  loop  until write  opertaion
 *                  has completed.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
// 等待操作完成
bool W25Q64::WaitForEnd()
{
    byte FLASH_Status = 0;

    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Read Status Register" instruction */
    this->_spi->Write(W25X_ReadStatusReg);

    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        /* Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        FLASH_Status = this->_spi->Write(0xFF);
    }
    while ((FLASH_Status &WIP_Flag)); /* Write in progress */

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();

    return true;
}

/*******************************************************************************
 * Function Name  : WriteEnable
 * Description    : Enables the write access to the FLASH.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::WriteEnable(void)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Write Enable" instruction */
    this->_spi->Write(W25X_WriteEnable);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
}

/*******************************************************************************
 * Function Name  : BulkErase
 * Description    : Erases the entire FLASH.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::BulkErase(void)
{
    /* Send write enable instruction */
    WriteEnable();

    /* Bulk Erase */
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();
    /* Send Bulk Erase instruction  */
    this->_spi->Write(W25X_ChipErase);
    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();

    /* Wait the end of Flash writing */
    this->WaitForEnd();
}

/*******************************************************************************
 * Function Name  : PageWrite
 * Description    : Writes more than one byte to the FLASH with a single WRITE
 *                  cycle(Page WRITE sequence). The number of byte can't exceed
 *                  the FLASH page size.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - addr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH,
 *                    must be equal or less than "SPI_FLASH_PageSize" value.
 * Output         : None
 * Return         : None
 *******************************************************************************/
// 写入一页
bool W25Q64::WritePage(uint addr, byte *pBuffer, uint NumByteToWrite)
{
    /* Enable the write access to the FLASH */
    WriteEnable();

    /* Select the FLASH: Chip Select low */
    this->_spi->Start();
    /* Send "Write to Memory " instruction */
    this->_spi->Write(W25X_PageProgram);
    /* Send addr high nibble address byte to write to */
    this->_spi->Write((addr &0xFF0000) >> 16);
    /* Send addr medium nibble address byte to write to */
    this->_spi->Write((addr &0xFF00) >> 8);
    /* Send addr low nibble address byte to write to */
    this->_spi->Write(addr &0xFF);

    if (NumByteToWrite > this->PageSize)
    {
        NumByteToWrite = this->PageSize;
        //printf("\n\r Err: this->PageWrite too large!");
    }

    /* while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /* Send the current byte */
        this->_spi->Write(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();

    /* Wait the end of Flash writing */
    this->WaitForEnd();
    return true;
}

/*******************************************************************************
 * Function Name  : Write
 * Description    : Writes block of data to the FLASH. In this function, the
 *                  number of WRITE cycles are reduced, using Page WRITE sequence.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - addr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH.
 * Output         : None
 * Return         : None
 *******************************************************************************/
// 写入数据
bool W25Q64::Write(uint addr, byte *pBuffer, uint NumByteToWrite)
{
    byte NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = addr % this->PageSize;
    count = this->PageSize - Addr;
    NumOfPage = NumByteToWrite / this->PageSize;
    NumOfSingle = NumByteToWrite % this->PageSize;

    if (Addr == 0)
    /* addr is this->PageSize aligned  */
    {
        if (NumOfPage == 0)
        /* NumByteToWrite < this->PageSize */
        {
            this->WritePage(addr, pBuffer, NumByteToWrite);
        }
        else
        /* NumByteToWrite > this->PageSize */
        {
            while (NumOfPage--)
            {
                this->WritePage(addr, pBuffer, this->PageSize);
                addr += this->PageSize;
                pBuffer += this->PageSize;
            }

            this->WritePage(addr, pBuffer, NumOfSingle);
        }
    }
    else
    /* addr is not this->PageSize aligned  */
    {
        if (NumOfPage == 0)
        /* NumByteToWrite < this->PageSize */
        {
            if (NumOfSingle > count)
            /* (NumByteToWrite + addr) > this->PageSize */
            {
                temp = NumOfSingle - count;

                WritePage(addr, pBuffer, count);
                addr += count;
                pBuffer += count;

                this->WritePage(addr, pBuffer, temp);
            }
            else
            {
                WritePage(addr, pBuffer, NumByteToWrite);
            }
        }
        else
        /* NumByteToWrite > this->PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / this->PageSize;
            NumOfSingle = NumByteToWrite % this->PageSize;

            WritePage(addr, pBuffer, count);
            addr += count;
            pBuffer += count;

            while (NumOfPage--)
            {
                this->WritePage(addr, pBuffer, this->PageSize);
                addr += this->PageSize;
                pBuffer += this->PageSize;
            }

            if (NumOfSingle != 0)
            {
                this->WritePage(addr, pBuffer, NumOfSingle);
            }
        }
    }
    return true;
}

//进入掉电模式
void W25Q64::PowerDown(void)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Power Down" instruction */
    this->_spi->Write(W25X_PowerDown);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
}

//唤醒
void W25Q64::WakeUp(void)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Power Down" instruction */
    this->_spi->Write(W25X_ReleasePowerDown);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop(); //等待TRES1
}

/*******************************************************************************
 * Function Name  : Read
 * Description    : Reads a block of data from the FLASH.
 * Input          : - pBuffer : pointer to the buffer that receives the data read
 *                    from the FLASH.
 *                  - ReadAddr : FLASH's internal address to read from.
 *                  - NumByteToRead : number of bytes to read from the FLASH.
 * Output         : None
 * Return         : None
 *******************************************************************************/
// 读取数据
bool W25Q64::Read(uint ReadAddr, byte *pBuffer, uint NumByteToRead)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Read from Memory " instruction */
    this->_spi->Write(W25X_ReadData);

    /* Send ReadAddr high nibble address byte to read from */
    this->_spi->Write((ReadAddr &0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte to read from */
    this->_spi->Write((ReadAddr &0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte to read from */
    this->_spi->Write(ReadAddr &0xFF);

    while (NumByteToRead--)
    /* while there is data to be read */
    {
        /* Read a byte from the FLASH */
        *pBuffer = this->_spi->Write(0xFF);
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
    return true;
}

/*******************************************************************************
 * Function Name  : StartReadSequence
 * Description    : Initiates a read data byte (READ) sequence from the Flash.
 *                  This is done by driving the /CS line low to select the device,
 *                  then the READ instruction is transmitted followed by 3 bytes
 *                  address. This function exit and keep the /CS line low, so the
 *                  Flash still being selected. With this technique the whole
 *                  content of the Flash is read with a single READ instruction.
 * Input          : - ReadAddr : FLASH's internal address to read from.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::StartReadSequence(uint ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Read from Memory " instruction */
    this->_spi->Write(W25X_ReadData);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    this->_spi->Write((ReadAddr &0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    this->_spi->Write((ReadAddr &0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    this->_spi->Write(ReadAddr &0xFF);
}

#if 0
    Spi spi(Spi1);
    W25Q64 w25q64(&spi);

    typedef enum
    {
        FAILED = 0, PASSED = !FAILED
    } TestStatus;


    /* 获取缓冲区的长度 */
    #define TxBufferSize1   (countof(TxBuffer1) - 1)
    #define RxBufferSize1   (countof(TxBuffer1) - 1)
    #define countof(a)      (sizeof(a) / sizeof(*(a)))
    #define BufferSize (countof(Tx_Buffer)-1)

    #define FLASH_WriteAddress     0x00000
    #define FLASH_ReadAddress      FLASH_WriteAddress
    #define FLASH_SectorToErase    FLASH_WriteAddress
    //#define  sFLASH_ID              0xEF3015     //W25X16
    //#define  sFLASH_ID              0xEF4015	 //W25Q16
    #define sFLASH_ID              0XEF4017    //W25Q64


    /* 发送缓冲区初始化 */
    byte Tx_Buffer[] = " 感谢您选用野火stm32开发板\r\n                http://firestm32.taobao.com";
    byte Rx_Buffer[BufferSize];

    /*
     * 函数名：Buffercmp
     * 描述  ：比较两个缓冲区中的数据是否相等
     * 输入  ：-pBuffer1     src缓冲区指针
     *         -pBuffer2     dst缓冲区指针
     *         -BufferLength 缓冲区长度
     * 输出  ：无
     * 返回  ：-PASSED pBuffer1 等于   pBuffer2
     *         -FAILED pBuffer1 不同于 pBuffer2
     */
    TestStatus Buffercmp(byte *pBuffer1, byte *pBuffer2, ushort BufferLength)
    {
        while (BufferLength--)
        {
            if (*pBuffer1 !=  *pBuffer2)
            {
                return FAILED;
            }

            pBuffer1++;
            pBuffer2++;
        }
        return PASSED;
    }


    void W25Q64Test()
    {
        TestStatus TransferStatus1 = FAILED;

        printf("\r\n 这是一个8Mbyte串行flash(W25Q64)实验 \r\n");

        spi.Open();
        /* Get SPI Flash Device ID */
        w25q64.DeviceID = w25q64.ReadDeviceID();

        Sys.Delay(10);

        printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", w25q64.ID, w25q64.DeviceID);

        /* Check the SPI Flash ID */
        if (w25q64.ID == sFLASH_ID)
        /* #define  sFLASH_ID  0XEF4017 */
        {
            printf("\r\n 检测到华邦串行flash W25Q64 !\r\n");

            /* Erase FLASH Sector to write on */
            w25q64.Erase(FLASH_SectorToErase);

            /* 将发送缓冲区的数据写到flash中 */
            w25q64.Write(FLASH_WriteAddress, Tx_Buffer, BufferSize);
            w25q64.Write(252, Tx_Buffer, BufferSize);
            printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);

            /* 将刚刚写入的数据读出来放到接收缓冲区中 */
            w25q64.Read(FLASH_ReadAddress, Rx_Buffer, BufferSize);
            printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);

            /* 检查写入的数据与读出的数据是否相等 */
            TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

            if (PASSED == TransferStatus1)
            {
                printf("\r\n 8M串行flash(W25Q64)测试成功!\n\r");
            }
            else
            {
                printf("\r\n 8M串行flash(W25Q64)测试失败!\n\r");
            }
        } // if (FlashID == sFLASH_ID)
        else
        {
            printf("\r\n 获取不到 W25Q64 ID!\n\r");
        }

        w25q64.PowerDown();
        printf("\r\n\n\r");
    }
#endif 































OutputPort nsspp;

Spi spi(Spi1);

// 设置操作地址
void W25Q128::SetAddr(uint addr)
{
	
}
// 等待操作完成
bool W25Q128::WaitForEnd()
{
	return false;
}

// 读取编号
uint W25Q128::ReadID()
{
	return 0;
}
void W25Q128::WriteEnable(void)
{
	
}
void W25Q128::StartReadSequence(uint ReadAddr)
{
	
}
W25Q128::W25Q128(Spi *spi)
{
	
}
W25Q128::~W25Q128()
{
	
}

// 擦除扇区
bool W25Q128::Erase(uint sector)
{
	return false;
}
// 擦除页
bool W25Q128::ErasePage(uint pageAddr)
{
	return false;
}

// 写入一页
bool W25Q128::WritePage(uint addr, byte *buf, uint count)
{
	return false;
}
// 读取一页
bool W25Q128::ReadPage(uint addr, byte *buf, uint count)
{
	return false;
}
// 写入数据
bool W25Q128::Write(uint addr, byte *buf, uint count)
{
	return false;
}
// 读取数据
bool W25Q128::Read(uint addr, byte *buf, uint count)
{
	return false;
}
//Reads FLASH identification.
uint W25Q128::ReadDeviceID(void) 
{
	return false;
}
//Erases the entire FLASH.
void W25Q128::BulkErase(void)
{
	
}
//唤醒
void W25Q128::WakeUp(void) 
{
	
}
//进入掉电模式
void W25Q128::PowerDown(void)
{
	
}

//W25X系列/Q系列芯片列表	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

extern ushort W25QXX_TYPE; //定义W25QXX芯片型号		   

////////////////////////////////////////////////////////////////////////////////// 
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

void W25QXX_Init(void);
ushort W25QXX_ReadID(void); //读取FLASH ID
byte W25QXX_ReadSR(void); //读取状态寄存器 
void W25QXX_Write_SR(byte sr); //写状态寄存器
void W25QXX_Write_Enable(void); //写使能 
void W25QXX_Write_Disable(void); //写保护
void W25QXX_Write_NoCheck(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite);
void W25QXX_Read(byte *pBuffer, uint ReadAddr, ushort NumByteToRead); //读取flash
void W25QXX_Write(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite); //写入flash
void W25QXX_Erase_Chip(void); //整片擦除
void W25QXX_Erase_Sector(uint Dst_Addr); //扇区擦除
void W25QXX_Wait_Busy(void); //等待空闲
void W25QXX_PowerDown(void); //进入掉电模式
void W25QXX_WAKEUP(void); //唤醒


ushort W25QXX_TYPE = W25Q128; //默认是W25Q128

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector 

//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{		
	nsspp.Invert=false;
	nsspp.OpenDrain=false;
	nsspp.Set(PG7);

	nsspp=1; //PG7输出1,防止NRF干扰SPI FLASH的通信 
	spi.Stop();
    
//	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
//    spi.Write(0xff); //启动传输
    W25QXX_TYPE = W25QXX_ReadID(); //读取FLASH ID.
}

//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
byte W25QXX_ReadSR(void)
{
    byte byte = 0;
    //使能器件   
	spi.Start();
    spi.Write(W25X_ReadStatusReg); //发送读取状态寄存器命令    
    byte = spi.Write(0Xff); //读取一个字节  
    //取消片选     
	spi.Stop();
    return byte;
}

//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25QXX_Write_SR(byte sr)
{
    //使能器件   
	spi.Start();
    spi.Write(W25X_WriteStatusReg); //发送写取状态寄存器命令    
    spi.Write(sr); //写入一个字节  
    //取消片选     
	spi.Stop();
    
}

//W25QXX写使能	
//将WEL置位   
void W25QXX_Write_Enable(void)
{
    //使能器件   
	spi.Start(); 
    spi.Write(W25X_WriteEnable); //发送写使能  
    //取消片选     	      
	spi.Stop();
}

//W25QXX写禁止	
//将WEL清零  
void W25QXX_Write_Disable(void)
{
     //使能器件   
	spi.Start();  
    spi.Write(W25X_WriteDisable); //发送写禁止指令    
    //取消片选     
	spi.Stop();
        	      
}

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
ushort W25QXX_ReadID(void)
{
    ushort Temp = 0;
     //使能器件   
	spi.Start();
    spi.Write(0x90); //发送读取ID命令	    
    spi.Write(0x00);
    spi.Write(0x00);
    spi.Write(0x00);
    Temp |= spi.Write(0xFF) << 8;
    Temp |= spi.Write(0xFF);
     //取消片选     
	spi.Stop();
    
    return Temp;
}

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(byte *pBuffer, uint ReadAddr, ushort NumByteToRead)
{
    ushort i;
     //使能器件   
	spi.Start();
    spi.Write(W25X_ReadData); //发送读取命令   
    spi.Write((byte)((ReadAddr) >> 16)); //发送24bit地址    
    spi.Write((byte)((ReadAddr) >> 8));
    spi.Write((byte)ReadAddr);
    for (i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = spi.Write(0XFF); //循环读数  
    }
    //取消片选     
	spi.Stop();
}

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25QXX_Write_Page(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
    ushort i;
    W25QXX_Write_Enable(); //SET WEL 
    //使能器件   
	spi.Start();  
    spi.Write(W25X_PageProgram); //发送写页命令   
    spi.Write((byte)((WriteAddr) >> 16)); //发送24bit地址    
    spi.Write((byte)((WriteAddr) >> 8));
    spi.Write((byte)WriteAddr);
    for (i = 0; i < NumByteToWrite; i++)
        spi.Write(pBuffer[i]);
    //循环写数  
    //取消片选     
	spi.Stop();
    W25QXX_Wait_Busy(); //等待写入结束
}

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
    ushort pageremain;
    pageremain = 256-WriteAddr % 256; //单页剩余的字节数		 	    
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite;
    //不大于256个字节
    while (1)
    {
        W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
        if (NumByteToWrite == pageremain)
            break;
        //写入结束了
        else
        //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain; //减去已经写入了的字节数
            if (NumByteToWrite > 256)
                pageremain = 256;
            //一次可以写入256个字节
            else
                pageremain = NumByteToWrite;
            //不够256个字节了
        }
    };
}

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
byte W25QXX_BUFFER[4096];
void W25QXX_Write(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
    uint secpos;
    ushort secoff;
    ushort secremain;
    ushort i;
    byte *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; //扇区地址  
    secoff = WriteAddr % 4096; //在扇区内的偏移
    secremain = 4096-secoff; //扇区剩余空间大小   
    //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite;
    //不大于4096个字节
    while (1)
    {
        W25QXX_Read(W25QXX_BUF, secpos *4096, 4096); //读出整个扇区的内容
        for (i = 0; i < secremain; i++)
        //校验数据
        {
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break;
            //需要擦除  	  
        }
        if (i < secremain)
        //需要擦除
        {
            W25QXX_Erase_Sector(secpos); //擦除这个扇区
            for (i = 0; i < secremain; i++)
            //复制
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            W25QXX_Write_NoCheck(W25QXX_BUF, secpos *4096, 4096); //写入整个扇区  

        }
        else
            W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);
        //写已经擦除了的,直接写入扇区剩余区间. 				   
        if (NumByteToWrite == secremain)
            break;
        //写入结束了
        else
        //写入未结束
        {
            secpos++; //扇区地址增1
            secoff = 0; //偏移位置为0 	 

            pBuffer += secremain; //指针偏移
            WriteAddr += secremain; //写地址偏移	   
            NumByteToWrite -= secremain; //字节数递减
            if (NumByteToWrite > 4096)
                secremain = 4096;
            //下一个扇区还是写不完
            else
                secremain = NumByteToWrite;
            //下一个扇区可以写完了
        }
    };
}

//擦除整个芯片		  
//等待时间超长...
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable(); //SET WEL 
    W25QXX_Wait_Busy();
    //使能器件   
	spi.Start();   
    spi.Write(W25X_ChipErase); //发送片擦除命令  
    //取消片选     
	spi.Stop();    	      
    W25QXX_Wait_Busy(); //等待芯片擦除结束
}

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25QXX_Erase_Sector(uint Dst_Addr)
{
    //监视falsh擦除情况,测试用   
    printf("fe:%x\r\n", Dst_Addr);
    Dst_Addr *= 4096;
    W25QXX_Write_Enable(); //SET WEL 	 
    W25QXX_Wait_Busy();
    //使能器件   
	spi.Start();
    spi.Write(W25X_SectorErase); //发送扇区擦除指令 
    spi.Write((byte)((Dst_Addr) >> 16)); //发送24bit地址    
    spi.Write((byte)((Dst_Addr) >> 8));
    spi.Write((byte)Dst_Addr);
    //取消片选     
	spi.Stop();	      
    W25QXX_Wait_Busy(); //等待擦除完成
}

//等待空闲
void W25QXX_Wait_Busy(void)
{
    while ((W25QXX_ReadSR() &0x01) == 0x01)
        ;
    // 等待BUSY位清空
}

//进入掉电模式
void W25QXX_PowerDown(void)
{
    //使能器件   
	spi.Start();
    spi.Write(W25X_PowerDown); //发送掉电命令  
    //取消片选     
	spi.Stop();  	      
    Sys.Delay(3);                              //等待TPD  
}

//唤醒
void W25QXX_WAKEUP(void)
{
    //使能器件   
	spi.Start(); 
    spi.Write(W25X_ReleasePowerDown); //  send W25X_PowerDown command 0xAB    
    //取消片选     
	spi.Stop();   	      
    Sys.Delay(3);                               //等待TRES1
}

//要写入到W25Q16的字符串数组
const byte TEXT_Buffer[] = 
{
    "Explorer STM32F4 SPI TEST"
};
#define SIZE sizeof(TEXT_Buffer)	

//W25Q128 w25q128(&spi);	
void w25q128test()
{
	
	
    byte datatemp[SIZE];
    uint FLASH_SIZE;
    W25QXX_Init(); //W25QXX初始化
    printf("SPI TEST\r\n");
    while (W25QXX_ReadID() != W25Q128)
    //检测不到W25Q128
    {
        printf("W25Q128 Check Failed!\r\n");
    }
    printf("W25Q128 Ready!\r\n");
    FLASH_SIZE = 16 * 1024 * 1024; //FLASH 大小为16字节
    printf("\r\n");

    printf("Start Write W25Q128....\r\n");
    W25QXX_Write((byte*)TEXT_Buffer, FLASH_SIZE - 100, SIZE); //从倒数第100个地址处开始,写入SIZE长度的数据
    printf("W25Q128 Write Finished!\r\n"); //提示传送完成
    printf("\r\n");

    printf("Start Read W25Q128....\r\n");
    W25QXX_Read(datatemp, FLASH_SIZE - 100, SIZE); //从倒数第100个地址处开始,读出SIZE个字节
    printf("The Data Readed Is:\r\n"); //提示传送完成
    printf((const char*)datatemp);

    printf("\r\n\r\n");
}
