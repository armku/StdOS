#include "AD7689.h"
#include "Sys.h"
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
	for (byte i = 0; i < MAXCH; i++)
    {
        this->FlagEnable[i] = 1;
    }
    this->chCurrent = 0;
    this->chDelayCnt = 0;
}

void CAD7689::SetSpi(SpiBase *spi)
{
	this->pspi = spi;
}

uint16_t CAD7689::AD_Read(void)
{
    uint16_t dat = 0;
	this->pspi->Start();
    delayMicroseconds(1);
	dat = this->pspi->Write16(0XFFFF);
	this->pspi->Stop();
    return dat;
}

uint16_t CAD7689::AD_Write(uint16_t sdat)
{
    uint16_t dat = 0;
	this->pspi->Start();
    delayMicroseconds(1);
	dat=this->pspi->Write16(sdat);
	this->pspi->Stop();
    return dat;
}

void CAD7689::Init(void)
{    
    //��ʼ������
	this->pspi->Start();
    delayMicroseconds(100);
	this->pspi->Stop();
    delayMicroseconds(600);
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
