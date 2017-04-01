#ifndef _BufferPort_H_
#define _BufferPort_H_

#include "Net\ITransport.h"
#include "Message\DataStore.h"

// �������˿ڡ�Ŀ���豸��ʱ�������ݣ�ֻ��Ҫ��������һ�ݸ���
class BufferPort
{
public:
    cstring	Name;
	ITransport*	Port;
	int			Speed;
	COM			Com;
	bool		Opened;

	IDataPort*	Led;	// ָʾ��

	ByteArray	Buf;	// ���ڽ������ݵĻ��������ⲿ�ڴ�ǰ���ô�С

    BufferPort();
    ~BufferPort();

	bool Open();
	void Close();

protected:
	virtual bool OnOpen(bool isNew);
	virtual void OnReceive(const Buffer& bs, void* param);

private:
	static uint OnReceive(ITransport* transport, Buffer& bs, void* param, void* param2);
};

#endif
