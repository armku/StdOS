#include "AT24CXX.h"

#define macI2C_WR	0		/* д����bit */
#define macI2C_RD	1		/* ������bit */

bool AT24CXX::Write(uint32_t addr, Buffer &bs)
{
	this->bufwr(addr, bs, 1);
	return true;
}

bool AT24CXX::Read(uint32_t addr, Buffer &bs)
{
	this->bufwr(addr, bs, 0);
	return true;
}
//д��ʱʱ��
AT24CXX::AT24CXX(EW24XXType devtype, byte devaddr, uint32_t wnms)
{
	this->deviceType = devtype;
	this->Address = devaddr;
	this->pageSize = this->jsPageSize(devtype);
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
/*
 *********************************************************************************************************
 *	�� �� ��: Read
 *	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
 *	��    �Σ�addr : ��ʼ��ַ
 *			 size : ���ݳ��ȣ���λΪ�ֽ�
 *			 pBuffer : ��Ŷ��������ݵĻ�����ָ��
 *	�� �� ֵ: 1 ��ʾʧ�ܣ�0��ʾ�ɹ�
 *********************************************************************************************************
 */
int AT24CXX::Read(uint32_t addr, void *pBuffer, int size, ushort bufpos)
{
	Buffer bs((byte*)pBuffer + bufpos, size);
	this->Read(addr, bs);
	return size;
}
/*
 *********************************************************************************************************
 *	�� �� ��: Write
 *	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
 *	��    �Σ�addr : ��ʼ��ַ
 *			 size : ���ݳ��ȣ���λΪ�ֽ�
 *			 pBuffer : ��Ŷ��������ݵĻ�����ָ��
 *	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
 *********************************************************************************************************
 */
int AT24CXX::Write(uint32_t addr, void *pBuffer, int size, ushort bufpos)
{
	Buffer bs((byte*)pBuffer + bufpos, size);
	this->Write(addr, bs);
	return size;
}

byte AT24CXX::Read(ushort address)
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
	if (this->deviceType > AT24C16)
	{
		/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
		this->IIC.WriteByte((byte)((address) >> 8));

		/* ��5�����ȴ�ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Readbytefail; /* EEPROM������Ӧ�� */
		}
	}
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((byte)address);

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
	ret = this->IIC.ReadByte(); /* ��1���ֽ� */
	
	this->IIC.Ack(false); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	
	/* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();
	return ret; /* ִ�гɹ� */

cmd_Readbytefail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
/* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();

	return ret;
}

bool AT24CXX::Write(ushort address, byte da)
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
		this->IIC.WriteByte((byte)((address >> 8)));

		/* ��5�����ȴ�ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Writebytefail; /* EEPROM������Ӧ�� */
		}
	}
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((byte)address);

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
//��д���в���1д 0��
int AT24CXX::bufwr(ushort addr, Buffer &bs, byte wr)
{
	uint32_t curAddr;
	uint32_t pageStart; //ҳ����ʼ��ַ
	uint32_t bytesLeave; //��ʣ�����ֽڶ�ȡ
	ushort bufaddr;

	pageStart = addr % this->pageSize;
	bytesLeave = bs.Length();
	curAddr = addr;
	bufaddr = 0;
	if (wr == 1)
	{
		this->pinWP = 0;
	}
	/*debug_printf("read count %d\r\n",size);*/
	if (pageStart)
	{
		//��ȡ����ҳ��ʼ��ַ������
		//һ���ܶ���
		if ((pageStart + bytesLeave) < this->pageSize)
		{
			if (wr)
			{
				this->writePage(bs.GetBuffer(), bufaddr, curAddr, bytesLeave);
			}
			else
			{
				this->readPage(bs.GetBuffer(), bufaddr, curAddr, bytesLeave);
			}
			bytesLeave -= bytesLeave;
			return 0;
		}
		//һ�ζ�ȡ����
		else
		{
			if (wr)
			{
				this->writePage(bs.GetBuffer(), bufaddr, curAddr, this->pageSize - pageStart);
			}
			else
			{
				this->readPage(bs.GetBuffer(), bufaddr, curAddr, this->pageSize - pageStart);
			}
			bytesLeave -= (this->pageSize - pageStart);
			curAddr += (this->pageSize - pageStart);
			bufaddr += (this->pageSize - pageStart);
		}
	}

	while (bytesLeave > 0)
	{
		if (bytesLeave > this->pageSize)
		{
			if (wr)
			{
				this->writePage(bs.GetBuffer(), bufaddr, curAddr, this->pageSize);
			}
			else
			{
				this->readPage(bs.GetBuffer(), bufaddr, curAddr, this->pageSize);
			}
			bytesLeave -= this->pageSize;
			curAddr += this->pageSize;
			bufaddr += this->pageSize;
		}
		else
		{
			debug_printf("read size %d\r\n", bytesLeave);
			if (wr)
			{
				this->writePage(bs.GetBuffer(), bufaddr, curAddr, bytesLeave);
			}
			else
			{
				debug_printf("read size1 %d\r\n", bytesLeave);
				this->readPage(bs.GetBuffer(), bufaddr, curAddr, bytesLeave);
			}

			curAddr += bytesLeave;
			bufaddr += bytesLeave;
			bytesLeave -= bytesLeave;
		}
	}
	if (wr == 1)
	{
		this->pinWP = 1;
	}
	return bs.Length();
}
int AT24CXX::bufwr(ushort addr, byte *buf, uint32_t size, ushort bufpos, byte wr) //��д���в���1д 0��
{
	Buffer bs(buf + bufpos, size);
	return this->bufwr(addr, bs, wr);
}

int AT24CXX::writePage(ushort addr, Buffer &bs) //ҳ��д
{
	uint32_t m;
	ushort usAddr;
	byte buftmp[256];

	//�Ƿ����ȣ�ֱ�ӷ���
	if (bs.Length() > 256)
		return;
	Buffer bstmp(buftmp,bs.Length());
	this->readPage(addr,bstmp);
	bool flagchgs = false;
	for (int i = 0; i < bs.Length(); i++)
	{
		if (bs[i] != bstmp[i])
		{
			flagchgs = true;
			break;
		}
	}
	//�洢ֵ��֮ǰ��ȣ�����Ҫ����
	if (!flagchgs)
		return;

	//debug_printf("ҳд\r\n"); //return;
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
		this->IIC.WriteByte((byte)((usAddr >> 8)));

		/* ��5�����ȴ�ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Writefail; /* EEPROM������Ӧ�� */
		}
	}
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((byte)usAddr);

	/* ��5�����ȴ�ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Writefail; /* EEPROM������Ӧ�� */
	}

	for (int i = 0; i < bs.Length(); i++)
	{

		/* ��6������ʼд������ */
		this->IIC.WriteByte(bs[i]);

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
int AT24CXX::writePage(byte *buf, ushort bufpos, ushort addr, uint32_t size) //ҳ��д
{
	Buffer bs(buf + bufpos, size);
	return this->writePage(addr, bs);
}
//ҳ�ڶ�
int AT24CXX::readPage(ushort addr, Buffer &bs)
{
	//debug_printf("read a page\r\n");
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
		this->IIC.WriteByte((byte)((addr) >> 8));

		/* ��5�����ȴ�ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Readfail; /* EEPROM������Ӧ�� */
		}
	}
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	this->IIC.WriteByte((byte)addr);

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
	for (int i = 0; i < bs.Length(); i++)
	{
		bs[i] = this->IIC.ReadByte(); /* ��1���ֽ� */

												/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != bs.Length() - 1)
		{
			this->IIC.Ack(true); /* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			this->IIC.Ack(false); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();
	return 0; /* ִ�гɹ� */

cmd_Readfail:  /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
			   /* ����I2C����ֹͣ�ź� */
	this->IIC.Stop();
	return 1;
}
int AT24CXX::readPage(byte *buf, ushort bufpos, ushort addr, uint32_t size) //ҳ�ڶ�
{
	Buffer bs(buf + bufpos, size);
	return this->readPage(addr, bs);
}

ushort AT24CXX::jsPageSize(uint32_t type) //����洢ҳ��С
{
	ushort ret = 8;
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

byte AT24CXX::checkDevice()
{
	byte ucAck;

	this->IIC.Start(); /* ���������ź� */
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	this->IIC.WriteByte((this->Address) | macI2C_WR);
	ucAck = this->IIC.WaitAck(); /*����豸��ACKӦ�� */

	this->IIC.Stop(); /* ����ֹͣ�ź� */

	return ucAck;
}

byte AT24CXX::CheckOk()
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
