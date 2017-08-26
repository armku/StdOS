#include "AD7124.h"

#if 1
//-8
OutputPort spi1nss(PE2,false,false);
OutputPort spi1sck(PE4,false,false);
OutputPort spi1miso(PE5,false,false);
OutputPort spi1mosi(PE6,false,false);
#else
//-4
OutputPort spi1nss(PG12,false,false);
OutputPort spi1sck(PG13,false,false);
OutputPort spi1miso(PG14,false,false);
OutputPort spi1mosi(PG15,false,false);
#endif
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

#if AD7124SPISOFT
    AD7124::AD7124(SpiSoft *spi)
#else 
    AD7124::AD7124(Spi *spi)
#endif 
{

}
//返回-1,表示转换未完成
int AD7124::Read(void)
{
    return 0;
}

//读取AD转换是否正常
bool AD7124::GetFlag(void)
{
    return true;
}

#define AD7124_ADC_CTRL_REG_DATA	AD7124_ADC_CTRL_REG_REF_EN | AD7124_ADC_CTRL_REG_POWER_MODE(1)//| AD7124_ADC_CTRL_REG_MODE(1)
#define AD7124_IO_CTRL1_REG_DATA	0
#define AD7124_IO_CTRL2_REG_DATA	0
#define AD7124_CFG0_REG_DATA	AD7124_CFG_REG_REF_SEL(0)|AD7124_CFG_REG_PGA(4)
#define AD7124_CFG1_REG_DATA	AD7124_CFG_REG_REF_SEL(1)|AD7124_CFG_REG_PGA(4)
#define AD7124_CFG2_REG_DATA	AD7124_CFG_REG_REF_SEL(2)|AD7124_CFG_REG_PGA(7)
#define AD7124_CFG3_REG_DATA	AD7124_CFG_REG_REF_SEL(2)|AD7124_CFG_REG_PGA(0)
#define AD7124_CFG4_REG_DATA	AD7124_CFG_REG_REF_SEL(2)|AD7124_CFG_REG_PGA(0)
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

/*
SPI总线位延时
*/
static void SPI_Delay(void)
{
    byte i;
    /*　
    下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
    CPU主频72MHz时，在内部Flash运行, MDK工程不优化
    循环次数为10时，SCL频率 = 205KHz 
    循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
    循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
    IAR工程编译效率高，不能设置为7
     */
    for (i = 0; i < 100; i++)
        ;
}

/*******************************************************************************
 * @brief		: 读写spi 1个字节
 * @param		: sendData
 *****************************************************************************/
byte spi8_run(byte sendData)
{
    byte spicnt = 8;
    byte rcvData = 0;
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
 * @brief		: 读写spi 2个字节
 * @param		: sendData
 *****************************************************************************/
uint16_t spi16_run(uint16_t sendData)
{
    byte spicnt = 16;
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
 * @brief		: 读写spi 4个字节
 * @param		: sendData
 *****************************************************************************/
uint32_t spi24_run(uint32_t sendData)
{
    byte spicnt = 24;
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
 * @brief		: SPI总线位延时
 * @param		: sendData
 *****************************************************************************/
uint32_t spi32_run(uint32_t sendData)
{
    byte spicnt = 32;
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

/*******************************************************************************
 * @brief		: 读取AD7124寄存器内容
 * @param		: reg   寄存器地址
 * @param		: bytes 寄存器大小
 * @retval		: 读取的内容
 *****************************************************************************/
uint32_t AD7124_Read_Reg(byte reg, byte bytes)
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
 * @brief		: 读取AD7124寄存器内容
 * @param		: reg   寄存器地址
 * @param		: bytes 寄存器大小
 * @retval		: 读取的内容
 *****************************************************************************/
uint32_t AD7124_Read_Reg_NoCS(byte reg, byte bytes)
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
 * @brief		: 写AD7124寄存器
 * @param		: reg	  寄存器地址
 * @param		: bytes 寄存器大小
 * @param		: data	写入的数据
 *****************************************************************************/
void AD7124_Write_Reg(byte reg, byte bytes, uint32_t data)
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

/*
ADC通道配置
*/
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

/*
读取AD7124 ID寄存器
*/
byte AD7124_Read_ID(void)
{
    auto retVal = AD7124_Read_Reg(AD7124_ID_REG, 1);
    printf("ID:0x%02x\n", retVal);
    return retVal;
}

/*
读取AD7124 状态寄存器
*/
byte AD7124_Read_Status(void)
{
    auto retVal = AD7124_Read_Reg(AD7124_STATUS_REG, AD7124_STATUS_REG_BYTES);
    printf("Status:0x%02x\n", retVal);
    return retVal;
}

/*
获取温度1线路电阻
*/
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

void AD7124_Init(void)
{    
	spi1nss=1; 
    spi1sck=1; 
    spi1mosi=0; 
    
    spi1nss=0;
	
    AD7124_Config(); //ADC 配置
    AD7124_Channel_Config(); //    
    spi1nss=0;
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/* Error codes */
#define INVALID_VAL -1 /* Invalid argument */
#define COMM_ERR    -2 /* Communication error on receive */
#define TIMEOUT     -3 /* A timeout has occured */

/***************************************************************************//**
* @brief Reads the value of the specified register without checking if the
*        device is ready to accept user requests.
*
* @param device - The handler of the instance of the driver.
* @param pReg - Pointer to the register structure holding info about the
*               register to be read. The read value is stored inside the
*               register structure.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_NoCheckReadRegister(ad7124_device *device, ad7124_st_reg* pReg)
{
	int32_t ret       = 0;
	uint8_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t i         = 0;
	uint8_t check8    = 0;
	uint8_t msgBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	if(!device || !pReg)
		return INVALID_VAL;

	/* Build the Command word */
	buffer[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_RD |
	  		AD7124_COMM_REG_RA(pReg->addr);

	/* Read data from the device */
	ret = SPI_Read(device->slave_select_id,
			buffer,
			((device->useCRC != AD7124_DISABLE_CRC) ? pReg->size + 1
							: pReg->size) + 1);
	if(ret < 0)
		return ret;

	/* Check the CRC */
	if(device->useCRC == AD7124_USE_CRC)
	{
		msgBuf[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_RD | 
		  		AD7124_COMM_REG_RA(pReg->addr);
		for(i = 1; i < pReg->size + 2; ++i)
		{
			msgBuf[i] = buffer[i];
		}
		check8 = AD7124_ComputeCRC8(msgBuf, pReg->size + 2);
	}

	if(check8 != 0)
	{
		/* ReadRegister checksum failed. */
		return COMM_ERR;
	}

	/* Build the result */
	pReg->value = 0;
	for(i = 1; i < pReg->size + 1; i++)
	{
		pReg->value <<= 8;
		pReg->value += buffer[i];
	}

	return ret;
}

/***************************************************************************//**
* @brief Writes the value of the specified register without checking if the
*        device is ready to accept user requests.
*
* @param device - The handler of the instance of the driver.
* @param reg - Register structure holding info about the register to be written
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_NoCheckWriteRegister(ad7124_device *device, ad7124_st_reg reg)
{
	int32_t ret      = 0;
	int32_t regValue = 0;
	uint8_t wrBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t i        = 0;
	uint8_t crc8     = 0;

	if(!device)
		return INVALID_VAL;

	/* Build the Command word */
	wrBuf[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_WR |
	  		AD7124_COMM_REG_RA(reg.addr);

	/* Fill the write buffer */
	regValue = reg.value;
	for(i = 0; i < reg.size; i++)
	{
		wrBuf[reg.size - i] = regValue & 0xFF;
		regValue >>= 8;
	}

	/* Compute the CRC */
	if(device->useCRC != AD7124_DISABLE_CRC)
	{
		crc8 = AD7124_ComputeCRC8(wrBuf, reg.size + 1);
		wrBuf[reg.size + 1] = crc8;
	}

	/* Write data to the device */
	ret = SPI_Write(device->slave_select_id,
			wrBuf,
			(device->useCRC != AD7124_DISABLE_CRC) ? reg.size + 2
							: reg.size + 1);

	return ret;
}

/***************************************************************************//**
* @brief Reads the value of the specified register only when the device is ready
*        to accept user requests. If the device ready flag is deactivated the
*        read operation will be executed without checking the device state.
*
* @param device - The handler of the instance of the driver.
* @param pReg - Pointer to the register structure holding info about the
*               register to be read. The read value is stored inside the
*               register structure.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_ReadRegister(ad7124_device *device, ad7124_st_reg* pReg)
{
	int32_t ret;
	
	if (pReg->addr != AD7124_ERR_REG && device->check_ready)
	{
		ret = AD7124_WaitForSpiReady(device, device->spi_rdy_poll_cnt);
		if (ret < 0)
			return ret;
	}
	ret = AD7124_NoCheckReadRegister(device, pReg);
	
	return ret;
}

/***************************************************************************//**
* @brief Writes the value of the specified register only when the device is
*        ready to accept user requests. If the device ready flag is deactivated
*        the write operation will be executed without checking the device state.
*
* @param device - The handler of the instance of the driver.
* @param reg - Register structure holding info about the register to be written
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WriteRegister(ad7124_device *device, ad7124_st_reg pReg)
{
	int32_t ret;
	
	if (device->check_ready)
	{  
		ret = AD7124_WaitForSpiReady(device, device->spi_rdy_poll_cnt);
		if (ret < 0)
			return ret;
	}
	ret = AD7124_NoCheckWriteRegister(device, pReg);
	
	return ret;
}

/***************************************************************************//**
* @brief Resets the device.
*
* @param device - The handler of the instance of the driver.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_Reset(ad7124_device *device)
{
	int32_t ret = 0;
	uint8_t wrBuf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	if(!device)
		return INVALID_VAL;

	ret = SPI_Write(device->slave_select_id, wrBuf, 8);

	/* Wait for the reset to complete */
	ret = AD7124_WaitToPowerOn(device, device->spi_rdy_poll_cnt);

	return ret;
}

/***************************************************************************//**
* @brief Waits until the device can accept read and write user actions.
*
* @param device - The handler of the instance of the driver.
* @param timeout - Count representing the number of polls to be done until the
*                  function returns.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WaitForSpiReady(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t ready = 0;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	while(!ready && --timeout)
	{
		/* Read the value of the Error Register */
		ret = AD7124_ReadRegister(device, &regs[AD7124_Error]);
		if(ret < 0)
			return ret;

		/* Check the SPI IGNORE Error bit in the Error Register */
		ready = (regs[AD7124_Error].value &
			 	AD7124_ERR_REG_SPI_IGNORE_ERR) == 0;
	}

	return timeout ? 0 : TIMEOUT;
}

/***************************************************************************//**
* @brief Waits until the device finishes the power-on reset operation.
*
* @param device - The handler of the instance of the driver.
* @param timeout - Count representing the number of polls to be done until the
*                  function returns.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WaitToPowerOn(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t powered_on = 0;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	while(!powered_on && timeout--)
	{
		ret = AD7124_ReadRegister(device, &regs[AD7124_Status]);
		if(ret < 0)
			return ret;

		/* Check the POR_FLAG bit in the Status Register */
		powered_on = (regs[AD7124_Status].value &
				AD7124_STATUS_REG_POR_FLAG) == 0;
	}

	return (timeout || powered_on) ? 0 : TIMEOUT;
}

/***************************************************************************//**
* @brief Waits until a new conversion result is available.
*
* @param device - The handler of the instance of the driver.
* @param timeout - Count representing the number of polls to be done until the
*                  function returns if no new data is available.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WaitForConvReady(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t ready = 0;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	while(!ready && --timeout)
	{
		/* Read the value of the Status Register */
		ret = AD7124_ReadRegister(device, &regs[AD7124_Status]);
		if(ret < 0)
			return ret;

		/* Check the RDY bit in the Status Register */
		ready = (regs[AD7124_Status].value &
				AD7124_STATUS_REG_RDY) == 0;
	}

	return timeout ? 0 : TIMEOUT;
}

/***************************************************************************//**
* @brief Reads the conversion result from the device.
*
* @param device - The handler of the instance of the driver.
* @param pData - Pointer to store the read data.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_ReadData(ad7124_device *device, int32_t* pData)
{
	ad7124_st_reg *regs;
	int32_t ret;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	/* Read the value of the Data Register */
	ret = AD7124_ReadRegister(device, &regs[AD7124_Data]);

	/* Get the read result */
	*pData = regs[AD7124_Data].value;

	return ret;
}

/***************************************************************************//**
* @brief Computes the CRC checksum for a data buffer.
*
* @param pBuf - Data buffer
* @param bufSize - Data buffer size in bytes
*
* @return Returns the computed CRC checksum.
*******************************************************************************/
uint8_t AD7124_ComputeCRC8(uint8_t * pBuf, uint8_t bufSize)
{
	uint8_t i   = 0;
	uint8_t crc = 0;

	while(bufSize)
	{
		for(i = 0x80; i != 0; i >>= 1)
		{
			if(((crc & 0x80) != 0) != ((*pBuf & i) != 0)) /* MSB of CRC register XOR input Bit from Data */
			{
				crc <<= 1;
				crc ^= AD7124_CRC8_POLYNOMIAL_REPRESENTATION;
			}
			else
			{
				crc <<= 1;
			}
		}
		pBuf++;
		bufSize--;
	}
	return crc;
}

/***************************************************************************//**
* @brief Updates the CRC settings.
*
* @param device - The handler of the instance of the driver.
*
* @return None.
*******************************************************************************/
void AD7124_UpdateCRCSetting(ad7124_device *device)
{
	ad7124_st_reg *regs;

	if(!device)
		return;

	regs = device->regs;
	
	/* Get CRC State. */
	if (regs[AD7124_Error_En].value & AD7124_ERREN_REG_SPI_CRC_ERR_EN)
	{
		device->useCRC = AD7124_USE_CRC;
	}
	else
	{
		device->useCRC = AD7124_DISABLE_CRC;
	}
}

/***************************************************************************//**
* @brief Updates the device SPI interface settings.
*
* @param device - The handler of the instance of the driver.
*
* @return None.
*******************************************************************************/
void AD7124_UpdateDevSpiSettings(ad7124_device *device)
{
	ad7124_st_reg *regs;

	if(!device)
		return;

	regs = device->regs;
	
	if (regs[AD7124_Error_En].value & AD7124_ERREN_REG_SPI_IGNORE_ERR_EN)
	{
		device->check_ready = 1;
	}
	else
	{
		device->check_ready = 0;
	}
}

/***************************************************************************//**
* @brief Initializes the AD7124.
*
* @param device - The handler of the instance of the driver.
* @param slave_select - The Slave Chip Select Id to be passed to the SPI calls.
* @param regs - The list of registers of the device (initialized or not) to be
*               added to the instance of the driver.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_Setup(ad7124_device *device, int slave_select,
			ad7124_st_reg *regs)
{
	int32_t ret;
	enum ad7124_registers regNr;

	if(!device || !regs)
		return INVALID_VAL;

	device->regs = regs;
	device->slave_select_id = slave_select;
	device->spi_rdy_poll_cnt = 25000;

	/* Initialize the SPI communication. */
	ret = SPI_Init(0, 1000000, 1, 0);
	if (ret < 0)
		return ret;

	/*  Reset the device interface.*/
	ret = AD7124_Reset(device);
	if (ret < 0)
		return ret;
	
	/* Update the device structure with power-on/reset settings */
	device->check_ready = 1;

	/* Initialize registers AD7124_ADC_Control through AD7124_Filter_7. */
	for(int regNr = AD7124_Status; (regNr < AD7124_Offset_0) && !(ret < 0);
		regNr++)
	{
		if (regs[regNr].rw == AD7124_RW)
		{
			ret = AD7124_WriteRegister(device, regs[regNr]);
			if (ret < 0)
		  		break;
		}
		
		/* Get CRC State and device SPI interface settings */
		if (regNr == AD7124_Error_En)
		{
			AD7124_UpdateCRCSetting(device);
			AD7124_UpdateDevSpiSettings(device);
		}
	}

	return ret;
}
////////////////////////////////////////////////////////////////////////
/***************************************************************************//**
 * @brief Initializes the SPI communication peripheral.
 *
 * @param lsbFirst - Transfer format (0 or 1).
 *                   Example: 0x0 - MSB first.
 *                            0x1 - LSB first.
 * @param clockFreq - SPI clock frequency (Hz).
 *                    Example: 1000 - SPI clock frequency is 1 kHz.
 * @param clockPol - SPI clock polarity (0 or 1).
 *                   Example: 0x0 - Idle state for clock is a low level; active
 *                                  state is a high level;
 *	                      0x1 - Idle state for clock is a high level; active
 *                                  state is a low level.
 * @param clockEdg - SPI clock edge (0 or 1).
 *                   Example: 0x0 - Serial output data changes on transition
 *                                  from idle clock state to active clock state;
 *                            0x1 - Serial output data changes on transition
 *                                  from active clock state to idle clock state.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful;
 *                           0 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char SPI_Init(unsigned char lsbFirst,
                       unsigned long clockFreq,
                       unsigned char clockPol,
                       unsigned char clockEdg)
{
    /* Add your code here. */
	return 0;
}

/***************************************************************************//**
 * @brief Reads data from SPI.
 *
 * @param slaveDeviceId - The ID of the selected slave device.
 * @param data - Data represents the write buffer as an input parameter and the
 *               read buffer as an output parameter.
 * @param bytesNumber - Number of bytes to read.
 *
 * @return Number of read bytes.
*******************************************************************************/
unsigned char SPI_Read(unsigned char slaveDeviceId,
                       unsigned char* data,
                       unsigned char bytesNumber)
{
    /* Add your code here. */
	for(auto i=0;i<bytesNumber;i++)
	{
		data[i]=spi8_run(0xff);
	}
	return bytesNumber;
}

/***************************************************************************//**
 * @brief Writes data to SPI.
 *
 * @param slaveDeviceId - The ID of the selected slave device.
 * @param data - Data represents the write buffer.
 * @param bytesNumber - Number of bytes to write.
 *
 * @return Number of written bytes.
*******************************************************************************/
unsigned char SPI_Write(unsigned char slaveDeviceId,
                        unsigned char* data,
                        unsigned char bytesNumber)
{
    /* Add your code here. */
	for(auto i=0;i<bytesNumber;i++)
	{
		spi8_run(data[i]);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
ad7124_st_reg ad7124_regs[AD7124_REG_NO] =
{
	{0x00, 0x00,   1, 2}, /* AD7124_Status */
	{0x01, 0x0000, 2, 1}, /* AD7124_ADC_Control */
	{0x02, 0x0000, 3, 2}, /* AD7124_Data */
	{0x03, 0x0000, 3, 1}, /* AD7124_IOCon1 */
	{0x04, 0x0000, 2, 1}, /* AD7124_IOCon2 */
	{0x05, 0x02,   1, 2}, /* AD7124_ID */
	{0x06, 0x0000, 3, 2}, /* AD7124_Error */
	{0x07, 0x0044, 3, 1}, /* AD7124_Error_En */
	{0x08, 0x00,   1, 2}, /* AD7124_Mclk_Count */
	{0x09, 0x8001, 2, 1}, /* AD7124_Channel_0 */
	{0x0A, 0x0001, 2, 1}, /* AD7124_Channel_1 */
	{0x0B, 0x0001, 2, 1}, /* AD7124_Channel_2 */
	{0x0C, 0x0001, 2, 1}, /* AD7124_Channel_3 */
	{0x0D, 0x0001, 2, 1}, /* AD7124_Channel_4 */
	{0x0E, 0x0001, 2, 1}, /* AD7124_Channel_5 */
	{0x0F, 0x0001, 2, 1}, /* AD7124_Channel_6 */
	{0x10, 0x0001, 2, 1}, /* AD7124_Channel_7 */
	{0x11, 0x0001, 2, 1}, /* AD7124_Channel_8 */
	{0x12, 0x0001, 2, 1}, /* AD7124_Channel_9 */
	{0x13, 0x0001, 2, 1}, /* AD7124_Channel_10 */
	{0x14, 0x0001, 2, 1}, /* AD7124_Channel_11 */
	{0x15, 0x0001, 2, 1}, /* AD7124_Channel_12 */
	{0x16, 0x0001, 2, 1}, /* AD7124_Channel_13 */
	{0x17, 0x0001, 2, 1}, /* AD7124_Channel_14 */
	{0x18, 0x0001, 2, 1}, /* AD7124_Channel_15 */
	{0x19, 0x0860, 2, 1}, /* AD7124_Config_0 */
	{0x1A, 0x0860, 2, 1}, /* AD7124_Config_1 */
	{0x1B, 0x0860, 2, 1}, /* AD7124_Config_2 */
	{0x1C, 0x0860, 2, 1}, /* AD7124_Config_3 */
	{0x1D, 0x0860, 2, 1}, /* AD7124_Config_4 */
	{0x1E, 0x0860, 2, 1}, /* AD7124_Config_5 */
	{0x1F, 0x0860, 2, 1}, /* AD7124_Config_6 */
	{0x20, 0x0860, 2, 1}, /* AD7124_Config_7 */
	{0x21, 0x060180, 3, 1}, /* AD7124_Filter_0 */
	{0x22, 0x060180, 3, 1}, /* AD7124_Filter_1 */
	{0x23, 0x060180, 3, 1}, /* AD7124_Filter_2 */
	{0x24, 0x060180, 3, 1}, /* AD7124_Filter_3 */
	{0x25, 0x060180, 3, 1}, /* AD7124_Filter_4 */
	{0x26, 0x060180, 3, 1}, /* AD7124_Filter_5 */
	{0x27, 0x060180, 3, 1}, /* AD7124_Filter_6 */
	{0x28, 0x060180, 3, 1}, /* AD7124_Filter_7 */
	{0x29, 0x800000, 3, 1}, /* AD7124_Offset_0 */
	{0x2A, 0x800000, 3, 1}, /* AD7124_Offset_1 */
	{0x2B, 0x800000, 3, 1}, /* AD7124_Offset_2 */
	{0x2C, 0x800000, 3, 1}, /* AD7124_Offset_3 */
	{0x2D, 0x800000, 3, 1}, /* AD7124_Offset_4 */
	{0x2E, 0x800000, 3, 1}, /* AD7124_Offset_5 */
	{0x2F, 0x800000, 3, 1}, /* AD7124_Offset_6 */
	{0x30, 0x800000, 3, 1}, /* AD7124_Offset_7 */
	{0x31, 0x500000, 3, 1}, /* AD7124_Gain_0 */
	{0x32, 0x500000, 3, 1}, /* AD7124_Gain_1 */
	{0x33, 0x500000, 3, 1}, /* AD7124_Gain_2 */
	{0x34, 0x500000, 3, 1}, /* AD7124_Gain_3 */
	{0x35, 0x500000, 3, 1}, /* AD7124_Gain_4 */
	{0x36, 0x500000, 3, 1}, /* AD7124_Gain_5 */
	{0x37, 0x500000, 3, 1}, /* AD7124_Gain_6 */
	{0x38, 0x500000, 3, 1}, /* AD7124_Gain_7 */
};
void ad7124test(void* param)
{
//	auto data = AD7124_Read_Reg(AD7124_DATA_REG, AD7124_DATA_REG_BYTES);
//    printf("data:0x%08x\n", data);
	AD7124_Read_ID();
}
void ad71248Test()
{
	AD7124_Init();
	
	Sys.AddTask(ad7124test,0,1000,2000,"7124-8test");
}

