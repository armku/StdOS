#ifndef __Enc28j60_H__
#define __Enc28j60_H__

#include "Kernel\Sys.h"
#include "Device\Spi.h"
#include "Device\Power.h"
#include "Net\ITransport.h"
#include "Net\Socket.h"

// Enc28j60��
class Enc28j60 : public ITransport
{
private:
	Spi* _spi;
	OutputPort _ce;
	OutputPort _reset;

	uint NextPacketPtr;

	UInt64	LastTime;		// ��¼���һ���յ����ݵ�ʱ�䣬��ʱ����
	int		ResetPeriod;	// ���������Ĭ��6000΢��
	uint	_ResetTask;		// ��������
public:
	//byte Mac[6];
	MacAddress Mac;
	byte Bank;

	Enc28j60();
	virtual ~Enc28j60();
	void Init(Spi* spi, Pin ce = P0, Pin reset = P0);

	byte ReadOp(byte op, byte addr);
	void WriteOp(byte op, byte addr, byte data);
	void ReadBuffer(byte* buf, int len);
	void WriteBuffer(const byte* buf, int len);
	// �趨�Ĵ�����ַ����
	void SetBank(byte addr);
	// ��ȡ�Ĵ���ֵ ���Ͷ��Ĵ�������͵�ַ
	byte ReadReg(byte addr);
	// д�Ĵ���ֵ ����д�Ĵ�������͵�ַ
	void WriteReg(byte addr, byte data);
	ushort PhyRead(byte addr);
	bool PhyWrite(byte addr, ushort data);
	void ClockOut(byte clock);
	bool Linked();

	// ��Դ�ȼ�����������͹���ģʽ��ʱ����
	virtual void ChangePower(int level);

	//void Init(byte mac[6]);
	byte GetRevision();

	bool Broadcast;
	// �����Ƿ���չ㲥���ݰ�
	void SetBroadcast(bool flag);
	// ��ʾ�Ĵ���״̬
	void ShowStatus();

	int	Error;	// �������
	void CheckError();

	//virtual const String ToString() const { return String("Enc28j60"); }

protected:
	virtual bool OnOpen();
	virtual void OnClose() { }

	virtual bool OnWrite(const Buffer& bs);
	virtual uint OnRead(Buffer& bs);
};

#endif
