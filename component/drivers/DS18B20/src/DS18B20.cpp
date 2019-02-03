#include "DS18B20.h"
#include "../../../../StdOS/Kernel/Sys.h"

void DS18B20::SetPin(Pin pin)
{
	this->_dio.Set(pin);
	
	this->_dio.OpenDrain=true;
	
	this->_dio.Invert=0;
	
	this->_dio.Open();
	this->_dio = 1;
}

/*
 *主机给从机发送复位脉冲
 */
void DS18B20::Rest()
{
    this->_dio=0;
    /* 主机至少产生480us的低电平复位信号 */
    delay_us(480);
    /* 主机在产生复位信号后，需将总线拉高 */
	this->_dio=1;
}

/*
 * 检测从机给主机返回的存在脉冲
 * 0：成功
 * 1：失败
 */
bool DS18B20::Presence()
{
    uint8_t pulse_time = 0;
    /* 主机设置为上拉输入 */
    /* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
     * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
     */
	this->_dio=1;
    while (this->_dio && pulse_time < 100)
    {
        pulse_time++;
        delay_us(1);
    }
    /* 经过100us后，存在脉冲都还没有到来*/
    if (pulse_time >= 100)
        return false;
    else
        pulse_time = 0;

    /* 存在脉冲到来，且存在的时间不能超过240us */
    while (!this->_dio && pulse_time < 240)
    {
        pulse_time++;
        delay_us(1);
    }
    if (pulse_time >= 240)
        return false;
    else
        return true;
}

/*
 * 从DS18B20读取一个bit
 */
uint8_t DS18B20::ReadBit()
{
    uint8_t dat;

    /* 读0和读1的时间至少要大于60us */
    /* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
    this->_dio=0;
	delay_us(10);

    /* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
    //delay_us(2);
	this->_dio=1;

    if (this->_dio)
        dat = 1;
    else
        dat = 0;
    /* 这个延时参数请参考时序图 */
    delay_us(45);

    return dat;
}

/*
 * 从DS18B20读一个字节，低位先行
 */
uint8_t DS18B20::ReadByte()
{
    uint8_t i, j, dat = 0;

    for (i = 0; i < 8; i++)
    {
        j = this->ReadBit();
        dat = (dat) | (j << i);
    }

    return dat;
}

/*
 * 写一个字节到DS18B20，低位先行
 */
void DS18B20::WriteByte(uint8_t dat)
{
    uint8_t i, testb;

    for (i = 0; i < 8; i++)
    {
        testb = dat &0x01;
        dat = dat >> 1;
        /* 写0和写1的时间至少要大于60us */
        if (testb)
        {
            this->_dio=0;
            /* 1us < 这个延时 < 15us */
            delay_us(8);

            this->_dio=1;
            delay_us(58);
        }
        else
        {
            this->_dio=0;
            /* 60us < Tx 0 < 120us */
            delay_us(70);

            this->_dio=1;
            /* 1us < Trec(恢复时间) < 无穷大*/
            delay_us(2);
        }
    }
}

void DS18B20::Start()
{
    this->Rest();
    this->Presence();
    this->WriteByte(0XCC); /* 跳过 ROM */
    this->WriteByte(0X44); /* 开始转换 */
}

uint8_t DS18B20::Init()
{
    this->Rest();
    return 0;
}
/**
 * @brief  在匹配 ROM 情况下获取 DS18B20 温度值 
 * @param  ds18b20_id：用于存放 DS18B20 序列号的数组的首地址
 * @retval 无
 */
void DS18B20::ReadId()
{  
  this->WriteByte(0x33); //读取序列号

  for (int uc = 0; uc < 8; uc++)
    this->id[uc] = this->ReadByte();

}
void DS18B20::SkipRom()
{
  this->Rest();

  this->Presence();

  this->WriteByte(0XCC); /* 跳过 ROM */

}
/*
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */
float DS18B20::GetTemp()
{
    uint8_t tpmsb, tplsb;
    short s_tem;
    float f_tem;

	this->SkipRom();
    this->WriteByte(0X44); /* 开始转换 */

    this->SkipRom();
#if 1  
    this->WriteByte(0XBE); /* 读温度值 */

    tplsb = this->ReadByte();
    tpmsb = this->ReadByte();
#endif
    s_tem = tpmsb << 8;
    s_tem = s_tem | tplsb;

    if (s_tem < 0)
    /* 负温度 */
        f_tem = (~s_tem + 1) *0.0625;
    else
        f_tem = s_tem * 0.0625;

    return f_tem;
}
