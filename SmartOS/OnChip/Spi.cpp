#include "Sys.h"
#include "Spi.h"
#include "Platform\stm32.h"

Spi::Spi(SPI spi)
{
	this->_index = 0xff;
	this->_index = spi;
}

void Spi::Init(CPOLTYPE cpol, CPHATYPE cpha)
{
	this->Retry = 200; //Ĭ�����Դ���Ϊ200
#if DEBUG
	debug_printf("Spi%d::Init\r\n", _index + 1);
#endif

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
		this->SetNss(PA4);
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
	this->Stop();
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
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
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
		SPI_Init(SPI1, &SPI_InitStructure);
		break;
	case Spi2:
		SPI_Init(SPI2, &SPI_InitStructure);
		break;
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
			//				byte afs[] = 
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
	this->Stop();
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
		SPI_Cmd(SPI1, ENABLE);
		break;
	case Spi2:
		SPI_Cmd(SPI2, ENABLE);
		break;
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

void Spi::SetPin(Pin clk, Pin miso, Pin mosi)
{
	this->_clk.SetPin(clk);
	this->_miso.SetPin(miso);
	this->_mosi.SetPin(mosi);
	
	this->_clk.pinMode(GPIO_AF_PP);
	this->_miso.pinMode(GPIO_AF_PP);
	this->_mosi.pinMode(GPIO_AF_PP);
}
void Spi::SetNss(Pin nss)
{
	this->_nss.SetPin(nss);
	this->_nss.pinMode(GPIO_Out_PP);
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
// ������д
byte Spi::Write(byte data)
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
		///* Loop while DR register in not emplty */
		//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

		///* Send byte through the SPI1 peripheral */
		//SPI_I2S_SendData(SPI1, byte);

		///* Wait to receive a byte */
		//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

		///* Return the byte read from the SPI bus */
		//return SPI_I2S_ReceiveData(SPI1);
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

ushort Spi::Write16(ushort data)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////SpiSoft////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
SpiSoft::SpiSoft()
{	

}
void SpiSoft::Init(CPOLTYPE cpol  , CPHATYPE cpha)
{
#ifdef STM32F0
	this->delayus = 0;
#elif defined STM32F1
	this->delayus = 10;
#elif defined STM32F4
	this->delayus = 0;
#endif
	this->CPOL = cpol;
	this->CPHA = cpha;
}

void SpiSoft::SetPin(Pin clk, Pin miso, Pin mosi)
{
	this->_clk.SetPin(clk);
	this->_mosi.SetPin(mosi);
	this->_miso.SetPin(miso);

	this->_clk.pinMode(GPIO_Out_PP);
	this->_mosi.pinMode(GPIO_Out_PP);//�˴����ô��󽫵����쳣
	this->_miso.pinMode(GPIO_IPU);	
}
void SpiSoft::SetNss(Pin nss)
{
	this->_nss.SetPin(nss);

	this->_nss.pinMode(GPIO_Out_PP);
	
}

/*---------------------------------------------------------
æ״̬�жϣ���ȴ�ʱ�䣬200 X 10 ms=2S
---------------------------------------------------------*/
byte SpiSoft::WaitBusy()
{
	ushort i;
	this->Start();
	i = 0;
	while (this->_miso.Read() > 0)
	{
		delay(10);
		i++;
		if (i > 200)
			return 1;
	}
	this->Stop();
	return 0;
}

//SPIд�ֽ�
byte SpiSoft::Write(byte data)
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
ushort SpiSoft::Write16(ushort data)
{
	ushort dat = 0;

	dat = this->Write((data >> 8) & 0xff);
	dat <<= 8;
	dat |= this->Write(data & 0xff);

	return dat;
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
}
void SpiSoft::Close()
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
