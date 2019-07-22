﻿#include "Kernel\TTime.h"
#include "Kernel\WaitHandle.h"

#include "Net\Socket.h"
#include "Net\NetworkInterface.h"

#include "Message\BinaryPair.h"

#include "TokenClient.h"
#include "TokenSession.h"

#include "TokenMessage.h"
#include "HelloMessage.h"
#include "LoginMessage.h"
#include "RegisterMessage.h"
#include "TokenDataMessage.h"
#include "TokenPingMessage.h"
#include "ErrorMessage.h"

#include "Security\RC4.h"

TokenClient* TokenClient::Current = nullptr;

//static void BroadcastHelloTask(void* param);

TokenClient::TokenClient()
	: Routes(String::Compare)
{
	Token = 0;

	Opened = false;
	Status = 0;

	LoginTime = 0;
	LastSend = 0;
	LastActive = 0;
	Delay = 0;
	MaxNotActive = 0;

	Master = nullptr;
	Cfg = nullptr;

	Received = nullptr;
	Param = nullptr;

	NextReport = -1;
	ReportLength = 0;

	_Expect = nullptr;

	assert(!Current, "只能有一个令牌客户端实例");
	Current = this;
}

void TokenClient::Open()
{
	if (Opened) return;

	TS("TokenClient::Open");

	// 令牌客户端定时任务
	_task = Sys.AddTask(&TokenClient::LoopTask, this, 0, 1000, "令牌客户");

	Cookie = Cfg->Token();

	// 启动时记为最后一次活跃接收
	LastActive = Sys.Ms();

	Opened = true;
}

void TokenClient::Close()
{
	if (!Opened) return;

	Sys.RemoveTask(_task);
	//Sys.RemoveTask(_taskBroadcast);

	if (Master)
	{
		delete Master;
		Master = nullptr;
	}

	auto& cs = Controls;
	for (int i = 0; i < cs.Count(); i++)
	{
		delete cs[i];
	}
	cs.Clear();

	Opened = false;
}

static TokenController* AddMaster(TokenClient& client)
{
	auto uri = client.Cfg->Uri();
	// 创建连接服务器的Socket
	auto socket = Socket::CreateRemote(uri);
	if (!socket) return nullptr;

	// 创建连接服务器的控制器
	auto ctrl = new TokenController();
	ctrl->_Socket = socket;

	// 创建客户端
	client.Master = ctrl;

	return ctrl;
}

static TokenController* AddControl(TokenClient& client, NetworkInterface* host, const NetUri& uri, ushort remotePort)
{
	// 创建连接服务器的Socket
	auto socket = host ? host->CreateClient(uri) : Socket::CreateClient(uri);
	if (!socket) return nullptr;

	// 创建连接服务器的控制器
	auto ctrl = new TokenController();
	ctrl->_Socket = socket;

	// 创建客户端
	socket->Remote.Address = IPAddress::Broadcast();
	socket->Remote.Port = remotePort;
	ctrl->ShowRemote = true;
	client.Controls.Add(ctrl);

	return ctrl;
}

void TokenClient::CheckNet()
{
	auto mst = Master;
	auto& cs = Controls;
	//assert(cs.Count() > 0, "令牌客户端还没设置控制器呢");

	// 现在是否已连接
	bool linked = true;

	// 检测主链接
	if (!mst)
	{
		linked = false;
		auto ctrl = AddMaster(*this);
		if (!ctrl) return;

		debug_printf("TokenClient::CheckNet %s 成功创建主连接\r\n", ctrl->_Socket->Host->Name);
	}
	// 检测主链接是否已经断开
	else if (!mst->_Socket->Host->Linked)
	{
		linked = false;
		debug_printf("TokenClient::CheckNet %s断开，切换主连接\r\n", mst->_Socket->Host->Name);

		delete mst;
		Master = nullptr;

		Status = 0;
		Token = 0;

		// 未连接时，加快网络检查速度
		Sys.SetTaskPeriod(_task, 1000);

		auto ctrl = AddMaster(*this);
		if (!ctrl) return;
	}

	// 从未连接转为已连接
	mst = Master;
	if (!linked && mst)
	{
		Delegate2<TokenMessage&, TokenController&> dlg(&TokenClient::OnReceive, this);
		mst->Received = dlg;
		mst->Open();

		//Cfg->ServerIP = mst->_Socket->Remote.Address.Value;
	}

	// 为其它网卡创建本地会话
	// 启动本地控制器
	if (_LocalReceive)
	{
		auto& nis = NetworkInterface::All;
		for (int k = 0; k < nis.Count(); k++)
		{
			if (!nis[k]->Active()) continue;

			// 检测该接口上是否创建了控制器
			bool flag = false;
			for (int i = 0; i < cs.Count(); i++)
			{
				if (cs[i]->_Socket->Host == nis[k])
				{
					flag = true;
					break;
				}
			}
			// 在该接口上创建控制器
			if (!flag)
			{
				debug_printf("TokenClient::CheckNet %s 创建本地监听 ", nis[k]->Name);

				NetUri uri(NetType::Udp, IPAddress::Broadcast(), Cfg->Port);
				auto ctrl = AddControl(*this, nis[k], uri, Cfg->Port);
				if (ctrl)
				{
					ctrl->Received = _LocalReceive;
					ctrl->Open();

					debug_printf("成功\r\n");
				}
				else
				{
					debug_printf("失败\r\n");
				}
			}
		}
		// 令牌广播使用素数，避免跟别的任务重叠
		//if (cs.Count() > 0 && _taskBroadcast == 0) _taskBroadcast = Sys.AddTask(BroadcastHelloTask, this, 7000, 37000, "令牌广播");
	}
	else if (cs.Count() > 0)
	{
		debug_printf("设置了本地令牌控制器，但是没有启用本地功能，你可能需要client->UserLocal()\r\n");
	}
}

// 启用内网功能。必须显式调用，否则内网功能不参与编译链接，以减少大小
void TokenClient::UseLocal()
{
	_LocalReceive = Delegate2<TokenMessage&, TokenController&>(&TokenClient::OnReceiveLocal, this);
}

bool TokenClient::Send(TokenMessage& msg, TokenController* ctrl)
{
	// 未登录之前，只能 握手、登录、注册
	if (Token == 0)
	{
		if (msg.Code != 0x01 && msg.Code != 0x02 && msg.Code != 0x07) return false;
	}

	if (!ctrl) ctrl = Master;
	if (!ctrl) return false;

	assert(ctrl, "TokenClient::Send");

	// 最后发送仅统计主控制器
	if (ctrl == Master) LastSend = Sys.Ms();

	return ctrl->Send(msg);
}

bool TokenClient::Reply(TokenMessage& msg, TokenController* ctrl)
{
	// 未登录之前，只能 握手、登录、注册
	if (ctrl->Token == 0)
	{
		if (msg.Code != 0x01 && msg.Code != 0x02 && msg.Code != 0x07) return false;
	}

	if (!ctrl) ctrl = Master;
	if (!ctrl) return false;

	assert(ctrl, "TokenClient::Reply");

	if (ctrl == Master) LastSend = Sys.Ms();

	return ctrl->Reply(msg);
}

void TokenClient::OnReceive(TokenMessage& msg, TokenController& ctrl)
{
	TS("TokenClient::OnReceive");

	LastActive = Sys.Ms();

	switch (msg.Code)
	{
	case 0x01:	OnHello(msg, &ctrl);	break;
	case 0x02:	OnLogin(msg, &ctrl);	break;
	case 0x03:	OnPing(msg, &ctrl);		break;
	case 0x07:	OnRegister(msg, &ctrl);	break;
	case 0x08:	OnInvoke(msg, &ctrl);	break;
	}
	// 握手登录心跳消息不需要转发
	if (msg.Code <= 0x03) return;

	// 消息转发
	if (Received)
	{
		Received(&ctrl, msg, Param);
	}
	else
	{
		switch (msg.Code)
		{
		case 0x05: OnRead(msg, &ctrl); break;
		case 0x06: OnWrite(msg, &ctrl); break;
		default:
			break;
		}
	}
}

void TokenClient::OnReceiveLocal(TokenMessage& msg, TokenController& ctrl)
{
	TS("TokenClient::OnReceiveLocal");
	debug_printf("LocalRev ");

	// 找到会话，如果不存在则创建
	auto remote = (IPEndPoint*)msg.State;
	if (!remote)
	{
		debug_printf("无法取得消息来源地址，设计错误！\r\n");
		return;
	}
	// remote->Show(false);

	// 根据远程地址，从会话列表中找到会话。如果会话不存在，则新建会话
	TokenSession* ss = nullptr;

	for (int i = 0; i < Sessions.Count(); i++)
	{
		ss = (TokenSession*)Sessions[i];
		if (ss && ss->Remote == *remote)
		{
			debug_printf("ss[%d] ", i);
			break;
		}
		ss = nullptr;
	}
	if (!ss)
	{
		debug_printf(" new TokenSession\r\n");
		ss = new TokenSession(*this, ctrl);
		ss->Remote = *remote;
	}

	TokenSession::Show(Sessions);

	ss->OnReceive(msg);
}

// 内网分发
void TokenClient::LocalSend(int start, const Buffer& bs)
{
	auto& cs = Sessions;
	if (cs.Count() == 0) return;

	debug_printf("LocalSend\r\n");

	TokenDataMessage dm;
	dm.Start = start;
	dm.Data = bs;

	TokenMessage msg;
	msg.Code = 0x06;
	dm.WriteMessage(msg);

	// 不要再发给云端
	//Send(msg);

	// 主动上报发给服务器的同时，也发给内网已登录用户
	for (int i = 0; i < cs.Count(); i++)
	{
		auto ss = (TokenSession*)cs[i];

		if (ss && ss->Status >= 2)
		{
			debug_printf("ss[%d]  ", i);
			ss->Send(msg);
		}
	}
}

// 常用系统级消息

// 定时任务
void TokenClient::LoopTask()
{
	TS("TokenClient::LoopTask");

	// 检查超时会话。倒序，因为可能有删除
	auto& cs = Sessions;
	// 列表保存32个本地会话
	for (int i = cs.Count() - 1; i >= 0 && cs.Count() > 32; i--)
	{
		auto ss = (TokenSession*)cs[i];
		// 5分钟不活跃 或 1分钟未登录 销毁
		if (ss && ss->CheckExpired()) delete ss;
	}

	// 最大不活跃时间ms，超过该时间时重启系统
	// WiFi触摸开关建议5~10分钟，网关建议5分钟
	// MaxNotActive 为零便不考虑重启
	if (MaxNotActive != 0 && LastActive + MaxNotActive < Sys.Ms()) Sys.Reboot();

	auto flag = Master;
	//if(!Master)
	{
		CheckNet();
		if (!Master) return;

		if (!flag) Sys.SetTaskPeriod(_task, 5000);
	}

	// 状态。0准备、1握手完成、2登录后
	switch (Status)
	{
	case 0:
		SayHello(false);
		break;
	case 1:
	{
		if (!Cfg->User())
			Register();
		else
		{
			Login();

			//Sys.Sleep(1000);
			//// 登录成功以后做一次内网广播
			//if (Status >= 2) SayHello(true);

			// 登录成功后，心跳一次，把数据同步上去
			Sys.Sleep(1000);
			if (Status >= 2) Ping();
		}

		break;
	}
	case 2:
		Ping();
		break;
	}

	CheckReport();
}

/*void BroadcastHelloTask(void* param)
{
	TS("TokenClient::BroadcastHello");

	auto client = (TokenClient*)param;
	client->SayHello(true);
}*/

// 发送发现消息，告诉大家我在这
// 请求：2版本 + S类型 + S名称 + 8本地时间 + 6本地IP端口 + S支持加密算法列表
// 响应：2版本 + S类型 + S名称 + 8本地时间 + 6对方IP端口 + 1加密算法 + N密钥
// 错误：0xFE + 1协议 + S服务器 + 2端口
void TokenClient::SayHello(bool broadcast)
{
	TS("TokenClient::SayHello");

	TokenMessage msg(0x01);

	HelloMessage ext;
	ext.Reply = false;
	ext.LocalTime = DateTime::Now().TotalMs();

	auto& cs = Controls;
	// 取第二通道为本地通道
	if (cs.Count() > 0)
	{
		auto ctrl = cs[0];
		auto sock = ctrl->_Socket;
		//ext.EndPoint.Address = sock->Local.Address;
		ext.EndPoint = sock->Local;
		ext.Protocol = sock->Protocol == NetType::Udp ? 17 : 6;
	}

	ext.Cipher = "RC4";
	ext.Name = Cfg->User();
	// 未注册时采用系统名称
	if (!ext.Name)
	{
		ext.Name = Sys.Name;
		ext.Key = Buffer(Sys.ID, 16);
	}

	ext.WriteMessage(msg);
	ext.Show(true);

	// 特殊处理广播，指定广播地址，避免因为内网发现改变了本地端口
	if (broadcast)
	{
		msg.OneWay = true;

		//msg.State	= &ctrl->_Socket->Remote;
		for (int i = 0; i < cs.Count(); i++)
		{
			auto ctrl = cs[i];
			msg.State = &ctrl->_Socket->Remote;
			if (ctrl->Port != nullptr)
			{
				Send(msg, ctrl);
			}
		}
	}
	else
		Send(msg);
}

// 握手响应
bool TokenClient::OnHello(TokenMessage& msg, TokenController* ctrl)
{
	if (!msg.Reply) return false;

	// 解析数据
	HelloMessage ext;
	ext.Reply = msg.Reply > 0;

	ext.ReadMessage(msg);
	ext.Show(true);

	// 如果收到响应，并且来自来源服务器
	if (msg.Error)
	{
		if (OnRedirect(ext)) return false;

		TS("TokenClient::OnHello_Error");

		Status = 0;
		Token = 0;

		// 未握手错误，马上重新握手
		if (ext.ErrCode == 0x7F) Sys.SetTask(_task, true, 0);
	}
	else
	{
		TS("TokenClient::OnHello_Reply");
		//如果已经登陆还接收到握手响应，这不属于正常的握手响应（IP冲突会导致）
		if (Status == 2) return false;
		// 通讯密码
		if (ext.Key.Length() > 0)
		{
			if (ctrl) ctrl->Key.Copy(0, ext.Key, 0, ext.Key.Length());

			debug_printf("握手得到通信密码：");
			ext.Key.Show(true);
		}
		Status = 1;

		// 握手完成后马上注册或登录
		Sys.SetTask(_task, true, 0);

		// 同步本地时间
		if (ext.LocalTime > 0) ((TTime&)Time).SetTime(ext.LocalTime / 1000);
	}

	return true;
}

bool TokenClient::OnRedirect(HelloMessage& msg)
{
	TS("TokenClient::OnRedirect");

	if (!(msg.ErrCode == 0xFE || msg.ErrCode == 0xFD)) return false;

	Cookie = msg.Cookie;

	// 0xFE永久改变厂商地址
	if (msg.ErrCode == 0xFE)
	{
		auto cfg = Cfg;
		cfg->Show();

		cfg->Server() = msg.Uri.ToString();
		cfg->Token() = msg.Cookie;

		cfg->Save();
		cfg->Show();
	}

	ChangeIPEndPoint(msg.Uri);

	return true;
}

bool TokenClient::ChangeIPEndPoint(const NetUri& uri)
{
	TS("TokenClient::ChangeIPEndPoint");

	debug_printf("ChangeIPEndPoint ");

	uri.ToString().Show(true);

	auto ctrl = Master;
	auto socket = ctrl->_Socket;
	if (socket == nullptr) return false;

	// 为了能够处理Tcp/Udp切换，重新建立连接

	ctrl->Close();
	delete socket;

	socket = Socket::CreateRemote(uri);
	ctrl->_Socket = socket;
	ctrl->Open();

	/*ctrl->Port->Close();
	socket->Remote.Port = uri.Port;
	// 让驱动随机拿到 Port
	socket->Local.Port = 0;
	socket->Server = uri.Host;*/

	//Cfg->ServerIP = socket->Remote.Address.Value;
	// 马上开始重新握手
	Status = 0;
	Sys.SetTask(_task, true, 0);

	return true;
}

// 注册
void TokenClient::Register()
{
	TS("TokenClient::Register");

	debug_printf("TokenClient::Register\r\n");

	RegisterMessage reg;
	reg.User = Buffer(Sys.ID, 16).ToHex();

	// 原始密码作为注册密码
	reg.Pass = Cfg->Pass();

	auto now = Sys.Ms();
	reg.Salt = Buffer(&now, 8);

	reg.Show(true);

	TokenMessage msg(7);
	reg.WriteMessage(msg);
	Send(msg);
}

void TokenClient::OnRegister(TokenMessage& msg, TokenController* ctrl)
{
	if (!msg.Reply) return;

	TS("TokenClient::OnRegister");

	if (msg.Error)
	{
		ErrorMessage em;
		em.Read(msg);

		debug_printf("注册失败，错误码 0x%02X！", em.ErrorCode);
		em.ErrorContent.Show(true);

		return;
	}

	auto cfg = Cfg;

	RegisterMessage reg;
	reg.ReadMessage(msg);
	cfg->User() = reg.User;
	cfg->Pass() = reg.Pass;

	cfg->Show();
	cfg->Save();

	Status = 0;

	Sys.SetTask(_task, true, 0);
}

// 登录
void TokenClient::Login()
{
	TS("TokenClient::Login");

	LoginMessage login;

	auto cfg = Cfg;
	login.User = cfg->User();

	// 原始密码对盐值进行加密，得到登录密码
	// auto now = Sys.Ms();
	auto now = DateTime::Now().TotalMs();
	auto arr = Buffer(&now, 8);
	ByteArray bs;
	bs	= arr;
	// login.Salt = arr;
	RC4::Encrypt(arr, cfg->Pass());
	// 散列明文和密码连接在一起
	auto pass = bs.ToHex();
	pass += arr.ToHex();
	login.Pass = pass;

	login.Cookie = Cookie;

	TokenMessage msg(2);
	login.WriteMessage(msg);
	login.Show(true);

	Send(msg);
}

bool TokenClient::OnLogin(TokenMessage& msg, TokenController* ctrl)
{
	if (!msg.Reply) return false;

	TS("TokenClient::OnLogin");

	Stream ms(msg.Data, msg.Length);

	LoginMessage logMsg;
	logMsg.ReadMessage(msg);
	logMsg.Show(true);

	if (logMsg.Error)
	{
		// 登录失败，清空令牌
		byte result = logMsg.ErrorCode;

		// 未登录错误，马上重新登录
		if (result == 0xF7)
		{
			// 任何错误，重新握手
			Status = 1;
			Token = 0;
			Register();
			return false;
		}

		Token = 0;
		Status = 0;

		if (result == 0x7F) Sys.SetTask(_task, true, 0);
	}
	else
	{
		Status = 2;
		debug_printf("登录成功！ ");

		Token = logMsg.Token;

		if (ctrl) ctrl->Token = Token;

		debug_printf("令牌：0x%08X ", Token);
		if (logMsg.Key.Length())
		{
			if (ctrl) ctrl->Key = logMsg.Key;

			debug_printf("通信密码：");
			logMsg.Key.Show();
		}

		debug_printf("\r\n");

		// 登录成功后加大心跳间隔
		Sys.SetTaskPeriod(_task, 60000);
	}

	return true;
}

void TokenClient::Reset(const String& reason)
{
	auto time = DateTime::Now().TotalMs();

	MemoryStream ms;
	BinaryPair bp(ms);
	bp.Set("time", (UInt64)time);
	bp.Set("reason", reason);

	auto buf = Buffer(ms.GetBuffer(), ms.Position());

	Invoke("Gateway/Reset", buf);

	debug_printf("设备500ms后重置\r\n");

	Sys.Sleep(500);

	Config::Current->RemoveAll();
	Sys.Reboot();
}

void TokenClient::Reboot(const String& reason)
{
	auto time = DateTime::Now().TotalMs();

	MemoryStream ms;
	BinaryPair bp(ms);
	bp.Set("time", (UInt64)time);
	bp.Set("reason", reason);

	auto buf = Buffer(ms.GetBuffer(), ms.Position());

	Invoke("Gateway/Reboot", buf);
}

// Ping指令用于保持与对方的活动状态
void TokenClient::Ping()
{
	TS("TokenClient::Ping");

	if (LastActive > 0 && LastActive + 300000 < Sys.Ms())
	{
		// 30秒无法联系，服务端可能已经掉线，重启Hello任务
		debug_printf("300秒无法联系，服务端可能已经掉线，重新开始握手\r\n");

		Master->Key.SetLength(0);

		Status = 0;

		Sys.SetTaskPeriod(_task, 5000);

		return;
	}

	// 30秒内发过数据，不再发送心跳
	if (LastSend > 0 && LastSend + 60000 > Sys.Ms()) return;

	TokenPingMessage pm;
	pm.Data = &Store.Data;

	TokenMessage msg(3);
	pm.WriteMessage(msg);

	Send(msg);
}

bool TokenClient::OnPing(TokenMessage& msg, TokenController* ctrl)
{
	TS("TokenClient::OnPing");

	if (!msg.Reply) return false;

	TokenPingMessage pm;
	pm.ReadMessage(msg);

	int cost = (int)(DateTime::Now().TotalMs() - pm.LocalTime);

	if (Delay)
		Delay = (Delay + cost) / 2;
	else
		Delay = cost;

	debug_printf("心跳延迟 %dms / %dms \r\n", cost, Delay);

	// 同步本地时间
	if (pm.ServerTime > 1000) ((TTime&)Time).SetTime(pm.ServerTime / 1000);

	return true;
}

/******************************** 数据区 ********************************/

void TokenClient::Read(int start, int size)
{
	TokenDataMessage dm;
	dm.Start = start;
	dm.Size = size;

	TokenMessage msg;
	msg.Code = 0x05;
	dm.WriteMessage(msg);

	Send(msg);
}

void TokenClient::Write(int start, const Buffer& bs)
{
	TokenDataMessage dm;
	dm.Start = start;
	dm.Data = bs;

	TokenMessage msg;
	msg.Code = 0x06;
	dm.WriteMessage(msg);

	Send(msg);

	// 主动上报发给服务器的同时，也发给内网已登录用户
	auto& cs = Controls;
	for (int i = 0; i < cs.Count(); i++)
	{
		auto ss = (TokenSession*)cs[i];
		if (ss && ss->Status >= 2)
		{
			debug_printf("ss[%d]  ", i);
			ss->Send(msg);
		}
	}
}

void TokenClient::Write(int start, byte dat)
{
	Write(start, Buffer(&dat, 1));
}

// 异步上传并等待响应
int TokenClient::WriteAsync(int start, const Buffer& bs, int msTimeout)
{
	// 如果别的任务正在上传，此次写入失败
	if (_Expect) return 0;

	WaitHandle handle;
	handle.State = (void*)start;

	_Expect = &handle;
	Write(start, bs);
	handle.WaitOne(msTimeout);
	if (_Expect == &handle) _Expect = nullptr;

	// 可能失败
	if (!handle.Result) return 1;

	return (int)handle.State;
}

void TokenClient::ReportAsync(int start, uint length)
{
	if (start + (int)length > Store.Data.Length())
	{
		debug_printf("布置异步上报数据失败\r\n");
		debug_printf("sta:%d  len:%d  data.len:%d\r\n", start, length, Store.Data.Length());
		return;
	}

	NextReport = start;
	ReportLength = length;

	// 延迟上报，期间有其它上报任务到来将会覆盖
	Sys.SetTask(_task, true, 20);
}

bool TokenClient::CheckReport()
{
	TS("TokenClient::CheckReport");

	auto offset = NextReport;
	int len = ReportLength;

	if (offset < 0) return false;

	// 检查索引，否则数组越界
	auto& bs = Store.Data;
	if (bs.Length() >= offset + len)
	{
		if (len == 1)
			Write(offset, bs[offset]);
		else
			Write(offset, Buffer(&bs[offset], len));
	}
	NextReport = -1;

	return true;
}

/*
请求：起始 + 大小
响应：起始 + 数据
*/
void TokenClient::OnRead(const TokenMessage& msg, TokenController* ctrl)
{
	if (msg.Reply) return;
	if (msg.Length < 2) return;

	auto rs = msg.CreateReply();

	TokenDataMessage dm;
	dm.ReadMessage(msg);
	// dm.Show(true);

	bool rt = true;
	if (dm.Start < 64)
		rt = dm.ReadData(Store);
	else if (dm.Start < 128)
	{
		dm.Start -= 64;
		rt = dm.ReadData(Cfg->ToArray());
		dm.Start += 64;
	}

	if (!rt)
	{
		debug_printf("rt == false\r\n");
		rs.Error = true;
		auto ms = rs.ToStream();
		BinaryPair bp(ms);
		bp.Set("ErrorCode", (byte)0x01);
		rs.Length = ms.Position();
	}
	else
	{
		debug_printf("Read ");
		dm.Show(true);
		dm.WriteMessage(rs);
	}

	Reply(rs, ctrl);
}

/*
请求：1起始 + N数据
响应：1起始 + 1大小 + N数据
错误：错误码2 + 1起始 + 1大小
*/
void TokenClient::OnWrite(const TokenMessage& msg, TokenController* ctrl)
{
	if (msg.Length < 2) return;

	TokenDataMessage dm;
	dm.ReadMessage(msg);
	debug_printf("Write ");
	dm.Show(true);

	if (msg.Reply) {
		// 拦截给同步方法
		auto handle = (WaitHandle*)_Expect;
		if (handle) {
			auto start = (uint)handle->State;
			if (start == dm.Start) {
				// 设置事件，通知等待任务退出循环
				handle->State = (void*)dm.Size;
				handle->Set();
			}
		}

		return;
	}

	bool rt = true;
	if (dm.Start < 64)
	{
		rt = dm.WriteData(Store, true);

		// 读取响应里面，一次性把数据全部读取出来
		if (rt)
		{
			dm.Start = 0;
			dm.Size = Store.Data.Length();
			//dm.Size		= 0;
			dm.Data = Store.Data;
		}
	}
	else if (dm.Start < 128)
	{
		dm.Start -= 64;
		auto bs = Cfg->ToArray();
		rt = dm.WriteData(bs, true);
		dm.Start += 64;

		Cfg->Save();
	}

	auto rs = msg.CreateReply();
	if (!rt)
		rs.Error = true;
	else
		dm.WriteMessage(rs);

	Reply(rs, ctrl);

	if (dm.Start >= 64 && dm.Start < 128)
	{
		debug_printf("\r\n 配置区被修改，200ms后重启\r\n");
		Sys.Sleep(200);
		Sys.Reboot();
	}

	// 再解析一次，发给本地
	dm.ReadMessage(msg);
	LocalSend(dm.Start, dm.Data);
}

/******************************** 远程调用 ********************************/

void TokenClient::Invoke(const String& action, const Buffer& bs)
{
	TokenMessage msg(8);

	MemoryStream ms;
	BinaryPair bp(ms);
	bp.Set("Action", action);
	ms.Write(bs);
	msg.SetData(Buffer(ms.GetBuffer(), ms.Position()));

	// auto ms = msg.ToStream();
	// BinaryPair bp(ms);
	// bp.Set("Action", action);
	// ms.Write(bs);
	Send(msg);
}

void TokenClient::OnInvoke(const TokenMessage& msg, TokenController* ctrl)
{
	auto rs = msg.CreateReply();
	auto ms2 = rs.ToStream();
	BinaryPair rsbp(ms2);
	// 考虑到结果可能比较大，允许扩容
	//auto ms	= rs.ToStream();
	//ms.CanResize	= true;

	MemoryStream ms;	// 不能用 rs.Data 数据区  后面Set数据会先写Result  这样就破坏了数据区
	BinaryPair bp(msg.ToStream());

	String action;
	if (!bp.Get("Action", action) || !action)
	{
		//rs.SetError(0x01, "请求错误");
		rsbp.Set("ErrorCode", (byte)0x01);
	}
	else
	{
		// 传入参数名值对以及结果缓冲区引用，业务失败时返回false并把错误信息放在结果缓冲区
		MemoryStream result;
		if (!OnInvoke(action, bp, result))
		{
			if (result.Position() > 0)
			{
				// rs.SetError(0x03, (cstring)result.GetBuffer());
				rsbp.Set("ErrorCode", (byte)0x03);
			}
			else
			{
				//rs.SetError(0x02, "执行出错");
				rsbp.Set("ErrorCode", (byte)0x02);
			}
			rs.Length = ms2.Position();
		}
		else
		{
			// 执行成功
			result.SetPosition(0);

			BinaryPair bprs(ms);
			bprs.Set("Result", Buffer(result.GetBuffer(), result.Length));

			// 数据流可能已经扩容
			rs.Data = ms.GetBuffer();
			rs.Length = ms.Position();
		}
	}

	Reply(rs, ctrl);
}

bool TokenClient::OnInvoke(const String& action, const Pair& args, Stream& result)
{

	IDelegate* dlg = nullptr;
	if (!Routes.TryGetValue(action.GetBuffer(), dlg) || !dlg) return false;

	auto inv = (InvokeHandler)dlg->Method;

	return inv(dlg->Target, args, result);
}

void TokenClient::Register(cstring action, InvokeHandler handler, void* param)
{
	if (handler)
	{
		auto dlg = new IDelegate();
		dlg->Method = (void*)handler;
		dlg->Target = param;

		Routes.Add(action, dlg);
	}
	else
	{
		IDelegate* dlg = nullptr;
		if (Routes.TryGetValue(action, dlg))
		{
			delete dlg;

			Routes.Remove(action);
		}
	}
}

// 快速建立令牌客户端，注册默认Api
TokenClient* TokenClient::Create(cstring server, const Buffer& store)
{
	// Flash最后一块作为配置区
	if (Config::Current == nullptr) Config::Current = &Config::CreateFlash();

	// 初始化令牌网
	auto tk = TokenConfig::Create(server, 3377);

	auto tc = new TokenClient();
	tc->Cfg = tk;
	tc->MaxNotActive = 8 * 60 * 1000;

	// 重启
	tc->Register("Gateway/Restart", &TokenClient::InvokeRestart, tc);
	// 重置
	tc->Register("Gateway/Reset", &TokenClient::InvokeReset, tc);
	// 设置远程地址
	tc->Register("Gateway/SetRemote", &TokenClient::InvokeSetRemote, tc);
	// 获取远程配置信息
	tc->Register("Gateway/GetRemote", &TokenClient::InvokeGetRemote, tc);
	// 获取所有Invoke命令
	tc->Register("Api/All", &TokenClient::InvokeGetAllApi, tc);

	if (store.Length()) tc->Store.Data.Set(store.GetBuffer(), store.Length());

	return tc;
}

bool TokenClient::InvokeRestart(void * param, const Pair& args, Stream& result)
{

	result.Write((byte)01);
	debug_printf("1000ms后重启\r\n");
	// Sys.AddTask([](void * param) {Sys.Reboot(); }, nullptr, 1000, 0, "Restart");
	Sys.Reboot(1000);

	return true;
}

bool TokenClient::InvokeReset(void * param, const Pair& args, Stream& result)
{
	BinaryPair res(result);
	result.Write((byte)01);
	Config::Current->RemoveAll();

	debug_printf("1000ms后重置\r\n");
	//Sys.Sleep(500);
	//Sys.Reboot();
	//Sys.AddTask([](void * param) {Sys.Reset(); }, nullptr, 1000, 0, "RestBoot");

	Sys.Reboot(1000);

	return true;
}

bool TokenClient::InvokeSetRemote(void * param, const Pair& args, Stream& result)
{
	BinaryPair res(result);

	String remote;
	byte fixd;
	// 远程地址
	if (!args.Get("remote", remote))
	{
		res.Set("SetRemote", (byte)0);
		return false;
	}
	// 是否用久跳转
	if (!args.Get("fixd", fixd))
	{
		res.Set("SetRemote", (byte)0);
		return false;
	}

	NetUri uri(remote);
	auto client = (TokenClient*)param;
	debug_printf("远程地址设置\r\n");
	uri.ToString().Show();

	// 永久改变地址
	if (fixd)
	{
		auto cfg = client->Cfg;
		cfg->Show();

		cfg->Server() = uri.ToString();

		cfg->Save();
		cfg->Show();
	}

	res.Set("SetRemote", (byte)1);
	client->ChangeIPEndPoint(uri);

	return true;
}

bool TokenClient::InvokeGetRemote(void * param, const Pair& args, Stream& result)
{
	debug_printf("获取远程地址\r\n");
	auto client = (TokenClient*)param;

	String remote;

	auto cfg = client->Cfg;
	cfg->Show();

	auto svr = cfg->Server();

	result.Write(svr);

	return true;
}

bool TokenClient::InvokeGetAllApi(void * param, const Pair& args, Stream& result)
{
	debug_printf("获取Apis\r\n");
	auto client = (TokenClient*)param;

	String apis;

	auto& keys = client->Routes.Keys();
	for (int i = 0; i < keys.Count(); i++)
	{
		if (i > 0)apis += ',';
		apis = apis + String(keys[i]);// keys[i];
	}

	//BinaryPair bp(result);
	//bp.Set("Apis", apis);
	result.Write(apis);

	return true;
}
