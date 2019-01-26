#ifndef _I2C_H_
#define _I2C_H_

#include "../Bsp/Porting.h"

//SCL		��©�������
//SDA		��©�������

// ���ģ��I2C
class SoftI2C
{
public:
	bool HasSecAddress;	// �豸�Ƿ����ӵ�ַ

	// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
    SoftI2C(uint32_t speedHz = 100000);    

	virtual void SetPin(Pin scl, Pin sda);

	virtual void Start();
	virtual void Stop();

	virtual void WriteByte(uint8_t dat);	
	virtual uint8_t ReadByte(bool ack);	
	virtual bool WaitAck(int retry=0);
private:
	virtual uint8_t ReadByte();
	virtual void Ack(bool ack = true);
private:
	int _delay;			// �����ٶ�ƥ�����ʱ

	mcuGpio	SCL;	// ʱ�ӡ���©���
	mcuGpio	SDA;	// ���ݡ���©�����ֱ�Ӿ߱���д����
};

/*
I2C�ĺ��ģ�SCLΪ��ʱSDA����ı䣬��ʼֹͣ���⡣
SCLΪ��ʱ��SDA�½��ر�ʾ��ʼ��SDA�����ر�ʾֹͣ��
�������ݺ�����SCL�ȴ����շ�ͨ��SDA��������ACKȷ�ϡ�
��ȡ����ʱ����������ṩ���������ṩ���ݣ�������SCL��
*/
#endif
