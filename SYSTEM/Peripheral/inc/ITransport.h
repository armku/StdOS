#pragma once

#include "Type.h"
#include "Buffer.h"

class ITransport;
class SerialPortOld;
#define NULL 0

//�ص���������,�������ݵ���ʱ����
typedef uint(*IOnUsartRead)(ITransport *transport, Buffer &bs, void *param);


typedef uint(*TransportHandler)(ITransport *transport, Buffer &bs, void *param); //��ʱ������

class ITransport
{
    public:
        void Open(){OnOpen();}
        //�򿪶˿�
        bool HasHandler()
        {
                return false;
        }
        uint Read(byte *buf, uint len)
        {
                return 10;
        }
        uint Write(byte *buf, uint len)
        {
                return len;
        }
        int OnReceive(byte *buf, uint len)
        {
                return len;
        }
        void SendData(byte data, uint times = 3000);
        void SendBuffer(byte *buff, int length =  - 1); //��������
        void SendBuffer(char *buff, int length =  - 1); //��������
        void Register(IOnUsartRead handler, void *param = NULL); // ע�����ݵ����¼�
		bool Opened;//�Ƿ��
	protected:
		virtual bool OnOpen(){return true;}	

};
