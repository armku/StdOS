#include "W25QXXX.h"
#include "stm32f10x.h"

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

// ���ò�����ַ
void W25Q64::SetAddr(uint addr){

}

// ��ȡ���
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
// ��ȡһҳ
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

// ��������
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
// ����ҳ
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
// �ȴ��������
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
    while ((FLASH_Status &WIP_Flag) == SET); /* Write in progress */

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
// д��һҳ
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
// д������
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

//�������ģʽ
void W25Q64::PowerDown(void)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Power Down" instruction */
    this->_spi->Write(W25X_PowerDown);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
}

//����
void W25Q64::WakeUp(void)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Power Down" instruction */
    this->_spi->Write(W25X_ReleasePowerDown);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop(); //�ȴ�TRES1
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
// ��ȡ����
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

#if 1
    Spi spi(Spi1);
    W25Q64 w25q64(&spi);

    typedef enum
    {
        FAILED = 0, PASSED = !FAILED
    } TestStatus;


    /* ��ȡ�������ĳ��� */
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


    /* ���ͻ�������ʼ�� */
    byte Tx_Buffer[] = " ��л��ѡ��Ұ��stm32������\r\n                http://firestm32.taobao.com";
    byte Rx_Buffer[BufferSize];

    /*
     * ��������Buffercmp
     * ����  ���Ƚ������������е������Ƿ����
     * ����  ��-pBuffer1     src������ָ��
     *         -pBuffer2     dst������ָ��
     *         -BufferLength ����������
     * ���  ����
     * ����  ��-PASSED pBuffer1 ����   pBuffer2
     *         -FAILED pBuffer1 ��ͬ�� pBuffer2
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
        __IO TestStatus TransferStatus1 = FAILED;

        printf("\r\n ����һ��8Mbyte����flash(W25Q64)ʵ�� \r\n");

        spi.Open();
        /* Get SPI Flash Device ID */
        w25q64.DeviceID = w25q64.ReadDeviceID();

        Sys.Delay(10);

        printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", w25q64.ID, w25q64.DeviceID);

        /* Check the SPI Flash ID */
        if (w25q64.ID == sFLASH_ID)
        /* #define  sFLASH_ID  0XEF4017 */
        {
            printf("\r\n ��⵽�����flash W25Q64 !\r\n");

            /* Erase FLASH Sector to write on */
            w25q64.Erase(FLASH_SectorToErase);

            /* �����ͻ�����������д��flash�� */
            w25q64.Write(FLASH_WriteAddress, Tx_Buffer, BufferSize);
            w25q64.Write(252, Tx_Buffer, BufferSize);
            printf("\r\n д�������Ϊ��%s \r\t", Tx_Buffer);

            /* ���ո�д������ݶ������ŵ����ջ������� */
            w25q64.Read(FLASH_ReadAddress, Rx_Buffer, BufferSize);
            printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);

            /* ���д�������������������Ƿ���� */
            TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

            if (PASSED == TransferStatus1)
            {
                printf("\r\n 8M����flash(W25Q64)���Գɹ�!\n\r");
            }
            else
            {
                printf("\r\n 8M����flash(W25Q64)����ʧ��!\n\r");
            }
        } // if (FlashID == sFLASH_ID)
        else
        {
            printf("\r\n ��ȡ���� W25Q64 ID!\n\r");
        }

        w25q64.PowerDown();
        printf("\r\n\n\r");
    }
#endif
