#include "W25QXXX.h"

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	 

/* Private define ------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////// 
//ָ���
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

// ��ȡ���
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

// ��������
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

// ����ҳ
bool W25Q64::ErasePage(uint32_t pageAddr)
{
    return this->EraseSector(pageAddr);
}

// �ȴ��������
//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00

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

//д����
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

// д������
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

//�������ģʽ
void W25Q64::PowerDown(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_PowerDown);
    this->_spi->Stop();
}

//����
void W25Q64::WakeUp(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_ReleasePowerDown);
    this->_spi->Stop(); //�ȴ�TRES1
}

// ��ȡ����
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
