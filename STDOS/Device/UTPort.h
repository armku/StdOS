#ifndef __UTPort_H__
#define __UTPort_H__

#include "Kernel\Sys.h"
#include "Device\SerialPort.h"
#include "Message/Pair.h"

enum  PacketEorrCode : byte
{
	Good = 1,
	CmdError = 2,	// �������
	NoPort = 3,		// û������˿�
	NotOpen = 4,	// û�д�
	CfgError = 5,	// ������Ϣ����
	Error = 6,		// δ֪����
};

enum PacketFlag : byte
{
	Save = 1,		// ����
	CycleDo = 1,	// ��ʱִ��
};

typedef union 
{
	PacketEorrCode  ErrorCode;
	PacketFlag		Flag;
}PacketUnion;


typedef struct
{
	byte PortID;	// �˿ں�
	byte Seq;		// paket���к� �ظ���Ϣ��ʱ��ԭ�����ز���У�� ÿ���˿ڸ�����һ��
	byte Type;		// ����ID/��������
	PacketUnion Error;	// ������뷵��ʱ��ʹ�ã��ն��룩  ���ݰ���ʶ�ƶ��·�������ʹ��  ƽʱΪ0
	ushort Length;	// ���ݳ���
	void * Next()const { return (void*)(&Length + Length + sizeof(Length)); };
}PacketHead;


// unvarnished transmission ͸���˿ڻ���
class UTPort
{
public:
	UTPort();
	byte	Seq;		// SEQ ֵ ���ڱ��ϰ����Ⱥ�˳��255 Ӧ���㹻
	String * Name;		// ���������
	virtual  void DoFunc(Buffer & packet, MemoryStream & ret) = 0;		// packet Ϊ�������retΪ����ֵ��
private:
};

// �ŵ������ط�ȥ   ��Ҫ���ڴ˴���
class UTCom : public UTPort
{
public:
	UTCom();
	byte State;		// 0 Port = null   1�ж˿�    2�ѳ�ʼ��   3��Open
	SerialPort * Port;
	virtual void DoFunc(Buffer & packet, MemoryStream & ret);

	void CreatePort() { Port = new SerialPort(); State = 1; };
	void DelPort() { if (Port)delete Port; State = 0; };

	bool ComConfig(Pair & data);
};


#endif
