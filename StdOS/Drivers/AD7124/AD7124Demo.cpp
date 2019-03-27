#include "AD7124.h"
#include "AD7124def.h"
#include "Sys.h"

#define _AD7124_TEST_CPP
#ifdef _AD7124_TEST_CPP
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
    delay(50);
    data = ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    debug_printf("data:0x%08x\n", data);
    pt100_res = (5110.0 *data) / (16777215.0 *16.0);
    debug_printf("R:%0.4f\n", pt100_res);
    //	temp = PT100_RtoT(pt100_res);
    //	debug_printf("T:%0.4f\n",temp);

    //	WriteReg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
    delay(50);
    data = ReadReg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    debug_printf("data:0x%08x\n", data);
    wire_res = (2.5 *data) / (16777215 *128 * 0.001);
    debug_printf("R:%0.4f\n", wire_res);

    pt100_res = pt100_res - wire_res;
    //	temp = PT100_RtoT(pt100_res);
    //	debug_printf("T:%0.4f\n",temp);

    return temp;
}

#if 1
SpiSoft spi1(CPOL_High, CPHA_1Edge);
AD7124 ad(&spi1);

void ad7124test(void *param)
{
	AD7124* ad7 = (AD7124*)param;
	
  uint8_t id=  ad7->ReadID();
 // uint8_t status=   ad7->ReadStatus();
	//   
	////uint32_t data = ad7->ReadRlt(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
	//uint32_t data = ad7->ReadRlt();

    debug_printf("id:%d\r\n", id);
	//debug_printf("data:0x%08x id:%d status:%d\r\n", data, id, status);
    /*double fdata = (2.5 *data) / (16777215 *128 * 0.001);
    debug_printf("R:%0.4f\r\n", fdata);*/
}
//Port cs;
//Port sck;
//Port din;
//Port dout;
void ad71248Test()
{
	spi1.SetPin(PC1, PC2, PC3);
    spi1.SetNss(PC0);
	spi1.CPHA = CPHA_1Edge;
	spi1.CPOL = CPOL_High;
	spi1.CPOL = CPOL_Low;
	spi1.Open();
    ad.Init_8();

	/*cs.OpenDrain = false;
	cs.Set(PC0);
	cs.Open();*/

	//sck.OpenDrain = false;
	//sck.Set(PC1);
	//sck.Open();

	//din.OpenDrain = false;
	//din.Set(PC2);
	//din.Open();

	//dout.OpenDrain = false;
	//dout.Set(PC3);
	//dout.Open();


	//cs = 0;
	//sck = 0;
	//din = 0;
	//dout = 0;
	
    Sys.AddTask(ad7124test, &ad, 1000, 1000, "7124-8test");
}
#endif
#endif
