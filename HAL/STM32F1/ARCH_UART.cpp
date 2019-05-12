#include "ARCH_UART.h"
#include "stm32f10x.h"

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
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_SendData(USART3, c);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);
}

void UART_4_send_byte(uint8_t c)
{
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_SendData(UART4, c);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) != SET);
}

void UART_5_send_byte(uint8_t c)
{
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_SendData(UART5, c);
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC) != SET);
}

///////////////////////////////////////////////////////////
#if 0

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
}
#endif
#if 0
void USART2_IRQHandler(void)
{
	uint8_t c;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		c = USART_ReceiveData(USART2);
		if(UART_2_rcv_IRQ)
		{
			UART_2_rcv_IRQ(c);
		}
	}
}

void USART3_IRQHandler(void)
{
	uint8_t c;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		c = USART_ReceiveData(USART3);
		if(UART_3_rcv_IRQ)
		{
			UART_3_rcv_IRQ(c);
		}
	}
}

void UART4_IRQHandler(void)
{
	uint8_t c;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		c = USART_ReceiveData(UART4);
		if(UART_4_rcv_IRQ)
		{
			UART_4_rcv_IRQ(c);
		}
	}
}

void UART5_IRQHandler(void)
{
	uint8_t c;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		c = USART_ReceiveData(UART5);
		if(UART_5_rcv_IRQ)
		{
			UART_5_rcv_IRQ(c);
		}
	}
}
#endif
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
	while(len--)
	{
		UART_1_send_byte(*data++);
	}
}

//通过串口（通讯串口）发送数据
void UART2_send_data(uint8_t *data, uint32_t len)
{
	while(len--)
	{
		UART_2_send_byte(*data++);
	}
}

//通过串口（通讯串口）发送数据
void UART3_send_data(uint8_t *data, uint32_t len)
{
	while(len--)
	{
		UART_3_send_byte(*data++);
	}
}

//通过串口（通讯串口）发送数据
void UART4_send_data(uint8_t *data, uint32_t len)
{
	while(len--)
	{
		UART_4_send_byte(*data++);
	}
}

//通过串口（通讯串口）发送数据
void UART5_send_data(uint8_t *data, uint32_t len)
{
	while(len--)
	{
		UART_5_send_byte(*data++);
	}
}

#ifdef __cplusplus
}
#endif
///////////////////////////////////////////////////////////
