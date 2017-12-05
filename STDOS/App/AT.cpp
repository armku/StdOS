#include "Net\ITransport.h"
#include "Device\SerialPort.h"
#include "AT.h"

AT::AT()
{
	
}
AT::~AT()
{
	
}

// 发送命令，自动检测并加上\r\n，等待响应OK
bool AT::SendCmd(const String& cmd, uint msTimeout)
{
	auto p=(SerialPort*)this->Port;
	if(this->Port)
	{
		p->Write(cmd);
		p->Printf("\r\n");
	}
}
