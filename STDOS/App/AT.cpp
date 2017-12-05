#include  "Sys.h"
#include "Net\ITransport.h"
#include "Device\SerialPort.h"
#include "AT.h"

static float latitude;
static float longitude;


AT::AT()
{
	latitude = 0;
	longitude = 0;
}
AT::~AT()
{
	
}
float AT::GetLatitude()
{
	return latitude;
}
float AT::GetLongitude()
{
	return longitude;
}
// 打开与关闭
bool AT::Open()
{	
	this->Port->Register(AT::OnPortReceive);
	this->Port->Open();
}
void AT::Close()
{
	
}
String AT::Send(const String& cmd, cstring expect, cstring expect2, uint msTimeout, bool trim)
{
	auto p=(SerialPort*)this->Port;
	if(this->Port)
	{
		p->Write(cmd);
		if(!cmd.EndsWith("\r\n"))
			p->Printf("\r\n");
		cmd.Show();
	}
	Sys.Sleep(msTimeout);
}
String AT::Send(const String& cmd, uint msTimeout, bool trim)
{
	return this->Send(cmd,"OK","ERROR",msTimeout,trim);;
}

// 发送命令，自动检测并加上\r\n，等待响应OK
bool AT::SendCmd(const String& cmd, uint msTimeout)
{	
	auto rstr=this->Send(cmd,msTimeout);
	return rstr.Contains("OK");
}
// 引发数据到达事件
uint AT::OnReceive(Buffer& bs, void* param)
{
	return 1;
}
uint AT::OnPortReceive(ITransport* sender, Buffer& bs, void* param, void* param2)
{
	bs.Show();
	//return this->OnReceive(bs, sender, param);
}
