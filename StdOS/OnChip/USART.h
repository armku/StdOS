#ifndef  _USART_H
#define _USART_H

#include <stdint.h>
#include "Configuration.h"
#include "Port.h"

class USARTHAL
{
public:
	USARTHAL(COM index, uint32_t baud, uint8_t priGroup = 3, uint8_t prePri = 7, uint8_t subPri = 1, bool remap = false, uint32_t remapvalue = 1);
	void Initialize();
	//////////////////////////
	///@bief 设置波特率
	///@param baudRate 波特率大小
	//////////////////////////
	void SetBaudRate(uint32_t baudRate);
	int SendTimeMs(int buflen)
	{
		if (this->mBaudrate < 100)
			return 1;
		return buflen * 8 * 1000 / this->mBaudrate + 1;
	}
	/*Port* RS485;*/
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
private:
	bool isBusySend;

	uint32_t            mBaudrate; //baudrate of usart
	uint32_t			mRemapvalue;// remap value	
	Port				mPortTx;	//tx
	Port				mPortRx;	//rx
	uint8_t             mIRQn;     //USART IRQn
	COM index;				//com index
	uint32_t            mUSARTRcc; //USART Clock
	bool           mRemap;    //gpio remap flag
	uint8_t             mPrePri;   //preemption priority
	uint8_t             mSubPri;   //sub priority
	uint8_t             mPriGroup; //priority group
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
