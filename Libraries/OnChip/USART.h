#ifndef  _USART_H
#define _USART_H

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "Configuration.h"
#include "Port.h"

/**
*@addtogroup USART_CONFIGURATION
*@{
*/
/******************************************************************************************************/
/****configuration，使用前请自行配置****/
#define USART_TX_BUFFER_SIZE     200              //USART BUFFER FIFO SIZE
#define USART_RX_BUFFER_SIZE     200              //USART BUFFER FIFO SIZE
#define USART_DMA_TX_BUFFER_SIZE 200             //USART DMA BUFFER SIZE

/*******************************************************************************************************/

class USART
{
private:
	bool isBusySend;

	uint32_t            mBaudrate; //baudrate of usart
	uint32_t			mRemapvalue;// remap value	
	uint16_t            mParity;   //parity of usart
	uint16_t            mWordLen;  //world length of usart
	uint16_t            mStopBits; //stop bits of usart
	uint16_t            mTxPin;    //Tx gpio pin
	uint16_t            mRxPin;    //Rx gpio pin
	uint8_t             mIRQn;     //USART IRQn
	GPIO_TypeDef*  mPort;     //GPIO port
	USART_TypeDef* mUSARTx;   //USARTx
	uint32_t            mGPIORcc;  //GPIO Clock
	uint32_t            mUSARTRcc; //USART Clock
	bool           mRemap;    //gpio remap flag
	uint8_t             mPrePri;   //preemption priority
	uint8_t             mSubPri;   //sub priority
	uint8_t             mPriGroup; //priority group
public:
	FIFOBuffer<uint8_t, USART_TX_BUFFER_SIZE>  mTxBuf;  //USART Tx Buffer
	FIFOBuffer<uint8_t, USART_RX_BUFFER_SIZE>  mRxBuf;  //USART Rx Buffer
private:
	uint8_t mPrecision;   //when show precision after dot "."  when use "<<" to show float value

	uint16_t mTxOverflow; //Tx overflow byte count
	uint16_t mRxOverflow; //Rx overflow byte count

private:
	void InitGPIO();
	void InitUSART();
	void InitNVIC();
public:
	USART(USART_TypeDef* USARTx, uint32_t baud, uint8_t priGroup = 3, uint8_t prePri = 7, uint8_t subPri = 1, bool remap = false, uint32_t remapvalue = 1, uint16_t parity = USART_Parity_No, uint16_t wordLen = USART_WordLength_8b, uint16_t stopBits = USART_StopBits_1);
	void Initialize();
	uint64_t LastRcvTime;
	int RxCnt;
	int TxCnt;
	//////////////////////////
	///@bief 设置波特率
	///@param baudRate 波特率大小
	//////////////////////////
	void SetBaudRate(uint32_t baudRate);

	virtual bool SendBytes(uint8_t txData[], uint16_t size);
	bool SendBytes(char txData[], uint16_t size) { return this->SendBytes((uint8_t*)txData, size); }
	virtual bool SendByte(uint8_t data);
	virtual bool GetBytes(uint8_t data[], uint16_t num);
	virtual bool GetByte(uint8_t &data);

	virtual uint16_t TxSize();
	virtual uint16_t RxSize();

	virtual uint16_t TxOverflowSize();
	virtual uint16_t RxOverflowSize();

	virtual void ClearRxBuf();
	virtual void ClearTxBuf();

	void IRQ();

	USART& operator<<(int val);
	USART& operator<<(double val);
	USART& operator<<(const char* pStr);
	int SendTimeMs(int buflen)
	{
		if (this->mBaudrate < 100)
			return 1;
		return buflen * 8 * 1000 / this->mBaudrate + 1;
	}
	OutputPort* RS485;
#ifdef USE_USART_DMA
private:
	uint8_t                   mDMAIRQn;
	DMA_Channel_TypeDef* mDMATxCh;
	uint32_t                  mDMATCFlag;
	uint32_t                  mDMAGLFlag;
	uint8_t                   mDMATxBuf[USART_DMA_TX_BUFFER_SIZE];
	void InitDMA();
public:
	void DMAIRQ();
#endif
};

/**
===========================================================================
|			remap value				|
usartName	|	0x00	|	0x01	|	0x11	|
usart1Tx	|	PA9		|	PB6		|			|
usart1Rx	|	PA10	|	PB7		|			|
usart2Tx	|	PA2		|	PD5		|			|
usart2Rx	|	PA3		|	PD6		|			|
usart3Tx	|	PB10	|	PC10	|	PD8		|
usart3Rx	|	PB11	|	PC11	|	PD9		|
===========================================================================
*/

//485通信 1：发送模式 0：接收模式
#endif // ! _USART_H
