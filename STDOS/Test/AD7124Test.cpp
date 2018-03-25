#include "Drivers\AD7124.h"
#include "Drivers\AD7124def.h"

//#define _AD7124_TEST_H
#ifdef _AD7124_TEST_H
/*******************************************************************************
 * @function	: GetTemp1
 * @brief		: 获取温度1
 * @param		: 无
 * @retval		: 返回温度值
 * @notes		: 
 *****************************************************************************/
float AD7124::GetTemp1()
{
    uint32_t data;
    float res;
    float temp;
    data = ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
    debug_printf("data:0x%08x\n", data);
    res = (5110.0 *data) / (16777215.0 *16.0);
    //	fdata = ((data - 8388608)/13584) - 272.5;
    debug_printf("R:%0.4f\n\r", res);
    //	temp = PT100_RtoT(res);
    //	debug_printf("T:%0.4f\n",temp);
    return temp;
}

/*******************************************************************************
 * @function	: GetTemp1Res
 * @brief		: 获取温度1线路电阻
 * @param		: 无
 * @retval		: 返回温度值
 * @notes		: 
 *****************************************************************************/
float AD7124::GetTemp1Res()
{
    uint32_t data;
//    float res;
    float temp;
    data = ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
    debug_printf("data:0x%08x\n\r", data);
//    res = (5110.0 *data) / (16777215.0 *16.0);
    //	debug_printf("R:%0.4f\n",res);
    //	temp = PT100_RtoT(res);
    debug_printf("T:%0.4f\n\r", temp);
    return temp;
}

float AD7124::Temp1Test()
{
    uint32_t data;
    float pt100_res;
    float wire_res;
    float temp;

    //	WriteReg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
    Sys.Sleep(50);
    data = ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    debug_printf("data:0x%08x\n", data);
    pt100_res = (5110.0 *data) / (16777215.0 *16.0);
    debug_printf("R:%0.4f\n", pt100_res);
    //	temp = PT100_RtoT(pt100_res);
    //	debug_printf("T:%0.4f\n",temp);

    //	WriteReg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
    Sys.Sleep(50);
    data = ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    debug_printf("data:0x%08x\n", data);
    wire_res = (2.5 *data) / (16777215 *128 * 0.001);
    debug_printf("R:%0.4f\n", wire_res);

    pt100_res = pt100_res - wire_res;
    //	temp = PT100_RtoT(pt100_res);
    //	debug_printf("T:%0.4f\n",temp);

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
	   
	auto data = ad7->ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    debug_printf("data:0x%08x\r\n", data);
    auto fdata = (2.5 *data) / (16777215 *128 * 0.001);
    debug_printf("R:%0.4f\r\n", fdata);
}

void ad71248Test()
{
	spi1.SetPin(PE4, PE5, PE6);
    spi1.SetNss(PE2);
    ad.AD7124_Init();

    Sys.AddTask(ad7124test, &ad, 1000, 2000, "7124-8test");
}
#endif
#endif
