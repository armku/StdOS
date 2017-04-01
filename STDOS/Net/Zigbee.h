#ifndef __Zigbee_H__
#define __Zigbee_H__

#include "Kernel\Sys.h"
#include "Device\Port.h"
#include "Net\ITransport.h"

// ZigbeeЭ��
// ��վ�������д�վ�յ�����վ����ֻ����վ�յ�
class Zigbee : public PackPort
{
private:
	OutputPort	_rst;

public:
	Zigbee();
	Zigbee(ITransport* port, Pin rst = P0);
	void Init(ITransport* port, Pin rst = P0);

	//virtual const String ToString() const { return String("Zigbee"); }

protected:
	virtual bool OnOpen();
	virtual void OnClose();
};

#endif
