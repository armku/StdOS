#ifndef __SerialPort_H__
#define __SerialPort_H__

#include "Core\Queue.h"
#include "Port.h"
#include "Net\ITransport.h"

// ������
class SerialPort : public ITransport
{
private:
	friend class ComProxy;
	int		_baudRate;
	uint8_t	_dataBits;
	uint8_t	_parity;
	uint8_t	_stopBits;

	void Init();

public:
	char 		Name[5];	// ���ơ�COMxx������1�ֽ�\0��ʾ����
	uint8_t		Remap;		// ��ӳ����
	OutputPort* RS485;		// RS485ʹ������
	int 		Error;		// �������
	int			ByteTime;	// �ֽڼ������С1ms
	Pin			Pins[2];	// Tx/Rx
	Port*		Ports[2];	// Tx/Rx
	COM			Index;

	void*	State;

	// �շ�������
	Queue	Txx;
	Queue	Rxx;

	SerialPort();
	SerialPort(COM index, int baudRate = 0);

	// ����ʱ�Զ��ر�
	virtual ~SerialPort();

	void Set(COM index, int baudRate = 0);
	void Set(uint8_t dataBits, uint8_t parity, uint8_t stopBits);

	int SendData(uint8_t data, int times = 3000);

	bool Flush(int times = 3000);

	void SetBaudRate(int baudRate = 0);

	virtual void Register(TransportHandler handler, void* param = nullptr);

	void OnRxHandler();

	static SerialPort* GetMessagePort();
protected:
	virtual bool OnOpen();

	virtual bool OnWrite(const Buffer& bs);
	virtual uint32_t OnRead(Buffer& bs);

private:
	void Set485(bool flag);

	void*	_task;
	uint32_t	_taskidRx;
	void ReceiveTask();

	void OnWrite2();//���÷����ж�
public:	
	void Printf(char *Data, ...);//ģ��printf
};

#endif
