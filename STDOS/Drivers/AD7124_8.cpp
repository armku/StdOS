#include "AD7124_8.h"
#include "AD7124def_8.h"

#if AD7124SPISOFT
    AD7124::AD7124(SpiSoft *spi)
    {
        this->pspi = spi;
    }
#else 
    AD7124::AD7124(Spi *spi)
    {
        this->pspi = spi;
    }
#endif 

ushort AD7124::Write16(ushort sendData)
{
    ushort ret = 0;
    ret = this->pspi->Write(sendData >> 8);
    ret <<= 8;
    ret += this->pspi->Write(sendData &0xff);
    return ret;

}


uint AD7124::Write24(uint sendData)
{
    uint ret = 0;
    ret = this->pspi->Write(sendData >> 16);
    ret <<= 8;
    ret += this->pspi->Write(sendData >> 8);
    ret <<= 8;
    ret += this->pspi->Write(sendData &0xff);
    return ret;

}

uint AD7124::Write32(uint sendData)
{
    uint ret = 0;
    ret = this->pspi->Write(sendData >> 24);
    ret <<= 8;
    ret += this->pspi->Write(sendData >> 16);
    ret <<= 8;
    ret += this->pspi->Write(sendData >> 8);
    ret <<= 8;
    ret += this->pspi->Write(sendData &0xff);
    return ret;
}

uint AD7124::ReadReg(byte reg, byte bytes)
{
    uint retVal;
    this->pspi->Start();
    this->pspi->Write(AD7124_RD | reg);
    if (bytes == 1)
    {
        retVal = this->pspi->Write(0xFF);
    }
    else if (bytes == 2)
    {
        retVal = Write16(0xFFFF);
    }
    else if (bytes == 3)
    {
        retVal = Write24(0xFFFFFF);
    }
    this->pspi->Stop();
    return retVal;
}
//��ȡADֵ
uint AD7124::ReadRlt()
{
	this->pspi->Stop();
	Sys.Delay(5);
	uint ret= this->ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
	Sys.Delay(5);
	this->pspi->Stop();
	this->pspi->Start();
	return ret;
}
//��ȡADֵ
uint AD7124::ReadRlt(byte& status)
{
	uint adin=this->ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES+1);
	status=adin&0xff;
	adin>>=8;
	return adin;
}
/*******************************************************************************
 * @function	: ReadReg
 * @brief		: ��ȡAD7124�Ĵ�������
 * @param		: reg   �Ĵ�����ַ
 * @param		: bytes �Ĵ�����С
 * @retval		: ��ȡ������
 * @notes		: 
 *****************************************************************************/
uint AD7124::ReadRegNoCS(byte reg, byte bytes)
{
    uint retVal;
    this->pspi->Write(AD7124_RD | reg);
    if (bytes == 1)
    {
        retVal = this->pspi->Write(0xFF);
    }
    else if (bytes == 2)
    {
        retVal = Write16(0xFFFF);
    }
    else if (bytes == 3)
    {
        retVal = Write24(0xFFFFFF);
    }
    return retVal;
}

/*******************************************************************************
 * @function	: WriteReg
 * @brief		: дAD7124�Ĵ���
 * @param		: reg	  �Ĵ�����ַ
 * @param		: bytes �Ĵ�����С
 * @param		: data	д�������
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
void AD7124::WriteReg(byte reg, byte bytes, uint data)
{
    this->pspi->Start();
    this->pspi->Write(AD7124_WR | reg);
    if (bytes == 1)
    {
        this->pspi->Write(data);
    }
    else if (bytes == 2)
    {
        Write16(data);
    }
    else if (bytes == 3)
    {
        Write24(data);
    }
    this->pspi->Stop();
	this->pspi->Start();
}

/*******************************************************************************
 * @function	: Init
 * @brief		: SPI�˿����ã�ģ��SPI
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
void AD7124::Init()
{
//    WriteReg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, AD7124_ADC_CTRL_REG_DATA);
//    WriteReg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, AD7124_IO_CTRL1_REG_DATA);
//    WriteReg(AD7124_IO_CTRL2_REG, AD7124_IO_CTRL2_REG_BYTES, AD7124_IO_CTRL2_REG_DATA);

//    WriteReg(AD7124_CFG0_REG, AD7124_CFG0_REG_BYTES, AD7124_CFG0_REG_DATA);
//    WriteReg(AD7124_CFG1_REG, AD7124_CFG1_REG_BYTES, AD7124_CFG1_REG_DATA);
//    WriteReg(AD7124_CFG2_REG, AD7124_CFG2_REG_BYTES, AD7124_CFG2_REG_DATA);
//    WriteReg(AD7124_CFG3_REG, AD7124_CFG3_REG_BYTES, AD7124_CFG3_REG_DATA);
//    WriteReg(AD7124_CFG4_REG, AD7124_CFG4_REG_BYTES, AD7124_CFG4_REG_DATA);

//    WriteReg(AD7124_FILT0_REG, AD7124_FILT0_REG_BYTES, AD7124_FILT0_REG_DATA);
//    WriteReg(AD7124_FILT1_REG, AD7124_FILT1_REG_BYTES, AD7124_FILT1_REG_DATA);
//    WriteReg(AD7124_FILT2_REG, AD7124_FILT2_REG_BYTES, AD7124_FILT2_REG_DATA);
//    WriteReg(AD7124_FILT3_REG, AD7124_FILT3_REG_BYTES, AD7124_FILT3_REG_DATA);
//    WriteReg(AD7124_FILT4_REG, AD7124_FILT4_REG_BYTES, AD7124_FILT4_REG_DATA);
//	
//    WriteReg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, AD7124_IO_CTRL1_REG_DATA_TEMP1_TEST);
//    AD7124_TEMP1_CH_CFG();
//    AD7124_TEMP1_RES_CH_CFG();
//    AD7124_FLOWA_CH_CFG();
//    AD7124_FLOWB_CH_CFG();
//    AD7124_BRDTEMP_CH_CFG();
//    AD7124_ADTEMP_CH_CFG();
	
	WriteReg(AD7124_CFG0_REG, 2, (1<<11)| (0 << 9) | (2 << 3)|(5<<0)); //˫���� ��·������Դ�ر�  ʹ���ڲ���׼��ѹԴ ����32������78.125mv�����ڲ��������ѹ
	WriteReg(AD7124_CFG1_REG, 2, (0<<11)| (0 << 9) | (2 << 3)|(5<<0)); //������ ��·������Դ�ر�  ʹ���ڲ���׼��ѹԴ ����32������78.125mv�����ڲ�����ü�ѹ
	WriteReg(AD7124_CFG1_REG, 2, (0<<11)| (0 << 9) | (2 << 3)|(0<<0)); //������ ��·������Դ�ر�  ʹ���ڲ���׼��ѹԴ ����1������2.5v�����ڲ��������¶�

	
	this->CHCONFIG[0]=	0x0000 |(0<<12)| (0 << 5) | (1 << 0);//ʹ�õ�һ������ AINP:0 AIN 1 �����ѹ
	this->CHCONFIG[1]=	0x0000 |(1<<12)| (2 << 5) | (3 << 0);//ʹ�õڶ������� AINP:2 AIN 3 ��ü�ѹ
	this->CHCONFIG[2]=	0x0000 |(0<<12)| (4 << 5) | (5 << 0);//ʹ�õ�һ������ AINP:4 AIN 5 С����ѹ
	this->CHCONFIG[3]=	0x0000 |(1<<12)| (6 << 5) | (7 << 0);//ʹ�õڶ������� AINP:6 AIN 7 С�ü�ѹ
	this->CHCONFIG[4]=	0x0000 |(1<<12)| (8 << 5) | (9 << 0);//ʹ�õڶ������� AINP:8 AIN 9 ����ѹ
	this->CHCONFIG[5]=	0x0000 |(1<<12)| (11 << 5) | (12 << 0);//ʹ�õڶ������� AINP:11 AIN 12 С�ü���
	this->CHCONFIG[6]=	0x0000 |(1<<12)| (14 << 5) | (19 << 0);//ʹ�õڶ������� AINP:14 AIN GND �����¶�
	
	
    WriteReg(AD7124_CH0_MAP_REG, 2, this->CHCONFIG[0]); 
	WriteReg(AD7124_CH1_MAP_REG, 2, this->CHCONFIG[1]); 
	WriteReg(AD7124_CH2_MAP_REG, 2, this->CHCONFIG[2]); 
	WriteReg(AD7124_CH3_MAP_REG, 2, this->CHCONFIG[3]); 
	WriteReg(AD7124_CH4_MAP_REG, 2, this->CHCONFIG[4]); 
	WriteReg(AD7124_CH5_MAP_REG, 2, this->CHCONFIG[5]); 
	WriteReg(AD7124_CH6_MAP_REG, 2, this->CHCONFIG[6]); 
	
//	WriteReg(AD7124_CH3_MAP_REG, 2, 0x0000 | (4 << 5) | (5 << 0)); //AINP:4 AIN 5 С����ѹ
//	WriteReg(AD7124_CH4_MAP_REG, 2, 0x0000 | (6 << 5) | (7 << 0)); //AINP:6 AIN 7 С�ü�ѹ
//	WriteReg(AD7124_CH5_MAP_REG, 2, 0x0000 | (8 << 5) | (9 << 0)); //AINP:8 AIN 9 ����ѹ
//	WriteReg(AD7124_CH6_MAP_REG, 2, 0x0000 | (11 << 5) | (12 << 0)); //AINP:11 AIN 12 С�ü���
//	WriteReg(AD7124_CH7_MAP_REG, 2, 0x8000 | (14 << 5) | (19 << 0)); //AINP:14 AIN DGND �����¶�
//	WriteReg(AD7124_CH8_MAP_REG, 2, 0x0000 | (0 << 5) | (1 << 0)); //AINP:0 AIN 1
//	WriteReg(AD7124_CH0_MAP_REG, 2, 0x0000 | (0 << 5) | (1 << 0)); //AINP:0 AIN 1
//	WriteReg(AD7124_CH10_MAP_REG, 2, 0x0000 | (0 << 5) | (1 << 0)); //AINP:0 AIN 1
//	WriteReg(AD7124_CH11_MAP_REG, 2, 0x0000 | (0 << 5) | (1 << 0)); //AINP:0 AIN 1
//	WriteReg(AD7124_CH12_MAP_REG, 2, 0x0000 | (0 << 5) | (1 << 0)); //AINP:0 AIN 1
//	WriteReg(AD7124_CH13_MAP_REG, 2, 0x0000 | (0 << 5) | (1 << 0)); //AINP:0 AIN 1
//	WriteReg(AD7124_CH14_MAP_REG, 2, 0x0000 | (0 << 5) | (1 << 0)); //AINP:0 AIN 1
//	WriteReg(AD7124_CH15_MAP_REG, 2, 0x0000 | (0 << 5) | (1 << 0)); //AINP:2 AIN 3

    WriteReg(AD7124_ADC_CTRL_REG, 2, (1<<10)|(1<<8)|(3<<6)); //״̬�Ĵ�������ʹ�� �ڲ���׼��ѹʹ�� ȫ����ģʽ
	
    this->pspi->Start();
}

/*******************************************************************************
 * @function	: ReadID
 * @brief		: ��ȡAD7124 ID�Ĵ���
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
byte AD7124::ReadID()
{
    byte retVal;
    retVal = ReadReg(AD7124_ID_REG, AD7124_ID_REG_BYTES);
    //debug_printf("ID:0x%02x\r\n", retVal);
    return retVal;
}

/*******************************************************************************
 * @function	: ReadStatus
 * @brief		: ��ȡAD7124 ״̬�Ĵ���
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
byte AD7124::ReadStatus()
{
    byte retVal;
    retVal = ReadReg(AD7124_STATUS_REG, AD7124_STATUS_REG_BYTES);
    //debug_printf("Status:0x%02x\r\n", retVal);
	this->pspi->Start();
    return retVal;
}

/*******************************************************************************
 * @function	: Reset
 * @brief		: ��λADоƬ
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
void AD7124::Reset()
{
    this->pspi->Start();
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Write(0xFF);
    this->pspi->Stop();
}
