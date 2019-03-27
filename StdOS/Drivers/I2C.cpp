#include "I2C.h"
#include "Sys.h"
// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
SoftI2C::SoftI2C(uint32_t speedHz)
{
	this->_delay=4;	
}

void SoftI2C::SetPin(Pin scl, Pin sda)
{
	this->SCL.SetPin(scl);
    this->SDA.SetPin(sda);

	this->SCL.pinMode(GPIO_Out_PP);
	this->SDA.pinMode(GPIO_Out_OD);//�˴����ô��󽫵���IIC�쳣
	
	this->SDA = 1;
	this->SCL = 1;
}

void SoftI2C::Start()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    this->SDA = 1;
	delayMicroseconds(1);
    this->SCL = 1;
	delayMicroseconds(1);
    this->SDA = 0;
    delayMicroseconds(1);
    this->SCL = 0;
    delayMicroseconds(1);
}

void SoftI2C::Stop()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    this->SDA = 0;
    this->SCL = 1;
    delayMicroseconds(1);
    this->SDA = 1;
}

void SoftI2C::WriteByte(uint8_t dat)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
    for (i = 0; i < 8; i++)
    {
        if (dat &0x80)
        {
            this->SDA = 1;
        }
        else
        {
            this->SDA = 0;
        }
        delayMicroseconds(1);
        this->SCL = 1;
        delayMicroseconds(1);
        this->SCL = 0;
        if (i == 7)
        {
            this->SDA = 1; // �ͷ�����
        }
        dat <<= 1; /* ����һ��bit */
        delayMicroseconds(1);
    }
}

uint8_t SoftI2C::ReadByte()
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        this->SCL = 1;
        delayMicroseconds(1);
        if (this->SDA.ReadInput())
        {
            value++;
        }
        this->SCL = 0;
        delayMicroseconds(1);
    }
    return value;
}
uint8_t SoftI2C::ReadByte(bool ack)
{
	uint8_t ret = this->ReadByte();
	this->Ack(ack);
	return ret;
}
void SoftI2C::Ack(bool ack)
{
    if (ack)
    {
        this->SDA = 0; /* CPU����SDA = 0 */
        delayMicroseconds(1);
        this->SCL = 1; /* CPU����1��ʱ�� */
        delayMicroseconds(1);
        this->SCL = 0;
        delayMicroseconds(1);
        this->SDA = 1;
        /*CPU�ͷ�SDA���� */
    }
    else
    {
        this->SDA = 1; /* CPU����SDA = 1 */
        delayMicroseconds(1);
        this->SCL = 1; /* CPU����1��ʱ�� */
        delayMicroseconds(1);
        this->SCL = 0;
        delayMicroseconds(1);
    }
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
bool SoftI2C::WaitAck(int retry)
{
    uint8_t re;

    this->SDA = 1; /* CPU�ͷ�SDA���� */
    delayMicroseconds(1);
    this->SCL = 1; /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    delayMicroseconds(1);

    if (this->SDA.ReadInput())
    /* CPU��ȡSDA����״̬ */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    this->SCL = 0;
    delayMicroseconds(1);
    return re;
}
