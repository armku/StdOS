#include "AT24CXX.h"

#define macI2C_WR	0		/* д����bit */
#define macI2C_RD	1		/* ������bit */

bool AT24CXX::Write(uint32_t addr, void * buf, int len, int bufpos)
{
	uint32_t curAddr;
	uint32_t pageStart; //ҳ����ʼ��ַ
	uint32_t bytesLeave; //��ʣ�����ֽڶ�ȡ
	uint16_t bufaddr;

	pageStart = addr % this->Block;
	bytesLeave = len;
	curAddr = addr;
	bufaddr = 0;
	
	this->pinWP = 0;
	if (pageStart)
	{
		//��ȡ����ҳ��ʼ��ַ������		
		if ((pageStart + bytesLeave) < this->Block)
		{
			//һ���ܶ���
			this->PageWrite(curAddr, buf, bytesLeave);
			return true;
		}
		else
		{
			//һ�ζ�ȡ�����
			this->PageWrite(curAddr, buf, this->Block - pageStart);
			bytesLeave -= (this->Block - pageStart);
			curAddr += (this->Block - pageStart);
			bufaddr += (this->Block - pageStart);
		}
	}

	while (bytesLeave > 0)
	{
		if (bytesLeave > this->Block)
		{
			//��ȡ��ҳ
			this->PageWrite(curAddr, ((uint8_t*)buf) + bufaddr, this->Block);
			bytesLeave -= this->Block;
			curAddr += this->Block;
			bufaddr += this->Block;
		}
		else
		{
			//��ȡʣ��ҳ
			this->PageWrite(curAddr, ((uint8_t*)buf) + bufaddr, bytesLeave);
			return true;
		}
	}
	this->pinWP = 1;
	return false;
}

bool AT24CXX::Read(uint32_t addr, void * buf, int len, int bufpos)
{
	uint32_t curAddr;
	uint32_t pageStart; //ҳ����ʼ��ַ
	uint32_t bytesLeave; //��ʣ�����ֽڶ�ȡ
	uint16_t bufaddr;

	pageStart = addr % this->Block;
	bytesLeave = len;
	curAddr = addr;
	bufaddr = 0;
	
	if (pageStart)
	{
		//��ȡ����ҳ��ʼ��ַ������		
		if ((pageStart + bytesLeave) < this->Block)
		{
			//һ���ܶ���
			this->PageRead(curAddr, buf, bytesLeave);
			return true;
		}		
		else
		{
			//һ�ζ�ȡ�����
			this->PageRead(curAddr, buf, this->Block - pageStart);
			bytesLeave -= (this->Block - pageStart);
			curAddr += (this->Block - pageStart);
			bufaddr += (this->Block - pageStart);
		}
	}

	while (bytesLeave > 0)
	{
		if (bytesLeave > this->Block)
		{
			//��ȡ��ҳ
			this->PageRead(curAddr, ((uint8_t*)buf) + bufaddr, this->Block);
			bytesLeave -= this->Block;
			curAddr += this->Block;
			bufaddr += this->Block;
		}
		else
		{
			//��ȡʣ��ҳ
			this->PageRead(curAddr, ((uint8_t*)buf) + bufaddr, bytesLeave);
			return true;
		}
	}
	return false;
}
//д��ʱʱ��
AT24CXX::AT24CXX(EW24XXType devtype, uint8_t devaddr, uint32_t wnms)
{
	this->deviceType = devtype;
	this->Address = devaddr;
	this->Block = this->jsPageSize(devtype);
	this->writedelaynms = wnms;
}
void AT24CXX::SetPin(Pin pinscl, Pin pinsda, Pin pinwriteprotect)
{
	this->IIC.SetPin(pinscl, pinsda);
	if (pinwriteprotect != P0)
	{
		this->pinWP.Set(pinwriteprotect);
		this->pinWP.OpenDrain = false;
		this->pinWP.Invert = false;
		this->pinWP.Open();
		this->pinWP = 1;
	}
}

uint8_t AT24CXX::Read(uint16_t address)
{
	uint8_t ret = 0;
	
	/* ��1��������I2C���������ź� */
	this->IIC.Start();

	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

	/* ��3�����ȴ�ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
	}
	if (this->deviceType > AT24C16)
	{
		/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
		this->IIC.WriteByte((uint8_t)((address) >> 8));

		/* ��5�����ȴ�ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
		}
	}
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((uint8_t)address);

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

bool AT24CXX::Write(uint16_t address, uint8_t da)
{
	uint32_t m;

	/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
	this->IIC.Stop();

	/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms
	CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
	 */
	for (m = 0; m < 1000; m++)
	{
		/* ��1��������I2C���������ź� */
		this->IIC.Start();

		/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
		this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

		/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
		if (this->IIC.WaitAck() == 0)
		{
			break;
		}
	}
	if (m == 1000)
	{
		goto cmd_Writebytefail; /* EEPROM����д��ʱ */
	}
	if (this->deviceType > AT24C16)
	{
		/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
		this->IIC.WriteByte((uint8_t)((address >> 8)));

		/* ��5�����ȴ�ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
		}
	}
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((uint8_t)address);

	/* ��5�����ȴ�ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
	}
	
	/* ��6������ʼд������ */
	this->IIC.WriteByte(da);

	/* ��7��������ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
	}

	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	this->IIC.Stop();
	Sys.Sleep(this->writedelaynms);
	return true;

cmd_Writebytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
/* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();
	return true;
}

//ҳ�ڶ������һҳ
int AT24CXX::PageRead(uint16_t addr, void * buf, int len)
{
	if (len > this->Block)
		return 1;

	/* ��1��������I2C���������ź� */
	this->IIC.Start();

	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

													/* ��3�����ȴ�ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readfail; /* EEPROM������Ӧ�� */
	}
	if (this->deviceType > AT24C16)
	{
		/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
		this->IIC.WriteByte((uint8_t)((addr) >> 8));

		/* ��5�����ȴ�ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Readfail; /* EEPROM������Ӧ�� */
		}
	}
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((uint8_t)addr);

	/* ��5�����ȴ�ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readfail; /* EEPROM������Ӧ�� */
	}

	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
	this->IIC.Start();

	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	this->IIC.WriteByte(this->Address | macI2C_RD); /* �˴��Ƕ�ָ�� */

													/* ��8��������ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readfail; /* EEPROM������Ӧ�� */
	}

	/* ��9����ѭ����ȡ���� */
	for (int i = 0; i < len; i++)
	{
		((uint8_t*)buf)[i] = this->IIC.ReadByte((i != len - 1) ? true : false); /* ��1���ֽ� */
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */		
	}
	/* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();
	return 0; /* ִ�гɹ� */

cmd_Readfail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
			   /* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();
	return 1;
}
//ҳ��д�����һҳ
int AT24CXX::PageWrite(uint16_t addr, void * buf, int len)
{
	if (len > this->Block)
		return 1;
	uint32_t m;
	uint16_t usAddr;
	uint8_t buftmp[256];

	//�Ƿ����ȣ�ֱ�ӷ���
	if (len > 256)
		return 0;
	this->PageRead(addr, buftmp, len);
	bool flagchgs = false;
	for (int i = 0; i < len; i++)
	{
		if (((uint8_t*)buf)[i] != buftmp[i])
		{
			flagchgs = true;
			break;
		}
	}
	//�洢ֵ��֮ǰ��ȣ�����Ҫ����
	if (!flagchgs)
		return 0;

	usAddr = addr;
	/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
	this->IIC.Stop();

	/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms
	CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
	*/
	for (m = 0; m < 1000; m++)
	{
		/* ��1��������I2C���������ź� */
		this->IIC.Start();

		/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
		this->IIC.WriteByte(this->Address | macI2C_WR); /* �˴���дָ�� */

														/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
		if (this->IIC.WaitAck() == 0)
		{
			break;
		}
	}
	if (m == 1000)
	{
		goto cmd_Writefail; /* EEPROM����д��ʱ */
	}
	if (this->deviceType > AT24C16)
	{
		/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
		this->IIC.WriteByte((uint8_t)((usAddr >> 8)));

		/* ��5�����ȴ�ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Writefail; /* EEPROM������Ӧ�� */
		}
	}
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((uint8_t)usAddr);

	/* ��5�����ȴ�ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Writefail; /* EEPROM������Ӧ�� */
	}

	for (int i = 0; i < len; i++)
	{

		/* ��6������ʼд������ */
		this->IIC.WriteByte(((uint8_t*)buf)[i]);

		/* ��7��������ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Writefail; /* EEPROM������Ӧ�� */
		}

		usAddr++; /* ��ַ��1 */
	}

	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	this->IIC.Stop();
	Sys.Sleep(this->writedelaynms);
	return 0;

cmd_Writefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
				/* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();
	return 1;
}

uint16_t AT24CXX::jsPageSize(uint32_t type) //����洢ҳ��С
{
	uint16_t ret = 8;
	switch (type)
	{
	case AT24C02:
	case AT24C04:
		ret = 8;
		break;
	case AT24C08:
	case AT24C16:
		ret = 16;
		break;
	case AT24C32:
	case AT24C64:
		ret = 32;
		break;
	case AT24C128:
	case AT24C256:
		ret = 64;
		break;
	case AT24C512:
	case AT24C1024:
		ret = 128;
		break;
	case AT24C2048:
		ret = 256;
		break;
	default:
		break;
	}
	return ret;
}

uint8_t AT24CXX::checkDevice()
{
	uint8_t ucAck;

	this->IIC.Start(); /* ���������ź� */
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	this->IIC.WriteByte((this->Address) | macI2C_WR);
	ucAck = this->IIC.WaitAck(); /*����豸��ACKӦ�� */

	this->IIC.Stop(); /* ����ֹͣ�ź� */

	return ucAck;
}

uint8_t AT24CXX::CheckOk()
{
	if (this->checkDevice() == 0)
	{
		return 1;
	}
	else
	{
		/* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
		this->IIC.Stop();
		return 0;
	}
}
