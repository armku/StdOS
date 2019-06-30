#include "ARCH_UART.h"
#include "stm32f10x.h"

Port* COM1RS485 = NULL;
Port* COM2RS485 = NULL;
Port* COM3RS485 = NULL;
Port* COM4RS485 = NULL;
Port* COM5RS485 = NULL;

#ifdef __cplusplus
extern "C" {
#endif

pFun_UART UART_1_rcv_IRQ = NULL;
pFun_UART UART_2_rcv_IRQ = NULL;
pFun_UART UART_3_rcv_IRQ = NULL;
pFun_UART UART_4_rcv_IRQ = NULL;
pFun_UART UART_5_rcv_IRQ = NULL;

void UART_1_send_byte(uint8_t c)
{
	*COM1RS485 = 1;//进入发送模式

	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
}

void UART_2_send_byte(uint8_t c)
{
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_SendData(USART2, c);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);
}

void UART_3_send_byte(uint8_t c)
{
	*COM3RS485 = 1;//进入发送模式

	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_SendData(USART3, c);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);
}

void UART_4_send_byte(uint8_t c)
{
	*COM4RS485 = 1;//进入发送模式

	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_SendData(UART4, c);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) != SET);
}

void UART_5_send_byte(uint8_t c)
{
	*COM5RS485 = 1;//进入发送模式

	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_SendData(UART5, c);
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC) != SET);
}

///////////////////////////////////////////////////////////
void USART1_IRQHandler(void)
{
	uint8_t c;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		c = USART_ReceiveData(USART1);
		if(UART_1_rcv_IRQ)
		{
			UART_1_rcv_IRQ(c);
		}
	}
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//传输完一条完整的数据就会进入这个
	{
		char ch = 0;
		ch = USART1->SR; //先读SR，然后读DR才能清除
		ch = USART1->DR;
		ch = ch;
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		//
		//if (COM1RS485 && mTxBuf.Size() == 0)
		{
			*COM1RS485 = 0;//进入接收模式
		}
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET || USART_GetITStatus(USART1, USART_IT_TC) != RESET)   //TxE and TC
	{
		//if (mTxBuf.Size() > 0)                                //still left some bytes of data
		//{
		//	mTxBuf.Get(data);                                //get one byte data from buffer
		//	USART1->DR = (data & (uint16_t)0x01FF);              //send one byte data
		//}
		//else                                               //all data send complete
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  //disable TxE
			USART_ITConfig(USART1, USART_IT_TC, DISABLE);   //disable TC
			//isBusySend = false;                              //set free state
		}
	}
}

void USART2_IRQHandler(void)
{
	uint8_t c;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		c = USART_ReceiveData(USART2);
		if (UART_2_rcv_IRQ)
		{
			UART_2_rcv_IRQ(c);
		}
	}
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)//传输完一条完整的数据就会进入这个
	{
		char ch = 0;
		ch = USART2->SR; //先读SR，然后读DR才能清除
		ch = USART2->DR;
		ch = ch;
	}
	if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		//
		//if (COM2RS485 && mTxBuf.Size() == 0)
		{
			//*COM2RS485 = 0;//进入接收模式
		}
	}
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET || USART_GetITStatus(USART2, USART_IT_TC) != RESET)   //TxE and TC
	{
		//if (mTxBuf.Size() > 0)                                //still left some bytes of data
		//{
		//	mTxBuf.Get(data);                                //get one byte data from buffer
		//	USART2->DR = (data & (uint16_t)0x01FF);              //send one byte data
		//}
		//else                                               //all data send complete
		{
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);  //disable TxE
			USART_ITConfig(USART2, USART_IT_TC, DISABLE);   //disable TC
			//isBusySend = false;                              //set free state
		}
	}
}

void USART3_IRQHandler(void)
{
	uint8_t c;

	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		c = USART_ReceiveData(USART3);
		if (UART_3_rcv_IRQ)
		{
			UART_3_rcv_IRQ(c);
		}
	}
	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)//传输完一条完整的数据就会进入这个
	{
		char ch = 0;
		ch = USART3->SR; //先读SR，然后读DR才能清除
		ch = USART3->DR;
		ch = ch;
	}
	if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		//
		//if (COM3RS485 && mTxBuf.Size() == 0)
		{
			*COM3RS485 = 0;//进入接收模式
		}
	}
	if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET || USART_GetITStatus(USART3, USART_IT_TC) != RESET)   //TxE and TC
	{
		//if (mTxBuf.Size() > 0)                                //still left some bytes of data
		//{
		//	mTxBuf.Get(data);                                //get one byte data from buffer
		//	USART3->DR = (data & (uint16_t)0x01FF);              //send one byte data
		//}
		//else                                               //all data send complete
		{
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);  //disable TxE
			USART_ITConfig(USART3, USART_IT_TC, DISABLE);   //disable TC
			//isBusySend = false;                              //set free state
		}
	}
}

void UART4_IRQHandler(void)
{
	uint8_t c;

	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		c = USART_ReceiveData(UART4);
		if (UART_4_rcv_IRQ)
		{
			UART_4_rcv_IRQ(c);
		}
	}
	if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)//传输完一条完整的数据就会进入这个
	{
		char ch = 0;
		ch = UART4->SR; //先读SR，然后读DR才能清除
		ch = UART4->DR;
		ch = ch;
	}
	if (USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		//
		//if (COM4RS485 && mTxBuf.Size() == 0)
		{
			*COM4RS485 = 0;//进入接收模式
		}
	}
	if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET || USART_GetITStatus(UART4, USART_IT_TC) != RESET)   //TxE and TC
	{
		//if (mTxBuf.Size() > 0)                                //still left some bytes of data
		//{
		//	mTxBuf.Get(data);                                //get one byte data from buffer
		//	USART4->DR = (data & (uint16_t)0x01FF);              //send one byte data
		//}
		//else                                               //all data send complete
		{
			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);  //disable TxE
			USART_ITConfig(UART4, USART_IT_TC, DISABLE);   //disable TC
			//isBusySend = false;                              //set free state
		}
	}
}

void UART5_IRQHandler(void)
{
	uint8_t c;

	if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{
		c = USART_ReceiveData(UART5);
		if (UART_5_rcv_IRQ)
		{
			UART_5_rcv_IRQ(c);
		}
	}
	if (USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)//传输完一条完整的数据就会进入这个
	{
		char ch = 0;
		ch = UART5->SR; //先读SR，然后读DR才能清除
		ch = UART5->DR;
		ch = ch;
	}
	if (USART_GetITStatus(UART5, USART_IT_TC) != RESET)
	{
		//
		//if (COM5RS485 && mTxBuf.Size() == 0)
		{
			*COM5RS485 = 0;//进入接收模式
		}
	}
	if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET || USART_GetITStatus(UART5, USART_IT_TC) != RESET)   //TxE and TC
	{
		//if (mTxBuf.Size() > 0)                                //still left some bytes of data
		//{
		//	mTxBuf.Get(data);                                //get one byte data from buffer
		//	USART5->DR = (data & (uint16_t)0x01FF);              //send one byte data
		//}
		//else                                               //all data send complete
		{
			USART_ITConfig(UART5, USART_IT_TXE, DISABLE);  //disable TxE
			USART_ITConfig(UART5, USART_IT_TC, DISABLE);   //disable TC
			//isBusySend = false;                              //set free state
		}
	}
}

///////////////////////////////////////////////////////////

void UART1_Dis_IRQ(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
}

void UART2_Dis_IRQ(void)
{
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
}

void UART3_Dis_IRQ(void)
{
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
}

void UART4_Dis_IRQ(void)
{
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
}

void UART5_Dis_IRQ(void)
{
	USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
}

///////////////////////////////////////////////////////////

void UART1_En_IRQ(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void UART2_En_IRQ(void)
{
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void UART3_En_IRQ(void)
{
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

void UART4_En_IRQ(void)
{
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
}

void UART5_En_IRQ(void)
{
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
}

//通过串口（通讯串口）发送数据
void UART1_send_data(uint8_t *data, uint32_t len)
{
	if (COM1RS485 != NULL)
	{
		*COM1RS485 = 1;//进入发送模式
	}
	while(len--)
	{
		UART_1_send_byte(*data++);
	}
	if (COM1RS485 != NULL)
	{
		*COM1RS485 = 0;//进入接收模式
	}
}

//通过串口（通讯串口）发送数据
void UART2_send_data(uint8_t *data, uint32_t len)
{
	if (COM2RS485 != NULL)
	{
		*COM2RS485 = 1;//进入发送模式
	}
	while(len--)
	{
		UART_2_send_byte(*data++);
	}
	if (COM2RS485 != NULL)
	{
		*COM2RS485 = 0;//进入接收模式
	}
}

//通过串口（通讯串口）发送数据
void UART3_send_data(uint8_t *data, uint32_t len)
{
	if (COM3RS485 != NULL)
	{
		*COM3RS485 = 1;//进入发送模式
	}
	while(len--)
	{
		UART_3_send_byte(*data++);
	}
	if (COM3RS485 != NULL)
	{
		*COM3RS485 = 0;//进入接收模式
	}
}

//通过串口（通讯串口）发送数据
void UART4_send_data(uint8_t *data, uint32_t len)
{
	if (COM4RS485 != NULL)
	{
		*COM4RS485 = 1;//进入发送模式
	}
	while(len--)
	{
		UART_4_send_byte(*data++);
	}
	if (COM4RS485 != NULL)
	{
		*COM4RS485 = 0;//进入接收模式
	}
}

//通过串口（通讯串口）发送数据
void UART5_send_data(uint8_t *data, uint32_t len)
{
	while(len--)
	{
		UART_5_send_byte(*data++);
	}
	if (COM5RS485 != NULL)
	{
		*COM5RS485 = 0;//进入接收模式
	}
}

#ifdef __cplusplus
}
#endif
///////////////////////////////////////////////////////////
