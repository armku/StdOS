#include "W25QXXX.h"

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
uint W25Q64::ReadID()
{
    uint Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

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

uint W25Q64::ReadDeviceID(void)
{
    uint Temp = 0;

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
bool W25Q64::EraseSector(uint SectorAddr)
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
bool W25Q64::ErasePage(uint pageAddr)
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
    byte FLASH_Status = 0;
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

bool W25Q64::WritePage(uint addr, byte *pBuffer, uint NumByteToWrite)
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
        //printf("\n\r Err: this->PageWrite too large!");
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
bool W25Q64::Read(uint ReadAddr, byte *pBuffer, uint NumByteToRead)
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

#if 0    

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
    #if W25QXXXUSESPISOFT
        SpiSoft spi64;
    #else 
        Spi spi64(Spi1);
    #endif 
    W25Q64 w25q64(&spi64);
    void W25Q64Test()
    {
        TestStatus TransferStatus1 = FAILED;

        printf("\r\n ����һ��8Mbyte����flash(W25Q64)ʵ�� \r\n");
        #if W25QXXXUSESPISOFT
            spi64.SetPin(PA5, PA6, PA7);
            spi64.CPOL = CPOL_High;
            spi64.CPHA = CPHA_2Edge;
        #else 
        #endif 
		spi64.SetNss(PA4);
        spi64.Open();

        /* Get SPI Flash Device ID */
        w25q64.DeviceID = w25q64.ReadDeviceID();
        //		return;
        Sys.Delay(10);

        printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", w25q64.ID, w25q64.DeviceID);

        /* Check the SPI Flash ID */
        if (w25q64.ID == sFLASH_ID)
        /* #define  sFLASH_ID  0XEF4017 */
        {
            printf("\r\n ��⵽�����flash W25Q64 !\r\n");

            /* Erase FLASH Sector to write on */
            w25q64.EraseSector(FLASH_SectorToErase);

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

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
uint W25Q128::ReadID()
{
    ushort Temp = 0;
    //ʹ������   
    this->_spi->Start();
    this->_spi->Write(W25X_ManufactDeviceID); //���Ͷ�ȡID����	    
    this->_spi->Write(0x00);
    this->_spi->Write(0x00);
    this->_spi->Write(0x00);
    Temp |= this->_spi->Write(0xFF) << 8;
    Temp |= this->_spi->Write(0xFF);
    //ȡ��Ƭѡ     
    this->_spi->Stop();

    return Temp;
}

#if W25QXXXUSESPISOFT
    W25Q128::W25Q128(SpiSoft *spi): W25Q64(spi)
#else 
    W25Q128::W25Q128(Spi *spi): W25Q64(spi)
#endif 
{
    this->PageSize = 4096;
}

// ��������
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
bool W25Q128::EraseSector(uint sectorAddr)
{
    //����falsh�������,������   
    printf("fe:%x\r\n", sectorAddr);
    sectorAddr *= 4096;
    this->WriteEnable(); //SET WEL 	 
    this->WaitForEnd();
    //ʹ������   
    this->_spi->Start();
    this->_spi->Write(W25X_SectorErase); //������������ָ�� 
    this->_spi->Write((byte)((sectorAddr) >> 16)); //����24bit��ַ    
    this->_spi->Write((byte)((sectorAddr) >> 8));
    this->_spi->Write((byte)sectorAddr);
    //ȡ��Ƭѡ     
    this->_spi->Stop();
    this->WaitForEnd(); //�ȴ��������

    return true;
}

// д��һҳ
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 

bool W25Q128::WritePage(uint addr, byte *buf, uint count)
{
    ushort i;
    this->WriteEnable(); //SET WEL 
    //ʹ������   
    this->_spi->Start();
    this->_spi->Write(W25X_PageProgram); //����дҳ����   
    this->_spi->Write((byte)((addr) >> 16)); //����24bit��ַ    
    this->_spi->Write((byte)((addr) >> 8));
    this->_spi->Write((byte)addr);
    for (i = 0; i < count; i++)
    {
        this->_spi->Write(buf[i]);
    }
    //ѭ��д��  
    //ȡ��Ƭѡ     
    this->_spi->Stop();
    this->WaitForEnd(); //�ȴ�д�����
    return true;
}

//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)
// д������
bool W25Q128::Write(uint WriteAddr, byte *pBuffer, uint NumByteToWrite)
{
    uint secpos;
    ushort secoff;
    ushort secremain;
    ushort i;
    byte *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; //������ַ  
    secoff = WriteAddr % 4096; //�������ڵ�ƫ��
    secremain = 4096-secoff; //����ʣ��ռ��С   
    //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite;
    //������4096���ֽ�
    while (1)
    {
        this->Read(secpos *4096, W25QXX_BUF, 4096); //������������������
        for (i = 0; i < secremain; i++)
        //У������
        {
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break;
            //��Ҫ����  	  
        }
        if (i < secremain)
        //��Ҫ����
        {
            this->EraseSector(secpos); //�����������
            for (i = 0; i < secremain; i++)
            //����
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            this->W25QXX_Write_NoCheck(W25QXX_BUF, secpos *4096, 4096); //д����������  

        }
        else
            this->W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);
        //д�Ѿ������˵�,ֱ��д������ʣ������. 				   
        if (NumByteToWrite == secremain)
            break;
        //д�������
        else
        //д��δ����
        {
            secpos++; //������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0 	 

            pBuffer += secremain; //ָ��ƫ��
            WriteAddr += secremain; //д��ַƫ��	   
            NumByteToWrite -= secremain; //�ֽ����ݼ�
            if (NumByteToWrite > 4096)
                secremain = 4096;
            //��һ����������д����
            else
                secremain = NumByteToWrite;
            //��һ����������д����
        }
    };
    return true;
}

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 
void W25Q128::W25QXX_Init(void)
{

    this->_spi->Stop();

    //	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
    //    spi.Write(0xff); //��������
    W25QXX_TYPE = this->ReadID(); //��ȡFLASH ID.
}

//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25Q128::W25QXX_Write_SR(byte sr)
{
    //ʹ������   
    this->_spi->Start();
    this->_spi->Write(W25X_WriteStatusReg); //����дȡ״̬�Ĵ�������    
    this->_spi->Write(sr); //д��һ���ֽ�  
    //ȡ��Ƭѡ     
    this->_spi->Stop();

}

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25Q128::W25QXX_Write_NoCheck(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
    ushort pageremain;
    pageremain = 256-WriteAddr % 256; //��ҳʣ����ֽ���		 	    
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite;
    //������256���ֽ�
    while (1)
    {
        this->WritePage(WriteAddr, pBuffer, pageremain);
        if (NumByteToWrite == pageremain)
            break;
        //д�������
        else
        //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain; //��ȥ�Ѿ�д���˵��ֽ���
            if (NumByteToWrite > 256)
                pageremain = 256;
            //һ�ο���д��256���ֽ�
            else
                pageremain = NumByteToWrite;
            //����256���ֽ���
        }
    };
}

#if 0
	//����ԭ�� ̽���߿�����
	#if W25QXXXUSESPISOFT
        SpiSoft spi128;
    #else 
        Spi spi128(Spi1);
    #endif     
    W25Q128 w25q128(&spi128);
    OutputPort nsspp;

    //Ҫд�뵽W25Q16���ַ�������
    const byte TEXT_Buffer[] = 
    {
        "Explorer STM32F4 SPI TEST"
    };

    void w25q128test()
    {
        nsspp.Invert = false;
        nsspp.OpenDrain = false;
        nsspp.Set(PG7);
        nsspp = 1; //PG7���1,��ֹNRF����SPI FLASH��ͨ�� 

		#if W25QXXXUSESPISOFT
			spi128.SetPin(PG6,PG7,PG8);//PB3, PB4, PB5);
            spi128.CPOL = CPOL_High;
            spi128.CPHA = CPHA_2Edge;
		#endif
		spi128.SetNss(PB14);
        byte datatemp[sizeof(TEXT_Buffer)];
        uint FLASH_SIZE;
        w25q128.W25QXX_Init(); //W25QXX��ʼ��
        printf("\r\nSPI TEST\r\n");
        while (w25q128.ReadID() != W25QXXX128)
        //��ⲻ��W25Q128
        {
            printf("W25Q128 Check Failed!\r\n");
			return;
        }
        printf("W25Q128 Ready!\r\n");
        FLASH_SIZE = 16 * 1024 * 1024; //FLASH ��СΪ16�ֽ�
        printf("\r\n");

        printf("Start Write W25Q128....\r\n");
        w25q128.Write(FLASH_SIZE - 100, (byte*)TEXT_Buffer, sizeof(TEXT_Buffer)); //�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
        printf("W25Q128 Write Finished!\r\n"); //��ʾ�������
        printf("\r\n");

        printf("Start Read W25Q128....\r\n");
        w25q128.Read(FLASH_SIZE - 100, datatemp, sizeof(TEXT_Buffer)); //�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
        printf("The Data Readed Is:\r\n"); //��ʾ�������
        printf((const char*)datatemp);

        printf("\r\n\r\n");
    }
#endif
