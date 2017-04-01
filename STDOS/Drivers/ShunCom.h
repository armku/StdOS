#ifndef __ShunCom_H__
#define __ShunCom_H__

#include "Kernel\Sys.h"
#include "Device\Port.h"
#include "Device\Power.h"
#include "Net\ITransport.h"
#include "Message\DataStore.h"

// �Ϻ�˳��ZigbeeЭ��
// ����ģʽʱ����վ��ָ����վ��ַ��㲥����վ����ֻ����վ�յ�
class ShunCom : public PackPort, public Power
{
public:
	OutputPort	Reset;	// ��λ
	IDataPort*	Led;	// ָʾ��

	OutputPort	Power;	// ��Դ
	OutputPort	Sleep;	// ˯��
	OutputPort	Config;	// ����

	byte	AddrLength;	// ��ַ���ȡ�Ĭ��0����վ������Ϊ2

	ShunCom();

	void Init(ITransport* port, Pin rst = P0);

	//virtual const String ToString() const { return String("ShunCom"); }

	// ��������ģʽ
	bool EnterConfig();
	// �˳�����ģʽ
	void ExitConfig();
	// ��ȡ������Ϣ
	void ShowConfig();
	void EraConfig();
	
	void PrintSrc(bool flag);
	// �����豸�����ͣ�00�������ġ�01����·�ɣ�02�����ն�
	void SetDevice(byte kind);
	// ��������Ƶ�㣬ע���С�ˣ�ShunCom��С�˴洢
	void SetChannel(byte chn);
	// ���÷���ģʽ00Ϊ�㲥��01Ϊ����ģʽ��02Ϊ��Ե�ģʽ
	void SetSend(byte mode);
	// ��������PanID,ͬһ����PanID������ͬ
    void SetPanID(ushort id);
	//��ԭzigbeeĬ������
	void ShunComReset();

public:
	// Open ִ�е��ⲿ���ú���
	Action	ExternalCfg;
private:
	virtual bool OnOpen();
    virtual void OnClose();

	// ��Դ�ȼ�����������͹���ģʽ��ʱ����
	virtual void ChangePower(int level);

	// �������ݵ����¼�
	virtual uint OnReceive(Buffer& bs, void* param);

	virtual bool OnWriteEx(const Buffer& bs, const void* opt);
};
#endif
