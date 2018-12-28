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

	this->Retry = 200; //Ĭ�����Դ���Ϊ200
}

Spi::Spi()
{
	this->Init();
	this->_index = 0xff;
}

// ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
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
	// �Զ������Ե����ٶ�speedHz�ķ�Ƶ
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

// ������д�����ֽ����鳤��Ϊ׼
void Spi::Write(void * buf, int len) {

}
void Spi::Read(void * buf, int len) {

}

// ����NSS����ʼ����
void Spi::Start()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 0;
	}
	// ��ʼ��һ����������������������
	this->Error = 0;
}

// ����NSS��ֹͣ����
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
æ״̬�жϣ���ȴ�ʱ�䣬200 X 10 ms=2S
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

//SPIд�ֽ�
uint8_t SpiSoft::Write(uint8_t data)
{
	uint8_t i;
	uint8_t ret = 0;
	if (this->CPOL == CPOL_Low)
	{
		//ʱ�Ӽ��ԣ�����ʱΪ��
		if (this->CPHA == CPHA_1Edge)
		{
			//ʱ����λ �ڴ���ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
			//����ͨ��
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
			//ʱ����λ �ڴ���ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
			//��Ҫ���
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
		//ʱ�Ӽ��ԣ�����ʱΪ��
		if (this->CPHA == CPHA_1Edge)
		{
			//ʱ����λ �ڴ���ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
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
			//ʱ����λ �ڴ���ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
			//����ͨ��			
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
	//���ԣ��ȴ�ɾ��
	for (int i = 0; i < 8; i++)
	{
		this->_clk = 0;
		this->_mosi = 1;
		Sys.Delay(this->delayus);
		this->_clk = 1;
		this->_mosi = 0;
	}
}

// ����NSS����ʼ����
void SpiSoft::Start()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 0;
	}
	// ��ʼ��һ����������������������
	//Error = 0;
}

// ����NSS��ֹͣ����
void SpiSoft::Stop()
{
	if (!this->_nss.Empty())
	{
		this->_nss = 1;
	}
}
// ������д
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
			// ��ʱ����
		}
		SPI_SendData8((SPI_TypeDef*)(this->_SPI), data);
		//�Ƿ��ͳɹ�
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
		}

		return SPI_ReceiveData8((SPI_TypeDef*)(this->_SPI)); //����ͨ��SPIx������յ�����            
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
			// ��ʱ����
		}
		SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);
		//�Ƿ��ͳɹ�
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
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
			// ��ʱ����
		}
		SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);

		//�Ƿ��ͳɹ�
		retry = Retry;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
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
	// �Զ������Ե����ٶ�speedHz�ķ�Ƶ
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
	// �Զ������Ե����ٶ�speedHz�ķ�Ƶ
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
	// �Զ������Ե����ٶ�speedHz�ķ�Ƶ
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
	//����Ϊ��ʷ����
	switch (this->_index)
	{
	case Spi1:
		//				// SPI����GPIO_AF_0������
		//        GPIO_PinAFConfig(_GROUP(ps[1]), _PIN(ps[1]), GPIO_AF_0);
		//        GPIO_PinAFConfig(_GROUP(ps[2]), _PIN(ps[2]), GPIO_AF_0);
		//        GPIO_PinAFConfig(_GROUP(ps[3]), _PIN(ps[3]), GPIO_AF_0)
		this->SetPin(PB3, PB4, PB5);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0); //PB3����Ϊ SPI1
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_0); //PB4����Ϊ SPI1
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0); //PB5����Ϊ SPI1
		this->_SPI = SPI1;
		break;
	case Spi2:
		this->SetPin(PB13, PB14, PB15);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0); //PB13����Ϊ SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0); //PB14����Ϊ SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0); //PB15����Ϊ SPI2
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
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ���ݴ�С8λ SPI���ͽ���8λ֡�ṹ
	if (this->CPOL == CPOL_Low)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ʱ�Ӽ��ԣ�����ʱΪ��
	}
	else if (this->CPOL == CPOL_High)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // ʱ�Ӽ��ԣ�����ʱΪ��
	}
	else {}
	if (this->CPHA == CPHA_1Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // ��1��������Ч��������Ϊ����ʱ��
	}
	else if (this->CPHA == CPHA_2Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // ��2��������Ч��������Ϊ����ʱ��
	}
	else {}
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����    
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // ��λ��ǰ��ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRCֵ����Ķ���ʽ
	switch (this->_index)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //�ж���Ч��
		SPI1->CR1 &= 0XFFC7; //λ3-5���㣬�������ò�����
		SPI1->CR1 |= SPI_BaudRatePrescaler_64; //����SPI1�ٶ� ����Ϊ21Mʱ��,����ģʽ 
		SPI_Cmd(SPI1, ENABLE); //ʹ��SPI1
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
	//����Ϊ��ʷ����
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
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ���ݴ�С8λ SPI���ͽ���8λ֡�ṹ
	if (this->CPOL == CPOL_Low)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ʱ�Ӽ��ԣ�����ʱΪ��
	}
	else if (this->CPOL == CPOL_High)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // ʱ�Ӽ��ԣ�����ʱΪ��
	}
	else {}
	if (this->CPHA == CPHA_1Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // ��1��������Ч��������Ϊ����ʱ��
	}
	else if (this->CPHA == CPHA_2Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // ��2��������Ч��������Ϊ����ʱ��
	}
	else {}
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����    
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // ��λ��ǰ��ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRCֵ����Ķ���ʽ
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
	//����Ϊ��ʷ����
	switch (this->_index)
	{
	case Spi1:
		this->SetPin(PB3, PB4, PB5);//this->SetPin(PB3, PB4, PB5, PB14);//this->SetPin(PA5, PA6, PA7, PA4);/ԭ��SPI1
			//				uint8_t afs[] = 
			//        {
			//            GPIO_AF_SPI1, GPIO_AF_SPI2, GPIO_AF_SPI3, GPIO_AF_SPI4, GPIO_AF_SPI5, GPIO_AF_SPI6
			//        };
			//        GPIO_PinAFConfig(_GROUP(ps[1]), _PIN(ps[1]), afs[_index]);
			//        GPIO_PinAFConfig(_GROUP(ps[2]), _PIN(ps[2]), afs[_index]);
			//        GPIO_PinAFConfig(_GROUP(ps[3]), _PIN(ps[3]), afs[_index]);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); //PB3����Ϊ SPI1
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); //PB4����Ϊ SPI1
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); //PB5����Ϊ SPI1
		this->_SPI = SPI1;
		break;
	case Spi2:
		this->SetPin(PB13, PB14, PB15);//this->SetPin(PB13, PB14, PB15, PB12); //ԭ��SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); //PB13����Ϊ SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); //PB14����Ϊ SPI2
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); //PB15����Ϊ SPI2
		this->_SPI = SPI2;
		break;
	case Spi3:
		this->SetPin(PC10, PC11, PC12);//this->SetPin(PC10, PC11, PC12, PA15);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3); //PC10����Ϊ SPI3
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3); //PC11����Ϊ SPI3
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3); //PC12����Ϊ SPI3
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
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ���ݴ�С8λ SPI���ͽ���8λ֡�ṹ
	if (this->CPOL == CPOL_Low)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ʱ�Ӽ��ԣ�����ʱΪ��
	}
	else if (this->CPOL == CPOL_High)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // ʱ�Ӽ��ԣ�����ʱΪ��
	}
	else {}
	if (this->CPHA == CPHA_1Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // ��1��������Ч��������Ϊ����ʱ��
	}
	else if (this->CPHA == CPHA_2Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // ��2��������Ч��������Ϊ����ʱ��
	}
	else {}
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����    
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // ��λ��ǰ��ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRCֵ����Ķ���ʽ
	switch (this->_index)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		//SPI1�ٶ����ú���
			//SPI�ٶ�=fAPB2/��Ƶϵ��
			//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
			//fAPB2ʱ��һ��Ϊ84Mhz��
		assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //�ж���Ч��
		SPI1->CR1 &= 0XFFC7; //λ3-5���㣬�������ò�����
		SPI1->CR1 |= SPI_BaudRatePrescaler_4; //����SPI1�ٶ� ����Ϊ21Mʱ��,����ģʽ 
		SPI_Cmd(SPI1, ENABLE); //ʹ��SPI1
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
		// ˫�ֽڲ�������ʱ�����ӱ�
		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
		}
		SPI_I2S_SendData16((SPI_TypeDef*)(this->_SPI), data);

		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
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
		// ˫�ֽڲ�������ʱ�����ӱ�
		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
		}
		SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);

		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
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
		// ˫�ֽڲ�������ʱ�����ӱ�
		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
		}
		SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);

		retry = Retry << 1;
		while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
		{
			if (--retry <= 0)
				return ++Error;
			// ��ʱ����
		}
		return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
	default:
		return 0;
	}
#endif
}
