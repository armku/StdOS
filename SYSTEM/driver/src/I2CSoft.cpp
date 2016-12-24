#include "I2CSoft.h"
//#include "delay.h"

CI2CSoft::CI2CSoft(PinPort pinscl, PinPort pinsda, uint32_t nus)
{
    this->psck = new CPort(pinscl);
    this->psda = new CPort(pinsda);
    this->delayus = nus;

    this->psck->SetModeOut_OD();
    this->psda->SetModeOut_OD();

    this->psck->Reset();
    this->psda->Set();
    this->psck->Set();
}

CI2CSoft::~CI2CSoft(){

}
void CI2CSoft::Init(){

}
void CI2CSoft::Start()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    this->psda->Set();
    this->psck->Set();
    this->delay();
    this->psda->Reset();
    this->delay();
    this->psck->Reset();
    this->delay();
}

void CI2CSoft::Stop()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    this->psda->Reset();
    this->psck->Set();
    this->delay();
    this->psda->Set();
}

void CI2CSoft::WriteByte(uint8_t _ucByte)
{
    uint8_t i;

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
        this->psck->Set();
        this->delay();
        this->psck->Reset();
        if (i == 7)
        {
            this->psda->Set(); // �ͷ�����
        }
        _ucByte <<= 1; /* ����һ��bit */
        this->delay();
    }
}

uint8_t CI2CSoft::ReadByte()
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        this->psck->Set();
        this->delay();
        if (this->psda->Read())
        {
            value++;
        }
        this->psck->Reset();
        this->delay();
    }
    return value;
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t CI2CSoft::WaitAck()
{
    uint8_t re;

    this->psda->Set(); /* CPU�ͷ�SDA���� */
    this->delay();
    this->psck->Set(); /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
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
    this->psck->Reset();
    this->delay();
    return re;
}

void CI2CSoft::Ack()
{
    this->psda->Reset(); /* CPU����SDA = 0 */
    this->delay();
    this->psck->Set(); /* CPU����1��ʱ�� */
    this->delay();
    this->psck->Reset();
    this->delay();
    this->psda->Set();
    /*CPU�ͷ�SDA���� */
}

void CI2CSoft::NAck()
{
    this->psda->Set(); /* CPU����SDA = 1 */
    this->delay();
    this->psck->Set(); /* CPU����1��ʱ�� */
    this->delay();
    this->psck->Reset();
    this->delay();
}

void CI2CSoft::delay(void)
{
    uint8_t i;

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
