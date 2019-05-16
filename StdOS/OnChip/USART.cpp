#include "stm32f10x.h"
#include "USART.h"
#include "BspPlatform/Interrupt.h"
#include "Configuration.h"
#include "Sys.h"

USART::USART(COM index1, uint32_t baud, uint8_t priGroup, uint8_t prePri, uint8_t subPri, bool remap, uint32_t remapvalue)
{
	this->index = index1;
	mBaudrate = baud;     //baudrate of usart
	mPriGroup = priGroup; //priority group
	mPrePri = prePri;   //preemption priority
	mSubPri = subPri;   //sub priority
	mRemap = remap;    //gpio remap flag
	mRemapvalue = remapvalue;
	mPrecision = 3;
	RxCnt = 0;
	TxCnt = 0;

	if (index == COM1)
	{
		mIRQn = USART1_IRQn;                                           //USART IRQn
		mUSARTRcc = RCC_APB2Periph_USART1;	                                //USARTx Clock
		if (mRemap)
		{
			this->mPortTx.SetPin(PB6);
			this->mPortRx.SetPin(PB7);
		}
		else
		{
			this->mPortTx.SetPin(PA9);
			this->mPortRx.SetPin(PA10);
		}
#ifdef USE_USART1_DMA
		mDMATxCh = DMA1_Channel4;       //DMA Tx Channel
		mDMAIRQn = DMA1_Channel4_IRQn;  //DMA IRQn
		mDMATCFlag = DMA1_FLAG_TC4;       //DMA TC Mask
		mDMAGLFlag = DMA1_IT_GL4;         //DMA IT GL mask
		pCOM1 = this;
#endif
	}
	else if (index == COM2)
	{
		mIRQn = USART2_IRQn;                                           //USART IRQn
		mUSARTRcc = RCC_APB1Periph_USART2;                             //USARTx Clock	
		if (mRemap)
		{
			this->mPortTx.SetPin(PD5);
			this->mPortRx.SetPin(PD6);
		}
		else
		{
			this->mPortTx.SetPin(PA2);
			this->mPortRx.SetPin(PA3);
		}
#ifdef USE_USART2_DMA
		mDMATxCh = DMA1_Channel7;       //DMA Tx Channel
		mDMAIRQn = DMA1_Channel7_IRQn;  //DMA IRQn
		mDMATCFlag = DMA1_FLAG_TC7;       //DMA TC Mask
		mDMAGLFlag = DMA1_IT_GL7;         //DMA IT GL mask
		pCOM2 = this;
#endif
	}
	else if (index == COM3)
	{
		mIRQn = USART3_IRQn;                                           //USART IRQn
		mUSARTRcc = RCC_APB1Periph_USART3;	                                //USARTx Clock
		if (mRemap)
		{
			this->mPortTx.SetPin(PC10);
			this->mPortRx.SetPin(PC11);
		}
		else
		{
			if (mRemapvalue == 0x01)
			{
				this->mPortTx.SetPin(PB10);
				this->mPortRx.SetPin(PB11);
			}
			else
			{
				this->mPortTx.SetPin(PD8);
				this->mPortRx.SetPin(PD9);
			}

		}
#ifdef USE_USART3_DMA
		mDMATxCh = DMA1_Channel2;       //DMA Tx Channel
		mDMAIRQn = DMA1_Channel2_IRQn;  //DMA IRQn
		mDMATCFlag = DMA1_FLAG_TC2;       //DMA TC Mask
		mDMAGLFlag = DMA1_IT_GL2;         //DMA IT GL mask
		pCOM3 = this;
#endif
	}

	Initialize();

}

void USART::Initialize()
{
	USART_TypeDef* mUSARTx;   //USARTx

	if (this->index == COM1)
	{
		mUSARTx = USART1;
	}
	else if (this->index == COM2)
	{
		mUSARTx = USART2;
	}
	else if (this->index == COM3)
	{
		mUSARTx = USART3;
	}
	else
	{
	}

	if (this->index == COM1)
	{
		RCC_APB2PeriphClockCmd(mUSARTRcc, ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(mUSARTRcc, ENABLE);
	}
	if (mRemap)
	{
		if (mUSARTx == USART1) GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
		if (mUSARTx == USART2) GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
		if (mUSARTx == USART3) GPIO_PinRemapConfig(mRemapvalue == 0X01 ? GPIO_PartialRemap_USART3 : GPIO_FullRemap_USART3, ENABLE);
	}
	this->mPortTx.pinMode(GPIO_AF_PP);
	this->mPortRx.pinMode(GPIO_IN_FLOATING);
	switch (mPriGroup)
	{
	case 0:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	break;
	case 1:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	break;
	case 2:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	break;
	case 3:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	break;
	case 4:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	break;
	default:NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	break;
	}
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = mIRQn;   //USART IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = mPrePri; //preemption priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = mSubPri; //sub priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //enable interrup
	NVIC_Init(&NVIC_InitStructure);                                 //initialize irq
#ifdef USE_USART_DMA
	NVIC_InitStructure.NVIC_IRQChannel = mDMAIRQn;//DMA IRQ
	NVIC_Init(&NVIC_InitStructure);
	//	mDMATxCh->CCR |= DMA_IT_TC;  //Enable DMA TX Channel TCIT 
	//	mDMATxCh->CCR |= DMA_IT_TE;  //Enable DMA TX Channel TEIT
#endif

	USART_InitTypeDef USART_InitStructure;                       //
	USART_InitStructure.USART_BaudRate = mBaudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(mUSARTx, &USART_InitStructure);

	USART_Cmd(mUSARTx, ENABLE);

	USART_ITConfig(mUSARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(mUSARTx, USART_IT_IDLE, ENABLE); //开启串口总线空闲中断
	USART_ITConfig(mUSARTx, USART_IT_TC, ENABLE);

#ifndef USE_USART_DMA	
	USART_ITConfig(mUSARTx, USART_IT_TC, DISABLE);
#endif	
	USART_ClearFlag(mUSARTx, USART_FLAG_TC);      //clear TC flag to make sure the first byte can be send correctly

#ifdef USE_USART_DMA
	InitDMA();
#endif
}

bool USART::SendBytes(uint8_t txData[], uint16_t size)
{
	if (RS485)
		*RS485 = 1;//发送模式

	USART_TypeDef* mUSARTx;   //USARTx

	if (this->index == COM1)
	{
		mUSARTx = USART1;
	}
	else if (this->index == COM2)
	{
		mUSARTx = USART2;
	}
	else if (this->index == COM3)
	{
		mUSARTx = USART3;
	}
	else
	{
	}

	if (mTxBuf.ResSize() < size)      //compare the unused bytes and sending bytes
	{
		return false;
	}
	mTxBuf.Puts(txData, size);                        //add data to Tx buffer, if overflow, return false
	if (isBusySend)                return true;       //USARTx is busy send data, return
	if (mTxBuf.Size() <= 0)        return true;       //have no data to send, return
	isBusySend = true;                               //set busy state, going to send data
#ifdef USE_USART_DMA
	if (mTxBuf.Size() <= USART_DMA_TX_BUFFER_SIZE)  //rest data bytes less than DMA buffer size
	{
		mDMATxCh->CNDTR = (uint32_t)mTxBuf.Size();        //send all data to DMA buffer
		mTxBuf.Gets(mDMATxBuf, mTxBuf.Size());
	}
	else                                           //rest data bytes more than DMA buffer size
	{
		mDMATxCh->CNDTR = USART_DMA_TX_BUFFER_SIZE;  //send max DMA buffer size data to DMA buffer
		mTxBuf.Gets(mDMATxBuf, USART_DMA_TX_BUFFER_SIZE);
	}
	DMA_Cmd(mDMATxCh, ENABLE); 	                   //enable DMA to send data
#else	
	USART_ClearITPendingBit(mUSARTx, USART_IT_TC); //Clear TC, otherwise the first byte may not able to send out
	USART_ClearITPendingBit(mUSARTx, USART_IT_TXE); //Clear TC, otherwise the first byte may not able to send out
	USART_ITConfig(mUSARTx, USART_IT_TC, ENABLE);  //Enable TC, going to send data
												   //USART_ITConfig(mUSARTx, USART_IT_TXE, ENABLE);  //Enable TC, going to send data
	USART_GetFlagStatus(mUSARTx, USART_FLAG_TC);   //read SR to clear flag, otherwise the first byte may not able to send out
												   //USART_GetFlagStatus(mUSARTx, USART_FLAG_TXE);   //read SR to clear flag, otherwise the first byte may not able to send out
	static uint8_t data = 0;
	mTxBuf.Get(data);                              //get one byte data from tx buffer
	USART_SendData(mUSARTx, data);                  //send one byte data
#endif	
	TxCnt += size;
	return true;
}


void USART::SetBaudRate(uint32_t baudRate)
{
	USART_TypeDef* mUSARTx;   //USARTx

	if (this->index == COM1)
	{
		mUSARTx = USART1;
	}
	else if (this->index == COM2)
	{
		mUSARTx = USART2;
	}
	else if (this->index == COM3)
	{
		mUSARTx = USART3;
	}
	else
	{
	}

	//	assert_param(IS_USART_BAUDRATE(baudRate));
	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;
	uint32_t usartxbase = 0;
	RCC_ClocksTypeDef RCC_ClocksStatus;

	usartxbase = (uint32_t)mUSARTx;

	/*---------------------------- USART BRR Configuration -----------------------*/
	/* Configure the USART Baud Rate -------------------------------------------*/
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	if (usartxbase == USART1_BASE)
	{
		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	}
	else
	{
		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
	}

	/* Determine the integer part */
	if ((mUSARTx->CR1 & ((uint16_t)0x8000)) != 0)//CR1_OVER8_Set=((uint16_t)0x8000)
	{
		/* Integer part computing in case Oversampling mode is 8 Samples */
		integerdivider = ((25 * apbclock) / (2 * (baudRate)));
	}
	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
	{
		/* Integer part computing in case Oversampling mode is 16 Samples */
		integerdivider = ((25 * apbclock) / (4 * (baudRate)));
	}
	tmpreg = (integerdivider / 100) << 4;

	/* Determine the fractional part */
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

	/* Implement the fractional part in the register */
	if ((mUSARTx->CR1 & ((uint16_t)0x8000)) != 0)
	{
		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
	}
	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
	{
		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
	}

	/* Write to USART BRR */
	mUSARTx->BRR = (uint16_t)tmpreg;
}

uint16_t USART::RxSize()
{
	return mRxBuf.Size();
}

USART& USART::operator<<(int val)
{
	uint8_t sign = 0, len = 0, data[10];
	if (val < 0)
	{
		sign = 1;
		val = -val;
	}
	do
	{
		len++;
		data[10 - len] = val % 10 + '0';
		val = val / 10;
	} while (val);
	if (sign == 1)
		data[10 - (++len)] = '-';
	SendBytes(data + 10 - len, len);
	return *this;
}
USART& USART::operator<<(double val)
{
	uint8_t sign = 0, len = 0, data[20];
	if (val < 0)
	{
		sign = 1;
		val = -val;
	}
	uint8_t prec = mPrecision;
	while (prec--)
		val *= 10;
	uint32_t t = val;
	do
	{
		if (++len == mPrecision + 1) data[20 - len] = '.';
		else
		{
			data[20 - len] = t % 10 + '0';
			t = t / 10;
		}
	} while (t || len < mPrecision + 2);
	//if(len==3) data[20-(++len)] = '.';
	//if(len==4) data[20-(++len)] = '0';
	if (sign == 1)
		data[20 - (++len)] = '-';
	SendBytes(data + 20 - len, len);
	return *this;
}
USART& USART::operator<<(const char* pStr)
{
	unsigned int length = 0;
	for (int i = 0; pStr[i] != '\0'; ++i)
	{
		++length;
	}
	SendBytes((uint8_t*)pStr, length);
	return *this;
}

#ifdef USE_USART_DMA
void USART::InitDMA()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                   //enable DMA clock

	DMA_DeInit(mDMATxCh);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&mUSARTx->DR;	       //DMA Tx data target address
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)mDMATxBuf;           //DMA data source address
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	   //DMA data direction: from memory to peripheral
	DMA_InitStructure.DMA_BufferSize = 0;                        //data size to send with DMA
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//peripheral address not increase
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	   //memory address increase
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//peripheral data unit: Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 //memory data unit: Byte
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;         //DMA mode: normal (not loop)
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;         //DMA priority: low
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;          //disable memory to memory

	DMA_Init(mDMATxCh, &DMA_InitStructure);                              //config DMA	   

	DMA_ITConfig(mDMATxCh, DMA_IT_TC, ENABLE);
	DMA_ITConfig(mDMATxCh, DMA_IT_TE, ENABLE);

	USART_DMACmd(mUSARTx, USART_DMAReq_Tx, ENABLE);                      //enable DMA Tx of USART
	DMA_Cmd(mDMATxCh, DISABLE);		                                       //disable DMA			
}


void USART::DMAIRQ()
{
	if (DMA_GetFlagStatus(mDMATCFlag) == SET) //DMA Tx Complete
	{
		DMA_ClearITPendingBit(mDMAGLFlag);   //Clear DMA global interrupt flag
		DMA_ClearFlag(mDMATCFlag);           //Clear DMA Tx complete flag
		DMA_Cmd(mDMATxCh, DISABLE);           //close dma

		if (mTxBuf.Size() > 0)
		{
			if (mTxBuf.Size() <= USART_DMA_TX_BUFFER_SIZE)  //the rest data bytes less than DMA buffer size
			{
				mDMATxCh->CNDTR = mTxBuf.Size();             //send all rest data to DMA buffer 
				mTxBuf.Gets(mDMATxBuf, mTxBuf.Size());
			}
			else                                           //the rest data bytes more than DMA buffer size
			{
				mDMATxCh->CNDTR = USART_DMA_TX_BUFFER_SIZE;  //send max DMA buffer size data to DMA buffer
				mTxBuf.Gets(mDMATxBuf, USART_DMA_TX_BUFFER_SIZE);
			}
			DMA_Cmd(mDMATxCh, ENABLE); 	                   //enable DMA
		}
		else
			isBusySend = false;               //将忙标志
	}
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////hal
USARTHAL::USARTHAL(COM index1, uint32_t baud, uint8_t priGroup, uint8_t prePri, uint8_t subPri, bool remap, uint32_t remapvalue)
{
	this->index = index1;
	mBaudrate = baud;     //baudrate of usart
	mPriGroup = priGroup; //priority group
	mPrePri = prePri;   //preemption priority
	mSubPri = subPri;   //sub priority
	mRemap = remap;    //gpio remap flag
	mRemapvalue = remapvalue;
	mPrecision = 3;

	if (index == COM1)
	{
		mIRQn = USART1_IRQn;                                           //USART IRQn
		mUSARTRcc = RCC_APB2Periph_USART1;	                                //USARTx Clock
		if (mRemap)
		{
			this->mPortTx.SetPin(PB6);
			this->mPortRx.SetPin(PB7);
		}
		else
		{
			this->mPortTx.SetPin(PA9);
			this->mPortRx.SetPin(PA10);
		}
#ifdef USE_USART1_DMA
		mDMATxCh = DMA1_Channel4;       //DMA Tx Channel
		mDMAIRQn = DMA1_Channel4_IRQn;  //DMA IRQn
		mDMATCFlag = DMA1_FLAG_TC4;       //DMA TC Mask
		mDMAGLFlag = DMA1_IT_GL4;         //DMA IT GL mask
		pCOM1 = this;
#endif

#ifdef USE_USART1
		/*pCOM1 = this;*/
#endif
	}
	else if (index == COM2)
	{
		mIRQn = USART2_IRQn;                                           //USART IRQn
		mUSARTRcc = RCC_APB1Periph_USART2;                             //USARTx Clock	
		if (mRemap)
		{
			this->mPortTx.SetPin(PD5);
			this->mPortRx.SetPin(PD6);
		}
		else
		{
			this->mPortTx.SetPin(PA2);
			this->mPortRx.SetPin(PA3);
		}
#ifdef USE_USART2_DMA
		mDMATxCh = DMA1_Channel7;       //DMA Tx Channel
		mDMAIRQn = DMA1_Channel7_IRQn;  //DMA IRQn
		mDMATCFlag = DMA1_FLAG_TC7;       //DMA TC Mask
		mDMAGLFlag = DMA1_IT_GL7;         //DMA IT GL mask
		pCOM2 = this;
#endif
#ifdef USE_USART2
		/*pCOM2 = this;*/
#endif
	}
	else if (index == COM3)
	{
		mIRQn = USART3_IRQn;                                           //USART IRQn
		mUSARTRcc = RCC_APB1Periph_USART3;	                                //USARTx Clock
		if (mRemap)
		{
			this->mPortTx.SetPin(PC10);
			this->mPortRx.SetPin(PC11);
		}
		else
		{
			if (mRemapvalue == 0x01)
			{
				this->mPortTx.SetPin(PB10);
				this->mPortRx.SetPin(PB11);
			}
			else
			{
				this->mPortTx.SetPin(PD8);
				this->mPortRx.SetPin(PD9);
			}

		}
#ifdef USE_USART3_DMA
		mDMATxCh = DMA1_Channel2;       //DMA Tx Channel
		mDMAIRQn = DMA1_Channel2_IRQn;  //DMA IRQn
		mDMATCFlag = DMA1_FLAG_TC2;       //DMA TC Mask
		mDMAGLFlag = DMA1_IT_GL2;         //DMA IT GL mask
		pCOM3 = this;
#endif
#ifdef USE_USART3
		/*pCOM3 = this;*/
#endif
	}

	Initialize();

}

void USARTHAL::Initialize()
{
	USART_TypeDef* mUSARTx;   //USARTx

	if (this->index == COM1)
	{
		mUSARTx = USART1;
	}
	else if (this->index == COM2)
	{
		mUSARTx = USART2;
	}
	else if (this->index == COM3)
	{
		mUSARTx = USART3;
	}
	else
	{
	}

	if (this->index == COM1)
	{
		RCC_APB2PeriphClockCmd(mUSARTRcc, ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(mUSARTRcc, ENABLE);
	}
	if (mRemap)
	{
		if (mUSARTx == USART1) GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
		if (mUSARTx == USART2) GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
		if (mUSARTx == USART3) GPIO_PinRemapConfig(mRemapvalue == 0X01 ? GPIO_PartialRemap_USART3 : GPIO_FullRemap_USART3, ENABLE);
	}
	this->mPortTx.pinMode(GPIO_AF_PP);
	this->mPortRx.pinMode(GPIO_IN_FLOATING);
	switch (mPriGroup)
	{
	case 0:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	break;
	case 1:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	break;
	case 2:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	break;
	case 3:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	break;
	case 4:	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	break;
	default:NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	break;
	}
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = mIRQn;   //USART IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = mPrePri; //preemption priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = mSubPri; //sub priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //enable interrup
	NVIC_Init(&NVIC_InitStructure);                                 //initialize irq
#ifdef USE_USART_DMA
	NVIC_InitStructure.NVIC_IRQChannel = mDMAIRQn;//DMA IRQ
	NVIC_Init(&NVIC_InitStructure);
	//	mDMATxCh->CCR |= DMA_IT_TC;  //Enable DMA TX Channel TCIT 
	//	mDMATxCh->CCR |= DMA_IT_TE;  //Enable DMA TX Channel TEIT
#endif

	USART_InitTypeDef USART_InitStructure;                       //
	USART_InitStructure.USART_BaudRate = mBaudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(mUSARTx, &USART_InitStructure);

	USART_Cmd(mUSARTx, ENABLE);

	USART_ITConfig(mUSARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(mUSARTx, USART_IT_IDLE, ENABLE); //开启串口总线空闲中断
	USART_ITConfig(mUSARTx, USART_IT_TC, ENABLE);

#ifndef USE_USART_DMA	
	USART_ITConfig(mUSARTx, USART_IT_TC, DISABLE);
#endif	
	USART_ClearFlag(mUSARTx, USART_FLAG_TC);      //clear TC flag to make sure the first byte can be send correctly

#ifdef USE_USART_DMA
	InitDMA();
#endif
}

void USARTHAL::SetBaudRate(uint32_t baudRate)
{
	USART_TypeDef* mUSARTx;   //USARTx

	if (this->index == COM1)
	{
		mUSARTx = USART1;
	}
	else if (this->index == COM2)
	{
		mUSARTx = USART2;
	}
	else if (this->index == COM3)
	{
		mUSARTx = USART3;
	}
	else
	{
	}

	//	assert_param(IS_USART_BAUDRATE(baudRate));
	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;
	uint32_t usartxbase = 0;
	RCC_ClocksTypeDef RCC_ClocksStatus;

	usartxbase = (uint32_t)mUSARTx;

	/*---------------------------- USART BRR Configuration -----------------------*/
	/* Configure the USART Baud Rate -------------------------------------------*/
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	if (usartxbase == USART1_BASE)
	{
		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	}
	else
	{
		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
	}

	/* Determine the integer part */
	if ((mUSARTx->CR1 & ((uint16_t)0x8000)) != 0)//CR1_OVER8_Set=((uint16_t)0x8000)
	{
		/* Integer part computing in case Oversampling mode is 8 Samples */
		integerdivider = ((25 * apbclock) / (2 * (baudRate)));
	}
	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
	{
		/* Integer part computing in case Oversampling mode is 16 Samples */
		integerdivider = ((25 * apbclock) / (4 * (baudRate)));
	}
	tmpreg = (integerdivider / 100) << 4;

	/* Determine the fractional part */
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

	/* Implement the fractional part in the register */
	if ((mUSARTx->CR1 & ((uint16_t)0x8000)) != 0)
	{
		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
	}
	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
	{
		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
	}

	/* Write to USART BRR */
	mUSARTx->BRR = (uint16_t)tmpreg;
}

#ifdef USE_USART_DMA
void USARTHAL::InitDMA()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                   //enable DMA clock

	DMA_DeInit(mDMATxCh);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)& mUSARTx->DR;	       //DMA Tx data target address
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)mDMATxBuf;           //DMA data source address
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	   //DMA data direction: from memory to peripheral
	DMA_InitStructure.DMA_BufferSize = 0;                        //data size to send with DMA
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//peripheral address not increase
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	   //memory address increase
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//peripheral data unit: Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 //memory data unit: Byte
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;         //DMA mode: normal (not loop)
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;         //DMA priority: low
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;          //disable memory to memory

	DMA_Init(mDMATxCh, &DMA_InitStructure);                              //config DMA	   

	DMA_ITConfig(mDMATxCh, DMA_IT_TC, ENABLE);
	DMA_ITConfig(mDMATxCh, DMA_IT_TE, ENABLE);

	USART_DMACmd(mUSARTx, USART_DMAReq_Tx, ENABLE);                      //enable DMA Tx of USART
	DMA_Cmd(mDMATxCh, DISABLE);		                                       //disable DMA			
}


void USARTHAL::DMAIRQ()
{
	if (DMA_GetFlagStatus(mDMATCFlag) == SET) //DMA Tx Complete
	{
		DMA_ClearITPendingBit(mDMAGLFlag);   //Clear DMA global interrupt flag
		DMA_ClearFlag(mDMATCFlag);           //Clear DMA Tx complete flag
		DMA_Cmd(mDMATxCh, DISABLE);           //close dma

		if (mTxBuf.Size() > 0)
		{
			if (mTxBuf.Size() <= USART_DMA_TX_BUFFER_SIZE)  //the rest data bytes less than DMA buffer size
			{
				mDMATxCh->CNDTR = mTxBuf.Size();             //send all rest data to DMA buffer 
				mTxBuf.Gets(mDMATxBuf, mTxBuf.Size());
			}
			else                                           //the rest data bytes more than DMA buffer size
			{
				mDMATxCh->CNDTR = USART_DMA_TX_BUFFER_SIZE;  //send max DMA buffer size data to DMA buffer
				mTxBuf.Gets(mDMATxBuf, USART_DMA_TX_BUFFER_SIZE);
			}
			DMA_Cmd(mDMATxCh, ENABLE); 	                   //enable DMA
		}
		else
			isBusySend = false;               //将忙标志
	}
}
#endif
