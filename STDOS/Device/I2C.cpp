#include "I2C.h"

I2C::I2C()
{
	
}
I2C::~I2C()
{
	
}
// ���豸
bool I2C::Open()
{
	return true;
}
// �ر��豸
void I2C::Close()
{
	
}
// �»Ự��ָ����ַд��
bool I2C::Write(int addr, const Buffer &bs)
{
    return false;
}

bool I2C::Write(int addr, byte data)
{
    return false;
}

// �»Ự��ָ����ַ��ȡ
uint I2C::Read(int addr, Buffer &bs)
{
    return 0;
}

byte I2C::Read(int addr)
{
    return 0;
}

ushort I2C::Read2(int addr)
{
    return 0;
}

uint I2C::Read4(int addr)
{
    return 0;
}

bool I2C::SendAddress(int addr, bool tx)
{
    return false;
}

bool I2C::SendSubAddr(int addr)
{
    return false;
}

// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
SoftI2C::SoftI2C(uint speedHz)
{
	this->_delay=4;
	this->SCL.OpenDrain = false;
    this->SDA.OpenDrain = true;//�˴����ô��󽫵���IIC�쳣
	this->SCL.Invert = false;
	this->SDA.Invert = false;
}
SoftI2C::~SoftI2C(){}

void SoftI2C::SetPin(Pin scl, Pin sda)
{
	this->SCL.Set(scl);
    this->SDA.Set(sda);
	
	this->SDA = 1;
	this->SCL = 1;

	this->SCL.Open();
	this->SDA.Open();
}

void SoftI2C::Start()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    this->SDA = 1;
	Sys.Delay(1);
    this->SCL = 1;
	Sys.Delay(1);
    this->SDA = 0;
    Sys.Delay(1);
    this->SCL = 0;
    Sys.Delay(1);
}

void SoftI2C::Stop()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    this->SDA = 0;
    this->SCL = 1;
    Sys.Delay(1);
    this->SDA = 1;
}

void SoftI2C::WriteByte(byte dat)
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
        Sys.Delay(1);
        this->SCL = 1;
        Sys.Delay(1);
        this->SCL = 0;
        if (i == 7)
        {
            this->SDA = 1; // �ͷ�����
        }
        dat <<= 1; /* ����һ��bit */
        Sys.Delay(1);
    }
}

byte SoftI2C::ReadByte()
{
    byte i;
    byte value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        this->SCL = 1;
        Sys.Delay(1);
        if (this->SDA.ReadInput())
        {
            value++;
        }
        this->SCL = 0;
        Sys.Delay(1);
    }
    return value;
}

void SoftI2C::Ack(bool ack)
{
    if (ack)
    {
        this->SDA = 0; /* CPU����SDA = 0 */
        Sys.Delay(1);
        this->SCL = 1; /* CPU����1��ʱ�� */
        Sys.Delay(1);
        this->SCL = 0;
        Sys.Delay(1);
        this->SDA = 1;
        /*CPU�ͷ�SDA���� */
    }
    else
    {
        this->SDA = 1; /* CPU����SDA = 1 */
        Sys.Delay(1);
        this->SCL = 1; /* CPU����1��ʱ�� */
        Sys.Delay(1);
        this->SCL = 0;
        Sys.Delay(1);
    }
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
bool SoftI2C::WaitAck(int retry)
{
    byte re;

    this->SDA = 1; /* CPU�ͷ�SDA���� */
    Sys.Delay(1);
    this->SCL = 1; /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    Sys.Delay(1);

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
    Sys.Delay(1);
    return re;
}
