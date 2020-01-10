#include "Sys.h"
#include "Spi.h"
#include "Platform\stm32.h"


Spi::Spi()
{
	Init();
}

Spi::Spi(SPI spi, uint speedHz, bool useNss)
{
	Init();

	Init(spi, speedHz, useNss);
}

Spi::~Spi()
{
    debug_printf("Spi::~Spi%d\r\n", _index + 1);

	Close();
}

void Spi::Init()
{
	_index	= 0xFF;
    Retry	= 200;
	Opened	= false;
	Pins[0]	= Pins[1]	= Pins[2]	= Pins[3]	= P0;
}

void Spi::Init(SPI spi, uint speedHz, bool useNss)
{
	_index = spi;

	OnInit();

	if(!useNss) Pins[0] = P0;

#if DEBUG
	int k = speedHz/1000;
	int m = k/1000;
	k -= m * 1000;
	if(k == 0)
		debug_printf("Spi%d::Init %dMHz Nss:%d\r\n", _index + 1, m, useNss);
	else
		debug_printf("Spi%d::Init %d.%dMHz Nss:%d\r\n", _index + 1, m, k, useNss);
#endif

	// �Զ������Ե����ٶ�speedHz�ķ�Ƶ
	int pre = GetPre(_index, speedHz);
	if(pre == -1) return;

    Speed = speedHz;
}

void Spi::SetPin(Pin clk, Pin miso, Pin mosi, Pin nss)
{
	if(nss != P0) Pins[0] = nss;
	if(clk != P0) Pins[1] = clk;
	if(miso != P0) Pins[2] = miso;
	if(mosi != P0) Pins[3] = mosi;
}

void Spi::GetPin(Pin* clk, Pin* miso, Pin* mosi, Pin* nss)
{
	if(nss) *nss = Pins[0];
	if(clk) *clk = Pins[1];
	if(miso) *miso = Pins[2];
	if(mosi) *mosi = Pins[3];
}

void Spi::Open()
{
	if(Opened) return;

#if DEBUG
	int k = Speed/1000;
	int m = k/1000;
	k -= m * 1000;
	if(k == 0)
		debug_printf("Spi%d::Open %dMHz\r\n", _index + 1, m);
	else
		debug_printf("Spi%d::Open %d.%dMHz\r\n", _index + 1, m, k);
#endif

	// �Զ������Ե����ٶ�speedHz�ķ�Ƶ
	uint speedHz = Speed;
	int pre = GetPre(_index, speedHz);
	if(pre == -1) return;

	Pin* ps = Pins;
    // �˿����ã�����Spi����ʱ���ͷ�
    debug_printf("\t CLK : ");
    _clk.Set(ps[1]).Open();
    debug_printf("\t MISO: ");
    _miso.Set(ps[2]).Open();
    debug_printf("\t MOSI: ");
    _mosi.Set(ps[3]).Open();

    if(ps[0] != P0)
    {
		debug_printf("    NSS : ");
		_nss.OpenDrain = false;
		//_nss.Set(ps[0]).Open();
		_nss.Init(ps[0], true).Open();
    }

    OnOpen();

	Stop();

	Opened = true;
}

void Spi::Close()
{
	if(!Opened) return;

    Stop();

	OnClose();

	debug_printf("\t CLK : ");
	_clk.Close();
	debug_printf("\t MISO: ");
	_miso.Close();
	debug_printf("\t MOSI: ");
	_mosi.Close();
	debug_printf("\t NSS : ");
	_nss.Close();

	Opened = false;
}

// ������д�����ֽ����鳤��Ϊ׼
void Spi::Write(const Buffer& bs)
{
	for(int i=0; i<bs.Length(); i++)
	{
		Write(bs[i]);
	}
}

void Spi::Read(Buffer& bs)
{
	for(int i=0; i<bs.Length(); i++)
	{
		bs[i] = Write(0x00);
	}
}

// ����NSS����ʼ����
void Spi::Start()
{
    if(!_nss.Empty()) _nss = true;

    // ��ʼ��һ����������������������
    Error = 0;
}

// ����NSS��ֹͣ����
void Spi::Stop()
{
    if(!_nss.Empty()) _nss = false;
}


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
