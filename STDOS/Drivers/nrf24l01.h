#ifndef _NRF24L01_H
    #define _NRF24L01_H

	#include "Spi.h"

    #define MAX_RT      0x10 //达到最大重发次数中断标志位
    #define TX_DS		0x20 //发送完成中断标志位	  // 
    #define RX_DR		0x40 //接收到数据中断标志位
	
	class NRF24L01 
	{
		public:
			void SPI_NRF_Init();
			byte SPI_NRF_RW(byte dat);
			byte SPI_NRF_ReadReg(byte reg);
			byte SPI_NRF_WriteReg(byte reg, byte dat);

			byte SPI_NRF_ReadBuf(byte reg, byte *pBuf, byte bytes);
			byte SPI_NRF_WriteBuf(byte reg, byte *pBuf, byte bytes);

			void NRF_TX_Mode();
			void NRF_RX_Mode();
			byte NRF_Rx_Dat(byte *rxbuf);
			byte NRF_Tx_Dat(byte *txbuf);
			byte NRF_Check();
			
		private:
			Spi*		_spi;
			OutputPort	_CE;
			InputPort	Irq;
			OutputPort	_Power;	// 设置控制2401电源的引脚  直接进行对2401的通断电操作，以免死机对setPower无效
	};

#endif
