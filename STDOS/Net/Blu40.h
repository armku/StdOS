#ifndef __BLU40_H__
#define __BLU40_H__

#include "Kernel\Sys.h"
#include "Device\Port.h"
#include "Net\ITransport.h"

// ˼���� CC2540
class Blu40 : public PackPort
{
private:
	OutputPort *_rts;
//	InputPort *_cts;
	OutputPort *_rst;
	OutputPort *_sleep;	// ����ʱ��������������˯�߲�����
	int _baudRate;

public:
	Blu40();
	Blu40(Pin rts,/*Pin cts = P0,*/Pin sleep=P0, OutputPort* rst = nullptr);
	virtual ~Blu40();
	void Init();
	void Init(Pin rts,/*Pin cts = P0,*/Pin sleep=P0, OutputPort* rst = nullptr);

	virtual void Register(TransportHandler handler, void* param = nullptr);
	virtual void Reset(void);

	// ���ò���·
	bool SetBP(int BP);
	// ��������Ƿ�ɹ� ʹ�ô󲿷�ָ��
	bool CheckSet();
	// ���÷����ź�ǿ�� DB��
	bool SetTPL(int TPLDB);
	// ������������
	bool SetName(cstring name);
	// ���ò�Ʒʶ���� Ӳ������code
	bool SetPID(ushort pid);

	//virtual const String ToString() const { return String("BLE4"); }

protected:
	virtual bool OnOpen();
	virtual void OnClose();
};
#endif
