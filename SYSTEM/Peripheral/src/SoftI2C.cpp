#include "SoftI2C.h"
//#include "delay.h"

CSoftI2C::CSoftI2C(Pin pinscl, Pin pinsda, uint nus)
{
	this->psck.OpenDrain=true;
    this->psck.Set(pinscl);
    this->psda = new InputPort(pinsda);
    this->delayus = nus;

    //this->psck->SetModeOut_OD();
    this->psda->SetModeOut_OD();

    this->psck=0;
    this->psda->Set();
    this->psck=1;
}

void CSoftI2C::Init(){

}
void CSoftI2C::Start()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    this->psda->Set();
    this->psck=1;
    this->delay();
    this->psda->Reset();
    this->delay();
    this->psck=0;
    this->delay();
}

void CSoftI2C::Stop()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    this->psda->Reset();
    this->psck=1;
    this->delay();
    this->psda->Set();
}

void CSoftI2C::WriteByte(byte _ucByte)
{
    byte i;

    /* �ȷ����ֽڵĸ�λbit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte &0x80)
        {
            this->psda->Set();
        }
        else
        {
            this->psda->Reset();
        }
        this->delay();
        this->psck=1;
        this->delay();
        this->psck=0;
        if (i == 7)
        {
            this->psda->Set(); // �ͷ�����
        }
        _ucByte <<= 1; /* ����һ��bit */
        this->delay();
    }
}

byte CSoftI2C::ReadByte()
{
    byte i;
    byte value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        this->psck=1;
        this->delay();
        if (this->psda->Read())
        {
            value++;
        }
        this->psck=0;
        this->delay();
    }
    return value;
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
byte CSoftI2C::WaitAck()
{
    byte re;

    this->psda->Set(); /* CPU�ͷ�SDA���� */
    this->delay();
    this->psck=1; /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    this->delay();

    if (this->psda->Read())
    /* CPU��ȡSDA����״̬ */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    this->psck=0;
    this->delay();
    return re;
}

void CSoftI2C::Ack()
{
    this->psda->Reset(); /* CPU����SDA = 0 */
    this->delay();
    this->psck=1; /* CPU����1��ʱ�� */
    this->delay();
    this->psck=0;
    this->delay();
    this->psda->Set();
    /*CPU�ͷ�SDA���� */
}

void CSoftI2C::NAck()
{
    this->psda->Set(); /* CPU����SDA = 1 */
    this->delay();
    this->psck=1; /* CPU����1��ʱ�� */
    this->delay();
    this->psck=0;
    this->delay();
}

void CSoftI2C::delay(void)
{
    byte i;

    /*��
    �����ʱ����ͨ��������AX-Pro�߼������ǲ��Եõ��ġ�
    CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
    ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
    ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
    ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 

    IAR���̱���Ч�ʸߣ���������Ϊ7
     */
    for (i = 0; i < 10; i++)
        ;
}
