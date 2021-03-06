#include "AD7689.h"
#include "Sys.h"
//配置信息
#define CFG		1	//配置更新	0：保持当前配置设置，1：覆盖寄存器的内容
#define INCC	7	//输入通道配置 7：单极性，INx以GND为参考
#define BW		0	//选择低通滤波器的带宽	0：1/4带宽，1：全带宽
#define REF		0	//基准电压源、缓冲器选择	0：内部基准电压源，REF=2.5V输出
#define SEQ		0	//通道序列器	0：禁用序列器
#define RB		1	//回读CFG寄存器 0：数据结束时回读当前配置，1：不回读配置内容

//0-本次读取的数据缓存保存位置(n-2); 2-AD7689通道选择
CAD7689::CAD7689()
{	
	for (byte i = 0; i < MAXCH; i++)
    {
        this->FlagEnable[i] = 1;
    }
    this->chCurrent = 0;
    this->chDelayCnt = 0;
}

void CAD7689::SetSpi(SpiBase_ARMKU *spi)
{
	this->pspi = spi;
}

ushort CAD7689::AD_Read(void)
{
    ushort dat = 0;
	this->pspi->Start();
    delayMicroseconds(1);
	dat = this->pspi->Write16(0XFFFF);
	this->pspi->Stop();
    return dat;
}

ushort CAD7689::AD_Write(ushort sdat)
{
    ushort dat = 0;
	this->pspi->Start();
    delayMicroseconds(1);
	dat=this->pspi->Write16(sdat);
	this->pspi->Stop();
    return dat;
}

void CAD7689::Init(void)
{    
    //初始化配置
	this->pspi->Start();
    delayMicroseconds(100);
	this->pspi->Stop();
    delayMicroseconds(600);
}

/*******************************************************************************
 * 函数名  : Routin
 * 描述    : 读取AD值,并切换通道
 * 输入    : 无
 * 输出    : 无
 * 返回    : 无 
 * 说明    : 无
 *******************************************************************************/
void CAD7689::Routin(void)
{
    ushort ch;
    ushort reg;
    byte chNext = 0;
    ushort adcur = 0;

    ch = this->chCurrent;

    chNext = this->getNextCH();
    reg = ((CFG << 13) | (INCC << 10) | (BW << 6) | (REF << 3) | (SEQ << 1) | (RB << 0)) | (chNext << 7);
    reg <<= 2;
    adcur = this->AD_Write(reg);
    if (ch != chNext)
    {
        this->ADBuf[ch] = adcur;
        this->volt[ch] = this->ADBuf[ch] *2.5 / 0XFFFF;
    }
    this->chCurrent = chNext;
}

//获取通道电压值
float CAD7689::getVolt(ushort ch)
{
    if (ch >= MAXCH)
    {
        return 0.0;
    }
    else
    {
        return this->volt[ch];
    }
}

//获取下次转换通道
byte CAD7689::getNextCH()
{
    byte ret = 0;

    ret = this->chCurrent;
    ret++;
    ret %= MAXCH;

    this->chDelayCnt++;
    if (this->chDelayCnt > CHDELAY)
    {
        this->chDelayCnt = 0;
    }
    else
    {
        ret = this->chCurrent;
    }

    return ret;
}
