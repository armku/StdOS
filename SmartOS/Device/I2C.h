#ifndef _I2C_H_
#define _I2C_H_

#include "Device\Port.h"

//SCL		��©�������
//SDA		��©�������

// I2C����
class I2C
{
public:
    int		Speed;		// �ٶ�
    int		Retry;		// �ȴ����Դ�����Ĭ��200
    int		Error;		// �������

	ushort	Address;	// �豸��ַ��7λ��10λ
	byte	SubWidth;	// �ӵ�ַռ�ֽ���

	bool	Opened;		// �Ƿ��Ѿ���

	I2C();
	virtual ~I2C();

	virtual void SetPin(Pin scl, Pin sda) = 0;
	virtual void GetPin(Pin* scl = nullptr, Pin* sda = nullptr) = 0;

	virtual bool Open();		// ���豸
	virtual void Close();		// �ر��豸

	virtual void Start() = 0;	// ��ʼ�Ự
	virtual void Stop() = 0;	// ֹͣ�Ự

	virtual void WriteByte(byte dat) = 0;	// д�뵥���ֽ�
	virtual byte ReadByte() = 0;			// ��ȡ�����ֽ�
	virtual void Ack(bool ack) = 0;			// ����Ack/Nak
	virtual bool WaitAck(bool ack) = 0;		// �ȴ�Ack/Nak

	// �»Ự��ָ����ַд��
	bool Write(int addr, const Buffer& bs);
	bool Write(int addr, byte data);
	// �»Ự��ָ����ַ��ȡ
	virtual uint Read(int addr, Buffer& bs);
	virtual byte Read(int addr);
	virtual ushort Read2(int addr);
	virtual uint Read4(int addr);

protected:
	virtual bool OnOpen() = 0;	// ���豸
	virtual void OnClose() = 0;	// �ⲿ�豸

	virtual bool SendAddress(int addr, bool tx);
	virtual bool SendSubAddr(int addr);
};

// I2C�Ự�ࡣ��ʼ��ʱ�򿪣���������������ʱ�ر�
class I2CScope
{
private:
	I2C* _iic;

public:
	I2CScope(I2C* iic)
	{
		_iic = iic;
		_iic->Start();
	}

	~I2CScope()
	{
		_iic->Stop();
	}
};

// Ӳ��I2C
class HardI2C : public I2C
{
private:
	void Init(byte index, uint speedHz);

public:
	// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
    //HardI2C(I2C_TypeDef* iic = I2C1, uint speedHz = 100000);
	HardI2C(byte index, uint speedHz = 100000);
    virtual ~HardI2C();

	virtual void SetPin(Pin scl, Pin sda);
	virtual void GetPin(Pin* scl = nullptr, Pin* sda = nullptr);

	virtual void Start();
	virtual void Stop();

	virtual void WriteByte(byte dat);
	virtual byte ReadByte();
	virtual void Ack(bool ack);
	virtual bool WaitAck(bool ack);

	//virtual bool Write(int addr, byte* buf, uint len);	// �»Ự��ָ����ַд�����ֽ�
	//virtual uint Read(int addr, byte* buf, uint len);	// �»Ự��ָ����ַ��ȡ����ֽ�

private:
    byte	_index;
	void*	_IIC;
	uint	_Event;

	AlternatePort SCL;
	AlternatePort SDA;

	virtual bool SendAddress(int addr, bool tx = true);

	void OnInit();
	virtual bool OnOpen();
	virtual void OnClose();
};

// ���ģ��I2C
class SoftI2C : public I2C
{
public:
	bool HasSecAddress;	// �豸�Ƿ����ӵ�ַ

	// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
    SoftI2C(uint speedHz = 100000);
    virtual ~SoftI2C();

	virtual void SetPin(Pin scl, Pin sda);
	virtual void GetPin(Pin* scl = nullptr, Pin* sda = nullptr);

	virtual void Start();
	virtual void Stop();

	virtual void WriteByte(byte dat);
	virtual byte ReadByte();
	virtual void Ack(bool ack);
	virtual bool WaitAck(bool ack);

private:
	int _delay;			// �����ٶ�ƥ�����ʱ

	OutputPort	SCL;	// ʱ�ӡ���©���
	OutputPort	SDA;	// ���ݡ���©�����ֱ�Ӿ߱���д����

	virtual bool OnOpen();
	virtual void OnClose();
	
	void Delay();
};

/*
������ʷ

2015-10-03
I2C�ĺ��ģ�SCLΪ��ʱSDA����ı䣬��ʼֹͣ���⡣
SCLΪ��ʱ��SDA�½��ر�ʾ��ʼ��SDA�����ر�ʾֹͣ��
�������ݺ�����SCL�ȴ����շ�ͨ��SDA��������ACKȷ�ϡ�
��ȡ����ʱ����������ṩ���������ṩ���ݣ�������SCL��
��л����@10068953

*/
class I2CBase
{
public:
	virtual void SetPin(Pin scl, Pin sda)=0;

	virtual void Start()=0;
	virtual void Stop()=0;

	virtual void WriteByte(byte dat)=0;
	virtual byte ReadByte(bool ack)=0;
	virtual bool WaitAck(int retry = 0)=0;
};
// ���ģ��I2C
class I2CSoft:public I2CBase
{
public:
	bool HasSecAddress;	// �豸�Ƿ����ӵ�ַ

	// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
	I2CSoft(uint speedHz = 100000);

	virtual void SetPin(Pin scl, Pin sda);

	virtual void Start();
	virtual void Stop();

	virtual void WriteByte(byte dat);	
	virtual byte ReadByte(bool ack);	
	virtual bool WaitAck(int retry=0);
private:
	virtual byte ReadByte();
	virtual void Ack(bool ack = true);
private:
	int _delay;			// �����ٶ�ƥ�����ʱ

	Port	SCL;	// ʱ�ӡ���©���
	Port	SDA;	// ���ݡ���©�����ֱ�Ӿ߱���д����
};

/*
I2C�ĺ��ģ�SCLΪ��ʱSDA����ı䣬��ʼֹͣ���⡣
SCLΪ��ʱ��SDA�½��ر�ʾ��ʼ��SDA�����ر�ʾֹͣ��
�������ݺ�����SCL�ȴ����շ�ͨ��SDA��������ACKȷ�ϡ�
��ȡ����ʱ����������ṩ���������ṩ���ݣ�������SCL��
*/
#endif
