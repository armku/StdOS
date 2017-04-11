#include "Type.h"
#include "I2C.h"

I2C::~I2C(){}
// ���豸
void I2C::Open(){}
// �ر��豸
void I2C::Close(){}
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

CSoftI2C::CSoftI2C(Pin pinscl, Pin pinsda)
{
    this->SCL.OpenDrain = true;
    this->SDA.OpenDrain = true;
    this->SCL.Set(pinscl);
    this->SDA.Set(pinsda);

    this->SCL = 0;
    this->SDA = 1;
    this->SCL = 1;
}

void CSoftI2C::Init(){

}
void CSoftI2C::Start()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    this->SDA = 1;
    this->SCL = 1;
    this->Delay();
    this->SDA = 0;
    this->Delay();
    this->SCL = 0;
    this->Delay();
}

void CSoftI2C::Stop()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    this->SDA = 0;
    this->SCL = 1;
    this->Delay();
    this->SDA = 1;
}

void CSoftI2C::WriteByte(byte dat)
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
        this->Delay();
        this->SCL = 1;
        this->Delay();
        this->SCL = 0;
        if (i == 7)
        {
            this->SDA = 1; // �ͷ�����
        }
        dat <<= 1; /* ����һ��bit */
        this->Delay();
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
        this->SCL = 1;
        this->Delay();
        if (this->SDA.ReadInput())
        {
            value++;
        }
        this->SCL = 0;
        this->Delay();
    }
    return value;
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
bool CSoftI2C::WaitAck(int retry)
{
    byte re;

    this->SDA = 1; /* CPU�ͷ�SDA���� */
    this->Delay();
    this->SCL = 1; /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    this->Delay();

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
    this->Delay();
    return re;
}

void CSoftI2C::Ack(bool ack)
{
    if (ack)
    {
        this->SDA = 0; /* CPU����SDA = 0 */
        this->Delay();
        this->SCL = 1; /* CPU����1��ʱ�� */
        this->Delay();
        this->SCL = 0;
        this->Delay();
        this->SDA = 1;
        /*CPU�ͷ�SDA���� */
    }
    else
    {
        this->SDA = 1; /* CPU����SDA = 1 */
        this->Delay();
        this->SCL = 1; /* CPU����1��ʱ�� */
        this->Delay();
        this->SCL = 0;
        this->Delay();
    }
}

void CSoftI2C::OnOpen(){}
void CSoftI2C::OnClose(){}
void CSoftI2C::GetPin(Pin *scl, Pin *sda){

}
void CSoftI2C::SetPin(Pin scl, Pin sda){

}
// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
CSoftI2C::CSoftI2C(uint speedHz){

}
CSoftI2C::~CSoftI2C(){}
void CSoftI2C::Delay(int us)
{

    #if 0
        //Sys.Delay(us);
        /*
        ��Ϊ��ϵͳ���룬����Delay���Ǻ�׼����������1us
        ����23λʱ��
        48M = 5;
        72M = 8;
        108M= 12;
        120m= 14;
         */
        // 72M = 4
        int t = Sys.Clock >> 21;
        while (t-- > 0)
            ;
    #endif 
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
