#include "Net\ITransport.h"
#include "Device\SerialPort.h"
#include "AT.h"

AT::AT()
{
	
}
AT::~AT()
{
	
}

// ��������Զ���Ⲣ����\r\n���ȴ���ӦOK
bool AT::SendCmd(const String& cmd, uint msTimeout)
{
	auto p=(SerialPort*)this->Port;
	if(this->Port)
	{
		p->Write(cmd);
		p->Printf("\r\n");
	}
}
