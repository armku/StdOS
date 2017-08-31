#include "AD7128.h"
#include "stm32f10x.h"
#include "Sys.h"
#include "Spi.h"

OutputPort spi1nss(PA4);
OutputPort spi1sck(PA5);
InputPort spi1miso(PA6 );
OutputPort spi1mosi(PA7);

SpiSoft spi1;
/*******************************************************************************
 * @function	: AD7124_SPI_Config
 * @brief		: SPI�˿����ã�ģ��SPI
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
void AD7124_SPI_Config(void)
{
	spi1nss=1;
	spi1sck=1;
 	spi1mosi=0;
    //SPI_MISO
//    GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;;//
//    GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

    spi1nss=0;	
	
	spi1sck.Invert = false;
    spi1miso.Invert = false;
    spi1mosi.Invert = false;
    spi1nss.Invert = false;
	
	spi1.SetPin(PA5,PA6,PA7);
	spi1.SetNss(PA4);
}

/*******************************************************************************
 * @function	: SPI_Delay
 * @brief		: SPI����λ��ʱ
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
static void SPI_Delay(void)
{
    uint8_t i;
    /*��
    �����ʱ����ͨ��������AX-Pro�߼������ǲ��Եõ��ġ�
    CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
    ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
    ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
    ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
    IAR���̱���Ч�ʸߣ���������Ϊ7
     */
    for (i = 0; i < 10; i++)
        ;
}

/*******************************************************************************
 * @function	: spi8_run
 * @brief		: ��дspi 1���ֽ�
 * @param		: sendData
 * @retval		: rcvData
 * @notes		: 
 *****************************************************************************/
uint8_t spi8_run(uint8_t sendData)
{
    uint8_t spicnt = 8;
    uint8_t rcvData = 0;
    spi1sck=1;
    spi1mosi=1;
    while (spicnt-- > 0)
    {
        if (sendData &0x80)
        {
            spi1mosi=1;
        }
        else
        {
            spi1mosi=0;
        }
        SPI_Delay();
        spi1sck=0;
        sendData <<= 1;
        SPI_Delay();
        SPI_Delay();
        spi1sck=1;
        rcvData <<= 1;
        rcvData |= spi1miso;
    }
    return rcvData;
}

/*******************************************************************************
 * @function	: spi16_run
 * @brief		: ��дspi 2���ֽ�
 * @param		: sendData
 * @retval		: rcvData
 * @notes		: 
 *****************************************************************************/
uint16_t spi16_run(uint16_t sendData)
{
    uint8_t spicnt = 16;
    uint16_t rcvData = 0;
    spi1sck=1;
    spi1mosi=1;
    while (spicnt-- > 0)
    {
        if (sendData &0x8000)
        {
            spi1mosi=1;
        }
        else
        {
            spi1mosi=0;
        }
        SPI_Delay();
        spi1sck=0;
        sendData <<= 1;
        SPI_Delay();
        SPI_Delay();
        spi1sck=1;
        rcvData <<= 1;
        rcvData |= spi1miso;
    }
    return rcvData;
}

/*******************************************************************************
 * @function	: spi24_run
 * @brief		: ��дspi 4���ֽ�
 * @param		: sendData
 * @retval		: rcvData
 * @notes		: 
 *****************************************************************************/
uint32_t spi24_run(uint32_t sendData)
{
    uint8_t spicnt = 24;
    uint32_t rcvData = 0;
    spi1sck=1;
    spi1mosi=1;
    while (spicnt-- > 0)
    {
        if (sendData &0x800000)
        {
            spi1mosi=1;
        }
        else
        {
            spi1mosi=0;
        }
        SPI_Delay();
        spi1sck=0;
        sendData <<= 1;
        SPI_Delay();
        SPI_Delay();
        spi1sck=1;
        rcvData <<= 1;
        rcvData |= spi1miso;
    }
    return rcvData;
}

/*******************************************************************************
 * @function	: SPI_Delay
 * @brief		: SPI����λ��ʱ
 * @param		: sendData
 * @retval		: rcvData
 * @notes		: 
 *****************************************************************************/
uint32_t spi32_run(uint32_t sendData)
{
    uint8_t spicnt = 32;
    uint32_t rcvData = 0;
    spi1sck=1;
    spi1mosi=1;
    while (spicnt-- > 0)
    {
        if (sendData &0x80000000)
        {
            spi1mosi=1;
        }
        else
        {
            spi1mosi=0;
        }
        SPI_Delay();
        spi1sck=0;
        sendData <<= 1;
        SPI_Delay();
        SPI_Delay();
        spi1sck=1;
        rcvData <<= 1;
        rcvData |= spi1miso;
    }
    return rcvData;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////7124
/*******************************************************************************
 * @function	: AD7124_Read_Reg
 * @brief		: ��ȡAD7124�Ĵ�������
 * @param		: reg   �Ĵ�����ַ
 * @param		: bytes �Ĵ�����С
 * @retval		: ��ȡ������
 * @notes		: 
 *****************************************************************************/
uint32_t AD7124_Read_Reg(uint8_t reg, uint8_t bytes)
{
    uint32_t retVal;
    spi1nss=0;
    spi8_run(AD7124_RD | reg);
    if (bytes == 1)
    {
        retVal = spi8_run(0xFF);
    }
    else if (bytes == 2)
    {
        retVal = spi16_run(0xFFFF);
    }
    else if (bytes == 3)
    {
        retVal = spi24_run(0xFFFFFF);
    }
    spi1nss=1;
    return retVal;
}

/*******************************************************************************
 * @function	: AD7124_Read_Reg
 * @brief		: ��ȡAD7124�Ĵ�������
 * @param		: reg   �Ĵ�����ַ
 * @param		: bytes �Ĵ�����С
 * @retval		: ��ȡ������
 * @notes		: 
 *****************************************************************************/
uint32_t AD7124_Read_Reg_NoCS(uint8_t reg, uint8_t bytes)
{
    uint32_t retVal;
    spi8_run(AD7124_RD | reg);
    if (bytes == 1)
    {
        retVal = spi8_run(0xFF);
    }
    else if (bytes == 2)
    {
        retVal = spi16_run(0xFFFF);
    }
    else if (bytes == 3)
    {
        retVal = spi24_run(0xFFFFFF);
    }
    return retVal;
}

/*******************************************************************************
 * @function	: AD7124_Write_Reg
 * @brief		: дAD7124�Ĵ���
 * @param		: reg	  �Ĵ�����ַ
 * @param		: bytes �Ĵ�����С
 * @param		: data	д�������
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
void AD7124_Write_Reg(uint8_t reg, uint8_t bytes, uint32_t data)
{
    spi1nss=0;
    spi8_run(AD7124_WR | reg);
    if (bytes == 1)
    {
        spi8_run(data);
    }
    else if (bytes == 2)
    {
        spi16_run(data);
    }
    else if (bytes == 3)
    {
        spi24_run(data);
    }
    spi1nss=1;
}

//ADC_CONTROL			�ڲ��ο���ѹʹ��	�͹���	����ת��ģʽ	�ڲ�ʱ�Ӳ����
//IO_CONTROL_1		��������� 	�����������	PDSW��	IOUT1�ر�	IOUT0�ر�	IOUT1_CH=0	IOUT0_CH=0
//IO_CONTROL_2		��ƫ�õ�ѹ
//CONFIG0					������	Burnout��	��׼��ѹ�������	ģ�����뻺�����	��׼��ѹREF1	PGA=16
//CONFIG1					������	Burnout��	��׼��ѹ�������	ģ�����뻺�����	��׼��ѹREF2	PGA=16
//CONFIG2					������	Burnout��	��׼��ѹ�������	ģ�����뻺�����	��׼��ѹ�ڲ�	PGA=128
//CONFIG3					������	Burnout��	��׼��ѹ�������	ģ�����뻺�����	��׼��ѹ�ڲ�	PGA=1
//CONFIG4					˫����	Burnout��	��׼��ѹ�������	ģ�����뻺�����	��׼��ѹ�ڲ�	PGA=1
#define AD7124_ADC_CTRL_REG_DATA	AD7124_ADC_CTRL_REG_REF_EN | AD7124_ADC_CTRL_REG_POWER_MODE(1)//| AD7124_ADC_CTRL_REG_MODE(1)
#define AD7124_IO_CTRL1_REG_DATA	0
#define AD7124_IO_CTRL2_REG_DATA	0
#define AD7124_CFG0_REG_DATA	AD7124_CFG_REG_REF_SEL(0)|AD7124_CFG_REG_PGA(4)
#define AD7124_CFG1_REG_DATA	AD7124_CFG_REG_REF_SEL(1)|AD7124_CFG_REG_PGA(4)
#define AD7124_CFG2_REG_DATA	AD7124_CFG_REG_REF_SEL(2)|AD7124_CFG_REG_PGA(7)
#define AD7124_CFG3_REG_DATA	AD7124_CFG_REG_REF_SEL(2)|AD7124_CFG_REG_PGA(0)
#define AD7124_CFG4_REG_DATA	AD7124_CFG_REG_REF_SEL(2)|AD7124_CFG_REG_PGA(0)
//FILTER0
//FILTER1
//FILTER2
//FILTER3
//FILTER4
/*
#define AD7124_FILT_REG_FILTER(x)         (((x) & 0x7) << 21)
#define AD7124_FILT_REG_REJ60             (1 << 20)
#define AD7124_FILT_REG_POST_FILTER(x)    (((x) & 0x7) << 17)
#define AD7124_FILT_REG_SINGLE_CYCLE      (1 << 16)
#define AD7124_FILT_REG_FS(x)             (((x) & 0x7FF) << 0)
 */
#define AD7124_FILT0_REG_DATA		AD7124_FILT_REG_FS(100)
#define AD7124_FILT1_REG_DATA		AD7124_FILT_REG_FS(100)
#define AD7124_FILT2_REG_DATA		AD7124_FILT_REG_FS(100)
#define AD7124_FILT3_REG_DATA		AD7124_FILT_REG_FS(100)
#define AD7124_FILT4_REG_DATA		AD7124_FILT_REG_FS(100)

void AD7124_Config(void)
{
    AD7124_Write_Reg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, AD7124_ADC_CTRL_REG_DATA);
    AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, AD7124_IO_CTRL1_REG_DATA);
    AD7124_Write_Reg(AD7124_IO_CTRL2_REG, AD7124_IO_CTRL2_REG_BYTES, AD7124_IO_CTRL2_REG_DATA);

    AD7124_Write_Reg(AD7124_CFG0_REG, AD7124_CFG0_REG_BYTES, AD7124_CFG0_REG_DATA);
    AD7124_Write_Reg(AD7124_CFG1_REG, AD7124_CFG1_REG_BYTES, AD7124_CFG1_REG_DATA);
    AD7124_Write_Reg(AD7124_CFG2_REG, AD7124_CFG2_REG_BYTES, AD7124_CFG2_REG_DATA);
    AD7124_Write_Reg(AD7124_CFG3_REG, AD7124_CFG3_REG_BYTES, AD7124_CFG3_REG_DATA);
    AD7124_Write_Reg(AD7124_CFG4_REG, AD7124_CFG4_REG_BYTES, AD7124_CFG4_REG_DATA);

    AD7124_Write_Reg(AD7124_FILT0_REG, AD7124_FILT0_REG_BYTES, AD7124_FILT0_REG_DATA);
    AD7124_Write_Reg(AD7124_FILT1_REG, AD7124_FILT1_REG_BYTES, AD7124_FILT1_REG_DATA);
    AD7124_Write_Reg(AD7124_FILT2_REG, AD7124_FILT2_REG_BYTES, AD7124_FILT2_REG_DATA);
    AD7124_Write_Reg(AD7124_FILT3_REG, AD7124_FILT3_REG_BYTES, AD7124_FILT3_REG_DATA);
    AD7124_Write_Reg(AD7124_FILT4_REG, AD7124_FILT4_REG_BYTES, AD7124_FILT4_REG_DATA);
}

/*******************************************************************************
 * @function	: AD7124_Init
 * @brief		: SPI�˿����ã�ģ��SPI
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
void AD7124_Init(void)
{
    AD7124_SPI_Config(); //SPI IO�˿�����
    AD7124_Config(); //ADC ����
    AD7124_Channel_Config(); //
    AD7124_ExInt_Config(); //�ⲿ�жϿ�����
    spi1nss=0;
}

//IO_CTRL_1_TEMP1				��������� 	�����������	PDSW��	IOUT1�ر�			IOUT0=500uA	IOUT1_CH=0		IOUT0_CH=AIN0
//IO_CTRL_1_TEMP1_RES		��������� 	�����������	PDSW��	IOUT1=1000uA	IOUT0�ر�		IOUT1_CH=AIN1	IOUT0_CH=0
//IO_CTRL_1_TEMP2				��������� 	�����������	PDSW��	IOUT1�ر�			IOUT0=500uA	IOUT1_CH=0		IOUT0_CH=AIN4
//IO_CTRL_1_TEMP2_RES		��������� 	�����������	PDSW��	IOUT1=1000uA	IOUT0�ر�		IOUT1_CH=AIN7	IOUT0_CH=0
#define AD7124_IO_CTRL1_REG_DATA_TEMP1			AD7124_IO_CTRL1_REG_IOUT1(0) | AD7124_IO_CTRL1_REG_IOUT0(4) | AD7124_IO_CTRL1_REG_IOUT_CH1(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(0)
#define AD7124_IO_CTRL1_REG_DATA_TEMP1_RES	AD7124_IO_CTRL1_REG_IOUT1(6) | AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH1(1) | AD7124_IO_CTRL1_REG_IOUT_CH0(0)
#define AD7124_IO_CTRL1_REG_DATA_TEMP2			AD7124_IO_CTRL1_REG_IOUT1(0) | AD7124_IO_CTRL1_REG_IOUT0(4) | AD7124_IO_CTRL1_REG_IOUT_CH1(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(4)
#define AD7124_IO_CTRL1_REG_DATA_TEMP2_RES	AD7124_IO_CTRL1_REG_IOUT1(6) | AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH1(7) | AD7124_IO_CTRL1_REG_IOUT_CH0(0)

//IO_CTRL_1_TEMP1_TEST	��������� 	�����������	PDSW��	IOUT1=500uA		IOUT0=500uA	IOUT1_CH=AIN1		IOUT0_CH=AIN0
#define AD7124_IO_CTRL1_REG_DATA_TEMP1_TEST	AD7124_IO_CTRL1_REG_IOUT1(4) | AD7124_IO_CTRL1_REG_IOUT0(4) | AD7124_IO_CTRL1_REG_IOUT_CH1(1) | AD7124_IO_CTRL1_REG_IOUT_CH0(0)

//�¶�1						CH0		ENABLE	CONFIG0		AINP[AIN2]		AINM[AIN3]
//�¶�1��·����		CH1		ENABLE	CONFIG2		AINP[AIN3]		AINM[AVSS]
//�¶�2						CH2		ENABLE	CONFIG1		AINP[AIN5]		AINM[AIN6]
//�¶�2��·����		CH3		ENABLE	CONFIG2		AINP[AIN6]		AINM[AVSS]
//����A						CH4		ENABLE	CONFIG3		AINP[AIN8]		AINM[AVSS]
//����B						CH5		ENABLE	CONFIG3		AINP[AIN9]		AINM[AVSS]
//TC1047					CH6		ENABLE	CONFIG3		AINP[AIN10]		AINM[AVSS]
//AD�ڲ��¶�			CH7		ENABLE	CONFIG4		AINP[�¶�]		AINM[AVSS]
#define AD7124_CH0_MAP_REG_DATA		AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(2) | AD7124_CH_MAP_REG_AINM(3)
#define AD7124_CH1_MAP_REG_DATA		AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(2) | AD7124_CH_MAP_REG_AINP(3) | AD7124_CH_MAP_REG_AINM(17)
#define AD7124_CH2_MAP_REG_DATA		AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(1) | AD7124_CH_MAP_REG_AINP(5) | AD7124_CH_MAP_REG_AINM(6)
#define AD7124_CH3_MAP_REG_DATA		AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(2) | AD7124_CH_MAP_REG_AINP(6) | AD7124_CH_MAP_REG_AINM(17)
#define AD7124_CH4_MAP_REG_DATA		AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(3) | AD7124_CH_MAP_REG_AINP(8) | AD7124_CH_MAP_REG_AINM(17)
#define AD7124_CH5_MAP_REG_DATA		AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(3) | AD7124_CH_MAP_REG_AINP(9) | AD7124_CH_MAP_REG_AINM(17)
#define AD7124_CH6_MAP_REG_DATA		AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(3) | AD7124_CH_MAP_REG_AINP(10)| AD7124_CH_MAP_REG_AINM(17)
#define AD7124_CH7_MAP_REG_DATA		AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(4) | AD7124_CH_MAP_REG_AINP(16)| AD7124_CH_MAP_REG_AINM(17)


#define AD7124_TEMP1_CH_CFG()					AD7124_Write_Reg(AD7124_CH0_MAP_REG,	AD7124_CH0_MAP_REG_BYTES, 	AD7124_CH0_MAP_REG_DATA);
#define AD7124_TEMP1_IO_CFG()					AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA_TEMP1);
#define AD7124_TEMP1_RES_CH_CFG()			AD7124_Write_Reg(AD7124_CH1_MAP_REG,	AD7124_CH1_MAP_REG_BYTES, 	AD7124_CH1_MAP_REG_DATA);
#define AD7124_TEMP1_RES_IO_CFG()			AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA_TEMP1_RES);
#define AD7124_TEMP2_CH_CFG()					AD7124_Write_Reg(AD7124_CH2_MAP_REG,	AD7124_CH2_MAP_REG_BYTES, 	AD7124_CH2_MAP_REG_DATA);
#define AD7124_TEMP2_IO_CFG()					AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA_TEMP2);
#define AD7124_TEMP2_RES_CH_CFG()			AD7124_Write_Reg(AD7124_CH3_MAP_REG,	AD7124_CH3_MAP_REG_BYTES, 	AD7124_CH3_MAP_REG_DATA);
#define AD7124_TEMP2_RES_IO_CFG()			AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA_TEMP2_RES);
#define AD7124_FLOWA_CH_CFG()					AD7124_Write_Reg(AD7124_CH4_MAP_REG,	AD7124_CH4_MAP_REG_BYTES, 	AD7124_CH4_MAP_REG_DATA);
#define AD7124_FLOWA_IO_CFG()					AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA);
#define AD7124_FLOWB_CH_CFG()					AD7124_Write_Reg(AD7124_CH5_MAP_REG,	AD7124_CH5_MAP_REG_BYTES, 	AD7124_CH5_MAP_REG_DATA);
#define AD7124_FLOWB_IO_CFG()					AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA);
#define AD7124_BRDTEMP_CH_CFG()				AD7124_Write_Reg(AD7124_CH6_MAP_REG,	AD7124_CH6_MAP_REG_BYTES, 	AD7124_CH6_MAP_REG_DATA);
#define AD7124_BRDTEMP_IO_CFG()				AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA);
#define AD7124_ADTEMP_CH_CFG()				AD7124_Write_Reg(AD7124_CH7_MAP_REG,	AD7124_CH7_MAP_REG_BYTES, 	AD7124_CH7_MAP_REG_DATA);
#define AD7124_ADTEMP_IO_CFG()				AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA);

#define AD7124_TEMP1_CH_CFG_CLR()				AD7124_Write_Reg(AD7124_CH0_MAP_REG,	AD7124_CH0_MAP_REG_BYTES, 	0);
#define AD7124_TEMP1_IO_CFG_CLR()				AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	0);
#define AD7124_TEMP1_RES_CH_CFG_CLR()		AD7124_Write_Reg(AD7124_CH1_MAP_REG,	AD7124_CH1_MAP_REG_BYTES, 	0);
#define AD7124_TEMP1_RES_IO_CFG_CLR()		AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	0);
#define AD7124_TEMP2_CH_CFG_CLR()				AD7124_Write_Reg(AD7124_CH2_MAP_REG,	AD7124_CH2_MAP_REG_BYTES, 	0);
#define AD7124_TEMP2_IO_CFG_CLR()				AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	0);
#define AD7124_TEMP2_RES_CH_CFG_CLR()		AD7124_Write_Reg(AD7124_CH3_MAP_REG,	AD7124_CH3_MAP_REG_BYTES, 	0);
#define AD7124_TEMP2_RES_IO_CFG_CLR()		AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	0);
#define AD7124_FLOWA_CH_CFG_CLR()				AD7124_Write_Reg(AD7124_CH4_MAP_REG,	AD7124_CH4_MAP_REG_BYTES,   0);
#define AD7124_FLOWA_IO_CFG_CLR()				AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES,  0);
#define AD7124_FLOWB_CH_CFG_CLR()				AD7124_Write_Reg(AD7124_CH5_MAP_REG,	AD7124_CH5_MAP_REG_BYTES, 	0);
#define AD7124_FLOWB_IO_CFG_CLR()				AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES,  0);
#define AD7124_BRDTEMP_CH_CFG_CLR()			AD7124_Write_Reg(AD7124_CH6_MAP_REG,	AD7124_CH6_MAP_REG_BYTES,   0);
#define AD7124_BRDTEMP_IO_CFG_CLR()			AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES,  0);
#define AD7124_ADTEMP_CH_CFG_CLR()			AD7124_Write_Reg(AD7124_CH7_MAP_REG,	AD7124_CH7_MAP_REG_BYTES,   0);
#define AD7124_ADTEMP_IO_CFG_CLR()			AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES,  0);

/*******************************************************************************
 * @function	: AD7124_Channel_Config
 * @brief		:	ADCͨ������
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
void AD7124_Channel_Config(void)
{
    AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, AD7124_IO_CTRL1_REG_DATA_TEMP1_TEST);
    AD7124_TEMP1_CH_CFG();
    AD7124_TEMP1_RES_CH_CFG();
    AD7124_FLOWA_CH_CFG();
    AD7124_FLOWB_CH_CFG();
    AD7124_BRDTEMP_CH_CFG();
    AD7124_ADTEMP_CH_CFG();
}

/*******************************************************************************
 * @function	: AD7124_Read_ID
 * @brief		: ��ȡAD7124 ID�Ĵ���
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
uint8_t AD7124_Read_ID(void)
{
    uint8_t retVal;
    retVal = AD7124_Read_Reg(AD7124_ID_REG, AD7124_ID_REG_BYTES);
    printf("ID:0x%02x\r\n", retVal);
    return retVal;
}

/*******************************************************************************
 * @function	: AD7124_Read_Status
 * @brief		: ��ȡAD7124 ״̬�Ĵ���
 * @param		: ��
 * @retval		: ��
 * @notes		: 
 *****************************************************************************/
uint8_t AD7124_Read_Status(void)
{
    uint8_t retVal;
    retVal = AD7124_Read_Reg(AD7124_STATUS_REG, AD7124_STATUS_REG_BYTES);
    printf("Status:0x%02x\r\n", retVal);
    return retVal;
}

/*******************************************************************************
 * @function	: AD7124_Get_Temp1
 * @brief		: ��ȡ�¶�1
 * @param		: ��
 * @retval		: �����¶�ֵ
 * @notes		: 
 *****************************************************************************/
float AD7124_Get_Temp1(void)
{
    uint32_t data;
    float res;
    float temp;
    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
    printf("data:0x%08x\n", data);
    res = (5110.0 *data) / (16777215.0 *16.0);
    //	fdata = ((data - 8388608)/13584) - 272.5;
    printf("R:%0.4f\n\r", res);
    //	temp = PT100_RtoT(res);
    //	printf("T:%0.4f\n",temp);
    return temp;
}

/*******************************************************************************
 * @function	: AD7124_Get_Temp1
 * @brief		: ��ȡ�¶�1��·����
 * @param		: ��
 * @retval		: �����¶�ֵ
 * @notes		: 
 *****************************************************************************/
float AD7124_Get_Temp1_Res(void)
{
    uint32_t data;
    float res;
    float temp;
    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
    printf("data:0x%08x\n\r", data);
    res = (5110.0 *data) / (16777215.0 *16.0);
    //	printf("R:%0.4f\n",res);
    //	temp = PT100_RtoT(res);
    printf("T:%0.4f\n\r", temp);
    return temp;
}

/*******************************************************************************
 * @function	: AD7124_Reset
 * @brief		: ��ȡ�¶�1��·����
 * @param		: ��
 * @retval		: �����¶�ֵ
 * @notes		: 
 *****************************************************************************/
void AD7124_Reset(void)
{
    spi1nss=0;
    spi8_run(0xFF);
    spi8_run(0xFF);
    spi8_run(0xFF);
    spi8_run(0xFF);
    spi8_run(0xFF);
    spi8_run(0xFF);
    spi8_run(0xFF);
    spi8_run(0xFF);
    spi1nss=1;
}

float AD7124_Temp1_Test(void)
{
    uint32_t data;
    float pt100_res;
    float wire_res;
    float temp;

    //	AD7124_Write_Reg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
    Sys.Sleep(50);
    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    printf("data:0x%08x\n", data);
    pt100_res = (5110.0 *data) / (16777215.0 *16.0);
    printf("R:%0.4f\n", pt100_res);
    //	temp = PT100_RtoT(pt100_res);
    //	printf("T:%0.4f\n",temp);

    //	AD7124_Write_Reg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
    Sys.Sleep(50);
    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    printf("data:0x%08x\n", data);
    wire_res = (2.5 *data) / (16777215 *128 * 0.001);
    printf("R:%0.4f\n", wire_res);

    pt100_res = pt100_res - wire_res;
    //	temp = PT100_RtoT(pt100_res);
    //	printf("T:%0.4f\n",temp);

    return temp;
}

void AD7124_Test(void)
{
    uint32_t data;
    float fdata;
    //	data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
    //	printf("data:0x%08x\n",data);
    //	fdata = 2.5*data/16777215;
    ////	fdata = ((data - 8388608)/13584) - 272.5;
    //	printf("Volt:%0.4f\n",fdata);

    data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);

    printf("data:0x%08x\r\n", data);
    fdata = (2.5 *data) / (16777215 *128 * 0.001);
    printf("R:%0.4f\r\n", fdata);
}

#define EXINT_PORT					GPIOA
#define EXINT_PIN						GPIO_Pin_6
#define RCC_EXINT_PORT			RCC_APB2Periph_GPIOA
#define RCC_EXINT_PORT_CMD	RCC_APB2PeriphClockCmd
#define RCC_EXINT_AFIO			RCC_APB2Periph_AFIO
#define RCC_EXINT_AFIO_CMD	RCC_APB2PeriphClockCmd

/**********************************************************************************
 * @function	: ExInt_Config
 * @brief  	: ????IO?
 * @param  	: ?
 * @retval 	: ?
 * @notes		: 
 *********************************************************************************/
void AD7124_ExInt_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //GPIO
    RCC_EXINT_PORT_CMD(RCC_EXINT_PORT, ENABLE); /* ??GPIO?? */
    RCC_EXINT_AFIO_CMD(RCC_EXINT_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = EXINT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(EXINT_PORT, &GPIO_InitStructure);
    //EXTI Line
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    //EXTI
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void AD7124_ExInt_Disable(void)
{
    //EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
}

#define AD7124_BUFSIZE 6
uint32_t ad7124Buf[AD7124_BUFSIZE];
uint8_t ad7124BufCnt = 0;
uint8_t ad7124Int = 0;

void EXTI9_5_IRQHandler(void)
{
    uint32_t data;
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        ad7124Int = 1;
        AD7124_ExInt_Disable();
        data = AD7124_Read_Reg_NoCS(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
        ad7124Buf[ad7124BufCnt++] = data;
        if (ad7124BufCnt == AD7124_BUFSIZE)
        {
            ad7124BufCnt = 0;
        }
        AD7124_ExInt_Config();
    }
}

//����
void AD7124_IntTask(void)
{
    uint8_t cnt;
    //	float pt100;
    //	float wire;
    //	float temp;
    //	float volt;
    //	float flow;
    if (ad7124Int == 0)
    {
        return ;
    }
    ad7124Int = 0;
    if (ad7124BufCnt == 0)
    {
        cnt = AD7124_BUFSIZE - 1;
    }
    else
    {
        cnt = ad7124BufCnt - 1;
    }
    //printf("ch(%d):%08x\n",cnt,ad7124Buf[cnt]);
    //PT100
    if (cnt == AD7124_BUFSIZE - 1)
    {
        //		//printf("ch0:0x%08x\n",ad7124Buf[0]);
        //		//printf("ch1:0x%08x\n",ad7124Buf[1]);
        //		pt100 = (5110.0*ad7124Buf[0])/(16777215.0*16.0);
        //		//printf("PT100:%0.4f\n",pt100);
        //		wire = (2.5*ad7124Buf[1])/(16777215*128*0.001);
        //		//printf("wire:%0.4f\n",wire);
        //		pt100 = pt100-wire;
        //		//printf("R:%0.4fR\n",pt100);
        //		temp = PT100_RtoT(pt100);
        //		printf("R=%0.4f, T=%0.4f\n",pt100,temp);
        //		
        //		volt = 2*(2.5*ad7124Buf[3])/16777215;
        //		flow = (volt-1.0)/4*2;
        //		printf("V=%0.4f, F=%0.4f\n",volt,flow);
        //		//printf("T:%0.4fC\n",temp);

        printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n", ad7124Buf[0], ad7124Buf[1], ad7124Buf[2], ad7124Buf[3], ad7124Buf[4], ad7124Buf[5]);
        //USART5_printf("01\n");
    }
}

void ad7124test(void *param)
{
    //	auto data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
    //    printf("data:0x%08x\n", data);
    AD7124_Read_ID();
    AD7124_Read_Status();
    AD7124_Test();
}

void ad71248Test()
{
    AD7124_Init();

    Sys.AddTask(ad7124test, 0, 1000, 2000, "7124-8test");
}
