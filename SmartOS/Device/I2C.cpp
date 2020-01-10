#include "Kernel\Sys.h"

#include "I2C.h"

I2C::I2C()
{
	Speed = 10000;
	Retry = 200;
	Error = 0;

	Address = 0x00;
	SubWidth = 0;

	Opened = false;
}

I2C::~I2C()
{
	Close();
}

bool I2C::Open()
{
	if (Opened) return true;

	if (!OnOpen()) return false;

	return Opened = true;
}

void I2C::Close()
{
	if (!Opened) return;

	OnClose();

	Opened = false;
}

/*
������ӻ�����ͨ��ʱ����ʱ��Ҫ�л����ݵ��շ��������磬����ĳһ����I2C ����
�ӿڵ�E2PROM �洢��ʱ����������洢������洢��Ԫ�ĵ�ַ��Ϣ���������ݣ���Ȼ����
��ȡ���еĴ洢���ݣ��������ݣ���
���л����ݵĴ��䷽��ʱ�����Բ����Ȳ���ֹͣ�����ٿ�ʼ�´δ��䣬����ֱ����һ��
������ʼ������I2C �������Ѿ�����æ��״̬�£���һ��ֱ�Ӳ�����ʼ�������������Ϊ��
����ʼ�������ظ���ʼ�����������ΪSr��
��������ʼ�������ظ���ʼ�������������ϲ�û��ʲô��ͬ��������������߼���
�档�ڽ��ж��ֽ����ݴ�������У�ֻҪ���ݵ��շ����������л�����Ҫ�õ��ظ���ʼ��
����
*/
bool I2C::SendAddress(int addr, bool tx)
{
	// 1��д��ģʽ��������û���ӵ�ַ���ȷ���д��ַ���ٷ����ӵ�ַ
	// 2����ȡģʽ�����û���ӵ�ַ���ȷ��Ͷ���ַ����ֱ�Ӷ�ȡ
	// 3����ȡģʽ��������ӵ�ַ���ȷ���д��ַ���ٷ����ӵ�ַ��Ȼ�����¿�ʼ�����Ͷ���ַ

	// ����д���ַ
	ushort d = (tx || SubWidth > 0) ? Address : (Address | 0x01);
	//debug_printf("I2C::SendAddr %02X \r\n", d);
	WriteByte((byte)d);
	if (!WaitAck(true))
	{
		debug_printf("I2C::SendAddr %02X �����豸δ���ӣ����ַ����\r\n", d);
		return false;
	}

	if (!SubWidth) return true;

	// �����ӵ�ַ
	if (!SendSubAddr(addr))
	{
		debug_printf("I2C::SendAddr %02X �����ӵ�ַ 0x%02X ʧ��\r\n", d, addr);
		return false;
	}

	if (tx) return true;

	d = Address | 0x01;
	// ��γ������������Ͷ�ȡ��ַ
	uint retry = 10;
	bool rs = false;
	while (retry-- && !rs)
	{
		Start();
		WriteByte((byte)d);
		rs = WaitAck(true);
	}
	if (!rs)
	{
		debug_printf("I2C::SendAddr %02X ���Ͷ�ȡ��ַʧ��\r\n", d);
		return false;
	}

	return rs;
}

bool I2C::SendSubAddr(int addr)
{
	//debug_printf("I2C::SendSubAddr addr=0x%02X \r\n", addr);

	// �����ӵ�ַ
	if (SubWidth > 0)
	{
		// ���ֽڷ���
		for (int k = SubWidth - 1; k >= 0; k--)
		{
			WriteByte(addr >> (k << 3));
			if (!WaitAck(true)) return false;
		}
	}

	return true;
}

// �»Ự��ָ����ַд�����ֽ�
WEAK bool I2C::Write(int addr, const Buffer& bs)
{
	debug_printf("I2C::Write addr=0x%02X ", addr);
	bs.Show(true);

	if (!Open()) return false;

	I2CScope ics(this);

	bool rs = false;
	for (int i = 0; i < 5; i++) {
		// �����豸��ַ
		rs = SendAddress(addr, true);
		if (rs) break;

		Stop();
		Sys.Sleep(1);
		Start();
	}
	if (!rs) return false;

	int len = bs.Length();
	for (int i = 0; i < len; i++)
	{
		WriteByte(bs[i]);
		// ���һ�β�Ҫ�ȴ�Ack����Ϊ��Щ�豸����Nak
		//if (i < len - 1 && !WaitAck(true)) return false;
		if (!WaitAck(i < len - 1)) return false;
		// EEPROM�����һ��ҲҪ��Ack���������
		//if (!WaitAck()) return false;
	}

	return true;
}

// �»Ự��ָ����ַ��ȡ����ֽ�
WEAK uint I2C::Read(int addr, Buffer& bs)
{
	debug_printf("I2C::Read addr=0x%02X size=%d \r\n", addr, bs.Length());

	if (!Open()) return 0;

	I2CScope ics(this);

	bool rs = false;
	for (int i = 0; i < 5; i++) {
		// �����豸��ַ
		rs = SendAddress(addr, false);
		if (rs) break;

		Stop();
		Sys.Sleep(1);
		Start();
	}
	if (!rs) return 0;

	uint count = 0;
	int len = bs.Length();
	for (int i = 0; i < len; i++)
	{
		bs[i] = ReadByte();
		count++;
		Ack(i < len - 1);	// ���һ�β���Ҫ����Ack
	}
	return count;
}

bool I2C::Write(int addr, byte data) { return Write(addr, Buffer(&data, 1)); }

byte I2C::Read(int addr)
{
	ByteArray bs(1);
	if (!Read(addr, bs)) return 0;

	return bs[0];
}

ushort I2C::Read2(int addr)
{
	ByteArray bs(2);
	if (!Read(addr, bs)) return 0;

	// С�ֽ���
	return (bs[1] << 8) | bs[0];
}

uint I2C::Read4(int addr)
{
	ByteArray bs(4);
	if (!Read(addr, bs)) return 0;

	// С�ֽ���
	return (bs[3] << 24) | (bs[2] << 16) | (bs[1] << 8) | bs[0];
}

/*HardI2C::HardI2C(I2C_TypeDef* iic, uint speedHz ) : I2C()
{
	assert_param(iic);

	I2C_TypeDef* g_I2Cs[] = I2CS;
	_index = 0xFF;
	for(int i=0; i<ArrayLength(g_I2Cs); i++)
	{
		if(g_I2Cs[i] == iic)
		{
			_index = i;
			break;
		}
	}

	Init(_index, speedHz);
}*/

HardI2C::HardI2C(byte index, uint speedHz) : I2C()
{
	Init(index, speedHz);
}

void HardI2C::Init(byte index, uint speedHz)
{
	_index = index;
	Speed = speedHz;

	OnInit();

	debug_printf("HardI2C_%d::Init %dHz \r\n", _index + 1, speedHz);
}

HardI2C::~HardI2C()
{
	Close();
}

void HardI2C::SetPin(Pin scl, Pin sda)
{
	SCL.Set(scl);
	SDA.Set(sda);
}

void HardI2C::GetPin(Pin* scl, Pin* sda)
{
	if (scl) *scl = SCL._Pin;
	if (sda) *sda = SDA._Pin;
}

/******************************** SoftI2C ********************************/
// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
I2CSoft_ARMKU::I2CSoft_ARMKU(uint speedHz)
{
	this->_delay=4;	
}

void I2CSoft_ARMKU::SetPin(Pin scl, Pin sda)
{
	this->SCL.SetPin(scl);
    this->SDA.SetPin(sda);

	this->SCL.pinMode(GPIO_Out_PP);
	this->SDA.pinMode(GPIO_Out_OD);//�˴����ô��󽫵���IIC�쳣
	
	this->SDA = 1;
	this->SCL = 1;
}

void I2CSoft_ARMKU::Start()
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

void I2CSoft_ARMKU::Stop()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    this->SDA = 0;
    this->SCL = 1;
    delayMicroseconds(1);
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
bool I2CSoft_ARMKU::WaitAck(int retry)
{
    byte re;

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
