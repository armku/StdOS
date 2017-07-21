/**
  ******************************************************************************
  * @file    ad7124_spi.c
  * @author  Zhaolc
  * @version V1.0
  * @date    2016-07-28
  * @brief   AD7124-8 spi��������
  ******************************************************************************
  * @attention
  * Ӳ��ƽ̨: MAIN-175*130-3050ZJ V1.0 (3050����������������)
  * ��Ȩ����: �ൺ����Ƽ����޹�˾
  * ��˾��ַ: http://www.brain-optics.com/
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "ad7124_spi.h"

/*******************************************************************************
  * @function	: AD7124_SPI_Config
  * @brief		: SPI�˿����ã�ģ��SPI
  * @param		: ��
  * @retval		: ��
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
	for(i = 0; i < 10; i++);
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
  * @brief		: ��дspi 2���ֽ�
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
  * @brief		: ��дspi 4���ֽ�
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
  * @brief		: SPI����λ��ʱ
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
