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

// 3�����ʿ�ѡ��  ��׼ģʽ100kbps������ģʽ400kbps���������3.4Mbps
SoftI2C::SoftI2C(uint speedHz) : I2C()
{
	Speed = speedHz;
	_delay = Sys.Clock / speedHz;
	Retry = 100;
	Error = 0;
	Address = 0x00;
}

SoftI2C::~SoftI2C()
{
	Close();
}

void SoftI2C::SetPin(Pin scl, Pin sda)
{
	//SCL.Set(scl);
	//SDA.Set(sda);
	// �����Զ���⵹�á�
	// һ��I2C��ʼ���Ǹߵ�ƽ��Ҳ������Ҫ����
	// ����Ϊ�˸�����ر��ߵ͵�ƽ����Ҫ����
	SCL.Init(scl, false);
	SDA.Init(sda, false);
}

void SoftI2C::GetPin(Pin* scl, Pin* sda)
{
	if (scl) *scl = SCL._Pin;
	if (sda) *sda = SDA._Pin;
}

bool SoftI2C::OnOpen()
{
	assert(!SCL.Empty() && !SDA.Empty(), "δ����I2C����");

	debug_printf("I2C::Open Addr=0x%02X SubWidth=%d \r\n", Address, SubWidth);

	// ��©���
	SCL.OpenDrain = true;
	SDA.OpenDrain = true;

	debug_printf("\tSCL: ");
	SCL.Open();
	debug_printf("\tSDA: ");
	SDA.Open();

	// �����߿���ʱ��������·���Ǹߵ�ƽ
	SCL = true;
	SDA = true;

	// SDA/SCL Ĭ������
	if (!SDA.ReadInput()) {
		debug_printf("��ʧ�ܣ�û����SDA�ϼ�⵽�ߵ�ƽ��\r\n");

		SCL.Close();
		SDA.Close();

		return false;
	}

	return true;
}

void SoftI2C::OnClose()
{
	SCL.Close();
	SDA.Close();
}

void SoftI2C::Delay()
{
	Sys.Delay(5);

	/*
	����23λʱ��
	48M = 5;
	72M = 8;
	108M= 12;
	120M= 14;
	*/
	//int t = Sys.Clock >> 21;
	//while (t-- > 0);
}

// ��ʼ���� �� SCL ���ڸߵ�ƽ�ڼ�ʱ��SDA �Ӹߵ�ƽ��͵�ƽ����ʱ������ʼ������
// ��������ʼ����������㴦��æ��״̬����ʼ�����������ΪS��
/*
sda		   ----____
scl		___--------____
*/
void SoftI2C::Start()
{
	//debug_printf("SoftI2C::Start \r\n");
	// ��SCL�ߵ�ƽ�ڼ䣬SDA�����½���
	SCL = true;

	// �½���
	SDA = true;
	Delay();
	SDA = false;

	Delay();
	SCL = false;
}

// ֹͣ���� �� SCL ���ڸߵ�ƽ�ڼ�ʱ��SDA �ӵ͵�ƽ��ߵ�ƽ����ʱ����ֹͣ������
// ������ֹͣ�����������ڿ���״̬��ֹͣ�������ΪP��
/*
sda		____----
scl		____----
*/
void SoftI2C::Stop()
{
	//debug_printf("SoftI2C::Stop \r\n");
	// ��SCL�ߵ�ƽ�ڼ䣬SDA����������
	SCL = false;
	SDA = false;
	Delay();

	SCL = true;
	Delay();
	SDA = true;
}

// �ȴ�Ack
bool SoftI2C::WaitAck(bool ack)
{
	int retry = Retry;

	// SDA ���ϵ����ݱ�����ʱ�ӵĸߵ�ƽ���ڱ����ȶ�
	SDA = true;
	SCL = true;
	Delay();

	if (ack) {
		// �ȴ�SDA�͵�ƽ
		while (SDA.ReadInput() == ack)
		{
			if (retry-- <= 0)
			{
				//debug_printf("SoftI2C::WaitAck Retry=%d �޷��ȵ�ACK \r\n", Retry);
				return false;
			}
		}
	}

	SCL = false;
	Delay();

	return true;
}

// ����Ack
/*
�� I2C ���ߴ������ݹ����У�ÿ����һ���ֽڣ���Ҫ��һ��Ӧ��״̬λ��
�������������ݵ��������ͨ��Ӧ��λ����֪��������
Ӧ��λ��ʱ����������������������Ӧ��λ������״̬����ѭ��˭����˭��������ԭ�򣬼������ɽ���������Ӧ��λ��
������ӻ���������ʱ��Ӧ��λ�ɴӻ������������Ӵӻ���������ʱ��Ӧ��λ������������

I2C ���߱�׼�涨��
Ӧ��λΪ0 ��ʾ������Ӧ��ACK�����������ΪA��Ϊ1 ���ʾ��Ӧ��NACK�����������ΪA��
������������LSB ֮��Ӧ���ͷ�SDA �ߣ�����SDA���������ܽ�ֹ�����Եȴ�����������Ӧ��λ��
����������ڽ��������һ���ֽڵ����ݣ����߲����ٽ��ո��������ʱ��Ӧ���� ����Ӧ����֪ͨ������
*/
void SoftI2C::Ack(bool ack)
{
	// SDA ���ϵ����ݱ�����ʱ�ӵĸߵ�ƽ���ڱ����ȶ�
	SCL = false;

	SDA = !ack;
	Delay();

	SCL = true;
	Delay();
	SCL = false;

	SDA = true;
	Delay();
}

void SoftI2C::WriteByte(byte dat)
{
	// SDA ���ϵ����ݱ�����ʱ�ӵĸߵ�ƽ���ڱ����ȶ�
	SCL = false;
	for (byte mask = 0x80; mask > 0; mask >>= 1)
	{
		SDA = (dat & mask) > 0;
		Delay();

		// ��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
		SCL = true;
		Delay();
		SCL = false;
		Delay();
	}

	// ������һ���ֽ����ݺ�Ҫ����Ҫ�ȴ��ӻ���Ӧ�𣬵ھ�λ
	SCL = false;	// ����sda�仯
	Delay();
	SDA = true;		// sda���ߵȴ�Ӧ�𣬵�sda=0ʱ����ʾ�ӻ���Ӧ��
	Delay();
}

byte SoftI2C::ReadByte()
{
	// SDA ���ϵ����ݱ�����ʱ�ӵĸߵ�ƽ���ڱ����ȶ�
	SDA = true;
	byte rs = 0;
	for (byte mask = 0x80; mask > 0; mask >>= 1)
	{
		SCL = true;		// ��ʱ����Ϊ��ʹ��������������Ч
		//Delay(2);
		// ��SCL���
		uint retry = 50;
		while (!SCL.ReadInput())
		{
			if (retry-- <= 0) break;
			Delay();
		}

		if (SDA.ReadInput()) rs |= mask;	//������λ
		SCL = false;	// ��ʱ����Ϊ�ͣ�׼����������λ
		Delay();
	}

	return rs;
}

/*
SDA ��SCL ����˫����·��ͨ��һ������Դ�������������ӵ����ĵ�Դ��ѹ��
�����߿���ʱ��������·���Ǹߵ�ƽ��

SDA ���ϵ����ݱ�����ʱ�ӵĸߵ�ƽ���ڱ����ȶ���
�����ߵĸ߻�͵�ƽ״ֻ̬����SCL�ߵ�ʱ���ź��ǵ͵�ƽʱ���ܸı䡣
��ʼ��ֹͣ���⣬��˴ӻ�������������ʼ��ֹͣ�źš�
*/
























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
