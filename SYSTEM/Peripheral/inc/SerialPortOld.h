#pragma once

#include "stm32f10x.h"
#include "Sys.h"
#include "FIFORing.h"
#include "ITransport.h"
#include "AlternatePort.h"
#include "InputPort.h"

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
		void Register(IOnUsartRead handler,SerialPortOld *sp); // 注册数据到达事件	
		void OnUsartReceive(byte * buf,ushort length);//从串口收到数据
		OutputPort *RS485; // RS485使能引脚
    private:
        byte _index;
        byte _parity;
        byte _dataBits;
        byte _stopBits;
        int _baudRate;
		IOnUsartRead OnRcv; 
		AlternatePort tx;
		InputPort rx;
};
