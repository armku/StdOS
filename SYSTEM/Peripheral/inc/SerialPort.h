#pragma once

#include "ITransport.h"
#include "Sys.h"
#include "stm32f10x.h"
#include "Port.h"
#include "InputPort.h"
#include "CString.h"

// ������
class SerialPort: public ITransport
{
    private:
        byte _index;
        byte _parity;
        byte _dataBits;
        byte _stopBits;
        int _baudRate;

        USART_TypeDef *_port;
        AlternatePort _tx;
        #if defined(STM32F0) || defined(STM32F4)
            AlternatePort _rx;
        #else 
            InputPort _rx;
        #endif 

        void Init();

    public:
        char Name[5]; // ���ơ�COMx������1�ֽ�\0��ʾ����
        bool IsRemap; // �Ƿ���ӳ��
        OutputPort *RS485; // RS485ʹ������
        int Error; // �������

        SerialPort();
        SerialPort(COM_Def index, int baudRate = 115200, byte parity = USART_Parity_No,byte dataBits = USART_WordLength_8b,byte stopBits = USART_StopBits_1)
        {
                Init();
                Init(index, baudRate, parity, dataBits, stopBits);
        }

        SerialPort(USART_TypeDef *com, int baudRate = 115200, byte parity = USART_Parity_No,  //����żУ��
        byte dataBits = USART_WordLength_8b,  //8λ���ݳ���
        byte stopBits = USART_StopBits_1); //1λֹͣλ
        // ����ʱ�Զ��ر�
        virtual ~SerialPort();

        void Init(byte index, int baudRate = 115200, byte parity = USART_Parity_No,  //����żУ��
        byte dataBits = USART_WordLength_8b,  //8λ���ݳ���
        byte stopBits = USART_StopBits_1); //1λֹͣλ

        void SendData(byte data, uint times = 3000);

        bool Flush(uint times = 3000);
        void GetPins(Pin *txPin, Pin *rxPin);
        virtual void Register(TransportHandler handler, void *param = NULL);

        virtual String ToString()
        {
                return Name;
        }

        static SerialPort *GetMessagePort();
    protected:
        virtual bool OnOpen();
        virtual void OnClose();
        virtual bool OnWrite(const byte *buf, uint size);
        virtual uint OnRead(byte *buf, uint size);
    private:
        static void OnUsartReceive(ushort num, void *param);
};
class SerialPortOld: public ITransport
{
    public:
        //115200 ����żУ�� 8λ���ݳ��� 1λֹͣλ
        SerialPortOld(COM_Def index, int baudRate = 115200, byte parity = USART_Parity_No, byte dataBits = USART_WordLength_8b, byte stopBits = USART_StopBits_1);
        char Name[5]; // ���ơ�COMx������1�ֽ�\0��ʾ����
        void SendData(byte data);
        void SendBuffer(byte *buff, int length =  - 1); //��������
        void SendBuffer(char *buff, int length =  - 1); //��������
        void Open();
        void Register(IOnUsartRead handler, SerialPortOld *sp); // ע�����ݵ����¼�	
        void OnUsartReceive(byte *buf, ushort length); //�Ӵ����յ�����
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
