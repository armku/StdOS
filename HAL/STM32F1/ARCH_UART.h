#ifndef _ARCH_UART_H
#define _ARCH_UART_H

#include <stdint.h>
#include "type.h"
#include "Port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
	{
		//byte data[1024];
		byte data[512];
		uint len;
	}
	_data_Q_t;

	//�������ݴ���
	typedef struct
	{
		int TimeCnt;//���յ����ݵ�ʱ��
		int UsartReadCnt;//���յ������ݳ���
		int UsartIdleCnt;//���ڿ���ʱ��
		int UsartFlag;//���յ�һ֡����
		int UsartReadCntCopy;
	}UsartKernel_T;
	
	typedef void (*pFun_UART)(byte c); 
	typedef void (*pFun_UART_buf)(byte *buf, uint len);
	
	
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
	
	void UART_1_send_byte(byte c);//������485�����
	void UART_2_send_byte(byte c);//������485�����
	void UART_3_send_byte(byte c);//������485�����
	void UART_4_send_byte(byte c);//������485�����
	void UART_5_send_byte(byte c);//������485�����
	
	//ͨ�����ڣ�ͨѶ���ڣ���������
	void UART1_send_data(byte *data, uint len);

//ͨ�����ڣ�ͨѶ���ڣ���������
	void UART2_send_data(byte *data, uint len);

//ͨ�����ڣ�ͨѶ���ڣ���������
	void UART3_send_data(byte *data, uint len);

//ͨ�����ڣ�ͨѶ���ڣ���������
	void UART4_send_data(byte *data, uint len);

//ͨ�����ڣ�ͨѶ���ڣ���������
	void UART5_send_data(byte *data, uint len);

	extern pFun_UART UART_1_rcv_IRQ;
	extern pFun_UART UART_2_rcv_IRQ  ;
	extern pFun_UART UART_3_rcv_IRQ  ;
	extern pFun_UART UART_4_rcv_IRQ  ;
	extern pFun_UART UART_5_rcv_IRQ  ;

	extern Port* COM1RS485;
	extern Port* COM2RS485;
	extern Port* COM3RS485;
	extern Port* COM4RS485;
	extern Port* COM5RS485;

#ifdef __cplusplus
}
#endif

#endif
