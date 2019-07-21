#include "PCF8563.h"
#define macI2C_WR	0		/* д����bit */
#define macI2C_RD	1		/* ������bit */
PCF8563::PCF8563()
{
	this->Address = 0XA0;
}

PCF8563::~PCF8563()
{
}

void PCF8563::SetDateTime(DateTime & dt)
{

}
void PCF8563::GetDateTime(DateTime& dt)
{
	
}
byte PCF8563::readaddr(byte add)
{
	byte ret = 0;
	/* ��1��������I2C���������ź� */
	this->IIC.Start();

	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

													/* ��3�����ȴ�ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
	}
	
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((byte)this->Address);

	/* ��5�����ȴ�ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
	}

	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
	this->IIC.Start();

	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	this->IIC.WriteByte(this->Address | macI2C_RD); /* �˴��Ƕ�ָ�� */

													/* ��8��������ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
	}

	/* ��9����ѭ����ȡ���� */

	ret = this->IIC.ReadByte(false); /* ��1���ֽ� */
	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */

	/* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();
	return ret; /* ִ�гɹ� */

cmd_Readbytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
				   /* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();

	return ret;
}


