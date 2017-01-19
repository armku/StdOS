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
		//115200 ����żУ�� 8λ���ݳ��� 1λֹͣλ
        SerialPortOld(COM_Def index, int baudRate = 115200, byte parity = USART_Parity_No,byte dataBits = USART_WordLength_8b, byte stopBits = USART_StopBits_1);
		char Name[5]; // ���ơ�COMx������1�ֽ�\0��ʾ����
		void SendData(byte data);
		void SendBuffer(byte *buff,int length=-1);//��������
		void SendBuffer(char *buff,int length=-1);//��������
		void Open();
		void Register(IOnUsartRead handler,SerialPortOld *sp); // ע�����ݵ����¼�	
		void OnUsartReceive(byte * buf,ushort length);//�Ӵ����յ�����
		OutputPort *RS485; // RS485ʹ������
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
