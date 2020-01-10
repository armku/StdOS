#include "Kernel\Sys.h"
#include "I2CSoft_ARMKU.h"

//��ʱ-΢��
void I2CBase_ARMKU::Delay(int us) const
{
    Sys.Delay(us);
}
//��ʱ-����
void I2CBase_ARMKU::Sleep(int ms) const
{
    Sys.Sleep(ms);
}

/******************************** SoftI2C ********************************/
// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
I2CSoft_ARMKU::I2CSoft_ARMKU(uint speedHz)
{
	this->_delay=4;	
}

void I2CSoft_ARMKU::SetPin(Pin scl, Pin sda)
{
	this->SCL.Set(scl);
    this->SDA.Set(sda);

    this->SCL.OpenDrain = false;
    this->SDA.OpenDrain = true;//�˴����ô��󽫵���IIC�쳣

    this->SCL.Invert = false;
    this->SDA.Invert = false;

    this->SCL.Open();
    this->SDA.Open();
	
	this->SDA = 1;
	this->SCL = 1;
}

void I2CSoft_ARMKU::Start()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    this->SDA = 1;
    this->Delay(1);
    this->SCL = 1;
    this->Delay(1);
    this->SDA = 0;
    this->Delay(1);
    this->SCL = 0;
    this->Delay(1);
}

void I2CSoft_ARMKU::Stop()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    this->SDA = 0;
    this->SCL = 1;
    this->Delay(1);
    this->SDA = 1;
}

void I2CSoft_ARMKU::WriteByte(byte dat)
{
    byte i;

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
        this->Delay(1);
        this->SCL = 1;
        this->Delay(1);
        this->SCL = 0;
        if (i == 7)
        {
            this->SDA = 1; // �ͷ�����
        }
        dat <<= 1; /* ����һ��bit */
        this->Delay(1);
    }
}

byte I2CSoft_ARMKU::ReadByte()
{
    byte i;
    byte value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        this->SCL = 1;
        this->Delay(1);
        if (this->SDA.ReadInput())
        {
            value++;
        }
        this->SCL = 0;
        this->Delay(1);
    }
    return value;
}
byte I2CSoft_ARMKU::ReadByte(bool ack)
{
	byte ret = this->ReadByte();
	this->Ack(ack);
	return ret;
}
void I2CSoft_ARMKU::Ack(bool ack)
{
    if (ack)
    {
        this->SDA = 0; /* CPU����SDA = 0 */
        this->Delay(1);
        this->SCL = 1; /* CPU����1��ʱ�� */
        this->Delay(1);
        this->SCL = 0;
        this->Delay(1);
        this->SDA = 1;
        /*CPU�ͷ�SDA���� */
    }
    else
    {
        this->SDA = 1; /* CPU����SDA = 1 */
        this->Delay(1);
        this->SCL = 1; /* CPU����1��ʱ�� */
        this->Delay(1);
        this->SCL = 0;
        this->Delay(1);
    }
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
bool I2CSoft_ARMKU::WaitAck(int retry)
{
    byte re;

    this->SDA = 1; /* CPU�ͷ�SDA���� */
    this->Delay(1);
    this->SCL = 1; /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    this->Delay(1);

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
    this->Delay(1);
    return re;
}
