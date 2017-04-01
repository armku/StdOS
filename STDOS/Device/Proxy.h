#ifndef __Proxy_H__
#define __Proxy_H__

#include "Kernel\Sys.h"
#include "Device\SerialPort.h"
#include "ProxyConfig.h"

class Proxy
{
public:
	cstring Name;			// �˿���
	MemoryStream* Cache;	// ����ռ�
	uint	UploadTaskId;	// �ϴ������ID
	uint	AutoTaskId;		// �Զ�����ID�������Ƕ�ʱWrite����
	int		TimeStamp;		// ʱ���

	int		CacheSize;		// �������ݰ�����
	int		BufferSize;		// ��������С
	bool	EnableStamp;	// ʱ�������
	bool	AutoStart;		// �Զ�����

	Proxy();
	bool Open();
	bool Close();

	bool SetConfig(Dictionary<cstring, int>& config, String& str);
	bool GetConfig(Dictionary<cstring, int>& config);
	virtual int	 Write(Buffer& data) = 0;
	virtual int  Read(Buffer& data, Buffer& input) = 0;
	void UploadTask();
	bool Upload(Buffer& data);

	void AutoTask();				// �Զ���������
	bool LoadConfig();				// ����������������
	void SaveConfig();				// ����������Ϣ

private:
	virtual bool OnOpen() = 0;
	virtual bool OnClose() = 0;
	virtual bool OnAutoTask() { return true; };	

	virtual bool OnSetConfig(Dictionary<cstring, int>& config, String& str) = 0;
	virtual bool OnGetConfig(Dictionary<cstring, int>& config) = 0;

	virtual bool OnGetConfig(Stream& cfg) { return true; };
	virtual bool OnSetConfig(Stream& cfg) { return true; };
};

class ComProxy : public Proxy
{
public:
	ComProxy(COM con);

	SerialPort port;

	byte	parity;
	byte	dataBits;
	byte	stopBits;
	int		baudRate;

	virtual bool OnSetConfig(Dictionary<cstring, int>& config, String& str) override;
	virtual bool OnGetConfig(Dictionary<cstring, int>& config) override;

	virtual int Write(Buffer& data) override;
	virtual int Read(Buffer& data, Buffer& input) override;

private:
	virtual bool OnOpen() override;
	virtual bool OnClose() override;

	virtual bool OnGetConfig(Stream& cfg) override;
	virtual bool OnSetConfig(Stream& cfg) override;

	static uint Dispatch(ITransport* port, Buffer& bs, void* param, void* param2);
};

#endif
