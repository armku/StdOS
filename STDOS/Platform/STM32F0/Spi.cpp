#include "Sys.h"
#include "Spi.h"
#include "Platform\stm32.h"

// ������д
uint8_t Spi::Write(uint8_t data)
{
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
}

int Spi::GetPre(int index, uint32_t &speedHz)
{
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
}
void Spi::OnInit()
{
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
}

void Spi::OnOpen()
{
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
}

void Spi::OnClose()
{
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
}
uint16_t Spi::Write16(uint16_t data)
{
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
}
