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
private:
	TransportHandler _handler;
	void* _param;
public:
	bool Opening;	// �Ƿ����ڴ�
    bool Opened;    // �Ƿ��

	ushort	MinSize;	// ���ݰ���С��С
	ushort	MaxSize;	// ���ݰ�����С

	// ��ʼ��
	ITransport();
	// ��������ȷ���ر�
	virtual ~ITransport();

	// �򿪴����
	bool Open();
	// �رմ����
	void Close();

	// ��������
	bool Write(const Buffer& bs);
	bool Write(const Buffer& bs, const void* opt);
	// ��������
	uint Read(Buffer& bs);

	// ע��ص�����
	virtual void Register(TransportHandler handler, void* param = nullptr);

protected:
	virtual bool OnOpen() { return true; }
	virtual void OnClose() { }
	virtual bool OnWrite(const Buffer& bs) = 0;
	virtual bool OnWriteEx(const Buffer& bs, const void* opt);
	virtual uint OnRead(Buffer& bs) = 0;

	// �Ƿ��лص�����
	bool HasHandler() { return _handler != nullptr; }

	// �������ݵ����¼�
	virtual uint OnReceive(Buffer& bs, void* param);
public:        
        uint Read(byte *buf, uint len);
        uint Write(byte *buf, uint len);
        int OnReceive(byte *buf, uint len);
        void SendData(byte data, uint times = 3000);
        void SendBuffer(byte *buff, int length =  - 1); //��������
        void SendBuffer(char *buff, int length =  - 1); //��������
    protected:        
        virtual bool OnWrite(const byte *buf, uint size);
		virtual uint OnRead(byte *buf, uint size);
        TransportHandler pHandler;

};

#endif
