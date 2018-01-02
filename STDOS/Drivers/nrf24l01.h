#ifndef _NRF24L01_H
    #define _NRF24L01_H

	#include "Spi.h"

    #define MAX_RT      0x10 //达到最大重发次数中断标志位
    #define TX_DS		0x20 //发送完成中断标志位	  // 
    #define RX_DR		0x40 //接收到数据中断标志位
	
	class NRF24L01 
	{
		public:
			NRF24L01();
			void Init();
			void Init(Spi* spi, Pin ce = P0, Pin irq = P0, Pin power = P0);

			void TX_Mode();
			void RX_Mode();
			byte Rx_Dat(byte *rxbuf);
			byte Tx_Dat(byte *txbuf);
			byte Check();
			
		//private:
			Spi*		_spi;
			OutputPort	_CE;
			OutputPort _CSN;
			InputPort	Irq;
			OutputPort	_Power;	// 设置控制2401电源的引脚  直接进行对2401的通断电操作，以免死机对setPower无效
		private:
			byte WriteBuf(byte reg, const Buffer& bs);
			byte ReadBuf(byte reg, Buffer& bs);
			byte ReadReg(byte reg);
			byte WriteReg(byte reg, byte dat);
		
			byte ReadBuf(byte reg, byte *pBuf, byte bytes);
			byte WriteBuf(byte reg, byte *pBuf, byte bytes);
	};

#endif
