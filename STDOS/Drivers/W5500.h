#ifndef _W5500_H_
#define _W5500_H_

#include "Net\NetworkInterface.h"
#include "Net\Socket.h"
#include "Device\Spi.h"

// W5500��̫������
class W5500 : public NetworkInterface
{
public:
	ushort		RetryTime;
	ushort		LowLevelTime;
	byte		RetryCount;
	bool		PingACK;

	uint		TaskID;

	IDataPort*	Led;	// ָʾ��

	// ����
	W5500();
    W5500(Spi* spi, Pin irq, Pin rst);
	W5500(SPI spi, Pin irq, Pin rst);
    virtual ~W5500();

	// ��ʼ��
	void Init();
    void Init(Spi* spi, Pin irq, Pin rst);
	void SetLed(Pin led);
	void SetLed(OutputPort& led);

	virtual bool Config();

	// ��д֡��֡�������ⲿ����   ������֡�����ڲ��Ķ�д��־��
	bool WriteFrame(ushort addr, const Buffer& bs, byte socket = 0 ,byte block = 0);
	bool ReadFrame(ushort addr, Buffer& bs, byte socket = 0 ,byte block = 0);

	// ��λ ����Ӳ����λ�������λ
	void Reset();

	// ��Դ�ȼ�����������͹���ģʽ��ʱ����
	virtual void ChangePower(int level);

	// ����״̬���
	void StateShow();
	// ����PHY״̬  �����Ƿ���������
	virtual bool CheckLink();
	// ���������·��״̬
	void PhyStateShow();

	cstring ToString() const { return "W5500"; }

	virtual Socket* CreateSocket(NetType type);

	// DNS������Ĭ�Ͻ�֧���ַ���IP��ַ����
	virtual IPAddress QueryDNS(const String& domain);
	// ����DNS
	virtual bool EnableDNS();
	// ����DHCP
	virtual bool EnableDHCP();

private:
	friend class HardSocket;
	friend class TcpClient;
	friend class UdpClient;

	Spi*		_spi;
    InputPort	Irq;
	OutputPort	Rst;

	// 8��Ӳ��socket
	List<Socket*>	Sockets;

	// spi ģʽ��Ĭ�ϱ䳤��
	ushort		PhaseOM;

	typedef IPAddress (*DnsHandler)(NetworkInterface* host, const String& domain);
	DnsHandler	_Dns;	// ��������ΪIP��ַ
	void*	_Dhcp;

	bool WriteByte(ushort addr, byte dat, byte socket = 0 ,byte block = 0);
	bool WriteByte2(ushort addr, ushort dat, byte socket = 0 ,byte block = 0);
	byte ReadByte(ushort addr, byte socket = 0 ,byte block = 0);
	ushort ReadByte2(ushort addr, byte socket = 0 ,byte block = 0);

	void SetAddress(ushort addr, byte rw, byte socket = 0 ,byte block = 0);

	// ����ر�
	virtual bool OnOpen();
	virtual void OnClose();
	// �������
	virtual bool OnLink(uint retry);

	// �жϽŻص�
	void OnIRQ(InputPort& port, bool down);
	void OnIRQ();
};

// W5500����
class W5500Config
{
public:
	SPI Spi;
	Pin	Irq;
	Pin Reset;
};

#endif
