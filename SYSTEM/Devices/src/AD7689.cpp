#include "AD7689.h"
#include "BasePort.h"

//������Ϣ
#define CFG		1	//���ø���	0�����ֵ�ǰ�������ã�1�����ǼĴ���������
#define INCC	7	//����ͨ������ 7�������ԣ�INx��GNDΪ�ο�
#define BW		0	//ѡ���ͨ�˲����Ĵ���	0��1/4����1��ȫ����
#define REF		0	//��׼��ѹԴ��������ѡ��	0���ڲ���׼��ѹԴ��REF=2.5V���
#define SEQ		0	//ͨ��������	0������������
#define RB		1	//�ض�CFG�Ĵ��� 0�����ݽ���ʱ�ض���ǰ���ã�1�����ض���������

//0-���ζ�ȡ�����ݻ��汣��λ��(n-2); 2-AD7689ͨ��ѡ��
CAD7689::CAD7689(PinPort pinsck, PinPort pinsdi, PinPort pinsdo, PinPort pincnv)
{
    this->ppinsck = new BasePort(pinsck);
    this->ppinsdi = new BasePort(pinsdi);
    this->ppinsdo = new BasePort(pinsdo);
    this->ppincnv = new BasePort(pincnv);

    for (byte i = 0; i < MAXCH; i++)
    {
        this->FlagEnable[i] = 1;
    }
    this->chCurrent = 0;
    this->chDelayCnt = 0;
}

CAD7689::~CAD7689(){

}
void CAD7689::Delay(uint nTime)
{
    uint i;
    for (i = 0; i < nTime; i++)
        ;
}

ushort CAD7689::AD_Read(void)
{
    ushort dat = 0;
    uint i;
    this->ppincnv->Reset();
    this->Delay(40);
    for (i = 0; i < 16; i++)
    {
        this->ppinsck->Set();
        this->Delay(40);
        dat <<= 1;
        dat += this->ppinsdo->Read();
        this->ppinsck->Reset();
        this->Delay(40);
    }
    this->ppincnv->Set();
    return dat;
}

ushort CAD7689::AD_Write(ushort sdat)
{
    ushort dat = 0;
    uint i;
    this->ppincnv->Reset();
    this->Delay(50);
    for (i = 0; i < 16; i++)
    {
        ((sdat &0x8000) > 0) ? this->ppinsdi->Set(): this->ppinsdi->Reset();
        dat <<= 1;
        dat += this->ppinsdo->Read();
        this->Delay(50);
        this->ppinsck->Reset();
        this->Delay(50);
        sdat <<= 1;
        this->ppinsck->Set();
    }
    this->Delay(50);
    this->ppinsdi->Reset();
    this->Delay(50);
    this->ppincnv->Set();
    return dat;
}

void CAD7689::Init(void)
{
    this->ppincnv->SetModeOut_PP();
    this->ppinsdo->SetModeINPUT_IPU();
    this->ppinsck->SetModeOut_PP();
    this->ppinsdi->SetModeOut_PP();

    this->ppinsck->Reset();
    this->ppinsdi->Reset();

    //��ʼ������
    this->ppincnv->Reset();
    this->Delay(100);
    this->ppincnv->Set();
    this->Delay(10000);

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

//��ȡͨ����ѹֵ
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
