﻿#include "Kernel\Sys.h"
#include "Kernel\Task.h"
#include "Kernel\TTime.h"
#include "Kernel\WaitHandle.h"

#include "Device\SerialPort.h"

#include "A67.h"

#include "Config.h"

#include "App\FlushPort.h"

#define NET_DEBUG DEBUG
//#define NET_DEBUG 0
#if NET_DEBUG
#define net_printf debug_printf
#else
#define net_printf(format, ...)
#endif

A67::A67() :GSM07() {
	At.DataKey = "+CIPRCV:";
}

// 数据到达
void A67::OnReceive(Buffer& bs)
{
	// +CIPRCV:61,xxx
	auto str = bs.AsString();
	int p = str.IndexOf(",");
	if (p < 0) p = str.IndexOf(":");
	if (p <= 0) return;

	int len = str.Substring(0, p).ToInt();
	// 检查长度
	if (p + 1 + len > bs.Length()) len = bs.Length() - p - 1;
	auto data = bs.Sub(p + 1, len);

	OnProcess(0, data, _Remote);
}

/******************************** 扩展指令 ********************************/
bool A67::GetGPS()
{
	return At.SendCmd("AT+GPS");
}

bool A67::SetGPS(bool enable, int rate)
{
	String cmd = "AT+GPS=";
	cmd = cmd + (enable ? "1" : "0");

	if (!At.SendCmd(cmd)) return false;

	if (rate == 0) return true;

	cmd = "AT+GPSRD=";
	cmd += rate;
	return At.SendCmd(cmd);
}

bool A67::GetAGPS()
{
	return At.SendCmd("AT+AGPS");
}


bool A67::SetAGPS(bool enable, int rate)
{
	String cmd = "AT+AGPS=";
	cmd = cmd + (enable ? "1" : "0");

	if (!At.SendCmd(cmd)) return false;

	if (rate == 0) return true;

	cmd = "AT+GPSRD=";
	cmd += rate;
	return At.SendCmd(cmd);
}

bool A67::CameraStart(int mode)
{
	String cmd = "AT+CAMSTART=";
	cmd += mode;

	return At.SendCmd(cmd);
}

bool A67::CameraStop() { return At.SendCmd("AT+CAMSTOP"); }
bool A67::CameraCapture() { return At.SendCmd("AT+CAMCAP"); }

String A67::CameraRead(int from, int to)
{
	String cmd = "AT+CAMRD=";
	cmd += from;

	if (to > 0) cmd = cmd + "," + to;
	cmd += "\r\n";

	return At.Send(cmd);
}

// "192.168.1.111/A6C/123.jpg",80
bool A67::CameraPost(const String& url, int port)
{
	String cmd = "AT+CAMPOST=";
	cmd = cmd + "\"" + url + "\"," + port;

	return At.SendCmd(cmd);
}

String A67::HttpGet(const String& url, int port)
{
	String cmd = "AT+HTTPGET=";
	cmd = cmd + url + "," + port;
	cmd += "\r\n";

	return At.Send(cmd, "");
}

bool A67::HttpPost(const String& url, int port, const Buffer& data)
{
	String cmd = "AT+HTTPPOST=";
	cmd = cmd + url + "," + port;

	return SendData(cmd, data);
}
