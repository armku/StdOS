#include "nrf24l01.h"
#include "stm32f10x.h"
 
    #define TX_PLOAD_WIDTH  4   //发射数据通道有效数据宽度0~32Byte 
    #define RX_PLOAD_WIDTH  4

	#define CHANAL 40	//频道选择 

    // SPI(nRF24L01) commands ,	NRF的SPI命令宏定义，详见NRF功能使用文档
    #define NRF_READ_REG    0x00  // Define read command to register
    #define NRF_WRITE_REG   0x20  // Define write command to register
    #define RD_RX_PLOAD 0x61  // Define RX payload register address
    #define WR_TX_PLOAD 0xA0  // Define TX payload register address
    #define FLUSH_TX    0xE1  // Define flush TX register command
    #define FLUSH_RX    0xE2  // Define flush RX register command
    #define REUSE_TX_PL 0xE3  // Define reuse TX payload register command
    #define NOP         0xFF  
        // Define No Operation, might be used to read status register

    // SPI(nRF24L01) registers(addresses) ，NRF24L01 相关寄存器地址的宏定义
    #define CONFIG      0x00  // 'Config' register address
    #define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
    #define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
    #define SETUP_AW    0x03  // 'Setup address width' register address
    #define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
    #define RF_CH       0x05  // 'RF channel' register address
    #define RF_SETUP    0x06  // 'RF setup' register address
    #define STATUS      0x07  // 'Status' register address
    #define OBSERVE_TX  0x08  // 'Observe TX' register address
    #define CD          0x09  // 'Carrier Detect' register address
    #define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
    #define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
    #define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
    #define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
    #define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
    #define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
    #define TX_ADDR     0x10  // 'TX address' register address
    #define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
    #define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
    #define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
    #define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
    #define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
    #define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
    #define FIFO_STATUS 0x17  // 'FIFO Status Register' register address

uint8_t RX_BUF[RX_PLOAD_WIDTH]; //接收数据缓存
uint8_t TX_BUF[TX_PLOAD_WIDTH]; //发射数据缓存

void Delay(__IO u32 nCount)
{
    for (; nCount != 0; nCount--)
        ;
}
NRF24L01::NRF24L01()
{
	this->Local[0]=0x34;
	this->Local[1]=0x43;
	this->Local[2]=0x10;
	this->Local[3]=0x10;
	this->Local[4]=0x01;
	
	this->Remote[0]=0x34;
	this->Remote[1]=0x43;
	this->Remote[2]=0x10;
	this->Remote[3]=0x10;
	this->Remote[4]=0x01;	
}
NRF24L01::~NRF24L01()
{
	
}
/**
 * @brief  SPI的 I/O配置
 * @param  无
 * @retval 无
 */
void NRF24L01::Init(Spi* spi, Pin ce, Pin irq, Pin power)
{
	#if 1
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启相应IO端口的时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
        RCC_APB2Periph_GPIOG, ENABLE);
    
    /*配置 SPI_NRF_SPI的 SCK,MISO,MOSI引脚，GPIOA^5,GPIOA^6,GPIOA^7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   	#endif
    /* 这是自定义的宏，用于拉高csn引脚，NRF进入空闲状态 */
    this->_CSN = 1;
	
	this->_CE.Set(ce);
	this->_CE.Invert=0;
	this->_CE.OpenDrain=true;
	this->_CE.Open();
	
	this->Irq.Set(irq);
	this->Irq.Floating=false;
	this->Irq.Invert=0;
	this->Irq.Pull=InputPort::UP;
	this->Irq.Open();
	
	this->_spi=spi;
	this->_spi->Open();

}

/**
 * @brief   用于向NRF特定的寄存器写入数据
 * @param   
 *		@arg reg:NRF的命令+寄存器地址
 *		@arg dat:将要向寄存器写入的数据
 * @retval  NRF的status寄存器的状态
 */
uint8_t NRF24L01::WriteReg(uint8_t reg, uint8_t dat)
{
    uint8_t status;
    this->_CE = 0;
    /*置低CSN，使能SPI传输*/
	this->_CSN = 0;

    /*发送命令及寄存器号 */
    status = this->_spi->Write(reg);

    /*向寄存器写入数据*/
    this->_spi->Write(dat);

    /*CSN拉高，完成*/
	this->_CSN = 1;

    /*返回状态寄存器的值*/
    return (status);
}

/**
 * @brief   用于从NRF特定的寄存器读出数据
 * @param   
 *		@arg reg:NRF的命令+寄存器地址
 * @retval  寄存器中的数据
 */
uint8_t NRF24L01::ReadReg(uint8_t reg)
{
    uint8_t reg_val;

    this->_CE = 0;
    /*置低CSN，使能SPI传输*/
	this->_CSN = 0;

    /*发送寄存器号*/
    this->_spi->Write(reg);

    /*读取寄存器的值 */
    reg_val = this->_spi->Write(NOP);

    /*CSN拉高，完成*/
	this->_CSN = 1;

    return reg_val;
}

/**
 * @brief   用于向NRF的寄存器中写入一串数据
 * @param   
 *		@arg reg : NRF的命令+寄存器地址
 *		@arg pBuf：用于存储将被读出的寄存器数据的数组，外部定义
 * 	@arg bytes: pBuf的数据长度
 * @retval  NRF的status寄存器的状态
 */
uint8_t NRF24L01::ReadBuf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_cnt;

    this->_CE = 0;
    /*置低CSN，使能SPI传输*/
	this->_CSN = 0;

    /*发送寄存器号*/
    status = this->_spi->Write(reg);

    /*读取缓冲区数据*/
    for (byte_cnt = 0; byte_cnt < bytes; byte_cnt++)
        pBuf[byte_cnt] = this->_spi->Write(NOP);
    //从NRF24L01读取数据  

    /*CSN拉高，完成*/
	this->_CSN = 1;

    return status; //返回寄存器状态值
}

/**
 * @brief   用于向NRF的寄存器中写入一串数据
 * @param   
 *		@arg reg : NRF的命令+寄存器地址
 *		@arg pBuf：存储了将要写入写寄存器数据的数组，外部定义
 * 	@arg bytes: pBuf的数据长度
 * @retval  NRF的status寄存器的状态
 */
uint8_t NRF24L01::WriteBuf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_cnt;
	this->_CE = 0;
    /*置低CSN，使能SPI传输*/
	this->_CSN = 0;

    /*发送寄存器号*/
    status = this->_spi->Write(reg);

    /*向缓冲区写入数据*/
    for (byte_cnt = 0; byte_cnt < bytes; byte_cnt++)
        this->_spi->Write(*pBuf++);
    //写数据到缓冲区 	 

    /*CSN拉高，完成*/
	this->_CSN = 1;

    return (status); //返回NRF24L01的状态 		
}

/**
 * @brief  配置并进入接收模式
 * @param  无
 * @retval 无
 */
void NRF24L01::RX_Mode()
{
    this->_CE = 0;

    this->WriteBuf(NRF_WRITE_REG + RX_ADDR_P0, this->Local, 5); 
        //写RX节点地址

    this->WriteReg(NRF_WRITE_REG + EN_AA, 0x01); //使能通道0的自动应答    

    this->WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01); //使能通道0的接收地址    

    this->WriteReg(NRF_WRITE_REG + RF_CH, CHANAL); //设置RF通信频率    

    this->WriteReg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); 
        //选择通道0的有效数据宽度      

    this->WriteReg(NRF_WRITE_REG + RF_SETUP, 0x0f); 
        //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   

    this->WriteReg(NRF_WRITE_REG + CONFIG, 0x0f); 
        //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 

    /*CE拉高，进入接收模式*/
	this->_CE = 1;

}

/**
 * @brief  配置发送模式
 * @param  无
 * @retval 无
 */
void NRF24L01::TX_Mode()
{
    this->_CE = 0;

    this->WriteBuf(NRF_WRITE_REG + TX_ADDR, this->Remote, 5); 
        //写TX节点地址 

    this->WriteBuf(NRF_WRITE_REG + RX_ADDR_P0, this->Local, 5); 
        //设置TX节点地址,主要为了使能ACK   

    this->WriteReg(NRF_WRITE_REG + EN_AA, 0x01); //使能通道0的自动应答    

    this->WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01); //使能通道0的接收地址  

    this->WriteReg(NRF_WRITE_REG + SETUP_RETR, 0x1a); 
        //设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次

    this->WriteReg(NRF_WRITE_REG + RF_CH, CHANAL); //设置RF通道为CHANAL

    this->WriteReg(NRF_WRITE_REG + RF_SETUP, 0x0f); 
        //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   

    this->WriteReg(NRF_WRITE_REG + CONFIG, 0x0e); 
        //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断

    /*CE拉高，进入发送模式*/
	this->_CE = 1;
    Delay(0xffff); //CE要拉高一段时间才进入发送模式
}

/**
 * @brief  主要用于NRF与MCU是否正常连接
 * @param  无
 * @retval SUCCESS/ERROR 连接正常/连接失败
 */
bool NRF24L01::Check()
{
    uint8_t buf[5] = 
    {
        0xC2, 0xC2, 0xC2, 0xC2, 0xC2
    };
    uint8_t buf1[5];
    uint8_t i;

    /*写入5个字节的地址.  */
    this->WriteBuf(NRF_WRITE_REG + TX_ADDR, buf, 5);

    /*读出写入的地址 */
    this->ReadBuf(TX_ADDR, buf1, 5);

    /*比较*/
    for (i = 0; i < 5; i++)
    {
        if (buf1[i] != 0xC2)
            break;
    }

    if (i == 5)
        return SUCCESS;
    //MCU与NRF成功连接 
    else
        return ERROR;
    //MCU与NRF不正常连接
}

/**
 * @brief   用于向NRF的发送缓冲区中写入数据
 * @param   
 *		@arg txBuf：存储了将要发送的数据的数组，外部定义	
 * @retval  发送结果，成功返回TXDS,失败返回MAXRT或ERROR
 */
uint8_t NRF24L01::Tx_Dat(uint8_t *txbuf)
{
    uint8_t state;

    /*ce为低，进入待机模式1*/    
	this->_CE = 0;

    /*写数据到TX BUF 最大 32个字节*/
    this->WriteBuf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);

    /*CE为高，txbuf非空，发送数据包 */
	this->_CE = 1;

    /*等待发送完成中断 */
    while (this->Irq != 0)
        ;

    /*读取状态寄存器的值 */
    state = this->ReadReg(STATUS);

    /*清除TX_DS或MAX_RT中断标志*/
    this->WriteReg(NRF_WRITE_REG + STATUS, state);

    this->WriteReg(FLUSH_TX, NOP); //清除TX FIFO寄存器 

    /*判断中断类型*/
    if (state &MAX_RT)
    //达到最大重发次数
        return MAX_RT;

    else if (state &TX_DS)
    //发送完成
        return TX_DS;
    else
        return ERROR;
    //其他原因发送失败
}

/**
 * @brief   用于从NRF的接收缓冲区中读出数据
 * @param   
 *		@arg rxBuf ：用于接收该数据的数组，外部定义	
 * @retval 
 *		@arg 接收结果
 */
uint8_t NRF24L01::Rx_Dat(uint8_t *rxbuf)
{
    uint8_t state;
   //进入接收状态
	this->_CE = 1;
    /*等待接收中断*/
    while (this->Irq != 0)
        ;

    //进入待机状态
	this->_CE = 0;
    /*读取status寄存器的值  */
    state = this->ReadReg(STATUS);

    /* 清除中断标志*/
    this->WriteReg(NRF_WRITE_REG + STATUS, state);

    /*判断是否接收到数据*/
    if (state &RX_DR)
    //接收到数据
    {
        this->ReadBuf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); //读取数据
        this->WriteReg(FLUSH_RX, NOP); //清除RX FIFO寄存器
        return RX_DR;
    }
    else
        return ERROR;
    //没收到任何数据
}
