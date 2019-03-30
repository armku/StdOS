#include "W25QXXX.h"

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	 

/* Private define ------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////// 
//指令表
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

// 读取编号
uint32_t W25Q64::ReadID()
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    this->_spi->Start();
    this->_spi->Write(W25X_JedecDeviceID);
    Temp0 = this->_spi->Write(0xFF);
    Temp1 = this->_spi->Write(0xFF);
    Temp2 = this->_spi->Write(0xFF);
    this->_spi->Stop();
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

#if W25QXXXUSESPISOFT
    W25Q64::W25Q64(SpiSoft *spi)
#else 
    W25Q64::W25Q64(Spi *spi)
#endif 
{
    this->_spi = spi;
    this->PageSize = 256;
}

uint32_t W25Q64::ReadDeviceID(void)
{
    uint32_t Temp = 0;

    this->_spi->Start();

    this->_spi->Write(W25X_DeviceID);
    this->_spi->Write(0XFF);
    this->_spi->Write(0XFF);
    this->_spi->Write(0XFF);

    Temp = this->_spi->Write(0XFF);

    this->_spi->Stop();

    this->ID = this->ReadID();
    return Temp;
}

// 擦除扇区
bool W25Q64::EraseSector(uint32_t SectorAddr)
{
    this->WriteEnable();
    this->WaitForEnd();
    this->_spi->Start();
    this->_spi->Write(W25X_SectorErase);
    this->_spi->Write((SectorAddr &0xFF0000) >> 16);
    this->_spi->Write((SectorAddr &0xFF00) >> 8);
    this->_spi->Write(SectorAddr &0xFF);
    this->_spi->Stop();
    this->WaitForEnd();
    return true;
}

// 擦除页
bool W25Q64::ErasePage(uint32_t pageAddr)
{
    return this->EraseSector(pageAddr);
}

// 等待操作完成
//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00

bool W25Q64::WaitForEnd()
{
    uint8_t FLASH_Status = 0;
    this->_spi->Start();
    this->_spi->Write(W25X_ReadStatusReg);
    do
    {
        FLASH_Status = this->_spi->Write(0xFF);
    }
    while ((FLASH_Status &WIP_Flag));
    this->_spi->Stop();

    return true;
}

void W25Q64::WriteEnable(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_WriteEnable);
    this->_spi->Stop();
}

//写保护
void W25Q64::WriteDisable(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_WriteDisable);
    this->_spi->Stop();
}

void W25Q64::EraseChip(void)
{
    this->WriteEnable();
    this->WaitForEnd();
    this->_spi->Start();
    this->_spi->Write(W25X_ChipErase);
    this->_spi->Stop();
    this->WaitForEnd();
}

bool W25Q64::WritePage(uint32_t addr, uint8_t *pBuffer, uint32_t NumByteToWrite)
{
    WriteEnable();
    this->_spi->Start();
    this->_spi->Write(W25X_PageProgram);
    this->_spi->Write((addr &0xFF0000) >> 16);
    this->_spi->Write((addr &0xFF00) >> 8);
    this->_spi->Write(addr &0xFF);

    if (NumByteToWrite > this->PageSize)
    {
        NumByteToWrite = this->PageSize;
        //StdPrintf("\n\r Err: this->PageWrite too large!");
    }

    while (NumByteToWrite--)
    {
        this->_spi->Write(*pBuffer);
        pBuffer++;
    }

    this->_spi->Stop();

    this->WaitForEnd();
    return true;
}

// 写入数据
bool W25Q64::Write(uint32_t addr, uint8_t *pBuffer, uint32_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

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
    this->_spi->Start();
    this->_spi->Write(W25X_PowerDown);
    this->_spi->Stop();
}

//唤醒
void W25Q64::WakeUp(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_ReleasePowerDown);
    this->_spi->Stop(); //等待TRES1
}

// 读取数据
bool W25Q64::Read(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumByteToRead)
{
    this->_spi->Start();
    this->_spi->Write(W25X_ReadData);
    this->_spi->Write((ReadAddr &0xFF0000) >> 16);
    this->_spi->Write((ReadAddr &0xFF00) >> 8);
    this->_spi->Write(ReadAddr &0xFF);

    for (int i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = this->_spi->Write(0xFF);
    }

    this->_spi->Stop();
    return true;
}
