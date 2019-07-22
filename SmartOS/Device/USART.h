#ifndef  _USART_H
#define _USART_H

#include "Configuration.h"
#include "Port.h"

class USARTHAL
{
public:
	USARTHAL(COM index, uint baud, byte priGroup = 3, byte prePri = 7, byte subPri = 1, bool remap = false, uint remapvalue = 1);
	void Initialize();
	//////////////////////////
	///@bief 设置波特率
	///@param baudRate 波特率大小
	//////////////////////////
	void SetBaudRate(uint baudRate);
	int SendTimeMs(int buflen)
	{
		if (this->mBaudrate < 100)
			return 1;
		return buflen * 8 * 1000 / this->mBaudrate + 1;
	}
	/*Port* RS485;*/
#ifdef USE_USART_DMA
private:
	byte                   mDMAIRQn;
	DMA_Channel_TypeDef* mDMATxCh;
	uint                  mDMATCFlag;
	uint                  mDMAGLFlag;
	byte                   mDMATxBuf[USART_DMA_TX_BUFFER_SIZE];
	void InitDMA();
public:
	void DMAIRQ();
#endif
private:
	bool isBusySend;

	uint            mBaudrate; //baudrate of usart
	uint			mRemapvalue;// remap value	
	Port				mPortTx;	//tx
	Port				mPortRx;	//rx
	byte             mIRQn;     //USART IRQn
	COM index;				//com index
	uint            mUSARTRcc; //USART Clock
	bool           mRemap;    //gpio remap flag
	byte             mPrePri;   //preemption priority
	byte             mSubPri;   //sub priority
	byte             mPriGroup; //priority group
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
