#include "Sys.h"
#include "Spi.h"
#include "Platform\stm32.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////SpiSoft////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
SpiSoft_ARMKU::SpiSoft_ARMKU()
{	

}
void SpiSoft_ARMKU::Init(CPOLTYPE cpol  , CPHATYPE cpha)
{
	this->delayus = 10;

	this->CPOL = cpol;
	this->CPHA = cpha;

	this->_clk.Open();
	this->_miso.Open();
	this->_mosi.Open();
	this->_nss.Open();

	this->_miso = 1;
	if (CPOL_High)
		this->_clk = 1;
	else
		this->_clk = 0;
	this->_mosi = 1;
	this->_nss = 1;
}

void SpiSoft_ARMKU::SetPin(Pin clk, Pin miso, Pin mosi)
{
	this->_clk.SetPin(clk);
	this->_mosi.SetPin(mosi);
	this->_miso.SetPin(miso);

	this->_clk.pinMode(GPIO_Out_PP);
	this->_mosi.pinMode(GPIO_Out_PP);//�˴����ô��󽫵����쳣
	this->_miso.pinMode(GPIO_IPU);	
}
void SpiSoft_ARMKU::SetNss(Pin nss)
{
	this->_nss.SetPin(nss);

	this->_nss.pinMode(GPIO_Out_PP);
	
}

/*---------------------------------------------------------
æ״̬�жϣ���ȴ�ʱ�䣬200 X 10 ms=2S
---------------------------------------------------------*/
byte SpiSoft_ARMKU::WaitBusy()
{
	ushort i;
	this->Start();
	i = 0;
	while (this->_miso.Read() > 0)
	{
		Sys.Sleep112233(10);
		i++;
		if (i > 200)
			return 1;
	}
	this->Stop();
	return 0;
}

//SPIд�ֽ�
byte SpiSoft_ARMKU::Write(byte data)
{
	byte i;
	byte ret = 0;
	if (this->CPOL == CPOL_Low)
	{
		//ʱ�Ӽ��ԣ�����ʱΪ��
		if (this->CPHA == CPHA_1Edge)
		{
			//ʱ����λ �ڴ���ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
			//����ͨ��
			for (i = 0; i < 8; i++)
			{
				this->_clk = 0;
				delayMicroseconds(this->delayus);
				if (data &(1 << (8 - i - 1)))
				{
					this->_mosi = 1;
				}
				else
				{
					this->_mosi = 0;
				}
				delayMicroseconds(this->delayus);
				ret <<= 1;				
				if (this->_miso.Read())
				{
					ret |= 1;
				}
				this->_clk = 1;
				delayMicroseconds(this->delayus);
			}			
			this->_clk = 0;
			delayMicroseconds(this->delayus);
		}
		else if (this->CPHA == CPHA_2Edge)
		{
			//ʱ����λ �ڴ���ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
			//��Ҫ���
			this->_clk = 0;
			delayMicroseconds(this->delayus);
			for (i = 0; i < 8; i++)
			{
				this->_clk = 1;
				delayMicroseconds(this->delayus);
				if (data &(1 << (8 - i - 1)))
				{
					this->_mosi = 1;
				}
				else
				{
					this->_mosi = 0;
				}
				delayMicroseconds(this->delayus);
				ret <<= 1;
				if (this->_miso.Read())
				{
					ret |= 1;
				}
				this->_clk = 0;
				delayMicroseconds(this->delayus);
			}			
		}
	}
	else if (this->CPOL == CPOL_High)
	{
		//ʱ�Ӽ��ԣ�����ʱΪ��
		if (this->CPHA == CPHA_1Edge)
		{
			//ʱ����λ �ڴ���ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
			for (i = 0; i < 8; i++)
			{
				this->_clk = 1;
				delayMicroseconds(this->delayus);
				if (data &(1 << (8 - i - 1)))
				{
					this->_mosi = 1;
				}
				else
				{
					this->_mosi = 0;
				}
				delayMicroseconds(this->delayus);							
				ret <<= 1;
				delayMicroseconds(this->delayus);
				if (this->_miso.Read())
				{
					ret |= 1;
				}
				this->_clk = 0;
				delayMicroseconds(this->delayus);
			}
			this->_clk = 1;
			delayMicroseconds(this->delayus);
		}
		else if (this->CPHA == CPHA_2Edge)
		{
			//ʱ����λ �ڴ���ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
			//����ͨ��			
			for (i = 0; i < 8; i++)
			{
				this->_clk = 0;
				delayMicroseconds(this->delayus);
				if (data &(1 << (8 - i - 1)))
				{
					this->_mosi = 1;
				}
				else
				{
					this->_mosi = 0;
				}
				delayMicroseconds(this->delayus);				
				ret <<= 1;
				if (this->_miso.Read())
				{
					ret |= 1;
				}
				this->_clk = 1;
				delayMicroseconds(this->delayus);
			}
		}
		else
		{
		}
	}
	else
	{
	}
	return ret;
}
ushort SpiSoft_ARMKU::Write16(ushort data)
{
	ushort dat = 0;

	dat = this->Write((data >> 8) & 0xff);
	dat <<= 8;
	dat |= this->Write(data & 0xff);

	return dat;
}

void SpiSoft_ARMKU::Open()
{
	if (this->CPOL)
	{
		this->_clk = 1;
	}
	else
	{
		this->_clk = 0;
	}
	this->Stop();
}
void SpiSoft_ARMKU::Close()
{
	//���ԣ��ȴ�ɾ��
	for (int i = 0; i < 8; i++)
	{
		this->_clk = 0;
		this->_mosi = 1;
		delayMicroseconds(this->delayus);
		this->_clk = 1;
		this->_mosi = 0;
	}
}

// ����NSS����ʼ����
void SpiSoft_ARMKU::Start()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 0;
	}
	// ��ʼ��һ����������������������
	//Error = 0;
}

// ����NSS��ֹͣ����
void SpiSoft_ARMKU::Stop()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 1;
	}
}
