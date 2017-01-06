#pragma once

#include "stm32f10x.h"
#include "Sys.h"
#include "FIFORing.h"
#include "ITransport.h"

class SerialPortOld:public ITransport
{
    public:
		//115200 无奇偶校验 8位数据长度 1位停止位
        SerialPortOld(COM_Def index, int baudRate = 115200, byte parity = USART_Parity_No,byte dataBits = USART_WordLength_8b, byte stopBits = USART_StopBits_1);
		char Name[5]; // 名称。COMx，后面1字节\0表示结束
		void SendData(byte data);
		void SendBuffer(byte *buff,int length=-1);//发送数据
		void SendBuffer(char *buff,int length=-1);//发送数据
		void Open();
    private:
        byte _index;
        byte _parity;
        byte _dataBits;
        byte _stopBits;
        int _baudRate;


};
extern uint com1timeidle; //串口1空闲时间
extern CFIFORing com1buf; //串口1接收缓冲区
