#include "nrf24l01.h"
#include "stm32f10x.h"
    

#define TX_ADR_WIDTH 	5  	//�����ַ���
    #define TX_PLOAD_WIDTH  4   //��������ͨ����Ч���ݿ��0~32Byte 

    #define RX_ADR_WIDTH    5
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

    #define NRF_Read_IRQ()		  GPIO_ReadInputDataBit ( GPIOC, GPIO_Pin_4)  
        //�ж�����
byte RX_BUF[RX_PLOAD_WIDTH]; //�������ݻ���
byte TX_BUF[TX_PLOAD_WIDTH]; //�������ݻ���
byte TX_ADDRESS[TX_ADR_WIDTH] = 
{
    0x34, 0x43, 0x10, 0x10, 0x01
}; // ����һ����̬���͵�ַ
byte RX_ADDRESS[RX_ADR_WIDTH] = 
{
    0x34, 0x43, 0x10, 0x10, 0x01
};

void Delay(__IO u32 nCount)
{
    for (; nCount != 0; nCount--)
        ;
}

/**
 * @brief  SPI�� I/O����
 * @param  ��
 * @retval ��
 */
void NRF24L01::SPI_NRF_Init()
{
//    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /*������ӦIO�˿ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
        RCC_APB2Periph_GPIOG, ENABLE);

    /*ʹ��SPI1ʱ��*/
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /*���� SPI_NRF_SPI�� SCK,MISO,MOSI���ţ�GPIOA^5,GPIOA^6,GPIOA^7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù���
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    /*����SPI_NRF_SPI��IRQ����*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* �����Զ���ĺ꣬��������csn���ţ�NRF�������״̬ */
    this->_CSN = 1;

//    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
//        //˫��ȫ˫��
//    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //��ģʽ
//    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //���ݴ�С8λ
//    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //ʱ�Ӽ��ԣ�����ʱΪ��
//    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
//        //��1��������Ч��������Ϊ����ʱ��
//    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�ź����������
//    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; 
//        //8��Ƶ��9MHz
//    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //��λ��ǰ
//    SPI_InitStructure.SPI_CRCPolynomial = 7;
//    SPI_Init(SPI1, &SPI_InitStructure);

//    /* Enable SPI1  */
//    SPI_Cmd(SPI1, ENABLE);
}

/**
 * @brief   ������NRF��/дһ�ֽ�����
 * @param   д�������
 *		@arg dat 
 * @retval  ��ȡ�õ�����
 */
byte NRF24L01::SPI_NRF_RW(byte dat)
{
	#if 0
    /* �� SPI���ͻ������ǿ�ʱ�ȴ� */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
        ;

    /* ͨ�� SPI2����һ�ֽ����� */
    SPI_I2S_SendData(SPI1, dat);

    /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
        ;

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
	#else
	return this->_spi->Write(dat);
	#endif
}

/**
 * @brief   ������NRF�ض��ļĴ���д������
 * @param   
 *		@arg reg:NRF������+�Ĵ�����ַ
 *		@arg dat:��Ҫ��Ĵ���д�������
 * @retval  NRF��status�Ĵ�����״̬
 */
byte NRF24L01::SPI_NRF_WriteReg(byte reg, byte dat)
{
    byte status;
    this->_CE = 0;
    /*�õ�CSN��ʹ��SPI����*/
	this->_CSN = 0;

    /*��������Ĵ����� */
    status = SPI_NRF_RW(reg);

    /*��Ĵ���д������*/
    SPI_NRF_RW(dat);

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
byte NRF24L01::SPI_NRF_ReadReg(byte reg)
{
    byte reg_val;

    this->_CE = 0;
    /*�õ�CSN��ʹ��SPI����*/
	this->_CSN = 0;

    /*���ͼĴ�����*/
    SPI_NRF_RW(reg);

    /*��ȡ�Ĵ�����ֵ */
    reg_val = SPI_NRF_RW(NOP);

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
byte NRF24L01::SPI_NRF_ReadBuf(byte reg, byte *pBuf, byte bytes)
{
    byte status, byte_cnt;

    this->_CE = 0;
    /*�õ�CSN��ʹ��SPI����*/
	this->_CSN = 0;

    /*���ͼĴ�����*/
    status = SPI_NRF_RW(reg);

    /*��ȡ����������*/
    for (byte_cnt = 0; byte_cnt < bytes; byte_cnt++)
        pBuf[byte_cnt] = SPI_NRF_RW(NOP);
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
byte NRF24L01::SPI_NRF_WriteBuf(byte reg, byte *pBuf, byte bytes)
{
    byte status, byte_cnt;
	this->_CE = 0;
    /*�õ�CSN��ʹ��SPI����*/
	this->_CSN = 0;

    /*���ͼĴ�����*/
    status = SPI_NRF_RW(reg);

    /*�򻺳���д������*/
    for (byte_cnt = 0; byte_cnt < bytes; byte_cnt++)
        SPI_NRF_RW(*pBuf++);
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
void NRF24L01::NRF_RX_Mode()
{
    this->_CE = 0;

    SPI_NRF_WriteBuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); 
        //дRX�ڵ��ַ

    SPI_NRF_WriteReg(NRF_WRITE_REG + EN_AA, 0x01); //ʹ��ͨ��0���Զ�Ӧ��    

    SPI_NRF_WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ    

    SPI_NRF_WriteReg(NRF_WRITE_REG + RF_CH, CHANAL); //����RFͨ��Ƶ��    

    SPI_NRF_WriteReg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); 
        //ѡ��ͨ��0����Ч���ݿ��      

    SPI_NRF_WriteReg(NRF_WRITE_REG + RF_SETUP, 0x0f); 
        //����TX�������,0db����,2Mbps,���������濪��   

    SPI_NRF_WriteReg(NRF_WRITE_REG + CONFIG, 0x0f); 
        //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

    /*CE���ߣ��������ģʽ*/
	this->_CE = 1;

}

/**
 * @brief  ���÷���ģʽ
 * @param  ��
 * @retval ��
 */
void NRF24L01::NRF_TX_Mode()
{
    this->_CE = 0;

    SPI_NRF_WriteBuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); 
        //дTX�ڵ��ַ 

    SPI_NRF_WriteBuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); 
        //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK   

    SPI_NRF_WriteReg(NRF_WRITE_REG + EN_AA, 0x01); //ʹ��ͨ��0���Զ�Ӧ��    

    SPI_NRF_WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ  

    SPI_NRF_WriteReg(NRF_WRITE_REG + SETUP_RETR, 0x1a); 
        //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

    SPI_NRF_WriteReg(NRF_WRITE_REG + RF_CH, CHANAL); //����RFͨ��ΪCHANAL

    SPI_NRF_WriteReg(NRF_WRITE_REG + RF_SETUP, 0x0f); 
        //����TX�������,0db����,2Mbps,���������濪��   

    SPI_NRF_WriteReg(NRF_WRITE_REG + CONFIG, 0x0e); 
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
byte NRF24L01::NRF_Check()
{
    byte buf[5] = 
    {
        0xC2, 0xC2, 0xC2, 0xC2, 0xC2
    };
    byte buf1[5];
    byte i;

    /*д��5���ֽڵĵ�ַ.  */
    SPI_NRF_WriteBuf(NRF_WRITE_REG + TX_ADDR, buf, 5);

    /*����д��ĵ�ַ */
    SPI_NRF_ReadBuf(TX_ADDR, buf1, 5);

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
byte NRF24L01::NRF_Tx_Dat(byte *txbuf)
{
    byte state;

    /*ceΪ�ͣ��������ģʽ1*/    
	this->_CE = 0;

    /*д���ݵ�TX BUF ��� 32���ֽ�*/
    SPI_NRF_WriteBuf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);

    /*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */
	this->_CE = 1;

    /*�ȴ���������ж� */
    while (NRF_Read_IRQ() != 0)
        ;

    /*��ȡ״̬�Ĵ�����ֵ */
    state = SPI_NRF_ReadReg(STATUS);

    /*���TX_DS��MAX_RT�жϱ�־*/
    SPI_NRF_WriteReg(NRF_WRITE_REG + STATUS, state);

    SPI_NRF_WriteReg(FLUSH_TX, NOP); //���TX FIFO�Ĵ��� 

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
byte NRF24L01::NRF_Rx_Dat(byte *rxbuf)
{
    byte state;
   //�������״̬
	this->_CE = 1;
    /*�ȴ������ж�*/
    while (NRF_Read_IRQ() != 0)
        ;

    //�������״̬
	this->_CE = 0;
    /*��ȡstatus�Ĵ�����ֵ  */
    state = SPI_NRF_ReadReg(STATUS);

    /* ����жϱ�־*/
    SPI_NRF_WriteReg(NRF_WRITE_REG + STATUS, state);

    /*�ж��Ƿ���յ�����*/
    if (state &RX_DR)
    //���յ�����
    {
        SPI_NRF_ReadBuf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); //��ȡ����
        SPI_NRF_WriteReg(FLUSH_RX, NOP); //���RX FIFO�Ĵ���
        return RX_DR;
    }
    else
        return ERROR;
    //û�յ��κ�����
}
