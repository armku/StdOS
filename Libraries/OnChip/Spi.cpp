#include "Sys.h"
#include "Spi.h"
#include "Platform\stm32.h"

void Spi::Init()
{
	this->_clk.Invert = false;
	this->_miso.Invert = false;
	this->_mosi.Invert = false;
	this->_nss.Invert = false;

	this->_clk.OpenDrain = false;
	this->_miso.OpenDrain = false;
	this->_mosi.OpenDrain = false;
	this->_nss.OpenDrain = false;

	this->Retry = 200; //默认重试次数为200
}

Spi::Spi()
{
	this->Init();
	this->_index = 0xff;
}

// 使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度
Spi::Spi(SPI spi, CPOLTYPE cpol, CPHATYPE cpha, uint32_t speedHz)
{
	this->CPOL = cpol;
	this->CPHA = cpha;
	this->Init();
	this->Init(spi, speedHz);
}

Spi::~Spi()
{
	debug_printf("Spi:Spi%d\r\n", _index + 1);

	this->Close();
}

void Spi::Init(SPI spi, uint32_t speedHz)
{
	this->_index = spi;
	this->Speed = speedHz;
#if DEBUG
	int k = speedHz / 1000;
	int m = k / 1000;
	k -= m * 1000;
	if (k == 0)
		debug_printf("Spi%d::Init %dMHz\r\n", _index + 1, m);
	else
		debug_printf("Spi%d::Init %d.%dMHz\r\n", _index + 1, m, k);
#endif 
	// 自动计算稍低于速度speedHz的分频
	int pre = GetPre(spi, speedHz);
	if (pre == -1)
		return;
	debug_printf("pre %x \r\n", pre);

	Speed = speedHz;

	this->OnInit();
}

void Spi::SetPin(Pin clk, Pin miso, Pin mosi)
{
	this->Pins[1] = clk;
	this->Pins[2] = miso;
	this->Pins[3] = mosi;

	this->_clk.Set(this->Pins[1]);
	this->_miso.Set(this->Pins[2]);
	this->_mosi.Set(this->Pins[3]);
}
void Spi::SetNss(Pin nss)
{
	this->Pins[0] = nss;
	this->_nss.Set(this->Pins[0]);
	this->Open();
}

void Spi::Open()
{
	this->Stop();
	this->OnOpen();
}

void Spi::Close()
{
	this->OnClose();
}

// 批量读写。以字节数组长度为准
void Spi::Write(void * buf, int len) {

}
void Spi::Read(void * buf, int len) {

}

// 拉低NSS，开始传输
void Spi::Start()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 0;
	}
	// 开始新一轮事务操作，错误次数清零
	this->Error = 0;
}

// 拉高NSS，停止传输
void Spi::Stop()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////SpiSoft////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
SpiSoft::SpiSoft(CPOLTYPE cpol, CPHATYPE cpha, uint32_t speedHz)
{
	this->_nss.Invert = false;
	this->_clk.Invert = false;
	this->_mosi.Invert = false;
	this->_miso.Invert = false;
	//this->delayus=speedHz;
#ifdef STM32F0
	this->delayus = 0;
#elif defined STM32F1
	this->delayus = 0;
#elif defined STM32F4
	this->delayus = 0;
#endif
	this->CPOL = cpol;
	this->CPHA = cpha;
}

void SpiSoft::SetPin(Pin clk, Pin miso, Pin mosi)
{
	this->_clk.Set(clk);
	this->_mosi.Set(mosi);
	this->_miso.Set(miso);

	this->Open();
}
void SpiSoft::SetNss(Pin nss)
{
	this->_nss.Set(nss);
	this->Open();
}

/*---------------------------------------------------------
忙状态判断，最长等待时间，200 X 10 ms=2S
---------------------------------------------------------*/
uint8_t SpiSoft::WaitBusy()
{
	uint16_t i;
	this->Start();
	i = 0;
	while (this->_miso.Read() > 0)
	{
		Sys.Sleep(10);
		i++;
		if (i > 200)
			return 1;
	}
	this->Stop();
	return 0;
}

//SPI写字节
uint8_t SpiSoft::Write(uint8_t data)
{
	uint8_t i;
	uint8_t ret = 0;
	if (this->CPOL == CPOL_Low)
	{
		//时钟极性，空闲时为低
		if (this->CPHA == CPHA_1Edge)
		{
			//时钟相位 在串行同步时钟的第一个跳变沿（上升或下降）数据被采样
			//测试通过
			for (i = 0; i < 8; i++)
			{
				if (data &(1 << (8 - i - 1)))
				{
					this->_mosi = 1;
				}
				else
				{
					this->_mosi = 0;
				}
				Sys.Delay(this->delayus);
				this->_clk = 1;
				Sys.Delay(this->delayus);
				this->_clk = 0;
				ret <<= 1;
				if (this->_miso.Read())
				{
					ret |= 1;
				}
			}
		}
		else if (this->CPHA == CPHA_2Edge)
		{
			//时钟相位 在串行同步时钟的第二个跳变沿（上升或下降）数据被采样
			//需要检查
			for (i = 0; i < 8; i++)
			{
				this->_clk = 1;
				Sys.Delay(this->delayus);
				if (data &(1 << (8 - i - 1)))
				{
					this->_mosi = 1;
				}
				else
				{
					this->_mosi = 0;
				}
				Sys.Delay(this->delayus);
				this->_clk = 0;
				ret <<= 1;
				if (this->_miso.Read())
				{
					ret |= 1;
				}
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
				if (data &(1 << (8 - i - 1)))
				{
					this->_mosi = 1;
				}
				else
				{
					this->_mosi = 0;
				}
				Sys.Delay(this->delayus);
				this->_clk = 0;
				Sys.Delay(this->delayus);
				this->_clk = 1;
				ret <<= 1;
				if (this->_miso.Read())
				{
					ret |= 1;
				}
			}
		}
		else if (this->CPHA == CPHA_2Edge)
		{
			//时钟相位 在串行同步时钟的第二个跳变沿（上升或下降）数据被采样
			//测试通过			
			for (i = 0; i < 8; i++)
			{
				this->_clk = 0;
				Sys.Delay(this->delayus);
				if (data &(1 << (8 - i - 1)))
				{
					this->_mosi = 1;
				}
				else
				{
					this->_mosi = 0;
				}
				Sys.Delay(this->delayus);
				this->_clk = 1;
				Sys.Delay(this->delayus);
				ret <<= 1;
				if (this->_miso.Read())
				{
					ret |= 1;
				}
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

void SpiSoft::Open()
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
	this->_clk.Open();
	this->_miso.Open();
	this->_mosi.Open();
	this->_nss.Open();
}
void SpiSoft::Close()
{
	//测试，等待删除
	for (int i = 0; i < 8; i++)
	{
		this->_clk = 0;
		this->_mosi = 1;
		Sys.Delay(this->delayus);
		this->_clk = 1;
		this->_mosi = 0;
	}
}

// 拉低NSS，开始传输
void SpiSoft::Start()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 0;
	}
	// 开始新一轮事务操作，错误次数清零
	//Error = 0;
}

// 拉高NSS，停止传输
void SpiSoft::Stop()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 1;
	}
}
// 基础读写
uint8_t Spi::Write(uint8_t data)
{
#if defined STM32F0
	int retry;
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		SPI_SendData8((SPI_TypeDef*)(this->_SPI), data);
		//是否发送成功
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}

		return SPI_ReceiveData8((SPI_TypeDef*)(this->_SPI)); //返回通过SPIx最近接收的数据            
	default:
		return  0;
	}
#elif defined STM32F1
	int retry;
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);
		//是否发送成功
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}

		return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
	default:
		return  0;
	}
#elif defined STM32F4
	int retry;
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);

		//是否发送成功
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}

		return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
	default:
		return  0;
	}
#endif
}

int Spi::GetPre(int index, uint32_t &speedHz)
{
#if defined STM32F0
	// 自动计算稍低于速度speedHz的分频
	uint16_t pre = SPI_BaudRatePrescaler_2;
	uint32_t clock = Sys.Clock >> 1;
	while (pre <= SPI_BaudRatePrescaler_256)
	{
		if (clock <= speedHz)
			break;
		clock >>= 1;
		pre += (SPI_BaudRatePrescaler_4 - SPI_BaudRatePrescaler_2);
	}
	if (pre > SPI_BaudRatePrescaler_256)
	{
		debug_printf("Spi%d::Init Error! speedHz=%d mush be dived with %dMHz\r\n", index, speedHz, Sys.Clock);
		return  -1;
	}

	speedHz = clock;
	return pre;
#elif defined STM32F1
	// 自动计算稍低于速度speedHz的分频
	uint16_t pre = SPI_BaudRatePrescaler_2;
	uint32_t clock = Sys.Clock >> 1;
	while (pre <= SPI_BaudRatePrescaler_256)
	{
		if (clock <= speedHz)
			break;
		clock >>= 1;
		pre += (SPI_BaudRatePrescaler_4 - SPI_BaudRatePrescaler_2);
	}
	if (pre > SPI_BaudRatePrescaler_256)
	{
		debug_printf("Spi%d::Init Error! speedHz=%d mush be dived with %dMHz\r\n", index, speedHz, Sys.Clock);
		return  -1;
	}

	speedHz = clock;
	return pre;
#elif defined STM32F4
	// 自动计算稍低于速度speedHz的分频
	uint16_t pre = SPI_BaudRatePrescaler_2;
	uint32_t clock = Sys.Clock >> 1;
	while (pre <= SPI_BaudRatePrescaler_256)
	{
		if (clock <= speedHz)
			break;
		clock >>= 1;
		pre += (SPI_BaudRatePrescaler_4 - SPI_BaudRatePrescaler_2);
	}
	if (pre > SPI_BaudRatePrescaler_256)
	{
		debug_printf("Spi%d::Init Error! speedHz=%d mush be dived with %dMHz\r\n", index, speedHz, Sys.Clock);
		return  -1;
	}

	speedHz = clock;
	return pre;
#endif
}
void Spi::OnInit()
{
#if defined STM32F0
	//以上为历史内容
	switch (this->_index)
	{
	case Spi1:
		//				// SPI都在GPIO_AF_0分组内
		//        GPIO_PinAFConfig(_GROUP(ps[1]), _PIN(ps[1]), GPIO_AF_0);
		//        GPIO_PinAFConfig(_GROUP(ps[2]), _PIN(ps[2]), GPIO_AF_0);
		//        GPIO_PinAFConfig(_GROUP(ps[3]), _PIN(ps[3]), GPIO_AF_0)
		this->SetPin(PB3, PB4, PB5);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0); //PB3复用为 SPI1
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_0); //PB4复用为 SPI1
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0); //PB5复用为 SPI1
		this->_SPI = SPI1;
		break;
	case Spi2:
		this->SetPin(PB13, PB14, PB15);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0); //PB13复用为 SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0); //PB14复用为 SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0); //PB15复用为 SPI2
		this->_SPI = SPI2;
		break;
	case Spi3:
		break;
	default:
		break;
	}
	/////////////////////////////////////////////////////////////
	SPI_InitTypeDef SPI_InitStructure;
	SPI_StructInit(&SPI_InitStructure);
	switch (this->_index)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		break;
	case Spi2:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		break;
	case Spi3:
		break;
	default:
		break;
	}
	//this->Stop();
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // 主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 数据大小8位 SPI发送接收8位帧结构
	if (this->CPOL == CPOL_Low)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // 时钟极性，空闲时为高
	}
	else if (this->CPOL == CPOL_High)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // 时钟极性，空闲时为高
	}
	else {}
	if (this->CPHA == CPHA_1Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // 第1个边沿有效，上升沿为采样时刻
	}
	else if (this->CPHA == CPHA_2Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // 第2个边沿有效，上升沿为采样时刻
	}
	else {}
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制    
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // 高位在前。指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRC值计算的多项式
	switch (this->_index)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //判断有效性
		SPI1->CR1 &= 0XFFC7; //位3-5清零，用来设置波特率
		SPI1->CR1 |= SPI_BaudRatePrescaler_64; //设置SPI1速度 设置为21M时钟,高速模式 
		SPI_Cmd(SPI1, ENABLE); //使能SPI1
		break;
	case Spi2:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		break;
	case Spi3:
		break;
	default:
		break;
	}
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Init((SPI_TypeDef*)(this->_SPI), &SPI_InitStructure);
		SPI_RxFIFOThresholdConfig((SPI_TypeDef*)(this->_SPI), SPI_RxFIFOThreshold_QF);
		break;
	default:
		break;
	}
#elif defined STM32F1
	//以上为历史内容
	switch (this->_index)
	{
	case Spi1:
		this->SetPin(PA5, PA6, PA7);//this->SetPin(PA5, PA6, PA7, PA4);                
		this->_SPI = SPI1;
		break;
	case Spi2:
		this->_SPI = SPI2;
		break;
	case Spi3:
		this->_SPI = SPI3;
		break;
	default:
		break;
	}
	/////////////////////////////////////////////////////////////
	SPI_InitTypeDef SPI_InitStructure;
	SPI_StructInit(&SPI_InitStructure);
	switch (this->_index)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		break;
	case Spi2:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		break;
	case Spi3:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		break;
	default:
		break;
	}
	//this->Stop();
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // 主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 数据大小8位 SPI发送接收8位帧结构
	if (this->CPOL == CPOL_Low)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // 时钟极性，空闲时为高
	}
	else if (this->CPOL == CPOL_High)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // 时钟极性，空闲时为高
	}
	else {}
	if (this->CPHA == CPHA_1Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // 第1个边沿有效，上升沿为采样时刻
	}
	else if (this->CPHA == CPHA_2Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // 第2个边沿有效，上升沿为采样时刻
	}
	else {}
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制    
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // 高位在前。指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRC值计算的多项式
	switch (this->_index)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		break;
	case Spi2:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		break;
	case Spi3:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		break;
	default:
		break;
	}
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Init((SPI_TypeDef*)(this->_SPI), &SPI_InitStructure);
		break;
	default:
		break;
	}
#elif defined STM32F4
	//以上为历史内容
	switch (this->_index)
	{
	case Spi1:
		this->SetPin(PB3, PB4, PB5);//this->SetPin(PB3, PB4, PB5, PB14);//this->SetPin(PA5, PA6, PA7, PA4);/原生SPI1
			//				uint8_t afs[] = 
			//        {
			//            GPIO_AF_SPI1, GPIO_AF_SPI2, GPIO_AF_SPI3, GPIO_AF_SPI4, GPIO_AF_SPI5, GPIO_AF_SPI6
			//        };
			//        GPIO_PinAFConfig(_GROUP(ps[1]), _PIN(ps[1]), afs[_index]);
			//        GPIO_PinAFConfig(_GROUP(ps[2]), _PIN(ps[2]), afs[_index]);
			//        GPIO_PinAFConfig(_GROUP(ps[3]), _PIN(ps[3]), afs[_index]);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); //PB3复用为 SPI1
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); //PB4复用为 SPI1
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); //PB5复用为 SPI1
		this->_SPI = SPI1;
		break;
	case Spi2:
		this->SetPin(PB13, PB14, PB15);//this->SetPin(PB13, PB14, PB15, PB12); //原生SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); //PB13复用为 SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); //PB14复用为 SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); //PB15复用为 SPI2
		this->_SPI = SPI2;
		break;
	case Spi3:
		this->SetPin(PC10, PC11, PC12);//this->SetPin(PC10, PC11, PC12, PA15);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3); //PC10复用为 SPI3
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3); //PC11复用为 SPI3
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3); //PC12复用为 SPI3
		this->_SPI = SPI3;
		break;
	default:
		break;
	}
	/////////////////////////////////////////////////////////////
	SPI_InitTypeDef SPI_InitStructure;
	SPI_StructInit(&SPI_InitStructure);
	switch (this->_index)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		break;
	case Spi2:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		break;
	case Spi3:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		break;
	default:
		break;
	}
	//this->Stop();
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // 主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 数据大小8位 SPI发送接收8位帧结构
	if (this->CPOL == CPOL_Low)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // 时钟极性，空闲时为高
	}
	else if (this->CPOL == CPOL_High)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // 时钟极性，空闲时为高
	}
	else {}
	if (this->CPHA == CPHA_1Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // 第1个边沿有效，上升沿为采样时刻
	}
	else if (this->CPHA == CPHA_2Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // 第2个边沿有效，上升沿为采样时刻
	}
	else {}
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制    
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // 高位在前。指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRC值计算的多项式
	switch (this->_index)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		//SPI1速度设置函数
			//SPI速度=fAPB2/分频系数
			//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
			//fAPB2时钟一般为84Mhz：
		assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //判断有效性
		SPI1->CR1 &= 0XFFC7; //位3-5清零，用来设置波特率
		SPI1->CR1 |= SPI_BaudRatePrescaler_4; //设置SPI1速度 设置为21M时钟,高速模式 
		SPI_Cmd(SPI1, ENABLE); //使能SPI1
		break;
	case Spi2:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		break;
	case Spi3:
		RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		break;
	default:
		break;
	}
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Init((SPI_TypeDef*)(this->_SPI), &SPI_InitStructure);
		break;
	default:
		break;
	}
#endif
}

void Spi::OnOpen()
{
#if defined STM32F0
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Cmd((SPI_TypeDef*)(this->_SPI), ENABLE);
		break;
	default:
		break;
	}
#elif defined STM32F1
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Cmd((SPI_TypeDef*)(this->_SPI), ENABLE);
		break;
	default:
		break;
	}
#elif defined STM32F4
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Cmd((SPI_TypeDef*)(this->_SPI), ENABLE);
		break;
	default:
		break;
	}
#endif
}

void Spi::OnClose()
{
#if defined STM32F0
	this->Stop();
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Cmd((SPI_TypeDef*)(this->_SPI), DISABLE);
		break;
	default:
		break;
	}
	SPI_I2S_DeInit((SPI_TypeDef*)(this->_SPI));
	debug_printf("    CLK : ");
	this->_clk.Set(P0);
	debug_printf("    MISO: ");
	this->_miso.Set(P0);
	debug_printf("    MOSI: ");
	this->_mosi.Set(P0);
	debug_printf("    NSS : ");
	this->_nss.Set(P0);

	this->Pins[0] = P0;
	this->Pins[1] = P0;
	this->Pins[2] = P0;
	this->Pins[3] = P0;
#elif defined STM32F1
	this->Stop();
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Cmd((SPI_TypeDef*)(this->_SPI), DISABLE);
		break;
	default:
		break;
	}
	SPI_I2S_DeInit((SPI_TypeDef*)(this->_SPI));
	debug_printf("    CLK : ");
	this->_clk.Set(P0);
	debug_printf("    MISO: ");
	this->_miso.Set(P0);
	debug_printf("    MOSI: ");
	this->_mosi.Set(P0);
	debug_printf("    NSS : ");
	this->_nss.Set(P0);

	this->Pins[0] = P0;
	this->Pins[1] = P0;
	this->Pins[2] = P0;
	this->Pins[3] = P0;
#elif defined STM32F4
	this->Stop();
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		SPI_Cmd((SPI_TypeDef*)(this->_SPI), DISABLE);
		break;
	default:
		break;
	}
	SPI_I2S_DeInit((SPI_TypeDef*)(this->_SPI));
	debug_printf("    CLK : ");
	this->_clk.Set(P0);
	debug_printf("    MISO: ");
	this->_miso.Set(P0);
	debug_printf("    MOSI: ");
	this->_mosi.Set(P0);
	debug_printf("    NSS : ");
	this->_nss.Set(P0);

	this->Pins[0] = P0;
	this->Pins[1] = P0;
	this->Pins[2] = P0;
	this->Pins[3] = P0;
#endif
}
uint16_t Spi::Write16(uint16_t data)
{
#if defined STM32F0
	int retry;
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		// 双字节操作，超时次数加倍
		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		SPI_I2S_SendData16((SPI_TypeDef*)(this->_SPI), data);

		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		return SPI_I2S_ReceiveData16((SPI_TypeDef*)(this->_SPI));
	default:
		return 0;
	}
#elif defined STM32F1
	int retry;
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		// 双字节操作，超时次数加倍
		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);

		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
	default:
		return 0;
	}
#elif defined STM32F4
	int retry;
	switch (this->_index)
	{
	case Spi1:
	case Spi2:
	case Spi3:
		// 双字节操作，超时次数加倍
		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);

		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// 超时处理
		}
		return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
	default:
		return 0;
	}
#endif
}
