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
	this->_mosi.pinMode(GPIO_Out_PP);//此处设置错误将导致异常
	this->_miso.pinMode(GPIO_IPU);	
}
void SpiSoft_ARMKU::SetNss(Pin nss)
{
	this->_nss.SetPin(nss);

	this->_nss.pinMode(GPIO_Out_PP);
	
}

/*---------------------------------------------------------
忙状态判断，最长等待时间，200 X 10 ms=2S
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

//SPI写字节
byte SpiSoft_ARMKU::Write(byte data)
{
	byte i;
	byte ret = 0;
	if (this->CPOL == CPOL_Low)
	{
		//时钟极性，空闲时为低
		if (this->CPHA == CPHA_1Edge)
		{
			//时钟相位 在串行同步时钟的第一个跳变沿（上升或下降）数据被采样
			//测试通过
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
			//时钟相位 在串行同步时钟的第二个跳变沿（上升或下降）数据被采样
			//需要检查
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
		//时钟极性，空闲时为高
		if (this->CPHA == CPHA_1Edge)
		{
			//时钟相位 在串行同步时钟的第一个跳变沿（上升或下降）数据被采样
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
			//时钟相位 在串行同步时钟的第二个跳变沿（上升或下降）数据被采样
			//测试通过			
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
	//测试，等待删除
	for (int i = 0; i < 8; i++)
	{
		this->_clk = 0;
		this->_mosi = 1;
		delayMicroseconds(this->delayus);
		this->_clk = 1;
		this->_mosi = 0;
	}
}

// 拉低NSS，开始传输
void SpiSoft_ARMKU::Start()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 0;
	}
	// 开始新一轮事务操作，错误次数清零
	//Error = 0;
}

// 拉高NSS，停止传输
void SpiSoft_ARMKU::Stop()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 1;
	}
}
