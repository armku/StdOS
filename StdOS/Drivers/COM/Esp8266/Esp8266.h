#ifndef _ESP8266_H
#define _ESP8266_H

#include "Port.h"
#include "../HAL/STM32F1/ARCH_UART.h"

class Esp8266
{
public:
	Esp8266();
	~Esp8266();

	void SetPinCH_PD(Pin p);
	void SetPinRST(Pin p);

	void Cmd(char* cmd, int len);

	void Init();
	void AT();
	bool ESP8266_Cmd(char* cmd, char* reply1, char* reply2, uint32_t waittime);
	int step;//处理步骤
//private:
	Port CH_PD;//
	Port RST;//
	pFun_UART send_byte;        // 串口发送一字节数据
	pFun_UART_buf send_buf;		// 发送缓冲区
};



#endif // !_ESP8266_H
