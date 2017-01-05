#include "ADS1246.h"
#include "Sys.h"

//下降沿芯片接收数据，上升沿芯片输出数据
//ADS1246寄存器列表
#define ADC_REG_BCS         0x00  		//开路电流源寄存器
#define ADC_REG_VBIAS       0x01  		//偏压寄存器
#define ADC_REG_MUX         0x02  		//多路控制寄存器  系统监视器
#define ADC_REG_SYS0        0x03  		//系统寄存器0  选择PGA和输出率
#define ADC_REG_OFC0        0x04  		//偏移校准系数寄存器0
#define ADC_REG_OFC1        0x05  		//偏移校准系数寄存器1 
#define ADC_REG_OFC2        0x06  		//偏移校准系数寄存器2
#define ADC_REG_FSC0        0x07  		//满量程校准系数寄存器0  
#define ADC_REG_FSC1        0x08  		//满量程校准系数寄存器1   
#define ADC_REG_FSC2        0x09  		//满量程校准系数寄存器2   
#define ADC_REG_ID          0x0A   		//ID寄存器    0x08

//ADS1246支持的增益列表
#define ADC_GAIN_1          0x00   		// 
#define ADC_GAIN_2          0x10   		// 
#define ADC_GAIN_4          0x20   		// 
#define ADC_GAIN_8          0x30   		// 
#define ADC_GAIN_16         0x40  		//  
#define ADC_GAIN_32         0x50   		// 
#define ADC_GAIN_64         0x60   		// 
#define ADC_GAIN_128        0x70   		// 

//ADS1246支持的转换速率列表
#define ADC_SPS_5           0x00    	//
#define ADC_SPS_10          0x01   		// 
#define ADC_SPS_20          0x02   		// 
#define ADC_SPS_40          0x03   		// 
#define ADC_SPS_80          0x04   		// 
#define ADC_SPS_160         0x05   		// 
#define ADC_SPS_320         0x06   		// 
#define ADC_SPS_640         0x07   		// 
#define ADC_SPS_1000        0x08   		// 
#define ADC_SPS_2000        0x09   		// 

//ADS1246转换模式设置
#define ADC_MODE_SINGLECOV      0x00        //单次转换模式  
#define ADC_MODE_CONTINUOUS     0x01        //连续转换模式 

//ADS1246命令码列表
#define ADC_CMD_WAKEUP      0x00            //退出睡眠模式  
#define ADC_CMD_SLEEP       0x02            //进入睡眠模式  
#define ADC_CMD_SYNC        0x04            //同步ADC转换  
#define ADC_CMD_RESET       0x06            //芯片复位  
#define ADC_CMD_NOP					0xFF            //空操作  
#define ADC_CMD_RDATA       0x12            //单次读取数据  
#define ADC_CMD_RDATAC      0x14            //连续读取数据  
#define ADC_CMD_SDATAC      0x16            //停止连续读取  
#define ADC_CMD_RREG        0x20            //读寄存器  
#define ADC_CMD_WREG        0x40            //写寄存器  
#define ADC_CMD_SYSOCAL     0x60            //系统偏移校准  
#define ADC_CMD_SYSGCAL     0x61            //系统增益校准  
#define ADC_CMD_SELFOCAL    0x62            //系统自校准  
#define ADC_CMD_RESTRICTED  0xF1            //  
CADS1246::CADS1246(Pin pincs, Pin pinsck, Pin pindin, Pin pindout, Pin pinrd, Pin pinreset)
{
    this->pspi = new CSoftSpi(pincs, pinsck, pindin, pindout,0);
    this->ppinrd.OpenDrain=true;
	this->ppinrd.Set(pinrd);
    //this->ppinrd->SetModeIN_FLOATING();
    this->ppinreset.Set(pinreset);
    this->ppinreset=0;
}

byte CADS1246::ReadReg(byte RegAddr)
{
    byte ret = 0;
    byte Cmd;

    this->pspi->portcs=0;


    Cmd = ADC_CMD_RREG | RegAddr;
    this->pspi->spi_writebyte(Cmd);
    this->pspi->spi_writebyte(0);
    ret = this->pspi->spi_writebyte(0X00);
    this->pspi->spi_readbyte(); //发送NOP
    this->pspi->portcs=1;

    return ret;

}

void CADS1246::WriteReg(byte RegAddr, byte da)
{
    byte Cmd;
    this->pspi->portcs=0;

    Cmd = ADC_CMD_WREG | RegAddr;
    this->pspi->spi_writebyte(Cmd);
    this->pspi->spi_writebyte(0);
    this->pspi->spi_writebyte(da);
    this->pspi->spi_readbyte(); //发送NOP
    this->pspi->portcs=1;
}

/*---------------------------------------------------------
数据读取
7F FF FF +MAX
00 00 01 +MIN
00 00 00 0
FF FF FF -MIN
80 00 00 -MAX
---------------------------------------------------------*/
float CADS1246::decodead(byte *da)
{
    float ret = 0;
    uint ret1 = 0;
    if (da[0] < 0x80)
    {
        ret1 = da[0];
        ret1 <<= 8;
        ret1 |= da[1];
        ret1 <<= 8;
        ret1 |= da[2];
        ret = ret1;
    }
    else
    {
        ret1 = da[0];
        ret1 <<= 8;
        ret1 |= da[1];
        ret1 <<= 8;
        ret1 |= da[2];
        ret1 ^= 0x00ffffff;
        ret1 += 1;
        ret = ret1;
        ret =  - ret;
    }
    return ret;
}

float CADS1246::Read(void) //返回-1,表示转换未完成
{
    byte Cmd[3];
    float Ret = 0;

    Cmd[0] = ADC_CMD_RDATA;
    this->pspi->portcs=0;
    if (this->ppinrd.ReadInput() != 0)
    {
        return  - 1;
    }

    this->pspi->spi_writebyte(Cmd[0]);
    Cmd[0] = this->pspi->spi_readbyte();
    Cmd[1] = this->pspi->spi_readbyte();
    Cmd[2] = this->pspi->spi_readbyte();
    this->pspi->spi_readbyte(); //发送NOP
    this->pspi->portcs=1;

    Ret = decodead(Cmd);
    return Ret;
}

void CADS1246::Init(void)
{
    this->ppinrd=1;
    this->pspi->portcs=1;
    this->ppinreset=0;
    Sys.Sleep(40);
    this->ppinreset=1;
    Sys.Sleep(20);
    this->pspi->portcs=0;
    this->WriteReg(ADC_REG_ID, 0x08); //DOUT兼容DRDY引脚   0X4A 00 08
    Sys.Sleep(40);
    this->WriteReg(ADC_REG_SYS0, ADC_SPS_20 | ADC_GAIN_1); //调整采样速度
    this->pspi->portcs=1;


    //打开中断，转换完成中断
}

//AD检查，正常返回0
byte CADS1246::Check(void)
{
    byte ret = 0;
    if (0x08 != this->ReadReg(ADC_REG_ID))
    {
        ret++;
    }
    if ((ADC_SPS_10 | ADC_GAIN_1) != this->ReadReg(ADC_REG_SYS0))
    {
        ret++;
    }
    return ret;
}
