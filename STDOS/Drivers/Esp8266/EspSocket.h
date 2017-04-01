#ifndef __EspSocket_H__
#define __EspSocket_H__

#include "Esp8266.h"

class EspSocket : public ITransport, public Socket
{
protected:
	Esp8266&	_Host;
	byte		_Index;
	int			_Error;

public:
	EspSocket(Esp8266& host, NetType protocol, byte idx);
	virtual ~EspSocket();

	// ��Socket
	virtual bool OnOpen();
	virtual void OnClose();

	virtual bool OnWrite(const Buffer& bs);
	virtual uint OnRead(Buffer& bs);

	// ��������
	virtual bool Send(const Buffer& bs);
	// ��������
	virtual uint Receive(Buffer& bs);

	// �յ�����
	virtual void OnProcess(const Buffer& bs, const IPEndPoint& remote);

protected:
	bool SendData(const String& cmd, const Buffer& bs);
};

#endif
