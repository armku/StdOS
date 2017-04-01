#ifndef _NetworkInterface_H_
#define _NetworkInterface_H_

#include "IPAddress.h"
#include "IPEndPoint.h"
#include "MacAddress.h"
#include "NetUri.h"
#include "Socket.h"

#include "Core\List.h"
#include "Core\Delegate.h"

class Socket;

// ��������
enum class NetworkType
{
	Wire	= 0,
	Station	= 1,
	AP		= 2,
	STA_AP	= 3,
};

// ����״̬
enum class NetworkStatus
{
	None	= 0,	// δ֪
	Up		= 1,	// ����
	Down	= 2,	// ����
};

// ����ӿ�
class NetworkInterface
{
public:
	cstring		Name;	// ����
	ushort		Speed;	// �ٶ�Mbps���������ȼ�
	bool		Opened;	// �ӿ����ϵ�򿪣���⵽��������
	bool		Linked;	// �ӿ��������������ͨ��
	NetworkType	Mode;	// ����ģʽ��0�������ߣ�1��STA��2��AP��3�ǻ��
	NetworkStatus	Status;	// ����״̬

	IPAddress	IP;		// ����IP��ַ
    IPAddress	Mask;	// ��������
	MacAddress	Mac;	// ����Mac��ַ

	IPAddress	Gateway;
	IPAddress	DNSServer;
	IPAddress	DNSServer2;

	Delegate<NetworkInterface&>	Changed;	// ����ı�

	NetworkInterface();
	// ������������������ΪӦ�ò����Ҫ�ͷŸýӿ�
	virtual ~NetworkInterface();

	// ����ر�
	bool Open();
	void Close();
	bool Active() const;

	// Ӧ������
	virtual bool Config() = 0;

	// ����ͼ�����������
	void InitConfig();
	bool LoadConfig();
	bool SaveConfig() const;
	void ShowConfig() const;

	virtual Socket* CreateSocket(NetType type) = 0;
	Socket* CreateClient(const NetUri& uri);
	Socket* CreateRemote(const NetUri& uri);

	// DNS������Ĭ�Ͻ�֧���ַ���IP��ַ����
	virtual IPAddress QueryDNS(const String& domain);
	// ����DNS
	virtual bool EnableDNS() { return false; }
	// ����DHCP
	virtual bool EnableDHCP() { return false; }

protected:
	uint	_taskLink;
	uint	_retry;

	// ����ر�
	virtual bool OnOpen()	= 0;
	virtual void OnClose()	= 0;

	// ѭ���������
	virtual void OnLoop();
	virtual bool OnLink(uint retry) { return true; }
	virtual bool CheckLink() { return Linked; }

	// ����ͼ�����������
	virtual void OnInitConfig() {}
	virtual void OnLoadConfig(const Buffer& buf) {}
	virtual void OnSaveConfig(Buffer& buf) const {}
	virtual void OnShowConfig() const {}

public:
	// ȫ�־�̬
	static List<NetworkInterface*> All;
};

/****************************** WiFiInterface ************************************/

// WiFi�ӿ�
class WiFiInterface : public NetworkInterface
{
public:
	String*		SSID;	// ����SSID
	String*		Pass;	// ��������

	WiFiInterface();
	virtual ~WiFiInterface();

	bool IsStation() const;
	bool IsAP() const;

	// WiFi��������·�����Ϳ��Խ������ؼ���
	Socket* CreateClient(const NetUri& uri);

protected:
	// ����ͼ�����������
	virtual void OnInitConfig();
	virtual void OnLoadConfig(const Buffer& buf);
	virtual void OnSaveConfig(Buffer& buf) const;
	virtual void OnShowConfig() const;
};

#endif
