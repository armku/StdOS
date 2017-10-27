#include "DS18B20.h"
#include "stm32f10x.h"
#include "Sys.h"

#define HIGH  1
#define LOW   0

#define DS18B20_CLK     RCC_APB2Periph_GPIOB
#define DS18B20_PIN     GPIO_Pin_10                  
#define DS18B20_PORT		GPIOB 

//读取引脚的电平
byte DS18B20::DS18B20_DATA_IN()
{
	return this->_dio;
	//return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10);
}

void DS18B20::SetPin(Pin pin)
{
	this->_dio.Set(pin);
	
	this->_dio.OpenDrain=true;
	
	this->_dio.Invert=0;
	
	this->_dio.Open();
}

typedef struct
{
    byte humi_int; //湿度的整数部分
    byte humi_deci; //湿度的小数部分
    byte temp_int; //温度的整数部分
    byte temp_deci; //温度的小数部分
    byte check_sum; //校验和

} DS18B20_Data_TypeDef;

/*
 * 函数名：DS18B20_Mode_IPU
 * 描述  ：使DS18B20-DATA引脚变为输入模式
 * 输入  ：无
 * 输出  ：无
 */
void DS18B20_Mode_IPU()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*选择要控制的DS18B20_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    /*设置引脚模式为浮空输入模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    /*调用库函数，初始化DS18B20_PORT*/
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}


/*
 * 函数名：DS18B20_Mode_Out_PP
 * 描述  ：使DS18B20-DATA引脚变为输出模式
 * 输入  ：无
 * 输出  ：无
 */
void DS18B20::DS18B20_Mode_Out_PP()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*选择要控制的DS18B20_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*调用库函数，初始化DS18B20_PORT*/
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 *主机给从机发送复位脉冲
 */
void DS18B20::DS18B20_Rst()
{
    /* 主机设置为推挽输出 */
    DS18B20_Mode_Out_PP();
	this->_dio=0;
    //DS18B20_DATA_OUT(LOW);
    /* 主机至少产生480us的低电平复位信号 */
    Sys.Delay(750);
    /* 主机在产生复位信号后，需将总线拉高 */
	this->_dio=1;
    //DS18B20_DATA_OUT(HIGH);
    /*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
    Sys.Delay(15);
}

/*
 * 检测从机给主机返回的存在脉冲
 * 0：成功
 * 1：失败
 */
byte DS18B20::DS18B20_Presence()
{
    byte pulse_time = 0;
    /* 主机设置为上拉输入 */
    DS18B20_Mode_IPU();
    /* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
     * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
     */
    while (DS18B20_DATA_IN() && pulse_time < 100)
    {
        pulse_time++;
        Sys.Delay(1);
    }
    /* 经过100us后，存在脉冲都还没有到来*/
    if (pulse_time >= 100)
        return 1;
    else
        pulse_time = 0;

    /* 存在脉冲到来，且存在的时间不能超过240us */
    while (!DS18B20_DATA_IN() && pulse_time < 240)
    {
        pulse_time++;
        Sys.Delay(1);
    }
    if (pulse_time >= 240)
        return 1;
    else
        return 0;
}

/*
 * 从DS18B20读取一个bit
 */
byte DS18B20::DS18B20_Read_Bit()
{
    byte dat;

    /* 读0和读1的时间至少要大于60us */
    DS18B20_Mode_Out_PP();
    /* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
    this->_dio=0;
	//DS18B20_DATA_OUT(LOW);
    Sys.Delay(10);

    /* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
    DS18B20_Mode_IPU();
    //Sys.Delay(2);

    if (DS18B20_DATA_IN() == SET)
        dat = 1;
    else
        dat = 0;
    /* 这个延时参数请参考时序图 */
    Sys.Delay(45);

    return dat;
}

/*
 * 从DS18B20读一个字节，低位先行
 */
byte DS18B20::DS18B20_Read_Byte()
{
    byte i, j, dat = 0;

    for (i = 0; i < 8; i++)
    {
        j = DS18B20_Read_Bit();
        dat = (dat) | (j << i);
    }

    return dat;
}

/*
 * 写一个字节到DS18B20，低位先行
 */
void DS18B20::DS18B20_Write_Byte(byte dat)
{
    byte i, testb;
    DS18B20_Mode_Out_PP();

    for (i = 0; i < 8; i++)
    {
        testb = dat &0x01;
        dat = dat >> 1;
        /* 写0和写1的时间至少要大于60us */
        if (testb)
        {
            //DS18B20_DATA_OUT(LOW);
			this->_dio=0;
            /* 1us < 这个延时 < 15us */
            Sys.Delay(8);

            //DS18B20_DATA_OUT(HIGH);
			this->_dio=1;
            Sys.Delay(58);
        }
        else
        {
            //DS18B20_DATA_OUT(LOW);
			this->_dio=0;
            /* 60us < Tx 0 < 120us */
            Sys.Delay(70);

            //DS18B20_DATA_OUT(HIGH);
			this->_dio=1;
            /* 1us < Trec(恢复时间) < 无穷大*/
            Sys.Delay(2);
        }
    }
}

void DS18B20::DS18B20_Start()
{
    DS18B20_Rst();
    DS18B20_Presence();
    DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
    DS18B20_Write_Byte(0X44); /* 开始转换 */
}

byte DS18B20::Init()
{
    DS18B20_Rst();

    return DS18B20_Presence();
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
    byte tpmsb, tplsb;
    short s_tem;
    float f_tem;

    DS18B20_Rst();
    DS18B20_Presence();
    DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
    DS18B20_Write_Byte(0X44); /* 开始转换 */

    DS18B20_Rst();
    DS18B20_Presence();
    DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
    DS18B20_Write_Byte(0XBE); /* 读温度值 */

    tplsb = DS18B20_Read_Byte();
    tpmsb = DS18B20_Read_Byte();

    s_tem = tpmsb << 8;
    s_tem = s_tem | tplsb;

    if (s_tem < 0)
    /* 负温度 */
        f_tem = (~s_tem + 1) *0.0625;
    else
        f_tem = s_tem * 0.0625;

    return f_tem;
}
