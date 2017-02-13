#pragma once

#include "Type.h"
#include "Buffer.h"

class ITransport;
class SerialPortOld;
#define NULL 0

//�ص���������,�������ݵ���ʱ����
typedef uint(*TransportHandler)(ITransport *transport, Buffer &bs, void *param); //��ʱ������

class ITransport
{
    public:
        void Open();
        //�򿪶˿�
        bool HasHandler();
        uint Read(byte *buf, uint len);
        uint Write(byte *buf, uint len);
        int OnReceive(byte *buf, uint len);
        void SendData(byte data, uint times = 3000);
        void SendBuffer(byte *buff, int length =  - 1); //��������
        void SendBuffer(char *buff, int length =  - 1); //��������
        void Register(TransportHandler handler, void *param = NULL); // ע�����ݵ����¼�
        bool Opened; //�Ƿ��
    protected:
        virtual bool OnOpen();
        virtual bool OnWrite(const byte *buf, uint size);
		virtual uint OnRead(byte *buf, uint size);
        TransportHandler pHandler;

};
