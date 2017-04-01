#ifndef __Esp8266_H__
#define __Esp8266_H__

#include "Device\Port.h"

#include "Net\Socket.h"
#include "Net\NetworkInterface.h"
#include "Net\ITransport.h"

#include "Message\DataStore.h"
#include "Message\Pair.h"

#include "App\AT.h"

// ��ô� Soket ǰ ��ע���жϣ�����ATָ�����뵽�ж�����ȥ  Ȼ����Ϣ���Գ�
// ���ſ� ESP8266  ģ��̼��汾 v1.3.0.2
class Esp8266 : public WiFiInterface
{
public:
	AT		At;		// AT��������

	NetworkType	WorkMode;	// ����ģʽ

	IDataPort*	Led;	// ָʾ��

	OutputPort	_Power;	// ��Դ
	OutputPort	_Reset;	// ����
	OutputPort	_LowPower;	// �͹���

	// ���Ӳ��socket
	void*	Sockets[5];

	Esp8266();
	virtual ~Esp8266();

	void Init(ITransport* port);
	void Init(COM idx, int baudrate = 115200);
	void Set(Pin power = P0, Pin rst = P0, Pin low = P0);

	virtual bool Config();
	void SetLed(Pin led);
	void SetLed(OutputPort& led);
	void RemoveLed();

	//virtual const String ToString() const { return String("Esp8266"); }
	virtual Socket* CreateSocket(NetType type);
	// ����DNS
	virtual bool EnableDNS();
	// ����DHCP
	virtual bool EnableDHCP();

	/******************************** ����ATָ�� ********************************/
	bool Test(int times = 10, int interval = 500);
	bool Reset(bool soft);
	String GetVersion();
	bool Sleep(uint ms);
	bool Echo(bool open);
	// �ָ��������ã�����д���б��浽Flash�Ĳ������ָ�ΪĬ�ϲ������ᵼ��ģ������
	bool Restore();

	/******************************** WiFi����ָ�� ********************************/
		// ��ȡģʽ
	NetworkType GetMode();
	// ����ģʽ����Ҫ����
	bool SetMode(NetworkType mode);

	// ����AP���
	String GetJoinAP();
	bool JoinAP(const String& ssid, const String& pass);
	bool UnJoinAP();
	bool SetAutoConn(bool enable);

	String LoadAPs();
	String GetAP();
	bool SetAP(const String& ssid, const String& pass, byte channel, byte ecn = 0, byte maxConnect = 4, bool hidden = false);
	// ��ѯ���ӵ�AP��Stations��Ϣ���޷���ѯDHCP����
	String LoadStations();

	bool GetDHCP(bool* sta, bool* ap);
	bool SetDHCP(NetworkType mode, bool enable);

	MacAddress GetMAC(bool sta);
	bool SetMAC(bool sta, const MacAddress& mac);

	IPAddress GetIP(bool sta);

	/******************************** TCP/IP ********************************/
	String GetStatus();
	bool GetMux();
	bool SetMux(bool enable);

	bool Update();

	bool Ping(const IPAddress& ip);

	bool SetIPD(bool enable);

	/******************************** ����ָ�� ********************************/
		// ���������������롣ƥ������Э��
	bool SetWiFi(const Pair& args, Stream& result);
	// ��ȡ�������ơ�
	bool GetWiFi(const Pair& args, Stream& result);
	// ��ȡ����APs
	String* APs;
	void GetAPsTask();
	bool GetAPs(const Pair& args, Stream& result);

private:
	uint		_task;		// ��������
	ByteArray	_Buffer;	// ���������ݰ�
	IPEndPoint	_Remote;	// ��ǰ���ݰ�Զ�̵�ַ

	// ����ر�
	virtual bool OnOpen();
	virtual void OnClose();
	// �������
	virtual bool OnLink(uint retry);

	bool CheckReady();
	void OpenAP();

	// �����յ������ݰ�
	void Process();

	// ���ݵ���
	void OnReceive(Buffer& bs);
};

// Esp8266����
class Esp8266Config
{
public:
	COM Com;
	int Baudrate;
	Pin	Power;
	Pin Reset;
	Pin	LowPower;
};

#endif
