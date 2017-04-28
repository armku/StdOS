#ifndef __ITransport_H__
#define __ITransport_H__

#include "Kernel\Sys.h"

class ITransport;

// ��������ݵ���ί�С��������ݻ�������ַ�ͳ��ȣ����з�������ʹ�øû��������������ݳ���
typedef uint (*TransportHandler)(ITransport* port, Buffer& bs, void* param, void* param2);

// ֡���ݴ���ӿ�
// ʵ����ȷ�������԰�����ʽ���䣬�������ݵ�ճ���Ͳ��
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

#endif
