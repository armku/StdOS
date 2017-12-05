#include  "Sys.h"
#include "Net\ITransport.h"
#include "Device\SerialPort.h"
#include "AT.h"


AT::AT()
{
	
}
AT::~AT()
{
	
}
String AT::Send(const String& cmd, uint msTimeout, bool trim)
{
	auto p=(SerialPort*)this->Port;
	if(this->Port)
	{
		p->Write(cmd);
		p->Printf("\r\n");
	}
	Sys.Sleep(msTimeout);
	return NULL;
}

// ��������Զ���Ⲣ����\r\n���ȴ���ӦOK
bool AT::SendCmd(const String& cmd, uint msTimeout)
{	
	auto rstr=this->Send(cmd,msTimeout);
	return rstr.Contains("OK");
}
