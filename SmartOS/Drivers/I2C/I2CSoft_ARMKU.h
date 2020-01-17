#ifndef _I2CSOFT_ARMKU_H
#define _I2CSOFT_ARMKU_H

#include "Device\Port.h"

//SCL		��©�������
//SDA		��©�������
class I2CBase_ARMKU
{
public:
	virtual void SetPin(Pin scl, Pin sda)=0;

	virtual void Start()=0;
	virtual void Stop()=0;

	virtual void WriteByte(byte dat)=0;
	virtual byte ReadByte(bool ack)=0;
	virtual bool WaitAck(int retry = 0)=0;
	void Delay(int us) const;//��ʱ-΢��
	void Sleep(int ms) const;//��ʱ-����
};
// ����ģ��I2C
class I2CSoft_ARMKU:public I2CBase_ARMKU
{
public:
	bool HasSecAddress;	// �豸�Ƿ����ӵ�ַ

	// ʹ�ö˿ں�����ٶȳ�ʼ������Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
	I2CSoft_ARMKU(uint speedHz = 100000);

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

	OutputPort	SCL;	// ʱ�ӡ���©���
	OutputPort	SDA;	// ���ݡ���©�����ֱ�Ӿ߱���д����
};

/*
I2C�ĺ��ģ�SCLΪ��ʱSDA�����ı䣬��ʼֹͣ���⡣
SCLΪ��ʱ��SDA�½��ر�ʾ��ʼ��SDA�����ر�ʾֹͣ��
�������ݺ�����SCL�ȴ����շ�ͨ��SDA��������ACKȷ�ϡ�
��ȡ����ʱ����������ṩ���������ṩ���ݣ�������SCL��
*/
#endif