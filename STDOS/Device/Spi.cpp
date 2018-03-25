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

    this->Retry = 200; //默认重试次数为200
}

Spi::Spi()
{
    this->Init();
    this->_index = 0xff;
}

// 使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度
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
    // 自动计算稍低于速度speedHz的分频
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

// 批量读写。以字节数组长度为准
void Spi::Write(const Buffer &bs){

}
void Spi::Read(Buffer &bs){

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
            //时钟相位 在串行同步时钟的第二个跳变沿（上升或下降）数据被采样
            //需要检查
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
        //时钟极性，空闲时为高
        if (this->CPHA == CPHA_1Edge)
        {
            //时钟相位 在串行同步时钟的第一个跳变沿（上升或下降）数据被采样
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
            //时钟相位 在串行同步时钟的第二个跳变沿（上升或下降）数据被采样
            //测试通过			
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
	//测试，等待删除
	for(int i=0;i<8;i++)
	{
		this->_clk=0;
		this->_mosi=1;
		Sys.Delay(this->delayus);
		this->_clk=1;
		this->_mosi=0;
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
