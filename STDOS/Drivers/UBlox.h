#ifndef _UBlox_H_
#define _UBlox_H_

#include "Net\ITransport.h"
#include "Message\DataStore.h"

#include "BufferPort.h"

// �����GPS������UBLOX
class UBlox : public BufferPort
{
public:
	cstring Header;	// ʶ��Ϊ���ݰ���ͷ���ַ���
	
	UBlox();

	void SetBaudRate(int baudRate);
	void SetRate();
	
	void EnterConfig();
	void SaveConfig();

protected:
	virtual bool OnOpen(bool isNew);
	virtual void OnReceive(const Buffer& bs, void* param);
};

#endif
