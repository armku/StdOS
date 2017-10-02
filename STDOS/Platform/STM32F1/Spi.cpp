#include "Sys.h"
#include "Spi.h"
#include "stm32f10x.h"

/* 串行外设接口(SPI)针脚 ------------------------------------------------------------------*/
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

// 基础读写
byte Spi::Write(byte data)
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
}

int Spi::GetPre(int index, uint &speedHz)
{
    // 自动计算稍低于速度speedHz的分频
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
	if(this->CPOL==CPOL_Low)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // 时钟极性，空闲时为高
	}
	else if(this->CPOL==CPOL_High)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // 时钟极性，空闲时为高
	}
	else{}
	if(this->CPHA==CPHA_1Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // 第1个边沿有效，上升沿为采样时刻
	}
	else if(this->CPHA==CPHA_2Edge)
	{
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // 第2个边沿有效，上升沿为采样时刻
	}
	else{}
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
}
