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
主机与从机进行通信时，有时需要切换数据的收发方向。例如，访问某一具有I2C 总线
接口的E2PROM 存储器时，主机先向存储器输入存储单元的地址信息（发送数据），然后再
读取其中的存储内容（接收数据）。
在切换数据的传输方向时，可以不必先产生停止条件再开始下次传输，而是直接再一次
产生开始条件。I2C 总线在已经处于忙的状态下，再一次直接产生起始条件的情况被称为重
复起始条件。重复起始条件常常简记为Sr。
正常的起始条件和重复起始条件在物理波形上并没有什么不同，区别仅仅是在逻辑方
面。在进行多字节数据传输过程中，只要数据的收发方向发生了切换，就要用到重复起始条
件。
*/
bool I2C::SendAddress(int addr, bool tx)
{
	// 1，写入模式，不管有没有子地址，先发送写地址，再发送子地址
	// 2，读取模式，如果没有子地址，先发送读地址，再直接读取
	// 3，读取模式，如果有子地址，先发送写地址，再发送子地址，然后重新开始并发送读地址

	// 发送写入地址
	ushort d = (tx || SubWidth > 0) ? Address : (Address | 0x01);
	//debug_printf("I2C::SendAddr %02X \r\n", d);
	WriteByte((byte)d);
	if (!WaitAck(true))
	{
		debug_printf("I2C::SendAddr %02X 可能设备未连接，或地址不对\r\n", d);
		return false;
	}

	if (!SubWidth) return true;

	// 发送子地址
	if (!SendSubAddr(addr))
	{
		debug_printf("I2C::SendAddr %02X 发送子地址 0x%02X 失败\r\n", d, addr);
		return false;
	}

	if (tx) return true;

	d = Address | 0x01;
	// 多次尝试启动并发送读取地址
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
		debug_printf("I2C::SendAddr %02X 发送读取地址失败\r\n", d);
		return false;
	}

	return rs;
}

bool I2C::SendSubAddr(int addr)
{
	//debug_printf("I2C::SendSubAddr addr=0x%02X \r\n", addr);

	// 发送子地址
	if (SubWidth > 0)
	{
		// 逐字节发送
		for (int k = SubWidth - 1; k >= 0; k--)
		{
			WriteByte(addr >> (k << 3));
			if (!WaitAck(true)) return false;
		}
	}

	return true;
}

// 新会话向指定地址写入多个字节
WEAK bool I2C::Write(int addr, const Buffer& bs)
{
	debug_printf("I2C::Write addr=0x%02X ", addr);
	bs.Show(true);

	if (!Open()) return false;

	I2CScope ics(this);

	bool rs = false;
	for (int i = 0; i < 5; i++) {
		// 发送设备地址
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
		// 最后一次不要等待Ack，因为有些设备返回Nak
		//if (i < len - 1 && !WaitAck(true)) return false;
		if (!WaitAck(i < len - 1)) return false;
		// EEPROM上最后一次也要等Ack，否则错乱
		//if (!WaitAck()) return false;
	}

	return true;
}

// 新会话从指定地址读取多个字节
WEAK uint I2C::Read(int addr, Buffer& bs)
{
	debug_printf("I2C::Read addr=0x%02X size=%d \r\n", addr, bs.Length());

	if (!Open()) return 0;

	I2CScope ics(this);

	bool rs = false;
	for (int i = 0; i < 5; i++) {
		// 发送设备地址
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
		Ack(i < len - 1);	// 最后一次不需要发送Ack
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

	// 小字节序
	return (bs[1] << 8) | bs[0];
}

uint I2C::Read4(int addr)
{
	ByteArray bs(4);
	if (!Read(addr, bs)) return 0;

	// 小字节序
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
// 使用端口和最大速度初始化，因为需要分频，实际速度小于等于该速度
I2CSoft_ARMKU::I2CSoft_ARMKU(uint speedHz)
{
	this->_delay=4;	
}

void I2CSoft_ARMKU::SetPin(Pin scl, Pin sda)
{
	this->SCL.SetPin(scl);
    this->SDA.SetPin(sda);

	this->SCL.pinMode(GPIO_Out_PP);
	this->SDA.pinMode(GPIO_Out_OD);//此处设置错误将导致IIC异常
	
	this->SDA = 1;
	this->SCL = 1;
}

void I2CSoft_ARMKU::Start()
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
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
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    this->SDA = 0;
    this->SCL = 1;
    delayMicroseconds(1);
    this->SDA = 1;
}

void I2CSoft_ARMKU::WriteByte(byte dat)
{
    byte i;

    /* 先发送字节的高位bit7 */
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
            this->SDA = 1; // 释放总线
        }
        dat <<= 1; /* 左移一个bit */
        delayMicroseconds(1);
    }
}

byte I2CSoft_ARMKU::ReadByte()
{
    byte i;
    byte value;

    /* 读到第1个bit为数据的bit7 */
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
        this->SDA = 0; /* CPU驱动SDA = 0 */
        delayMicroseconds(1);
        this->SCL = 1; /* CPU产生1个时钟 */
        delayMicroseconds(1);
        this->SCL = 0;
        delayMicroseconds(1);
        this->SDA = 1;
        /*CPU释放SDA总线 */
    }
    else
    {
        this->SDA = 1; /* CPU驱动SDA = 1 */
        delayMicroseconds(1);
        this->SCL = 1; /* CPU产生1个时钟 */
        delayMicroseconds(1);
        this->SCL = 0;
        delayMicroseconds(1);
    }
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
bool I2CSoft_ARMKU::WaitAck(int retry)
{
    byte re;

    this->SDA = 1; /* CPU释放SDA总线 */
    delayMicroseconds(1);
    this->SCL = 1; /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    delayMicroseconds(1);

    if (this->SDA.ReadInput())
    /* CPU读取SDA口线状态 */
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
