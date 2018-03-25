#include "Sys.h"
#include "Spi.h"

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
Spi::Spi(SPI spi,CPOLTYPE cpol,CPHATYPE cpha, uint32_t speedHz)
{
	this->CPOL=cpol;
	this->CPHA=cpha;
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
    if (pre ==  - 1)
        return ;
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
void Spi::Write(const Buffer &bs){

}
void Spi::Read(Buffer &bs){

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
SpiSoft::SpiSoft(CPOLTYPE cpol,CPHATYPE cpha,uint32_t speedHz)
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
                if (data &(1 << (8-i - 1)))
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
                if (data &(1 << (8-i - 1)))
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
                if (data &(1 << (8-i - 1)))
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
                if (data &(1 << (8-i - 1)))
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
        {}
    }
    else
    {}
        return ret;
}

void SpiSoft::Open()
{
	if(this->CPOL)
	{
		this->_clk=1;
	}
	else
	{
		this->_clk=0;
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
	for(int i=0;i<8;i++)
	{
		this->_clk=0;
		this->_mosi=1;
		Sys.Delay(this->delayus);
		this->_clk=1;
		this->_mosi=0;
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
