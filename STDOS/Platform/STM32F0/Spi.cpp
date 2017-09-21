#include "Sys.h"
#include "Spi.h"
#include "stm32f0xx.h"


/* ��������ӿ�(SPI)��� ------------------------------------------------------------------*/
//#define SPI_NSS_PINS  {4, 28, 15} // PA4, PB12, PA15
//#define SPI_SCLK_PINS {5, 29, 19} // PA5, PB13, PB3
//#define SPI_MISO_PINS {6, 30, 20} // PA6, PB14, PB4
//#define SPI_MOSI_PINS {7, 31, 21} // PA7, PB15, PB5

#define SPI_PINS_FULLREMAP	{\
/*	NSS  CLK  MISO MOSI	*/\
{PA4, PA5, PA6, PA7 },\
{PB12,PB13,PB14,PB15},\
{PA15,PB3, PB4, PB5 }\
}

// ������д
byte Spi::Write(byte data)
{
    switch (this->_index)
    {
        case Spi1:
        case Spi2:
        case Spi3:
            int retry = Retry;
            while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
            {
                if (--retry <= 0)
                    return ++Error;
                // ��ʱ����
            }
            #ifdef STM32F0
                SPI_SendData8((SPI_TypeDef*)(this->_SPI), data);
            #elif defined STM32F1
                SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);
            #elif defined STM32F4
                SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);
            #else 

            #endif 

            //�Ƿ��ͳɹ�
            retry = Retry;
            while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
            {
                if (--retry <= 0)
                    return ++Error;
                // ��ʱ����
            }

            #ifdef STM32F0
                return SPI_ReceiveData8((SPI_TypeDef*)(this->_SPI)); //����ͨ��SPIx������յ�����
            #elif defined STM32F1
                return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
            #elif defined STM32F4 
                return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
            #endif 
        default:
            return  0;
    }
}

int Spi::GetPre(int index, uint &speedHz)
{
    // �Զ������Ե����ٶ�speedHz�ķ�Ƶ
    ushort pre = SPI_BaudRatePrescaler_2;
    uint clock = Sys.Clock >> 1;
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
        return  - 1;
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
            #ifdef STM32F0
                //				// SPI����GPIO_AF_0������
                //        GPIO_PinAFConfig(_GROUP(ps[1]), _PIN(ps[1]), GPIO_AF_0);
                //        GPIO_PinAFConfig(_GROUP(ps[2]), _PIN(ps[2]), GPIO_AF_0);
                //        GPIO_PinAFConfig(_GROUP(ps[3]), _PIN(ps[3]), GPIO_AF_0)
				this->SetPin(PB3, PB4, PB5);
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0); //PB3����Ϊ SPI1
                GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_0); //PB4����Ϊ SPI1
                GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0); //PB5����Ϊ SPI1
            #elif defined STM32F1
                this->SetPin(PA5, PA6, PA7);//this->SetPin(PA5, PA6, PA7, PA4);                
            #elif defined STM32F4				
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
            #endif 
            this->_SPI = SPI1;
            break;
        case Spi2:
            #ifdef STM32F0
				this->SetPin(PB13, PB14, PB15);
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0); //PB13����Ϊ SPI2
                GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0); //PB14����Ϊ SPI2
                GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0); //PB15����Ϊ SPI2
            #elif defined STM32F1
                //this->SetPin(PA5, PA6, PA7);//this->SetPin(PA5, PA6, PA7, PA4);                
            #elif defined STM32F4
                this->SetPin(PB13, PB14, PB15);//this->SetPin(PB13, PB14, PB15, PB12); //ԭ��SPI2
                GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); //PB13����Ϊ SPI2
                GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); //PB14����Ϊ SPI2
                GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); //PB15����Ϊ SPI2
            #endif 
            this->_SPI = SPI2;
            break;
        case Spi3:
            #ifdef STM32F0

            #elif defined STM32F1
                //this->SetPin(PA5, PA6, PA7);//this->SetPin(PA5, PA6, PA7, PA4);                
            #elif defined STM32F4
                this->SetPin(PC10, PC11, PC12);//this->SetPin(PC10, PC11, PC12, PA15);
                GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3); //PC10����Ϊ SPI3
                GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3); //PC11����Ϊ SPI3
                GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3); //PC12����Ϊ SPI3
            #endif 
			#if defined(STM32F1) || defined(STM32F4) 
            this->_SPI = SPI3;
			#elif defined STM32F0
			#endif
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
			#if defined(STM32F1) || defined(STM32F4) 
            RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
			#elif defined STM32F0
			#endif
            break;
        default:
            break;
    }
    //this->Stop();
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // ��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ���ݴ�С8λ SPI���ͽ���8λ֡�ṹ
	if(this->CPOL==CPOL_Low)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ʱ�Ӽ��ԣ�����ʱΪ��
	}
	else if(this->CPOL==CPOL_High)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // ʱ�Ӽ��ԣ�����ʱΪ��
	}
	else{}
	if(this->CPHA==CPHA_1Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // ��1��������Ч��������Ϊ����ʱ��
	}
	else if(this->CPHA==CPHA_2Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // ��2��������Ч��������Ϊ����ʱ��
	}
	else{}
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����    
    #ifdef STM32F0
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    #elif defined STM32F1
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    #elif defined STM32F4
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    #endif 
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // ��λ��ǰ��ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7; // CRCֵ����Ķ���ʽ
    switch (this->_index)
    {
        case Spi1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
            #ifdef STM32F0
				assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //�ж���Ч��
                SPI1->CR1 &= 0XFFC7; //λ3-5���㣬�������ò�����
                SPI1->CR1 |= SPI_BaudRatePrescaler_64; //����SPI1�ٶ� ����Ϊ21Mʱ��,����ģʽ 
                SPI_Cmd(SPI1, ENABLE); //ʹ��SPI1
            #elif defined STM32F1

            #elif defined STM32F4
                //SPI1�ٶ����ú���
                //SPI�ٶ�=fAPB2/��Ƶϵ��
                //@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
                //fAPB2ʱ��һ��Ϊ84Mhz��
                assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //�ж���Ч��
                SPI1->CR1 &= 0XFFC7; //λ3-5���㣬�������ò�����
                SPI1->CR1 |= SPI_BaudRatePrescaler_4; //����SPI1�ٶ� ����Ϊ21Mʱ��,����ģʽ 
                SPI_Cmd(SPI1, ENABLE); //ʹ��SPI1
            #endif 
            break;
        case Spi2:
            RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            break;
        case Spi3:
			#if defined(STM32F1) || defined(STM32F4) 
            RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
			#elif defined STM32F0
			#endif
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
			#ifdef STM32F0
				SPI_RxFIFOThresholdConfig((SPI_TypeDef*)(this->_SPI), SPI_RxFIFOThreshold_QF);
			#endif
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
ushort Spi::Write16(ushort data)
{
    switch (this->_index)
    {
        case Spi1:
        case Spi2:
        case Spi3:
            // ˫�ֽڲ�������ʱ�����ӱ�
            int retry = Retry << 1;
            while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_TXE) == RESET)
            {
                if (--retry <= 0)
                    return ++Error;
                // ��ʱ����
            }
            #ifdef STM32F0
                SPI_I2S_SendData16((SPI_TypeDef*)(this->_SPI), data);
            #elif defined STM32F1
                SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);
            #elif defined STM32F4
                SPI_I2S_SendData((SPI_TypeDef*)(this->_SPI), data);
            #endif 

            retry = Retry << 1;
            while (SPI_I2S_GetFlagStatus((SPI_TypeDef*)(this->_SPI), SPI_I2S_FLAG_RXNE) == RESET)
            {
                if (--retry <= 0)
                    return ++Error;
                // ��ʱ����
            }
            #ifdef STM32F0
                return SPI_I2S_ReceiveData16((SPI_TypeDef*)(this->_SPI));
            #elif defined STM32F1
                return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
            #elif defined STM32F4
                return SPI_I2S_ReceiveData((SPI_TypeDef*)(this->_SPI));
            #endif 
        default:
            return 0;
    }
}
