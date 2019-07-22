#include "HMI.h"
#include "Device\SerialPort.h"
#include "Kernel\Sys.h"

#define NET_DEBUG DEBUG
//#define NET_DEBUG 0
#if NET_DEBUG
#define net_printf debug_printf
#else
#define net_printf(format, ...)
#endif

/******************************** HMI ********************************/
HMI::HMI()
{
	Port = nullptr;
}

HMI::~HMI()
{
	delete Port;
}

// ��ʼ��HMI����
void HMI::Init(COM idx, int baudrate)
{
	auto srp = new SerialPort(idx, baudrate);
	srp->Tx.SetCapacity(0x200);
	srp->Rx.SetCapacity(0x200);
	srp->MaxSize = 512;

	Init(srp);
}

// ע��HMI���ڵ����ݵ���
void HMI::Init(ITransport* port)
{
	Port = port;
	if (Port) Port->Register(OnPortReceive, this);
}

// ���ݵ���
uint HMI::OnPortReceive(ITransport* sender, Buffer& bs, void* param, void* param2)
{
	auto esp = (HMI*)param;
	return esp->OnReceive(bs, param2);
}

// �����յ�������
uint HMI::OnReceive(Buffer& bs, void* param)
{
	if (bs.Length() == 0) return 0;
	debug_printf("HMI::�յ���");
	bs.AsString().Show(true);
	return 0;
}

// ��HMI��������ָ��
void HMI::Send(const String& cmd)
{
	Port->Write(cmd);
	SenDFlag();
}

// ��HMI����ָ�������
void HMI::SenDFlag()
{
	byte by[3] = { 0xFF,0xFF,0xFF };
	Buffer bs(by, 3);
	Port->Write(bs);
}

