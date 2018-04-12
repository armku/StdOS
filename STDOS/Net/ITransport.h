#ifndef __ITransport_H__
#define __ITransport_H__

#include "Kernel\Sys.h"

class ITransport;

// ��������ݵ���ί�С��������ݻ�������ַ�ͳ��ȣ����з�������ʹ�øû��������������ݳ���
typedef uint32_t (*TransportHandler)(ITransport* port, Buffer& bs, void* param, void* param2);

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

	uint16_t	MinSize;	// ���ݰ���С��С
	uint16_t	MaxSize;	// ���ݰ�����С

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
	uint32_t Read(Buffer& bs);

	// ע��ص�����
	virtual void Register(TransportHandler handler, void* param = nullptr);

protected:
	virtual bool OnOpen() { return true; }
	virtual void OnClose() { }
	virtual bool OnWrite(const Buffer& bs) = 0;
	virtual bool OnWriteEx(const Buffer& bs, const void* opt);
	virtual uint32_t OnRead(Buffer& bs) = 0;

	// �Ƿ��лص�����
	bool HasHandler() { return _handler != nullptr; }

	// �������ݵ����¼�
	virtual uint32_t OnReceive(Buffer& bs, void* param);
};

// ���ݿڰ�װ
class PackPort : public ITransport
{
private:

public:
	ITransport*	Port;	// �����

	PackPort();
	virtual ~PackPort();

	virtual void Set(ITransport* port);

protected:
	virtual bool OnOpen();
    virtual void OnClose();

    virtual bool OnWrite(const Buffer& bs);
	virtual uint32_t OnRead(Buffer& bs);

	static uint32_t OnPortReceive(ITransport* sender, Buffer& bs, void* param, void* param2);
};

#endif
