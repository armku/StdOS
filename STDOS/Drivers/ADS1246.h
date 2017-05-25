#pragma once 
#include "Type.h"
#include "Spi.h"
#include "Port.h"

class CSoftSpi
{
public:
    CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus = 0); //��ʱʱ��Ĭ��0us   
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
    uint delayus;//��ʱʱ��
};

class CADS1246
{
    public:
        CADS1246(Pin pincs, Pin pinsck, Pin pindin, Pin pindout, InputPort& pinrd, Pin pinreset);       
        byte ReadReg(byte RegAddr);
        void WriteReg(byte RegAddr, byte da);
        int Read(void); //����-1,��ʾת��δ���
        void Init(void);
        byte Check(void); //AD��飬��������0
		bool GetFlag(void);//��ȡADת���Ƿ�����
    private:
		int flagOK;//adת������
        int decodead(byte *da);
	
		CSoftSpi *pspi; //SPI�ӿ�
        InputPort *ppinrd; //����׼���ýӿ�
        OutputPort *ppinreset; //��λ�ӿ�		
};
