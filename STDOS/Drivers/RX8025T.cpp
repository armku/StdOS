#include "RX8025T.h"

/* RX8025T �Ĵ�����ַ */
#define SEC_ADDR			0x00
#define MIN_ADDR			0x01
#define HOUR_ADDR			0x02
#define WEEK_ADDR			0x03
#define DAY_ADDR			0x04
#define MONTH_ADDR		0x05
#define YEAR_ADDR			0x06
#define RAM_ADDR			0x07
#define MIN_ALM_ADDR	0x08
#define HOUR_ALM_ADDR	0x09
#define WEEK_ALM_ADDR	0x0A
#define DAY_ALM_ADDR	0x0A
#define TMR_CNT0_ADDR	0x0B
#define TMR_CNT1_ADDR	0x0C
#define EXT_REG_ADDR	0x0D
#define FLG_REG_ADDR	0x0E
#define CTL_REG_ADDR	0x0F


RX8025T::RX8025T(){}
RX8025T::RX8025T(Pin pinsck, Pin pinsda, byte devaddr, uint wnms)
{
    this->IIC.SetPin(pinsck, pinsda);
}

byte BCD2HEX(byte val)
{
    byte i;

    i = val &0x0f; //��λ�룬i�õ�����λ����
    val >>= 4; //������λ��������λ�Ƶ�����λ��λ�ã��õ�����λ��ֵ��
    val &= 0x0f; //��ֹ��λʱ��λ����1��ֻ��������λ��ֵ
    val *= 10; //��λ��ֵ����10
    i += val; //Ȼ�������λ��ֵ��ӡ�
    return i; //���õ���ʮ����������
}

/**
 * @brief RX8025_DEC2BCD  ʮ����תBCD��
 * @param val      BCD��
 * @return
 */
byte DEC2BCD(byte val)
{
    byte i;

    i = val / 10; //ȡ��ʮλ
    val %= 10; //ȡ�ø�λ
    i <<= 4; //����4λ
    i += val;

    return i;
}

void RX8025T::Init()
{
    this->devaddr = 0X64;
    //    this->IIC.Start();
    //    this->IIC.WriteByte(this->devaddr);
    //    this->IIC.WriteByte(0xe0);
    //    this->IIC.WriteByte(0x20);
    //    this->IIC.Stop();

    buf[0] = 0x00;
    RX8025_Write(0x07, buf, 1); //��ʹ�þ��ȵ���

    buf[0] = 0x24; //0010 0100,bit5:1 24Сʱ�ƣ�bit2:0 INTA���1HZ��ƽ���������壬�½������ʱͬ��
    RX8025_Write(0x0E, buf, 1); //����24Сʱ��

    RX8025_Read(0x0F, buf, 1); //�ȶ��Ĵ���
    buf[0] &= 0xFB; //CTFG����Ϊ0
    RX8025_Write(0x0F, buf, 1); //ֻ��������ƽģʽ����ʹ��

}


//��RX8025�Ĵ���
void RX8025T::RX8025_Read(byte sadd, byte *buf, byte len)
{
    byte i;

    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr);
    //this->IIC.WaitAck();

    this->IIC.WriteByte(sadd << 4 | 0x01);
    //this->IIC.WaitAck();

    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr | 1);
    //this->IIC.WaitAck();

    for (i = 0; i < len - 1; i++)
    {
        buf[i] = this->IIC.Read(0xff);
        this->IIC.Ack(true);
    }
    buf[i] = this->IIC.Read(0xff);

    this->IIC.Ack(false);
    this->IIC.Stop();
}

//дRX8025�Ĵ���
void RX8025T::RX8025_Write(byte sadd, byte *buf, byte len)
{
    byte i;

    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr);
    this->IIC.WriteByte(sadd << 4);

    for (i = 0; i < len; i++)
    {
        this->IIC.WriteByte(buf[i]);
    }
    this->IIC.Stop();

}

void RX8025T::LoadTime(DateTime &dt)
{
    //	this->RX8025_Read(0,this->buf,10);
    //	dt.Second=this->buf[0];
    //	dt.Minute=this->buf[1];
    //	dt.Hour=this->buf[2];
    //	
    //	dt.Day=this->buf[3];
    //	dt.Month=this->buf[5];
    //	dt.Year=this->buf[6];

    RX8025_Read(0, buf, 16); //��ȡ��ǰʱ��

    dt.Year = BCD2HEX(buf[6]) + 2000; //��
    dt.Month = BCD2HEX(buf[5]); //��
    dt.Day = BCD2HEX(buf[4]); //��
    dt.Hour = BCD2HEX(buf[2]); //ʱ
    dt.Minute = BCD2HEX(buf[1]); //��
    dt.Second = BCD2HEX(buf[0]); //��
}

void RX8025T::SaveTime(DateTime &dt)
{
    buf[SEC_ADDR] = DEC2BCD(dt.Second);
    buf[MIN_ADDR] = DEC2BCD(dt.Minute);
    buf[HOUR_ADDR] = DEC2BCD(dt.Hour);
    //	buf[WEEK_ADDR]=DEC2BCD(dt.DayOfWeek);
    buf[DAY_ADDR] = DEC2BCD(dt.Day);
    buf[MONTH_ADDR] = DEC2BCD(dt.Month);
    buf[YEAR_ADDR] = DEC2BCD(dt.Year - 2000);
    this->RX8025_Write(SEC_ADDR, buf, 7);
}

#if 0
    RX8025T rx8025(PB6, PB7);
    DateTime now;
    void Rx8025Refresh(void *param)
    {
        RX8025T *rx = (RX8025T*)param;
        rx->LoadTime(now);
        now.Show();
        debug_printf("\r\n");
    }
    void RX8025Test()
    {
        rx8025.Init();
        now.Year = 2017;
        now.Month = 7;
        now.Day = 20;
        //		now.DayOfWeek=4;
        now.Hour = 11;
        now.Minute = 11;
        now.Second = 2;
        rx8025.SaveTime(now);
        Sys.AddTask(Rx8025Refresh, &rx8025, 100, 1000, "TimeUp");
    }

#endif

/////����Ϊʾ�����룬����ͨ��
#ifdef __RX8025T_H_lafjlsfslflsfkjsf
#ifndef __RX8025T_H
#define __RX8025T_H

#include "stdint.h"
#include "data_handler.h"

extern TIME_T rx8025Time;
extern uint8_t rx8025tIntFlag;
//�ⲿ�ӿ�
void RX8025T_Init(void);
uint8_t RX8025T_CheckDevice(void);
uint8_t RX8025T_ReadTime(void);
uint8_t RX8025T_WriteTime(void);
uint8_t RX8025T_ReadTest(void);
uint8_t RX8025T_1sOutput_Start(void);
uint8_t RX8025T_1sOutput_Stop(void);
void RX8025T_IntTask(void);

#endif
#include "stm32f10x.h"
#include "rx8025t.h"
#include "data_handler.h"
#include "stdio.h"
#include "delay.h"

/* SCL SDA IO����Ӳ���궨�� */
#define SDA_PORT					GPIOC
#define SDA_PIN						GPIO_Pin_2
#define RCC_SDA_PORT			RCC_APB2Periph_GPIOC
#define RCC_SDA_PORT_CMD	RCC_APB2PeriphClockCmd

#define SCL_PORT					GPIOC
#define SCL_PIN						GPIO_Pin_1
#define RCC_SCL_PORT			RCC_APB2Periph_GPIOC
#define RCC_SCL_PORT_CMD	RCC_APB2PeriphClockCmd

/* ��дSCL��SDA �궨�� */
#define SCL_1()  GPIO_SetBits(SCL_PORT, SCL_PIN)
#define SCL_0()  GPIO_ResetBits(SCL_PORT, SCL_PIN)
#define SDA_1()  GPIO_SetBits(SDA_PORT, SDA_PIN)
#define SDA_0()  GPIO_ResetBits(SDA_PORT, SDA_PIN)

#define SDA_READ()  GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN)	/* ��SDA����״̬ */

static void I2C_Config(void);
static void I2C_Delay(void);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_SendByte(uint8_t data);
static uint8_t I2C_ReadByte(void);
static uint8_t I2C_WaitAck(void);
static void I2C_Ack(void);
static void I2C_NAck(void);
uint8_t I2C_CheckDevice(void);

/**********************************************************************************
	* @function	: I2C_Config
  * @brief  	: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽ
  * @param  	: ��
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
static void I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//SCL
	RCC_SCL_PORT_CMD(RCC_SCL_PORT, ENABLE);  /* ��GPIOʱ�� */
	GPIO_InitStructure.GPIO_Pin = SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  /* ��©��� */
	GPIO_Init(SCL_PORT, &GPIO_InitStructure); 
	//SDA
	RCC_SCL_PORT_CMD(RCC_SDA_PORT, ENABLE);  /* ��GPIOʱ�� */
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  /* ��©��� */
	GPIO_Init(SDA_PORT, &GPIO_InitStructure); 
	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	I2C_Stop();
}

/**********************************************************************************
	* @function	: I2C_Delay
  * @brief  	: I2C����λ�ӳ٣����400KHz
  * @param  	: ��
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
static void I2C_Delay(void)
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
	for(i = 0; i < 60; i++);
}

/**********************************************************************************
	* @function	: I2C_Start
  * @brief  	: CPU����I2C���������ź�
  * @param  	: ��
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
static void I2C_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	SDA_1();
	SCL_1();
	I2C_Delay();I2C_Delay(); 
	SDA_0();
	I2C_Delay();
	SCL_0();
	I2C_Delay();
}

/**********************************************************************************
	* @function	: I2C_Stop
  * @brief  	: CPU����I2C����ֹͣ�ź�
  * @param  	: ��
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
static void I2C_Stop(void)
{
   /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
  SDA_0();
	SCL_1();
	I2C_Delay(); I2C_Delay();
	SDA_1();
}

/**********************************************************************************
	* @function	: I2C_SendByte
  * @brief  	: CPU��I2C�����豸����8bit����
  * @param  	: data �� �ȴ����͵��ֽ�
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
static void I2C_SendByte(uint8_t data)
{
	uint8_t i;
	/* �ȷ����ֽڵĸ�λbit7 */
	for(i = 0; i < 8; i++)
	{
		if(data &0x80){
				SDA_1();
		}
		else{
				SDA_0();
		}
		I2C_Delay();
		SCL_1();
		I2C_Delay(); I2C_Delay();
		SCL_0();
		if (i == 7){
				SDA_1();  // �ͷ�����
		}
		data <<= 1;  /* ����һ��bit */
		I2C_Delay();
	}
}

/**********************************************************************************
	* @function	: I2C_ReadByte
  * @brief  	: CPU��I2C�����豸��ȡ8bit����
  * @param  	: ��
  * @retval 	: ����������
	* @notes		: 
  *********************************************************************************/
static uint8_t I2C_ReadByte(void)
{
	uint8_t i; 
	uint8_t data;
	/* ������1��bitΪ���ݵ�bit7 */
	data = 0; 
	for(i = 0; i < 8; i++)
	{
		data <<= 1;
		SCL_1();
		I2C_Delay(); I2C_Delay(); 
		if( SDA_READ()){
				data++; 
		}
		SCL_0();
		I2C_Delay();
	}
	return data; 
}

/**********************************************************************************
	* @function	: I2C_WaitAck
  * @brief  	: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * @param  	: ��
  * @retval 	: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
	* @notes		: 
  *********************************************************************************/
static uint8_t I2C_WaitAck(void)
{
	uint8_t re; 
	SDA_1();  /* CPU�ͷ�SDA���� */
	I2C_Delay(); 
	SCL_1();  /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	I2C_Delay(); I2C_Delay(); 
	if (SDA_READ()){	/* CPU��ȡSDA����״̬ */
			re = 1; 
	}
	else{
			re = 0; 
	}
	SCL_0(); 
	I2C_Delay(); 
	return re; 
}

/**********************************************************************************
	* @function	: I2C_Ack
  * @brief  	: CPU����1��ACK�ź�
  * @param  	: ��
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
static void I2C_Ack(void)
{
	SDA_0();  /* CPU����SDA = 0 */
	I2C_Delay(); 
	SCL_1();  /* CPU����1��ʱ�� */
	I2C_Delay(); I2C_Delay(); 
	SCL_0(); 
	I2C_Delay(); 
	SDA_1();  /* CPU�ͷ�SDA���� */
}

/**********************************************************************************
	* @function	: I2C_NAck
  * @brief  	: CPU����1��NACK�ź�
  * @param  	: ��
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
static void I2C_NAck(void)
{
	SDA_1();  /* CPU����SDA = 1 */
	I2C_Delay(); 
	SCL_1();  /* CPU����1��ʱ�� */
	I2C_Delay(); I2C_Delay(); 
	SCL_0(); 
	I2C_Delay();
}

/*********************************************************************************/
/* RX8025T I2C��д��ַ */
#define RX8025T_RD_ADDR		0x65
#define RX8025T_WR_ADDR		0x64

/* RX8025T �Ĵ�����ַ */
#define SEC_ADDR			0x00
#define MIN_ADDR			0x01
#define HOUR_ADDR			0x02
#define WEEK_ADDR			0x03
#define DAY_ADDR			0x04
#define MONTH_ADDR		0x05
#define YEAR_ADDR			0x06
#define RAM_ADDR			0x07
#define MIN_ALM_ADDR	0x08
#define HOUR_ALM_ADDR	0x09
#define WEEK_ALM_ADDR	0x0A
#define DAY_ALM_ADDR	0x0A
#define TMR_CNT0_ADDR	0x0B
#define TMR_CNT1_ADDR	0x0C
#define EXT_REG_ADDR	0x0D
#define FLG_REG_ADDR	0x0E
#define CTL_REG_ADDR	0x0F

TIME_T rx8025Time;

#define EXINT_PORT					GPIOC
#define EXINT_PIN						GPIO_Pin_3
#define RCC_EXINT_PORT			RCC_APB2Periph_GPIOC
#define RCC_EXINT_PORT_CMD	RCC_APB2PeriphClockCmd
#define RCC_EXINT_AFIO			RCC_APB2Periph_AFIO
#define RCC_EXINT_AFIO_CMD	RCC_APB2PeriphClockCmd

/**********************************************************************************
	* @function	: ExInt_Config
  * @brief  	: �ⲿ�ж�IO��
  * @param  	: ��
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
static void ExInt_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//GPIO
	RCC_EXINT_PORT_CMD(RCC_EXINT_PORT, ENABLE);  /* ��GPIOʱ�� */
	RCC_EXINT_AFIO_CMD(RCC_EXINT_AFIO, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = EXINT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(EXINT_PORT, &GPIO_InitStructure);
	//EXTI Line
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
	//EXTI
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

#include "wnd_hgsample_run.h"
uint8_t rx8025tIntFlag = 0;
void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
		//rx8025tIntFlag = 1;
		hgSampleTaskRunStep++;
	}
}

/**********************************************************************************
	* @function	: RX8025T_Init
  * @brief  	: RX8025T IO�ڳ�ʼ��
  * @param  	: ��
  * @retval 	: ��
	* @notes		: 
  *********************************************************************************/
void RX8025T_Init(void)
{
	I2C_Config();
	ExInt_Config();
	RX8025T_1sOutput_Start();
}

/**********************************************************************************
	* @function	: RX8025T_CheckDevice
  * @brief  	: RX8025T ͨ�ż��
  * @param  	: ��
  * @retval 	: 1 - RX8025T ��Ӧ��
							��0 - RX8025T ��Ӧ��
	* @notes		: 
  *********************************************************************************/
uint8_t RX8025T_CheckDevice(void)
{
	uint8_t re;
	
	I2C_Start();  /* ���������ź� */
	I2C_SendByte(RX8025T_RD_ADDR);
	re = I2C_WaitAck();
	I2C_Stop();  /* ����ֹͣ�ź� */
	return re;
}

/**********************************************************************************
	* @function	: RX8025T_ReadTime
  * @brief  	: ��ȡRX8025Tʱ��
  * @param  	: ��
  * @retval 	: 1 - ��ȡʧ��
							��0 - ��ȡ�ɹ�
	* @notes		: ��ȡ����� rx8025Time �ṹ����
  *********************************************************************************/
uint8_t RX8025T_ReadTime(void)
{
	I2C_Start();
	I2C_SendByte(RX8025T_WR_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(SEC_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_Start();
	I2C_SendByte(RX8025T_RD_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	rx8025Time.sec = BCD_to_Dec(I2C_ReadByte());
	I2C_Ack();
	rx8025Time.min = BCD_to_Dec(I2C_ReadByte());
	I2C_Ack();
	rx8025Time.hour = BCD_to_Dec(I2C_ReadByte());
	I2C_Ack();
	rx8025Time.week = BCD_to_Dec(I2C_ReadByte());
	I2C_Ack();
	rx8025Time.day = BCD_to_Dec(I2C_ReadByte());
	I2C_Ack();
	rx8025Time.month = BCD_to_Dec(I2C_ReadByte());
	I2C_Ack();
	rx8025Time.year = 2000 + BCD_to_Dec(I2C_ReadByte());
	I2C_NAck();
	I2C_Stop();
	return 0;
}

/**********************************************************************************
	* @function	: RX8025T_WriteTime
  * @brief  	: ����RX8025Tʱ��
  * @param  	: ��
  * @retval 	: 1 - ����ʧ��
							��0 - ���óɹ�
	* @notes		: �� rx8025Time �ṹ���е�ʱ��д��RX8025T��
  *********************************************************************************/
uint8_t RX8025T_WriteTime(void)
{
	I2C_Start();
	I2C_SendByte(RX8025T_WR_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(SEC_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(Dec_to_BCD(rx8025Time.sec));
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(Dec_to_BCD(rx8025Time.min));
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(Dec_to_BCD(rx8025Time.hour));
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(rx8025Time.week);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(Dec_to_BCD(rx8025Time.day));
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(Dec_to_BCD(rx8025Time.month));
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(Dec_to_BCD(rx8025Time.year - 2000));
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_Stop();
	return 0;
}

/**********************************************************************************
	* @function	: RX8025T_ReadTime
  * @brief  	: ��ȡRX8025Tʱ��
  * @param  	: ��
  * @retval 	: 1 - ��ȡʧ��
							��0 - ��ȡ�ɹ�
	* @notes		: ��ȡ����� rx8025Time �ṹ����
  *********************************************************************************/
uint8_t RX8025T_ReadTest(void)
{
	I2C_Start();
	I2C_SendByte(RX8025T_WR_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(DAY_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_Start();
	I2C_SendByte(RX8025T_RD_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	rx8025Time.day = BCD_to_Dec(I2C_ReadByte());
	I2C_Ack();
	rx8025Time.month = BCD_to_Dec(I2C_ReadByte());
	I2C_Ack();
	rx8025Time.year = 2000 + BCD_to_Dec(I2C_ReadByte());
	I2C_NAck();
	I2C_Stop();
	return 0;
}

/**********************************************************************************
	* @function	: RX8025T_WriteTime
  * @brief  	: RX8025T�����������ʼ
  * @param  	: ��
  * @retval 	: 1 - ����ʧ��
							��0 - ���óɹ�
	* @notes		: 
  *********************************************************************************/
uint8_t RX8025T_1sOutput_Start(void)
{
	I2C_Start();
	I2C_SendByte(RX8025T_WR_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(EXT_REG_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(0x00);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(0x00);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(0x60);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_Stop();
	return 0;
}

/**********************************************************************************
	* @function	: RX8025T_1sOutput_Stop
  * @brief  	: RX8025T�������������
  * @param  	: ��
  * @retval 	: 1 - ����ʧ��
							��0 - ���óɹ�
	* @notes		: 
  *********************************************************************************/
uint8_t RX8025T_1sOutput_Stop(void)
{
	I2C_Start();
	I2C_SendByte(RX8025T_WR_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(EXT_REG_ADDR);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(0x00);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(0x00);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_SendByte(0x40);
	if(I2C_WaitAck() == 1){
		return 1;
	}
	I2C_Stop();
	return 0;
}

uint8_t testCnt = 0;
/**********************************************************************************
	* @function	: RX8025T_IntTask
  * @brief  	: RX8025T����������������ж�����
  * @param  	: ��
  * @retval 	: ��
	* @notes		: ������
  *********************************************************************************/
void RX8025T_IntTask(void)
{
	if(rx8025tIntFlag == 1){
		rx8025tIntFlag = 0;
		testCnt++;
		if(testCnt == 10){
			testCnt = 0;
			RX8025T_1sOutput_Stop();
			Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);
			RX8025T_1sOutput_Start();
		}
		RX8025T_ReadTime();
		printf("%04d/%02d/%02d ",rx8025Time.year,rx8025Time.month,rx8025Time.day);
		printf("%02d:%02d:%02d\n",rx8025Time.hour,rx8025Time.min,rx8025Time.sec);
	}
}
#endif
