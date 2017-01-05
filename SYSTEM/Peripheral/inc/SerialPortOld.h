#pragma once

#include "stm32f10x.h"
#include "Sys.h"
#include "FIFORing.h"

class SerialPortOld
{
    public:
        SerialPortOld(COM_Def index, int baudRate = 115200, byte parity = USART_Parity_No,  //����żУ��
        byte dataBits = USART_WordLength_8b,  //8λ���ݳ���
        byte stopBits = USART_StopBits_1); //1λֹͣλ
		char Name[5]; // ���ơ�COMx������1�ֽ�\0��ʾ����
		void SendData(byte data);
    private:
        byte _index;
        byte _parity;
        byte _dataBits;
        byte _stopBits;
        int _baudRate;


};
extern uint com1timeidle; //����1����ʱ��

extern CFIFORing com1buf; //����1���ջ�����
