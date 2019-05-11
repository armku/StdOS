#ifndef _ARCH_UART_H
#define _ARCH_UART_H

#include <stdint.h>
#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
	{
		//uint8_t data[1024];
		uint8_t data[512];
		uint32_t len;
	}
	_data_Q_t;
	
	typedef void (*pFun_UART)(uint8_t c); 
	
	
	void UART1_Dis_IRQ(void);
	void UART2_Dis_IRQ(void);
	void UART3_Dis_IRQ(void);
	void UART4_Dis_IRQ(void);
	void UART5_Dis_IRQ(void);
	
	void UART1_En_IRQ(void);
	void UART2_En_IRQ(void);
	void UART3_En_IRQ(void);
	void UART4_En_IRQ(void);
	void UART5_En_IRQ(void);
	
	void UART_1_send_byte(uint8_t c);
	void UART_2_send_byte(uint8_t c);
	void UART_3_send_byte(uint8_t c);
	void UART_4_send_byte(uint8_t c);
	void UART_5_send_byte(uint8_t c);
	
	//通过串口（通讯串口）发送数据
	void UART1_send_data(uint8_t *data, uint32_t len);

//通过串口（通讯串口）发送数据
	void UART2_send_data(uint8_t *data, uint32_t len);

//通过串口（通讯串口）发送数据
	void UART3_send_data(uint8_t *data, uint32_t len);

//通过串口（通讯串口）发送数据
	void UART4_send_data(uint8_t *data, uint32_t len);

//通过串口（通讯串口）发送数据
	void UART5_send_data(uint8_t *data, uint32_t len);

	extern pFun_UART UART_1_rcv_IRQ;
	extern pFun_UART UART_2_rcv_IRQ  ;
	extern pFun_UART UART_3_rcv_IRQ  ;
	extern pFun_UART UART_4_rcv_IRQ  ;
	extern pFun_UART UART_5_rcv_IRQ  ;

#ifdef __cplusplus
}
#endif

#endif
