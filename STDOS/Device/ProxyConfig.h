#ifndef __ProxyCfg_H__
#define __ProxyCfg_H__

#include "Kernel\Sys.h"
#include "Config.h"

class ProxyConfig : public ConfigBase
{
public:
	int Length;
	byte AutoStart;			// �Զ�����
	byte EnableStamp;		// ʱ�������
	int		CacheSize;		// �������ݰ�����
	int		BufferSize;		// ��������С

	byte PortCfg[64];		// �����Զ�������
	// byte FixedCmd[128];		// ��ʱ���������+����

	byte TagEnd;

	ProxyConfig(cstring name = nullptr);
	virtual void Init();
};


#endif
