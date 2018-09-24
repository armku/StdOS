#include "nrf24l01.h"
#include "stm32f10x.h"
 
    #define TX_PLOAD_WIDTH  4   //��������ͨ����Ч���ݿ��0~32Byte 
    #define RX_PLOAD_WIDTH  4

	#define CHANAL 40	//Ƶ��ѡ�� 

    // SPI(nRF24L01) commands ,	NRF��SPI����궨�壬���NRF����ʹ���ĵ�
    #define NRF_READ_REG    0x00  // Define read command to register
    #define NRF_WRITE_REG   0x20  // Define write command to register
    #define RD_RX_PLOAD 0x61  // Define RX payload register address
    #define WR_TX_PLOAD 0xA0  // Define TX payload register address
    #define FLUSH_TX    0xE1  // Define flush TX register command
    #define FLUSH_RX    0xE2  // Define flush RX register command
    #define REUSE_TX_PL 0xE3  // Define reuse TX payload register command
    #define NOP         0xFF  
        // Define No Operation, might be used to read status register

    // SPI(nRF24L01) registers(addresses) ��NRF24L01 ��ؼĴ�����ַ�ĺ궨��
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

uint8_t RX_BUF[RX_PLOAD_WIDTH]; //�������ݻ���
uint8_t TX_BUF[TX_PLOAD_WIDTH]; //�������ݻ���

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
 * @brief  SPI�� I/O����
 * @param  ��
 * @retval ��
 */
void NRF24L01::Init(Spi* spi, Pin ce, Pin irq, Pin power)
{
	#if 1
    GPIO_InitTypeDef GPIO_InitStructure;

    /*������ӦIO�˿ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
        RCC_APB2Periph_GPIOG, ENABLE);
    
    /*���� SPI_NRF_SPI�� SCK,MISO,MOSI���ţ�GPIOA^5,GPIOA^6,GPIOA^7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù���
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   	#endif
    /* �����Զ���ĺ꣬��������csn���ţ�NRF�������״̬ */
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
 * @brief   ������NRF�ض��ļĴ���д������
 * @param   
 *		@arg reg:NRF������+�Ĵ�����ַ
 *		@arg dat:��Ҫ��Ĵ���д�������
 * @retval  NRF��status�Ĵ�����״̬
 */
uint8_t NRF24L01::WriteReg(uint8_t reg, uint8_t dat)
{
    uint8_t status;
    this->_CE = 0;
    /*�õ�CSN��ʹ��SPI����*/
	this->_CSN = 0;

    /*��������Ĵ����� */
    status = this->_spi->Write(reg);

    /*��Ĵ���д������*/
    this->_spi->Write(dat);

    /*CSN���ߣ����*/
	this->_CSN = 1;

    /*����״̬�Ĵ�����ֵ*/
    return (status);
}

/**
 * @brief   ���ڴ�NRF�ض��ļĴ�����������
 * @param   
 *		@arg reg:NRF������+�Ĵ�����ַ
 * @retval  �Ĵ����е�����
 */
uint8_t NRF24L01::ReadReg(uint8_t reg)
{
    uint8_t reg_val;

    this->_CE = 0;
    /*�õ�CSN��ʹ��SPI����*/
	this->_CSN = 0;

    /*���ͼĴ�����*/
    this->_spi->Write(reg);

    /*��ȡ�Ĵ�����ֵ */
    reg_val = this->_spi->Write(NOP);

    /*CSN���ߣ����*/
	this->_CSN = 1;

    return reg_val;
}

/**
 * @brief   ������NRF�ļĴ�����д��һ������
 * @param   
 *		@arg reg : NRF������+�Ĵ�����ַ
 *		@arg pBuf�����ڴ洢���������ļĴ������ݵ����飬�ⲿ����
 * 	@arg bytes: pBuf�����ݳ���
 * @retval  NRF��status�Ĵ�����״̬
 */
uint8_t NRF24L01::ReadBuf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_cnt;

    this->_CE = 0;
    /*�õ�CSN��ʹ��SPI����*/
	this->_CSN = 0;

    /*���ͼĴ�����*/
    status = this->_spi->Write(reg);

    /*��ȡ����������*/
    for (byte_cnt = 0; byte_cnt < bytes; byte_cnt++)
        pBuf[byte_cnt] = this->_spi->Write(NOP);
    //��NRF24L01��ȡ����  

    /*CSN���ߣ����*/
	this->_CSN = 1;

    return status; //���ؼĴ���״ֵ̬
}

/**
 * @brief   ������NRF�ļĴ�����д��һ������
 * @param   
 *		@arg reg : NRF������+�Ĵ�����ַ
 *		@arg pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
 * 	@arg bytes: pBuf�����ݳ���
 * @retval  NRF��status�Ĵ�����״̬
 */
uint8_t NRF24L01::WriteBuf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_cnt;
	this->_CE = 0;
    /*�õ�CSN��ʹ��SPI����*/
	this->_CSN = 0;

    /*���ͼĴ�����*/
    status = this->_spi->Write(reg);

    /*�򻺳���д������*/
    for (byte_cnt = 0; byte_cnt < bytes; byte_cnt++)
        this->_spi->Write(*pBuf++);
    //д���ݵ������� 	 

    /*CSN���ߣ����*/
	this->_CSN = 1;

    return (status); //����NRF24L01��״̬ 		
}

/**
 * @brief  ���ò��������ģʽ
 * @param  ��
 * @retval ��
 */
void NRF24L01::RX_Mode()
{
    this->_CE = 0;

    this->WriteBuf(NRF_WRITE_REG + RX_ADDR_P0, this->Local, 5); 
        //дRX�ڵ��ַ

    this->WriteReg(NRF_WRITE_REG + EN_AA, 0x01); //ʹ��ͨ��0���Զ�Ӧ��    

    this->WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ    

    this->WriteReg(NRF_WRITE_REG + RF_CH, CHANAL); //����RFͨ��Ƶ��    

    this->WriteReg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); 
        //ѡ��ͨ��0����Ч���ݿ��      

    this->WriteReg(NRF_WRITE_REG + RF_SETUP, 0x0f); 
        //����TX�������,0db����,2Mbps,���������濪��   

    this->WriteReg(NRF_WRITE_REG + CONFIG, 0x0f); 
        //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

    /*CE���ߣ��������ģʽ*/
	this->_CE = 1;

}

/**
 * @brief  ���÷���ģʽ
 * @param  ��
 * @retval ��
 */
void NRF24L01::TX_Mode()
{
    this->_CE = 0;

    this->WriteBuf(NRF_WRITE_REG + TX_ADDR, this->Remote, 5); 
        //дTX�ڵ��ַ 

    this->WriteBuf(NRF_WRITE_REG + RX_ADDR_P0, this->Local, 5); 
        //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK   

    this->WriteReg(NRF_WRITE_REG + EN_AA, 0x01); //ʹ��ͨ��0���Զ�Ӧ��    

    this->WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ  

    this->WriteReg(NRF_WRITE_REG + SETUP_RETR, 0x1a); 
        //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

    this->WriteReg(NRF_WRITE_REG + RF_CH, CHANAL); //����RFͨ��ΪCHANAL

    this->WriteReg(NRF_WRITE_REG + RF_SETUP, 0x0f); 
        //����TX�������,0db����,2Mbps,���������濪��   

    this->WriteReg(NRF_WRITE_REG + CONFIG, 0x0e); 
        //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�

    /*CE���ߣ����뷢��ģʽ*/
	this->_CE = 1;
    Delay(0xffff); //CEҪ����һ��ʱ��Ž��뷢��ģʽ
}

/**
 * @brief  ��Ҫ����NRF��MCU�Ƿ���������
 * @param  ��
 * @retval SUCCESS/ERROR ��������/����ʧ��
 */
bool NRF24L01::Check()
{
    uint8_t buf[5] = 
    {
        0xC2, 0xC2, 0xC2, 0xC2, 0xC2
    };
    uint8_t buf1[5];
    uint8_t i;

    /*д��5���ֽڵĵ�ַ.  */
    this->WriteBuf(NRF_WRITE_REG + TX_ADDR, buf, 5);

    /*����д��ĵ�ַ */
    this->ReadBuf(TX_ADDR, buf1, 5);

    /*�Ƚ�*/
    for (i = 0; i < 5; i++)
    {
        if (buf1[i] != 0xC2)
            break;
    }

    if (i == 5)
        return SUCCESS;
    //MCU��NRF�ɹ����� 
    else
        return ERROR;
    //MCU��NRF����������
}

/**
 * @brief   ������NRF�ķ��ͻ�������д������
 * @param   
 *		@arg txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
 * @retval  ���ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
 */
uint8_t NRF24L01::Tx_Dat(uint8_t *txbuf)
{
    uint8_t state;

    /*ceΪ�ͣ��������ģʽ1*/    
	this->_CE = 0;

    /*д���ݵ�TX BUF ��� 32���ֽ�*/
    this->WriteBuf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);

    /*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */
	this->_CE = 1;

    /*�ȴ���������ж� */
    while (this->Irq != 0)
        ;

    /*��ȡ״̬�Ĵ�����ֵ */
    state = this->ReadReg(STATUS);

    /*���TX_DS��MAX_RT�жϱ�־*/
    this->WriteReg(NRF_WRITE_REG + STATUS, state);

    this->WriteReg(FLUSH_TX, NOP); //���TX FIFO�Ĵ��� 

    /*�ж��ж�����*/
    if (state &MAX_RT)
    //�ﵽ����ط�����
        return MAX_RT;

    else if (state &TX_DS)
    //�������
        return TX_DS;
    else
        return ERROR;
    //����ԭ����ʧ��
}

/**
 * @brief   ���ڴ�NRF�Ľ��ջ������ж�������
 * @param   
 *		@arg rxBuf �����ڽ��ո����ݵ����飬�ⲿ����	
 * @retval 
 *		@arg ���ս��
 */
uint8_t NRF24L01::Rx_Dat(uint8_t *rxbuf)
{
    uint8_t state;
   //�������״̬
	this->_CE = 1;
    /*�ȴ������ж�*/
    while (this->Irq != 0)
        ;

    //�������״̬
	this->_CE = 0;
    /*��ȡstatus�Ĵ�����ֵ  */
    state = this->ReadReg(STATUS);

    /* ����жϱ�־*/
    this->WriteReg(NRF_WRITE_REG + STATUS, state);

    /*�ж��Ƿ���յ�����*/
    if (state &RX_DR)
    //���յ�����
    {
        this->ReadBuf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); //��ȡ����
        this->WriteReg(FLUSH_RX, NOP); //���RX FIFO�Ĵ���
        return RX_DR;
    }
    else
        return ERROR;
    //û�յ��κ�����
}
