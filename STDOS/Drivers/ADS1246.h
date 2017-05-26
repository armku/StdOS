#pragma once 
#include "Type.h"
#include "Spi.h"
#include "Port.h"

class CSoftSpi
{
public:
    CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus = 0); //延时时间默认0us   
    byte Init();
    byte WaitBusy();
    byte Write(byte da);
    byte spi_readbyte(void);
public:
    OutputPort *pClk;
    OutputPort *pportdi;
    InputPort *pportdo;
    OutputPort *pportcs;
private:
    uint delayus;//延时时间
};

class CADS1246
{
    public:
        CADS1246(Pin pincs, Pin pinsck, Pin pindin, Pin pindout, InputPort& pinrd, Pin pinreset);       
        byte ReadReg(byte RegAddr);
        void WriteReg(byte RegAddr, byte da);
        int Read(void); //返回-1,表示转换未完成
        void Init(void);
        byte Check(void); //AD检查，正常返回0
		bool GetFlag(void);//读取AD转换是否正常
    private:
		int flagOK;//ad转换正常
        int decodead(byte *da);
	
		CSoftSpi *pspi; //SPI接口
        InputPort *ppinrd; //数据准备好接口
        OutputPort *ppinreset; //复位接口		
};
