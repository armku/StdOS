#ifndef _NRF24L01_H
    #define _NRF24L01_H

	#include "Spi.h"

    #define MAX_RT      0x10 //�ﵽ����ط������жϱ�־λ
    #define TX_DS		0x20 //��������жϱ�־λ	  // 
    #define RX_DR		0x40 //���յ������жϱ�־λ
	
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
			OutputPort	_Power;	// ���ÿ���2401��Դ������  ֱ�ӽ��ж�2401��ͨ�ϵ����������������setPower��Ч
		private:
			byte WriteBuf(byte reg, const Buffer& bs);
			byte ReadBuf(byte reg, Buffer& bs);
			byte ReadReg(byte reg);
			byte WriteReg(byte reg, byte dat);
		
			byte ReadBuf(byte reg, byte *pBuf, byte bytes);
			byte WriteBuf(byte reg, byte *pBuf, byte bytes);
	};

#endif
