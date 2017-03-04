#include "Sys.h"
#include "Port.h"
#include "Spi.h"
#include "string.h"

// NSS/CLK/MISO/MOSI
#define SPIS {SPI1,SPI2,SPI3}
#define SPI_PINS_FULLREMAP {PA4,PA5,PA6,PA7,PB12,PB13,PB14,PB15,PA15,PB3,PB4,PB5}   //��Ҫ����
int GetPre(int index, uint *speedHz)
{
    // �Զ������Ե����ٶ�speedHz�ķ�Ƶ
    ushort pre = SPI_BaudRatePrescaler_2;
    uint clock = Sys.Clock >> 1;
    while (pre <= SPI_BaudRatePrescaler_256)
    {
        if (clock <=  *speedHz)
            break;
        clock >>= 1;
        pre += (SPI_BaudRatePrescaler_4 - SPI_BaudRatePrescaler_2);
    }
    if (pre > SPI_BaudRatePrescaler_256)
    {
        debug_printf("Spi%d::Init Error! speedHz=%d mush be dived with %d\r\n", index,  *speedHz, Sys.Clock);
        return  - 1;
    }

    *speedHz = clock;
    return pre;
}

Spi::Spi(int spiIndex, uint speedHz, bool useNss)
{
    SPI_TypeDef *g_Spis[] = SPIS;
    _index = 0xFF;
    Retry = 200;
    Init(g_Spis[spiIndex], speedHz, useNss);
    #if DEBUG
        int k = Speed / 1000;
        int m = k / 1000;
        k -= m * 1000;
        if (k == 0)
            debug_printf("Spi%d::Open %dMHz\r\n", _index + 1, m);
        else
            debug_printf("Spi%d::Open %d.%dMHz\r\n", _index + 1, m, k);
    #endif 

    // �Զ������Ե����ٶ�speedHz�ķ�Ƶ    
    int pre = GetPre(_index, &speedHz);
    if (pre ==  - 1)
        return ;

    const Pin g_Spi_Pins_Map[][4] = SPI_PINS_FULLREMAP;
    // �˿����ã�����Spi����ʱ���ͷ�
    debug_printf("    CLK : ");
    this->pClk = new AlternatePort(g_Spi_Pins_Map[_index][1]);
    debug_printf("    MISO: ");
    this->pMiso = new AlternatePort(g_Spi_Pins_Map[_index][2]);
    debug_printf("    MOSI: ");
    this->pMosi = new AlternatePort(g_Spi_Pins_Map[_index][3]);
    if (useNss)
    {
        this->pNss = new OutputPort(g_Spi_Pins_Map[_index][0]);
    }
    else
    {
        this->pNss = new OutputPort(P0);
    }

    // ʹ��SPIʱ��
    switch (_index)
    {
        case 0:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
            break;
            #if defined(STM32F1) || defined(STM32F4)
            case 1:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
                break;
            case 2:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
                break;
                #if defined(STM32F4)
                case 3:
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
                    break;
                case 4:
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
                    break;
                case 5:
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);
                    break;
                #endif 
            #endif 
    }

    #if defined(STM32F0)
        // SPI����GPIO_AF_0������
        GPIO_PinAFConfig(_GROUP(ps[1]), _PIN(ps[1]), GPIO_AF_0);
        GPIO_PinAFConfig(_GROUP(ps[2]), _PIN(ps[2]), GPIO_AF_0);
        GPIO_PinAFConfig(_GROUP(ps[3]), _PIN(ps[3]), GPIO_AF_0);
    #elif defined(STM32F4)
        byte afs[] = 
        {
            GPIO_AF_SPI1, GPIO_AF_SPI2, GPIO_AF_SPI3, GPIO_AF_SPI4, GPIO_AF_SPI5, GPIO_AF_SPI6
        };
        GPIO_PinAFConfig(_GROUP(ps[1]), _PIN(ps[1]), afs[_index]);
        GPIO_PinAFConfig(_GROUP(ps[2]), _PIN(ps[2]), afs[_index]);
        GPIO_PinAFConfig(_GROUP(ps[3]), _PIN(ps[3]), afs[_index]);
    #endif 

    Stop();
    SPI_I2S_DeInit(SPI);
    //SPI_DeInit(SPI);    // SPI_I2S_DeInit�ĺ궨�����

    SPI_InitTypeDef sp;
    SPI_StructInit(&sp);
    sp.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
    sp.SPI_Mode = SPI_Mode_Master; // ��ģʽ
    sp.SPI_DataSize = SPI_DataSize_8b; // ���ݴ�С8λ SPI���ͽ���8λ֡�ṹ
    sp.SPI_CPOL = SPI_CPOL_Low; // ʱ�Ӽ��ԣ�����ʱΪ��
    sp.SPI_CPHA = SPI_CPHA_1Edge; // ��1��������Ч��������Ϊ����ʱ��
    sp.SPI_NSS = SPI_NSS_Soft; // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    sp.SPI_BaudRatePrescaler = pre; // 8��Ƶ��9MHz ���岨����Ԥ��Ƶ��ֵ
    sp.SPI_FirstBit = SPI_FirstBit_MSB; // ��λ��ǰ��ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    sp.SPI_CRCPolynomial = 7; // CRCֵ����Ķ���ʽ

    SPI_Init(SPI, &sp);
    SPI_Cmd(SPI, ENABLE);

    Stop();
}

Spi::~Spi()
{
    debug_printf("Spi:Spi%d\r\n", _index + 1);

    Close();
}

void Spi::Init(SPI_TypeDef *spi, uint speedHz, bool useNss)
{
    assert_param(spi);

    SPI_TypeDef *g_Spis[] = SPIS;
    this->_index = 0xFF;
    for (int i = 0; i < ArrayLength(g_Spis); i++)
    {
        if (g_Spis[i] == spi)
        {
            _index = i;
            break;
        }
    }
    assert_param(_index < ArrayLength(g_Spis));

    this->SPI = g_Spis[_index];


    #if DEBUG
        int k = speedHz / 1000;
        int m = k / 1000;
        k -= m * 1000;
        if (k == 0)
            debug_printf("Spi%d::Init %dMHz Nss:%d\r\n", _index + 1, m, useNss);
        else
            debug_printf("Spi%d::Init %d.%dMHz Nss:%d\r\n", _index + 1, m, k, useNss);
    #endif 

    // �Զ������Ե����ٶ�speedHz�ķ�Ƶ
    int pre = GetPre(_index, &speedHz);
    if (pre ==  - 1)
        return ;

    Speed = speedHz;
}

void Spi::Close()
{
    Stop();

    SPI_Cmd(SPI, DISABLE);
    SPI_I2S_DeInit(SPI);

    debug_printf("    CLK : ");
    this->pClk->Set(P0);
    debug_printf("    MISO: ");
    this->pMiso->Set(P0);
    debug_printf("    MOSI: ");
    this->pMosi->Set(P0);
    debug_printf("    NSS : ");
    this->pNss->Set(P0);
}

byte Spi::Write(byte data)
{
    int retry = Retry;
    while (SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        if (--retry <= 0)
            return ++Error;
        // ��ʱ����
    }

    #ifndef STM32F0
        SPI_I2S_SendData(SPI, data);
    #else 
        SPI_SendData8(SPI, data);
    #endif 

    retry = Retry;
    while (SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE) == RESET)
    //�Ƿ��ͳɹ�
    {
        if (--retry <= 0)
            return ++Error;
        // ��ʱ����
    }
    #ifndef STM32F0
        return SPI_I2S_ReceiveData(SPI);
    #else 
        return SPI_ReceiveData8(SPI); //����ͨ��SPIx������յ�����
    #endif 
}

ushort Spi::Write16(ushort data)
{
    // ˫�ֽڲ�������ʱ�����ӱ�
    int retry = Retry << 1;
    while (SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        if (--retry <= 0)
            return ++Error;
        // ��ʱ����
    }

    #ifndef STM32F0
        SPI_I2S_SendData(SPI, data);
    #else 
        SPI_I2S_SendData16(SPI, data);
    #endif 

    retry = Retry << 1;
    while (SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if (--retry <= 0)
            return ++Error;
        // ��ʱ����
    }

    #ifndef STM32F0
        return SPI_I2S_ReceiveData(SPI);
    #else 
        return SPI_I2S_ReceiveData16(SPI);
    #endif 
}

// ����NSS����ʼ����
void Spi::Start()
{
    if (!this->pNss->Empty())
        *this->pNss = false;

    // ��ʼ��һ����������������������
    Error = 0;
}

// ����NSS��ֹͣ����
void Spi::Stop()
{
    if (!this->pNss->Empty())
        *this->pNss = true;
}
