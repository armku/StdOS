#include "Sys.h"
#include "Port.h"
#include "Spi.h"
#include "string.h"

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

Spi::Spi()
{
    Init();
}

Spi::Spi(SPI_TypeDef *spi, uint speedHz, bool useNss)
{
    Init();

    Init(spi, speedHz, useNss);
}

Spi::~Spi()
{
    debug_printf("Spi:Spi%d\r\n", _index + 1);

    Close();
}

void Spi::Init()
{
    _index = 0xFF;
    Retry = 200;
    Opened = false;
}

void Spi::Init(SPI_TypeDef *spi, uint speedHz, bool useNss)
{
    assert_param(spi);
    #if 0
        SPI_TypeDef *g_Spis[] = SPIS;
        _index = 0xFF;
        for (int i = 0; i < ArrayLength(g_Spis); i++)
        {
            if (g_Spis == spi)
            {
                _index = i;
                break;
            }
        }
        assert_param(_index < ArrayLength(g_Spis));

        SPI = g_Spis[_index];

        Pin g_Spi_Pins_Map[][4] = SPI_PINS_FULLREMAP;
        Pin *ps = g_Spi_Pins_Map[_index]; //ѡ��spi����
        memcpy(Pins, ps, sizeof(Pins));
    #endif 
    if (!useNss)
        Pins[0] = P0;

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

void Spi::SetPin(Pin clk, Pin miso, Pin mosi, Pin nss)
{
    if (nss != P0)
        Pins[0] = nss;
    if (clk != P0)
        Pins[1] = clk;
    if (miso != P0)
        Pins[2] = miso;
    if (mosi != P0)
        Pins[3] = mosi;
}

void Spi::GetPin(Pin *clk, Pin *miso, Pin *mosi, Pin *nss)
{
    if (nss)
         *nss = Pins[0];
    if (clk)
         *clk = Pins[1];
    if (miso)
         *miso = Pins[2];
    if (mosi)
         *mosi = Pins[3];
}

void Spi::Open()
{
    if (Opened)
        return ;

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
    uint speedHz = Speed;
    int pre = GetPre(_index, &speedHz);
    if (pre ==  - 1)
        return ;

    Pin *ps = Pins;
    // �˿����ã�����Spi����ʱ���ͷ�
    debug_printf("    CLK : ");
    _clk.Set(ps[1]);
    debug_printf("    MISO: ");
    _miso.Set(ps[2]);
    debug_printf("    MOSI: ");
    _mosi.Set(ps[3]);

    if (ps[0] != P0)
    {
        debug_printf("    NSS : ");
        _nss.OpenDrain = false;
        _nss.Set(ps[0]);
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

    Opened = true;
}

void Spi::Close()
{
    if (!Opened)
        return ;

    Stop();

    SPI_Cmd(SPI, DISABLE);
    SPI_I2S_DeInit(SPI);

    debug_printf("    CLK : ");
    _clk.Set(P0);
    debug_printf("    MISO: ");
    _miso.Set(P0);
    debug_printf("    MOSI: ");
    _mosi.Set(P0);
    debug_printf("    NSS : ");
    _nss.Set(P0);

    Opened = false;
}

byte Spi::Write(byte data)
{
    if (!Opened)
        Open();

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
    if (!Opened)
        Open();

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
    if (!_nss.Empty())
        _nss = false;

    // ��ʼ��һ����������������������
    Error = 0;
}

// ����NSS��ֹͣ����
void Spi::Stop()
{
    if (!_nss.Empty())
        _nss = true;
}
