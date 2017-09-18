#include "AD7124.h"
#include "AD7124def.h"

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

ushort AD7124::spi16_run(ushort sendData)
{
    ushort ret = 0;
    ret = this->pspi->Write(sendData >> 8);
    ret <<= 8;
    ret += this->pspi->Write(sendData &0xff);
    return ret;

}


uint AD7124::spi24_run(uint sendData)
{
    uint ret = 0;
    ret = this->pspi->Write(sendData >> 16);
    ret <<= 8;
    ret += this->pspi->Write(sendData >> 8);
    ret <<= 8;
    ret += this->pspi->Write(sendData &0xff);
    return ret;

}

uint AD7124::spi32_run(uint sendData)
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

uint AD7124::AD7124_Read_Reg(byte reg, byte bytes)
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
        retVal = spi16_run(0xFFFF);
    }
    else if (bytes == 3)
    {
        retVal = spi24_run(0xFFFFFF);
    }
    this->pspi->Stop();
    return retVal;
}

/*******************************************************************************
 * @function	: AD7124_Read_Reg
 * @brief		: 读取AD7124寄存器内容
 * @param		: reg   寄存器地址
 * @param		: bytes 寄存器大小
 * @retval		: 读取的内容
 * @notes		: 
 *****************************************************************************/
uint AD7124::AD7124_Read_Reg_NoCS(byte reg, byte bytes)
{
    uint retVal;
    this->pspi->Write(AD7124_RD | reg);
    if (bytes == 1)
    {
        retVal = this->pspi->Write(0xFF);
    }
    else if (bytes == 2)
    {
        retVal = spi16_run(0xFFFF);
    }
    else if (bytes == 3)
    {
        retVal = spi24_run(0xFFFFFF);
    }
    return retVal;
}

/*******************************************************************************
 * @function	: AD7124_Write_Reg
 * @brief		: 写AD7124寄存器
 * @param		: reg	  寄存器地址
 * @param		: bytes 寄存器大小
 * @param		: data	写入的数据
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
void AD7124::AD7124_Write_Reg(byte reg, byte bytes, uint data)
{
    this->pspi->Start();
    this->pspi->Write(AD7124_WR | reg);
    if (bytes == 1)
    {
        this->pspi->Write(data);
    }
    else if (bytes == 2)
    {
        spi16_run(data);
    }
    else if (bytes == 3)
    {
        spi24_run(data);
    }
    this->pspi->Stop();
}

void AD7124::AD7124_Config(void)
{
    AD7124_Write_Reg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, AD7124_ADC_CTRL_REG_DATA);
    AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, AD7124_IO_CTRL1_REG_DATA);
    AD7124_Write_Reg(AD7124_IO_CTRL2_REG, AD7124_IO_CTRL2_REG_BYTES, AD7124_IO_CTRL2_REG_DATA);

    AD7124_Write_Reg(AD7124_CFG0_REG, AD7124_CFG0_REG_BYTES, AD7124_CFG0_REG_DATA);
    AD7124_Write_Reg(AD7124_CFG1_REG, AD7124_CFG1_REG_BYTES, AD7124_CFG1_REG_DATA);
    AD7124_Write_Reg(AD7124_CFG2_REG, AD7124_CFG2_REG_BYTES, AD7124_CFG2_REG_DATA);
    AD7124_Write_Reg(AD7124_CFG3_REG, AD7124_CFG3_REG_BYTES, AD7124_CFG3_REG_DATA);
    AD7124_Write_Reg(AD7124_CFG4_REG, AD7124_CFG4_REG_BYTES, AD7124_CFG4_REG_DATA);

    AD7124_Write_Reg(AD7124_FILT0_REG, AD7124_FILT0_REG_BYTES, AD7124_FILT0_REG_DATA);
    AD7124_Write_Reg(AD7124_FILT1_REG, AD7124_FILT1_REG_BYTES, AD7124_FILT1_REG_DATA);
    AD7124_Write_Reg(AD7124_FILT2_REG, AD7124_FILT2_REG_BYTES, AD7124_FILT2_REG_DATA);
    AD7124_Write_Reg(AD7124_FILT3_REG, AD7124_FILT3_REG_BYTES, AD7124_FILT3_REG_DATA);
    AD7124_Write_Reg(AD7124_FILT4_REG, AD7124_FILT4_REG_BYTES, AD7124_FILT4_REG_DATA);
}

/*******************************************************************************
 * @function	: AD7124_Init
 * @brief		: SPI端口配置，模拟SPI
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
void AD7124::AD7124_Init(void)
{
    AD7124_Config(); //ADC 配置
    AD7124_Channel_Config(); //
    this->pspi->Start();
}

/*******************************************************************************
 * @function	: AD7124_Channel_Config
 * @brief		:	ADC通道配置
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
void AD7124::AD7124_Channel_Config(void)
{
    AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, AD7124_IO_CTRL1_REG_DATA_TEMP1_TEST);
    AD7124_TEMP1_CH_CFG();
    AD7124_TEMP1_RES_CH_CFG();
    AD7124_FLOWA_CH_CFG();
    AD7124_FLOWB_CH_CFG();
    AD7124_BRDTEMP_CH_CFG();
    AD7124_ADTEMP_CH_CFG();
}

/*******************************************************************************
 * @function	: AD7124_Read_ID
 * @brief		: 读取AD7124 ID寄存器
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
byte AD7124::AD7124_Read_ID(void)
{
    byte retVal;
    retVal = AD7124_Read_Reg(AD7124_ID_REG, AD7124_ID_REG_BYTES);
    printf("ID:0x%02x\r\n", retVal);
    return retVal;
}

/*******************************************************************************
 * @function	: AD7124_Read_Status
 * @brief		: 读取AD7124 状态寄存器
 * @param		: 无
 * @retval		: 无
 * @notes		: 
 *****************************************************************************/
byte AD7124::AD7124_Read_Status(void)
{
    byte retVal;
    retVal = AD7124_Read_Reg(AD7124_STATUS_REG, AD7124_STATUS_REG_BYTES);
    printf("Status:0x%02x\r\n", retVal);
    return retVal;
}

/*******************************************************************************
 * @function	: AD7124_Get_Temp1
 * @brief		: 获取温度1
 * @param		: 无
 * @retval		: 返回温度值
 * @notes		: 
 *****************************************************************************/
float AD7124::AD7124_Get_Temp1(void)
{
    uint data;
    float res;
    float temp;
    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
    printf("data:0x%08x\n", data);
    res = (5110.0 *data) / (16777215.0 *16.0);
    //	fdata = ((data - 8388608)/13584) - 272.5;
    printf("R:%0.4f\n\r", res);
    //	temp = PT100_RtoT(res);
    //	printf("T:%0.4f\n",temp);
    return temp;
}

/*******************************************************************************
 * @function	: AD7124_Get_Temp1
 * @brief		: 获取温度1线路电阻
 * @param		: 无
 * @retval		: 返回温度值
 * @notes		: 
 *****************************************************************************/
float AD7124::AD7124_Get_Temp1_Res(void)
{
    uint data;
    float res;
    float temp;
    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
    printf("data:0x%08x\n\r", data);
    res = (5110.0 *data) / (16777215.0 *16.0);
    //	printf("R:%0.4f\n",res);
    //	temp = PT100_RtoT(res);
    printf("T:%0.4f\n\r", temp);
    return temp;
}

/*******************************************************************************
 * @function	: AD7124_Reset
 * @brief		: 获取温度1线路电阻
 * @param		: 无
 * @retval		: 返回温度值
 * @notes		: 
 *****************************************************************************/
void AD7124::AD7124_Reset(void)
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

float AD7124::AD7124_Temp1_Test(void)
{
    uint data;
    float pt100_res;
    float wire_res;
    float temp;

    //	AD7124_Write_Reg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
    Sys.Sleep(50);
    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    printf("data:0x%08x\n", data);
    pt100_res = (5110.0 *data) / (16777215.0 *16.0);
    printf("R:%0.4f\n", pt100_res);
    //	temp = PT100_RtoT(pt100_res);
    //	printf("T:%0.4f\n",temp);

    //	AD7124_Write_Reg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
    Sys.Sleep(50);
    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    printf("data:0x%08x\n", data);
    wire_res = (2.5 *data) / (16777215 *128 * 0.001);
    printf("R:%0.4f\n", wire_res);

    pt100_res = pt100_res - wire_res;
    //	temp = PT100_RtoT(pt100_res);
    //	printf("T:%0.4f\n",temp);

    return temp;
}
#if 0
SpiSoft spi1(CPOL_High, CPHA_1Edge);
AD7124 ad(&spi1);

void ad7124test(void *param)
{
	auto ad7 = (AD7124*)param;
	
    ad7->AD7124_Read_ID();
    ad7->AD7124_Read_Status();
	   
	auto data = ad7->AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    printf("data:0x%08x\r\n", data);
    auto fdata = (2.5 *data) / (16777215 *128 * 0.001);
    printf("R:%0.4f\r\n", fdata);
}

void ad71248Test()
{
	spi1.SetPin(PE4, PE5, PE6);
    spi1.SetNss(PE2);
    ad.AD7124_Init();

    Sys.AddTask(ad7124test, &ad, 1000, 2000, "7124-8test");
}
#endif
