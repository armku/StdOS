#ifndef __GSM07_H__
#define __GSM07_H__

#include "Device\Port.h"

#include "App\AT.h"

#include "Net\Socket.h"
#include "Net\NetworkInterface.h"
#include "Net\ITransport.h"

#include "Message\DataStore.h"
#include "Message\Pair.h"

// GPRS��ATָ� GSM 07.07
class GSM07 : public NetworkInterface
{
public:
	AT		At;		// AT��������
	cstring	APN;
	bool	Mux;	// ������·socketģʽ�����ͬʱ����4·

	IDataPort*	Led;	// ָʾ��

	OutputPort	_Power;	// ��Դ
	OutputPort	_Reset;	// ����
	OutputPort	_LowPower;	// �͹���

	// ���Ӳ��socket
	void*	Sockets[5];

	Delegate<Buffer&>	Received;

	//ushort	Country;	// ����MCC 3λ
	//ushort	Network;	// ����MNC 2λ
	uint	Network;	// ����MCC + ����MNC 3λ+2λ
	ushort	Area;		// ��վ����
	ushort	CellID;		// ��վ����

	GSM07();
	virtual ~GSM07();

	void Init(ITransport* port);
	void Init(COM idx, int baudrate = 115200);
	void Set(Pin power = P0, Pin rst = P0, Pin low = P0);

	virtual bool Config();
	void SetLed(Pin led);
	void SetLed(OutputPort& led);
	void RemoveLed();

	virtual Socket* CreateSocket(NetType type);

	/******************************** ����ָ�� ********************************/
	bool Test(int times = 10, int interval = 500);
	bool Reset(bool soft);
	String GetVersion();
	bool Sleep(uint ms);
	bool Echo(bool open);
	// �ָ��������ã�����д���б��浽Flash�Ĳ������ָ�ΪĬ�ϲ������ᵼ��ģ������
	bool Restore();
	String GetIMSI();
	String GetIMEI();
	// ��ѯSIM��CCID��Ҳ�������ڲ�ѯSIM�Ƿ����߲��
	String GetCCID();

	/******************************** ������� ********************************/
	// ��ȡ��Ӫ���б�
	String GetMobiles();
	// ��ȡ��ǰ��Ӫ��
	String GetMobile();
	// ��������Ƿ�ע��
	bool QueryRegister();
	String QuerySignal();

	bool AttachMT(bool enable);
	bool SetAPN(cstring apn, bool issgp = false);
	bool SetPDP(bool enable);
	IPAddress GetIP();
	bool SetClass(cstring mode);

	/******************************** TCP/IP ********************************/
	int IPStart(const NetUri& remote);
	bool IPSend(int index, const Buffer& data);
	bool SendData(const String& cmd, const Buffer& bs);
	bool IPClose(int index);
	bool IPShutdown(int index);
	String IPStatus();
	bool SetAutoSendTimer(bool enable, ushort time);
	IPAddress DNSGetIP(const String& domain);
	bool IPMux(bool enable);
	bool IPHeartConfig(int index, int mode, int value);
	bool IPHeart(int index, bool enable);

	// ͸������
	bool IPTransparentConfig(int mode, int value);
	bool IPTransparent(bool enable);

private:
	IPEndPoint	_Remote;	// ��ǰ���ݰ�Զ�̵�ַ

	// ����ر�
	virtual bool OnOpen();
	virtual void OnClose();
	// �������
	virtual bool OnLink(uint retry);

	bool CheckReady();

	// ���ݵ���
	void OnReceive(Buffer& bs);
};

class GSMSocket : public ITransport, public Socket
{
protected:
	GSM07&	_Host;
	byte	_Index;
	int		_Error;

public:
	GSMSocket(GSM07& host, NetType protocol, byte idx);
	virtual ~GSMSocket();

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
};

class GSMTcp : public GSMSocket
{
public:
	GSMTcp(GSM07& host, byte idx);

	virtual String& ToStr(String& str) const { return str + "Tcp_" + Local.Port; }
};

class GSMUdp : public GSMSocket
{
public:
	GSMUdp(GSM07& host, byte idx);

	virtual bool SendTo(const Buffer& bs, const IPEndPoint& remote);

	virtual String& ToStr(String& str) const { return str + "Udp_" + Local.Port; }

private:
	virtual bool OnWriteEx(const Buffer& bs, const void* opt);
};

#endif
