#include "AD7689.h"

//������Ϣ
#define CFG		1	//���ø���	0�����ֵ�ǰ�������ã�1�����ǼĴ���������
#define INCC	7	//����ͨ������ 7�������ԣ�INx��GNDΪ�ο�
#define BW		0	//ѡ���ͨ�˲����Ĵ���	0��1/4����1��ȫ����
#define REF		0	//��׼��ѹԴ��������ѡ��	0���ڲ���׼��ѹԴ��REF=2.5V���
#define SEQ		0	//ͨ��������	0������������
#define RB		1	//�ض�CFG�Ĵ��� 0�����ݽ���ʱ�ض���ǰ���ã�1�����ض���������

//0-���ζ�ȡ�����ݻ��汣��λ��(n-2); 2-AD7689ͨ��ѡ��
CAD7689::CAD7689()
{
	this->ppinsck.Invert=false;
    this->ppinsdi.Invert=false;
    this->ppinsdo.Invert=false;
    this->ppincnv.Invert=false;  
	
	for (byte i = 0; i < MAXCH; i++)
    {
        this->FlagEnable[i] = 1;
    }
    this->chCurrent = 0;
    this->chDelayCnt = 0;
}
CAD7689::CAD7689(Pin pinsck, Pin pinsdi, Pin pinsdo, Pin pincnv):CAD7689()
{
	this->SetPin(pinsck,pinsdi,pinsdo,pincnv);
}
void CAD7689::SetPin(Pin pinsck, Pin pinsdi, Pin pinsdo, Pin pincnv)
{
	this->ppinsck.Set(pinsck);
    this->ppinsdi.Set(pinsdi);
    this->ppinsdo.Set(pinsdo);
    this->ppincnv.Set(pincnv);
}

uint16_t CAD7689::AD_Read(void)
{
    uint16_t dat = 0;
    uint32_t i;
    this->ppincnv=0;
    Sys.Delay(40);
    for (i = 0; i < 16; i++)
    {
        this->ppinsck=1;
        Sys.Delay(40);
        dat <<= 1;
        dat += this->ppinsdo.Read();
        this->ppinsck=0;
        Sys.Delay(40);
    }
    this->ppincnv=1;
    return dat;
}

uint16_t CAD7689::AD_Write(uint16_t sdat)
{
    uint16_t dat = 0;
    uint32_t i;
    this->ppincnv=0;
    Sys.Delay(50);
    for (i = 0; i < 16; i++)
    {
        ((sdat &0x8000) > 0) ? this->ppinsdi=1: this->ppinsdi=0;
        dat <<= 1;
        dat += this->ppinsdo.Read();
        Sys.Delay(50);
        this->ppinsck=0;
        Sys.Delay(50);
        sdat <<= 1;
        this->ppinsck=1;
    }
    Sys.Delay(50);
    this->ppinsdi=0;
    Sys.Delay(50);
    this->ppincnv=1;
    return dat;
}

void CAD7689::Init(void)
{    
    //this->ppinsdo->SetModeINPUT_IPU();
    
    this->ppinsck=0;
    this->ppinsdi=0;

    //��ʼ������
    this->ppincnv=0;
    Sys.Delay(100);
    this->ppincnv=1;
    Sys.Delay(10000);

}

/*******************************************************************************
 * ������  : Routin
 * ����    : ��ȡADֵ,���л�ͨ��
 * ����    : ��
 * ���    : ��
 * ����    : �� 
 * ˵��    : ��
 *******************************************************************************/
void CAD7689::Routin(void)
{
    uint16_t ch;
    uint16_t reg;
    byte chNext = 0;
    uint16_t adcur = 0;

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

//��ȡͨ����ѹֵ
float CAD7689::getVolt(uint16_t ch)
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

//��ȡ�´�ת��ͨ��
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
