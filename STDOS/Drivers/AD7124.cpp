#include "AD7124.h"















#ifdef _AD7124_H_1

#ifndef __AD7124_SPI_H
#define __AD7124_SPI_H

#include "stdint.h"

/*
	(SPI1 : AD7124-8)
	PA4		:	SPI1-NSS	
	PA5		:	SPI1-SCK
	PA6		:	SPI1-MISO
	PA7		:	SPI1-MOSI
*/

/*********************** 模拟SPI通信引脚 **************************************/
#define SPI_NSS_PIN			GPIO_Pin_4
#define SPI_NSS_PORT		GPIOA

#define SPI_SCK_PIN			GPIO_Pin_5
#define SPI_SCK_PORT		GPIOA

#define SPI_MISO_PIN		GPIO_Pin_6
#define SPI_MISO_PORT		GPIOA

#define SPI_MOSI_PIN		GPIO_Pin_7
#define SPI_MOSI_PORT		GPIOA

/********************  **************************************/
#define SPI_NSS_L()		(SPI_NSS_PORT->BRR = SPI_NSS_PIN)
#define SPI_NSS_H()		(SPI_NSS_PORT->BSRR = SPI_NSS_PIN)
#define SPI_SCK_L()		(SPI_SCK_PORT->BRR = SPI_SCK_PIN)
#define SPI_SCK_H()		(SPI_SCK_PORT->BSRR = SPI_SCK_PIN)
#define SPI_MOSI_L()	(SPI_MOSI_PORT->BRR = SPI_MOSI_PIN)
#define SPI_MOSI_H()	(SPI_MOSI_PORT->BSRR = SPI_MOSI_PIN)

#define SPI_MISO()		GPIO_ReadInputDataBit(SPI_MISO_PORT,SPI_MISO_PIN)

void AD7124_SPI_Config(void);
uint8_t spi8_run(uint8_t sendData);
uint16_t spi16_run(uint16_t sendData);
uint32_t spi24_run(uint32_t sendData);
uint32_t spi32_run(uint32_t sendData);

#endif
#ifndef __AD7124_8_H
#define __AD7124_8_H

/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "stdint.h"

extern uint32_t ad7124Buf[];
/************************ AD7124 COMMS寄存器读写控制 *************************************/
#define AD7124_WR				0x00
#define AD7124_RD				0x40

/* AD7124 Register Map */
#define AD7124_COMM_REG      0x00
#define AD7124_STATUS_REG    0x00
#define AD7124_ADC_CTRL_REG  0x01
#define AD7124_DATA_REG      0x02
#define AD7124_IO_CTRL1_REG  0x03
#define AD7124_IO_CTRL2_REG  0x04
#define AD7124_ID_REG        0x05
#define AD7124_ERR_REG       0x06
#define AD7124_ERREN_REG     0x07
#define AD7124_CH0_MAP_REG   0x09
#define AD7124_CH1_MAP_REG   0x0A
#define AD7124_CH2_MAP_REG   0x0B
#define AD7124_CH3_MAP_REG   0x0C
#define AD7124_CH4_MAP_REG   0x0D
#define AD7124_CH5_MAP_REG   0x0E
#define AD7124_CH6_MAP_REG   0x0F
#define AD7124_CH7_MAP_REG   0x10
#define AD7124_CH8_MAP_REG   0x11
#define AD7124_CH9_MAP_REG   0x12
#define AD7124_CH10_MAP_REG  0x13
#define AD7124_CH11_MAP_REG  0x14
#define AD7124_CH12_MAP_REG  0x15
#define AD7124_CH13_MAP_REG  0x16
#define AD7124_CH14_MAP_REG  0x17
#define AD7124_CH15_MAP_REG  0x18
#define AD7124_CFG0_REG      0x19
#define AD7124_CFG1_REG      0x1A
#define AD7124_CFG2_REG      0x1B
#define AD7124_CFG3_REG      0x1C
#define AD7124_CFG4_REG      0x1D
#define AD7124_CFG5_REG      0x1E
#define AD7124_CFG6_REG      0x1F
#define AD7124_CFG7_REG      0x20
#define AD7124_FILT0_REG     0x21
#define AD7124_FILT1_REG     0x22
#define AD7124_FILT2_REG     0x23
#define AD7124_FILT3_REG     0x24
#define AD7124_FILT4_REG     0x25
#define AD7124_FILT5_REG     0x26
#define AD7124_FILT6_REG     0x27
#define AD7124_FILT7_REG     0x28
#define AD7124_OFFS0_REG     0x29
#define AD7124_OFFS1_REG     0x2A
#define AD7124_OFFS2_REG     0x2B
#define AD7124_OFFS3_REG     0x2C
#define AD7124_OFFS4_REG     0x2D
#define AD7124_OFFS5_REG     0x2E
#define AD7124_OFFS6_REG     0x2F
#define AD7124_OFFS7_REG     0x30
#define AD7124_GAIN0_REG     0x31
#define AD7124_GAIN1_REG     0x32
#define AD7124_GAIN2_REG     0x33
#define AD7124_GAIN3_REG     0x34
#define AD7124_GAIN4_REG     0x35
#define AD7124_GAIN5_REG     0x36
#define AD7124_GAIN6_REG     0x37
#define AD7124_GAIN7_REG     0x38

/* AD7124 Register Bytes */
#define AD7124_COMM_REG_BYTES    	 1
#define AD7124_STATUS_REG_BYTES    1
#define AD7124_ADC_CTRL_REG_BYTES  2
#define AD7124_DATA_REG_BYTES      3
#define AD7124_IO_CTRL1_REG_BYTES  3
#define AD7124_IO_CTRL2_REG_BYTES  2
#define AD7124_ID_REG_BYTES        1
#define AD7124_ERR_REG_BYTES       3
#define AD7124_ERREN_REG_BYTES     3
#define AD7124_CH0_MAP_REG_BYTES   2
#define AD7124_CH1_MAP_REG_BYTES   2
#define AD7124_CH2_MAP_REG_BYTES   2
#define AD7124_CH3_MAP_REG_BYTES   2
#define AD7124_CH4_MAP_REG_BYTES   2
#define AD7124_CH5_MAP_REG_BYTES   2
#define AD7124_CH6_MAP_REG_BYTES   2
#define AD7124_CH7_MAP_REG_BYTES   2
#define AD7124_CH8_MAP_REG_BYTES   2
#define AD7124_CH9_MAP_REG_BYTES   2
#define AD7124_CH10_MAP_REG_BYTES  2
#define AD7124_CH11_MAP_REG_BYTES  2
#define AD7124_CH12_MAP_REG_BYTES  2
#define AD7124_CH13_MAP_REG_BYTES  2
#define AD7124_CH14_MAP_REG_BYTES  2
#define AD7124_CH15_MAP_REG_BYTES  2
#define AD7124_CFG0_REG_BYTES      2
#define AD7124_CFG1_REG_BYTES      2
#define AD7124_CFG2_REG_BYTES      2
#define AD7124_CFG3_REG_BYTES      2
#define AD7124_CFG4_REG_BYTES      2
#define AD7124_CFG5_REG_BYTES      2
#define AD7124_CFG6_REG_BYTES      2
#define AD7124_CFG7_REG_BYTES      2
#define AD7124_FILT0_REG_BYTES     3
#define AD7124_FILT1_REG_BYTES     3
#define AD7124_FILT2_REG_BYTES     3
#define AD7124_FILT3_REG_BYTES     3
#define AD7124_FILT4_REG_BYTES     3
#define AD7124_FILT5_REG_BYTES     3
#define AD7124_FILT6_REG_BYTES     3
#define AD7124_FILT7_REG_BYTES     3
#define AD7124_OFFS0_REG_BYTES     3
#define AD7124_OFFS1_REG_BYTES     3
#define AD7124_OFFS2_REG_BYTES     3
#define AD7124_OFFS3_REG_BYTES     3
#define AD7124_OFFS4_REG_BYTES     3
#define AD7124_OFFS5_REG_BYTES     3
#define AD7124_OFFS6_REG_BYTES     3
#define AD7124_OFFS7_REG_BYTES     3
#define AD7124_GAIN0_REG_BYTES     3
#define AD7124_GAIN1_REG_BYTES     3
#define AD7124_GAIN2_REG_BYTES     3
#define AD7124_GAIN3_REG_BYTES     3
#define AD7124_GAIN4_REG_BYTES     3
#define AD7124_GAIN5_REG_BYTES     3
#define AD7124_GAIN6_REG_BYTES     3
#define AD7124_GAIN7_REG_BYTES     3

/* Communication Register bits */
#define AD7124_COMM_REG_WEN    (0 << 7)
#define AD7124_COMM_REG_WR     (0 << 6)
#define AD7124_COMM_REG_RD     (1 << 6)
#define AD7124_COMM_REG_RA(x)  ((x) & 0x3F)

/* Status Register bits */
#define AD7124_STATUS_REG_RDY          (1 << 7)
#define AD7124_STATUS_REG_ERROR_FLAG   (1 << 6)
#define AD7124_STATUS_REG_POR_FLAG     (1 << 4)
#define AD7124_STATUS_REG_CH_ACTIVE(x) ((x) & 0xF)

/* ADC_Control Register bits */
#define AD7124_ADC_CTRL_REG_DOUT_RDY_DEL   (1 << 12)
#define AD7124_ADC_CTRL_REG_CONT_READ      (1 << 11)
#define AD7124_ADC_CTRL_REG_DATA_STATUS    (1 << 10)
#define AD7124_ADC_CTRL_REG_CS_EN          (1 << 9)
#define AD7124_ADC_CTRL_REG_REF_EN         (1 << 8)
#define AD7124_ADC_CTRL_REG_POWER_MODE(x)  (((x) & 0x3) << 6)
#define AD7124_ADC_CTRL_REG_MODE(x)        (((x) & 0xF) << 2)
#define AD7124_ADC_CTRL_REG_CLK_SEL(x))    (((x) & 0x3) << 0)

/* IO_Control_1 Register bits */
#define AD7124_IO_CTRL1_REG_GPIO_DAT2     (1 << 23)
#define AD7124_IO_CTRL1_REG_GPIO_DAT1     (1 << 22)
#define AD7124_IO_CTRL1_REG_GPIO_CTRL2    (1 << 19)
#define AD7124_IO_CTRL1_REG_GPIO_CTRL1    (1 << 18)
#define AD7124_IO_CTRL1_REG_PDSW          (1 << 15)
#define AD7124_IO_CTRL1_REG_IOUT1(x)      (((x) & 0x7) << 11)
#define AD7124_IO_CTRL1_REG_IOUT0(x)      (((x) & 0x7) << 8)
#define AD7124_IO_CTRL1_REG_IOUT_CH1(x)   (((x) & 0xF) << 4)
#define AD7124_IO_CTRL1_REG_IOUT_CH0(x)   (((x) & 0xF) << 0)

/*IO_Control_1 AD7124-8 specific bits */
#define AD7124_8_IO_CTRL1_REG_GPIO_DAT4     (1 << 23)
#define AD7124_8_IO_CTRL1_REG_GPIO_DAT3     (1 << 22)
#define AD7124_8_IO_CTRL1_REG_GPIO_DAT2     (1 << 21)
#define AD7124_8_IO_CTRL1_REG_GPIO_DAT1     (1 << 20)
#define AD7124_8_IO_CTRL1_REG_GPIO_CTRL4    (1 << 19)
#define AD7124_8_IO_CTRL1_REG_GPIO_CTRL3    (1 << 18)
#define AD7124_8_IO_CTRL1_REG_GPIO_CTRL2    (1 << 17)
#define AD7124_8_IO_CTRL1_REG_GPIO_CTRL1    (1 << 16)

/* IO_Control_2 Register bits */
#define AD7124_IO_CTRL2_REG_GPIO_VBIAS7   (1 << 15)
#define AD7124_IO_CTRL2_REG_GPIO_VBIAS6   (1 << 14)
#define AD7124_IO_CTRL2_REG_GPIO_VBIAS5   (1 << 11)
#define AD7124_IO_CTRL2_REG_GPIO_VBIAS4   (1 << 10)
#define AD7124_IO_CTRL2_REG_GPIO_VBIAS3   (1 << 5)
#define AD7124_IO_CTRL2_REG_GPIO_VBIAS2   (1 << 4)
#define AD7124_IO_CTRL2_REG_GPIO_VBIAS1   (1 << 1)
#define AD7124_IO_CTRL2_REG_GPIO_VBIAS0   (1 << 0)

/*IO_Control_2 AD7124-8 specific bits */
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS15  (1 << 15)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS14  (1 << 14)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS13  (1 << 13)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS12  (1 << 12)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS11  (1 << 11)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS10  (1 << 10)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS9   (1 << 9)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS8   (1 << 8)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS7   (1 << 7)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS6   (1 << 6)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS5   (1 << 5)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS4   (1 << 4)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS3   (1 << 3)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS2   (1 << 2)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS1   (1 << 1)
#define AD7124_8_IO_CTRL2_REG_GPIO_VBIAS0   (1 << 0)

/* ID Register bits */
#define AD7124_ID_REG_DEVICE_ID(x)   (((x) & 0xF) << 4)
#define AD7124_ID_REG_SILICON_REV(x) (((x) & 0xF) << 0)

/* Error Register bits */
#define AD7124_ERR_REG_LDO_CAP_ERR        (1 << 19)
#define AD7124_ERR_REG_ADC_CAL_ERR        (1 << 18)
#define AD7124_ERR_REG_ADC_CONV_ERR       (1 << 17)
#define AD7124_ERR_REG_ADC_SAT_ERR        (1 << 16)
#define AD7124_ERR_REG_AINP_OV_ERR        (1 << 15)
#define AD7124_ERR_REG_AINP_UV_ERR        (1 << 14)
#define AD7124_ERR_REG_AINM_OV_ERR        (1 << 13)
#define AD7124_ERR_REG_AINM_UV_ERR        (1 << 12)
#define AD7124_ERR_REG_REF_DET_ERR        (1 << 11)
#define AD7124_ERR_REG_DLDO_PSM_ERR       (1 << 9)
#define AD7124_ERR_REG_ALDO_PSM_ERR       (1 << 7)
#define AD7124_ERR_REG_SPI_IGNORE_ERR     (1 << 6)
#define AD7124_ERR_REG_SPI_SLCK_CNT_ERR   (1 << 5)
#define AD7124_ERR_REG_SPI_READ_ERR       (1 << 4)
#define AD7124_ERR_REG_SPI_WRITE_ERR      (1 << 3)
#define AD7124_ERR_REG_SPI_CRC_ERR        (1 << 2)
#define AD7124_ERR_REG_MM_CRC_ERR         (1 << 1)

/* Error_En Register bits */
#define AD7124_ERREN_REG_MCLK_CNT_EN           (1 << 22)
#define AD7124_ERREN_REG_LDO_CAP_CHK_TEST_EN   (1 << 21)
#define AD7124_ERREN_REG_LDO_CAP_CHK(x)        (((x) & 0x3) << 19)
#define AD7124_ERREN_REG_ADC_CAL_ERR_EN        (1 << 18)
#define AD7124_ERREN_REG_ADC_CONV_ERR_EN       (1 << 17)
#define AD7124_ERREN_REG_ADC_SAT_ERR_EN        (1 << 16)
#define AD7124_ERREN_REG_AINP_OV_ERR_EN        (1 << 15)
#define AD7124_ERREN_REG_AINP_UV_ERR_EN        (1 << 14)
#define AD7124_ERREN_REG_AINM_OV_ERR_EN        (1 << 13)
#define AD7124_ERREN_REG_AINM_UV_ERR_EN        (1 << 12)
#define AD7124_ERREN_REG_REF_DET_ERR_EN        (1 << 11)
#define AD7124_ERREN_REG_DLDO_PSM_TRIP_TEST_EN (1 << 10)
#define AD7124_ERREN_REG_DLDO_PSM_ERR_ERR      (1 << 9)
#define AD7124_ERREN_REG_ALDO_PSM_TRIP_TEST_EN (1 << 8)
#define AD7124_ERREN_REG_ALDO_PSM_ERR_EN       (1 << 7)
#define AD7124_ERREN_REG_SPI_IGNORE_ERR_EN     (1 << 6)
#define AD7124_ERREN_REG_SPI_SCLK_CNT_ERR_EN   (1 << 5)
#define AD7124_ERREN_REG_SPI_READ_ERR_EN       (1 << 4)
#define AD7124_ERREN_REG_SPI_WRITE_ERR_EN      (1 << 3)
#define AD7124_ERREN_REG_SPI_CRC_ERR_EN        (1 << 2)
#define AD7124_ERREN_REG_MM_CRC_ERR_EN         (1 << 1)

/* Channel Registers 0-15 bits */
#define AD7124_CH_MAP_REG_CH_ENABLE    (1 << 15)
#define AD7124_CH_MAP_REG_SETUP(x)     (((x) & 0x7) << 12)
#define AD7124_CH_MAP_REG_AINP(x)      (((x) & 0x1F) << 5)    
#define AD7124_CH_MAP_REG_AINM(x)      (((x) & 0x1F) << 0)

/* Configuration Registers 0-7 bits */
#define AD7124_CFG_REG_BIPOLAR     (1 << 11)
#define AD7124_CFG_REG_BURNOUT(x)  (((x) & 0x3) << 9)
#define AD7124_CFG_REG_REF_BUFP    (1 << 8)
#define AD7124_CFG_REG_REF_BUFM    (1 << 7)
#define AD7124_CFG_REG_AIN_BUFP    (1 << 6)
#define AD7124_CFG_REG_AINN_BUFM   (1 << 5)
#define AD7124_CFG_REG_REF_SEL(x)  ((x) & 0x3) << 3
#define AD7124_CFG_REG_PGA(x)      (((x) & 0x7) << 0)

/* Filter Register 0-7 bits */
#define AD7124_FILT_REG_FILTER(x)         (((x) & 0x7) << 21)
#define AD7124_FILT_REG_REJ60             (1 << 20)
#define AD7124_FILT_REG_POST_FILTER(x)    (((x) & 0x7) << 17)
#define AD7124_FILT_REG_SINGLE_CYCLE      (1 << 16)
#define AD7124_FILT_REG_FS(x)             (((x) & 0x7FF) << 0)

/******************************************************************************/
/* Functions Prototypes                                                       */
/******************************************************************************/
void AD7124_Init(void);
void AD7124_Config(void);
void AD7124_ExInt_Config(void);

uint32_t AD7124_Read_Reg(uint8_t reg, uint8_t bytes);
void AD7124_Write_Reg(uint8_t reg, uint8_t bytes, uint32_t data);

void AD7124_Channel_Config(void);

void AD7124_Test(void);

uint8_t AD7124_Read_ID(void);
uint8_t AD7124_Read_Status(void);
void AD7124_Reset(void);
void AD7124_Test(void);

void AD7124_IntTask(void);
#endif

#include "stm32f10x.h"
#include "ad7124_spi.h"

/*******************************************************************************
  * @function	: AD7124_SPI_Config
  * @brief		: SPI端口配置，模拟SPI
  * @param		: 无
  * @retval		: 无
  * @notes		: 
  *****************************************************************************/
void AD7124_SPI_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//SPI_NSS
	GPIO_InitStructure.GPIO_Pin = SPI_NSS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_NSS_PORT, &GPIO_InitStructure);
	SPI_NSS_H();
	//SPI_SCK
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
	GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SPI_SCK_PORT, SPI_SCK_PIN);
	SPI_SCK_H();
	//SPI_MOSI
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SPI_MOSI_PORT, SPI_MOSI_PIN);
	SPI_MOSI_L();
	//SPI_MISO
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;;//
	GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);
	
	SPI_NSS_L();
}

/*******************************************************************************
	* @function	: SPI_Delay
  * @brief		: SPI总线位延时
  * @param		: 无
  * @retval		: 无
	* @notes		: 
  *****************************************************************************/
static void SPI_Delay(void)
{
	uint8_t i; 
	/*　
	下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
	CPU主频72MHz时，在内部Flash运行, MDK工程不优化
	循环次数为10时，SCL频率 = 205KHz 
	循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	IAR工程编译效率高，不能设置为7
	 */
	for(i = 0; i < 10; i++);
}

/*******************************************************************************
	* @function	: spi8_run
  * @brief		: 读写spi 1个字节
  * @param		: sendData
  * @retval		: rcvData
	* @notes		: 
  *****************************************************************************/
uint8_t spi8_run(uint8_t sendData)
{
	uint8_t spicnt = 8;
	uint8_t rcvData = 0;
	SPI_SCK_H();
	SPI_MOSI_H();
	while(spicnt-->0)
	{
		if(sendData & 0x80){
			SPI_MOSI_H();
		}
		else{
			SPI_MOSI_L();
		}
		SPI_Delay();
		SPI_SCK_L();
		sendData <<= 1;
		SPI_Delay();SPI_Delay();
		SPI_SCK_H();
		rcvData <<=1;
		rcvData |= SPI_MISO();
	}
	return rcvData;
}

/*******************************************************************************
	* @function	: spi16_run
  * @brief		: 读写spi 2个字节
  * @param		: sendData
  * @retval		: rcvData
	* @notes		: 
  *****************************************************************************/
uint16_t spi16_run(uint16_t sendData)
{
	uint8_t spicnt = 16;
	uint16_t rcvData = 0;
	SPI_SCK_H();
	SPI_MOSI_H();
	while(spicnt-->0)
	{
		if(sendData & 0x8000){
			SPI_MOSI_H();
		}
		else{
			SPI_MOSI_L();
		}
		SPI_Delay();
		SPI_SCK_L();
		sendData <<= 1;
		SPI_Delay();SPI_Delay();
		SPI_SCK_H();
		rcvData <<= 1;
		rcvData |= SPI_MISO();
	}
	return rcvData;
}

/*******************************************************************************
	* @function	: spi24_run
  * @brief		: 读写spi 4个字节
  * @param		: sendData
  * @retval		: rcvData
	* @notes		: 
  *****************************************************************************/
uint32_t spi24_run(uint32_t sendData)
{
	uint8_t spicnt = 24;
	uint32_t rcvData = 0;
	SPI_SCK_H();
	SPI_MOSI_H();
	while(spicnt-->0)
	{
		if(sendData & 0x800000){
			SPI_MOSI_H();
		}
		else{
			SPI_MOSI_L();
		}
		SPI_Delay();
		SPI_SCK_L();
		sendData <<= 1;
		SPI_Delay();SPI_Delay();
		SPI_SCK_H();
		rcvData <<= 1;
		rcvData |= SPI_MISO();
	}
	return rcvData;
}

/*******************************************************************************
	* @function	: SPI_Delay
  * @brief		: SPI总线位延时
  * @param		: sendData
  * @retval		: rcvData
	* @notes		: 
  *****************************************************************************/
uint32_t spi32_run(uint32_t sendData)
{
	uint8_t spicnt = 32;
	uint32_t rcvData = 0;
	SPI_SCK_H();
	SPI_MOSI_H();
	while(spicnt-->0)
	{
		if(sendData & 0x80000000){
			SPI_MOSI_H();
		}
		else{
			SPI_MOSI_L();
		}
		SPI_Delay();
		SPI_SCK_L();
		sendData <<= 1;
		SPI_Delay();SPI_Delay();
		SPI_SCK_H();
		rcvData <<= 1;
		rcvData |= SPI_MISO();
	}
	return rcvData;
}

/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "stm32f10x.h"
#include "ad7124.h"
#include "ad7124_spi.h"
#include "stdio.h"
#include "pt100.h"
#include "delay.h"
#include "usart5.h"
/*******************************************************************************
  * @function	: AD7124_Read_Reg
  * @brief		: 读取AD7124寄存器内容
  * @param		: reg   寄存器地址
  * @param		: bytes 寄存器大小
  * @retval		: 读取的内容
  * @notes		: 
  *****************************************************************************/
uint32_t AD7124_Read_Reg(uint8_t reg, uint8_t bytes)
{
	uint32_t retVal;
	SPI_NSS_L();
	spi8_run(AD7124_RD | reg);
	if(bytes == 1){
		retVal = spi8_run(0xFF);
	}
	else if(bytes == 2){
		retVal = spi16_run(0xFFFF);
	}
	else if(bytes == 3){
		retVal = spi24_run(0xFFFFFF);
	}
	SPI_NSS_H();
	return retVal;
}

/*******************************************************************************
  * @function	: AD7124_Read_Reg
  * @brief		: 读取AD7124寄存器内容
  * @param		: reg   寄存器地址
  * @param		: bytes 寄存器大小
  * @retval		: 读取的内容
  * @notes		: 
  *****************************************************************************/
uint32_t AD7124_Read_Reg_NoCS(uint8_t reg, uint8_t bytes)
{
	uint32_t retVal;
	spi8_run(AD7124_RD | reg);
	if(bytes == 1){
		retVal = spi8_run(0xFF);
	}
	else if(bytes == 2){
		retVal = spi16_run(0xFFFF);
	}
	else if(bytes == 3){
		retVal = spi24_run(0xFFFFFF);
	}
	return retVal;
}

/*******************************************************************************
  * @function	: AD7124_Write_Reg
  * @brief		: 写AD7124寄存器
  * @param		: reg	  寄存器地址
  * @param		: bytes 寄存器大小
  * @param		: data	写入的数据
  * @retval		: 无
  * @notes		: 
  *****************************************************************************/
void AD7124_Write_Reg(uint8_t reg, uint8_t bytes, uint32_t data)
{
	SPI_NSS_L();
	spi8_run(AD7124_WR | reg);
	if(bytes == 1){
		spi8_run(data);
	}
	else if(bytes == 2){
		spi16_run(data);
	}
	else if(bytes == 3){
		spi24_run(data);
	}
	SPI_NSS_H();	
}

//ADC_CONTROL			内部参考电压使能	低功率	连续转换模式	内部时钟不输出
//IO_CONTROL_1		无数字输出 	数字输出禁用	PDSW关	IOUT1关闭	IOUT0关闭	IOUT1_CH=0	IOUT0_CH=0
//IO_CONTROL_2		无偏置电压
//CONFIG0					单极性	Burnout关	基准电压缓冲禁用	模拟输入缓冲禁用	基准电压REF1	PGA=16
//CONFIG1					单极性	Burnout关	基准电压缓冲禁用	模拟输入缓冲禁用	基准电压REF2	PGA=16
//CONFIG2					单极性	Burnout关	基准电压缓冲禁用	模拟输入缓冲禁用	基准电压内部	PGA=128
//CONFIG3					单极性	Burnout关	基准电压缓冲禁用	模拟输入缓冲禁用	基准电压内部	PGA=1
//CONFIG4					双极性	Burnout关	基准电压缓冲禁用	模拟输入缓冲禁用	基准电压内部	PGA=1
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
  * @brief		: SPI端口配置，模拟SPI
  * @param		: 无
  * @retval		: 无
  * @notes		: 
  *****************************************************************************/
void AD7124_Init(void)
{
	AD7124_SPI_Config();		//SPI IO端口配置
	AD7124_Config();				//ADC 配置
	AD7124_Channel_Config();		//
	AD7124_ExInt_Config();	//外部中断口配置
	SPI_NSS_L();
}

//IO_CTRL_1_TEMP1				无数字输出 	数字输出禁用	PDSW关	IOUT1关闭			IOUT0=500uA	IOUT1_CH=0		IOUT0_CH=AIN0
//IO_CTRL_1_TEMP1_RES		无数字输出 	数字输出禁用	PDSW关	IOUT1=1000uA	IOUT0关闭		IOUT1_CH=AIN1	IOUT0_CH=0
//IO_CTRL_1_TEMP2				无数字输出 	数字输出禁用	PDSW关	IOUT1关闭			IOUT0=500uA	IOUT1_CH=0		IOUT0_CH=AIN4
//IO_CTRL_1_TEMP2_RES		无数字输出 	数字输出禁用	PDSW关	IOUT1=1000uA	IOUT0关闭		IOUT1_CH=AIN7	IOUT0_CH=0
#define AD7124_IO_CTRL1_REG_DATA_TEMP1			AD7124_IO_CTRL1_REG_IOUT1(0) | AD7124_IO_CTRL1_REG_IOUT0(4) | AD7124_IO_CTRL1_REG_IOUT_CH1(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(0)
#define AD7124_IO_CTRL1_REG_DATA_TEMP1_RES	AD7124_IO_CTRL1_REG_IOUT1(6) | AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH1(1) | AD7124_IO_CTRL1_REG_IOUT_CH0(0)
#define AD7124_IO_CTRL1_REG_DATA_TEMP2			AD7124_IO_CTRL1_REG_IOUT1(0) | AD7124_IO_CTRL1_REG_IOUT0(4) | AD7124_IO_CTRL1_REG_IOUT_CH1(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(4)
#define AD7124_IO_CTRL1_REG_DATA_TEMP2_RES	AD7124_IO_CTRL1_REG_IOUT1(6) | AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH1(7) | AD7124_IO_CTRL1_REG_IOUT_CH0(0)

//IO_CTRL_1_TEMP1_TEST	无数字输出 	数字输出禁用	PDSW关	IOUT1=500uA		IOUT0=500uA	IOUT1_CH=AIN1		IOUT0_CH=AIN0
#define AD7124_IO_CTRL1_REG_DATA_TEMP1_TEST	AD7124_IO_CTRL1_REG_IOUT1(4) | AD7124_IO_CTRL1_REG_IOUT0(4) | AD7124_IO_CTRL1_REG_IOUT_CH1(1) | AD7124_IO_CTRL1_REG_IOUT_CH0(0)

//温度1						CH0		ENABLE	CONFIG0		AINP[AIN2]		AINM[AIN3]
//温度1线路电阻		CH1		ENABLE	CONFIG2		AINP[AIN3]		AINM[AVSS]
//温度2						CH2		ENABLE	CONFIG1		AINP[AIN5]		AINM[AIN6]
//温度2线路电阻		CH3		ENABLE	CONFIG2		AINP[AIN6]		AINM[AVSS]
//流量A						CH4		ENABLE	CONFIG3		AINP[AIN8]		AINM[AVSS]
//流量B						CH5		ENABLE	CONFIG3		AINP[AIN9]		AINM[AVSS]
//TC1047					CH6		ENABLE	CONFIG3		AINP[AIN10]		AINM[AVSS]
//AD内部温度			CH7		ENABLE	CONFIG4		AINP[温度]		AINM[AVSS]
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
  * @brief		:	ADC通道配置
  * @param		: 无
  * @retval		: 无
  * @notes		: 
  *****************************************************************************/
void AD7124_Channel_Config(void)
{
	AD7124_Write_Reg(AD7124_IO_CTRL1_REG, AD7124_IO_CTRL1_REG_BYTES, 	AD7124_IO_CTRL1_REG_DATA_TEMP1_TEST);
	AD7124_TEMP1_CH_CFG();
	AD7124_TEMP1_RES_CH_CFG();
	AD7124_FLOWA_CH_CFG();
	AD7124_FLOWB_CH_CFG();
	AD7124_BRDTEMP_CH_CFG();
	AD7124_ADTEMP_CH_CFG();
}

/*******************************************************************************
  * @function	: AD7124_Read_ID
  * @brief		: 读取AD7124 ID寄存器
  * @param		: 无
  * @retval		: 无
  * @notes		: 
  *****************************************************************************/
uint8_t AD7124_Read_ID(void)
{
	uint8_t retVal;
	retVal = AD7124_Read_Reg(AD7124_ID_REG, AD7124_ID_REG_BYTES);
	printf("ID:0x%02x\n",retVal);
	return retVal;
}

/*******************************************************************************
  * @function	: AD7124_Read_Status
  * @brief		: 读取AD7124 状态寄存器
  * @param		: 无
  * @retval		: 无
  * @notes		: 
  *****************************************************************************/
uint8_t AD7124_Read_Status(void)
{
	uint8_t retVal;
	retVal = AD7124_Read_Reg(AD7124_STATUS_REG, AD7124_STATUS_REG_BYTES);
	printf("Status:0x%02x\n",retVal);
	return retVal;
}
/*******************************************************************************
  * @function	: AD7124_Get_Temp1
  * @brief		: 获取温度1
  * @param		: 无
  * @retval		: 返回温度值
  * @notes		: 
  *****************************************************************************/
float AD7124_Get_Temp1(void)
{
	uint32_t data;
	float res;
	float temp;
	data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
	printf("data:0x%08x\n",data);
	res = (5110.0*data)/(16777215.0*16.0);
//	fdata = ((data - 8388608)/13584) - 272.5;
	printf("R:%0.4f\n",res);
	temp = PT100_RtoT(res);
	printf("T:%0.4f\n",temp);
	return temp;
}

/*******************************************************************************
  * @function	: AD7124_Get_Temp1
  * @brief		: 获取温度1线路电阻
  * @param		: 无
  * @retval		: 返回温度值
  * @notes		: 
  *****************************************************************************/
float AD7124_Get_Temp1_Res(void)
{
	uint32_t data;
	float res;
	float temp;
	data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
	printf("data:0x%08x\n",data);
	res = (5110.0*data)/(16777215.0*16.0);
	printf("R:%0.4f\n",res);
	temp = PT100_RtoT(res);
	printf("T:%0.4f\n",temp);
	return temp;
}

/*******************************************************************************
  * @function	: AD7124_Reset
  * @brief		: 获取温度1线路电阻
  * @param		: 无
  * @retval		: 返回温度值
  * @notes		: 
  *****************************************************************************/
void AD7124_Reset(void)
{
	SPI_NSS_L();
	spi8_run(0xFF);
	spi8_run(0xFF);
	spi8_run(0xFF);
	spi8_run(0xFF);
	spi8_run(0xFF);
	spi8_run(0xFF);
	spi8_run(0xFF);
	spi8_run(0xFF);
	SPI_NSS_H();
}

float AD7124_Temp1_Test(void)
{
	uint32_t data;
	float pt100_res;
	float wire_res;
	float temp;

//	AD7124_Write_Reg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
	Delay_ms(50);
	data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
	
	printf("data:0x%08x\n",data);
	pt100_res = (5110.0*data)/(16777215.0*16.0);
	printf("R:%0.4f\n",pt100_res);
	temp = PT100_RtoT(pt100_res);
	printf("T:%0.4f\n",temp);
	
//	AD7124_Write_Reg(AD7124_ADC_CTRL_REG, AD7124_ADC_CTRL_REG_BYTES, 0X0004);
	Delay_ms(50);
	data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
	
	printf("data:0x%08x\n",data);
	wire_res = (2.5*data)/(16777215*128*0.001);
	printf("R:%0.4f\n",wire_res);
	
	pt100_res = pt100_res-wire_res;
	temp = PT100_RtoT(pt100_res);
	printf("T:%0.4f\n",temp);
	
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
	
	printf("data:0x%08x\n",data);
	fdata = (2.5*data)/(16777215*128*0.001);
	printf("R:%0.4f\n",fdata);
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
	RCC_EXINT_PORT_CMD(RCC_EXINT_PORT, ENABLE);  /* ??GPIO?? */
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
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		ad7124Int = 1; 
		AD7124_ExInt_Disable();
		data = AD7124_Read_Reg_NoCS(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
		ad7124Buf[ad7124BufCnt++] = data;
		if(ad7124BufCnt == AD7124_BUFSIZE){
			ad7124BufCnt = 0;
		}
		AD7124_ExInt_Config();
	}
}

//测试
void AD7124_IntTask(void)
{
	uint8_t cnt;
//	float pt100;
//	float wire;
//	float temp;
//	float volt;
//	float flow;
	if(ad7124Int == 0){
		return;
	}
	ad7124Int = 0;
	if(ad7124BufCnt == 0){
		cnt = AD7124_BUFSIZE-1;
	}else{
		cnt = ad7124BufCnt-1;
	}
	//printf("ch(%d):%08x\n",cnt,ad7124Buf[cnt]);
	//PT100
	if(cnt == AD7124_BUFSIZE-1)
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
		
		USART5_printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",ad7124Buf[0],ad7124Buf[1],ad7124Buf[2],ad7124Buf[3],ad7124Buf[4],ad7124Buf[5]);
		//USART5_printf("01\n");
	}
}


#endif
