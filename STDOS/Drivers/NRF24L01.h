#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include "Kernel\Sys.h"
#include "Device\Spi.h"
#include "Net\ITransport.h"
#include "Device\Power.h"
#include "Message\DataStore.h"
#include "APP\FlushPort.h"

// NRF24L01��
class NRF24L01 : public ITransport, public Power
{
public:
    byte Channel;		// ͨѶƵ��������Ƶ�ʺţ���2400MHZ�����ϼ�
	byte Local[5];		// ���ص�ַ
	byte Remote[5];		// Զ�̵�ַ

	bool DynPayload;	// ��̬����
	bool AutoAnswer;	// �Զ�Ӧ��Ĭ������
	ushort Speed;		// ��Ƶ�����ʣ���λkbps��Ĭ��250kbps����ѡ1000kbps/2000kbps���ٶ�Խ�ʹ���ԽԶ
	byte RadioPower;	// ���书�ʡ���8�������0x07����7dBm�����
	bool Master;		// �Ƿ����ڵ㡣

	ushort	Error;		// �������������������������Զ�����

	NRF24L01();
    virtual ~NRF24L01();
    void Init(Spi* spi, Pin ce = P0, Pin irq = P0, Pin power = P0);

    bool Check();
	bool Config();		// ��ɻ��������趨��Ĭ�ϳ�ʼ��Ϊ����ģʽ
	bool GetPower();	// ��ȡ��ǰ��Դ״̬
	bool SetPowerMode(bool on);	// ���õ�ǰ��Դ״̬�������Ƿ�ɹ�
	bool GetMode();		// ��ȡ��ǰģʽ�Ƿ����ģʽ
    bool SetMode(bool isReceive, const Buffer& addr);	// �л��շ�ģʽ�������������趨
	void SetAddress();	// ���õ�ַ

	// ��Դ�ȼ�����������͹���ģʽ��ʱ����
	virtual void ChangePower(int level);

	typedef int (*FuncBufInt)(const Buffer&);
	FuncBufInt	FixData;// �������ݵ�ί��

	byte Status;
	byte FifoStatus;
	void ShowStatus();

	IDataPort*	Led;	// ���ݵ�
	void SetLed(OutputPort& led);
	void SetLed(Pin led);

	//virtual String ToString() const { return String("R24"); }
	virtual String& ToStr(String& str) const { return str + "R24"; }

private:

	virtual bool OnOpen();
    virtual void OnClose();

    virtual bool OnWrite(const Buffer& bs);
	virtual uint OnRead(Buffer& bs);

	// �������ݵ����¼�
	//virtual uint OnReceive(Buffer& bs, void* param);
	virtual bool OnWriteEx(const Buffer& bs, const void* opt);
	
	bool SendTo(const Buffer& bs, const Buffer& addr);

    Spi*		_spi;
    OutputPort	_CE;
    InputPort	Irq;
	OutputPort	_Power;	// ���ÿ���2401��Դ������  ֱ�ӽ��ж�2401��ͨ�ϵ����������������setPower��Ч

    byte WriteBuf(byte reg, const Buffer& bs);
    byte ReadBuf(byte reg, Buffer& bs);
    byte ReadReg(byte reg);
    byte WriteReg(byte reg, byte dat);

	void AddError();

	// ��������
	static void ReceiveTask(void* param);
	uint _tidOpen;
	uint _tidRecv;
	void OnIRQ(InputPort& port, bool down);
	void OnIRQ();
};

#endif
