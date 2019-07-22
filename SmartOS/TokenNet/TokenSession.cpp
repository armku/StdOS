﻿#include "Kernel\TTime.h"

#include "Net\Socket.h"

#include "Message\BinaryPair.h"

#include "TokenSession.h"

#include "TokenMessage.h"
#include "HelloMessage.h"
#include "LoginMessage.h"
#include "RegisterMessage.h"
#include "TokenDataMessage.h"
#include "ErrorMessage.h"

#include "Security\RC4.h"
#include "Security\Crc.h"

TokenSession::TokenSession(TokenClient& client, TokenController& ctrl) :
	Client(client),
	Control(ctrl)
{
	Status = 0;
	LastActive = Sys.Ms();

	client.Sessions.Add(this);
}

TokenSession::~TokenSession()
{
	Client.Sessions.Remove(this);
}

bool TokenSession::Send(TokenMessage& msg)
{
	// 未登录之前，只能 握手、登录、注册
	if (Token == 0)
	{
		if (msg.Code != 0x01 && msg.Code != 0x02 && msg.Code != 0x07) return false;
	}
	assert(&Control, "还没有Control呢");

	msg.State = &Remote;

	return Control.Send(msg);
}

void TokenSession::OnReceive(TokenMessage& msg)
{
	TS("TokenSession::OnReceive");

	LastActive = Sys.Ms();
	// if (Token == 0 && msg.Code > 1 && Key.Length() == 0)
	if ((Token == 0 && msg.Code > 2) || msg.ErrorCode == DecryptError)	// 解密失败 直接让他重新来过
	{
		auto rs = msg.CreateReply();

		rs.Code = 0x01;
		rs.Error = true;

		HelloMessage ext;
		ext.ErrCode = 0x7F;

		ext.WriteMessage(rs);

		Control.Reply(rs);

		return;
	}

	switch (msg.Code)
	{
	case 0x01:
		OnHello(msg);
		break;
	case 0x02:
		OnLogin(msg);
		break;
	case 0x03:
		OnPing(msg);
		break;
	case 0x05:
	case 0x06:
	case 0x08:
		// 读写和调用，交给客户端处理
		Client.OnReceive(msg, Control);
		break;
	}
}

bool TokenSession::OnHello(TokenMessage& msg)
{
	if (msg.Reply) return false;

	// 解析数据
	HelloMessage ext;
	ext.Reply = msg.Reply > 0;

	ext.ReadMessage(msg);
	ext.Show(true);

	TS("TokenSession::OnHello");

	DeviceID = ext.Name;

	BinaryPair bp(msg.ToStream());
	if (bp.Get("Action"))
	{
		msg.Code = 0x08;
		auto tokenback = Control.Token;
		Control.Token = 1;

		Client.OnReceive(msg, Control);

		Control.Token = tokenback;
		return true;
	}

	// 同步本地时间
	//if (ext.LocalTime > 0 && DateTime::Now().Year < 2016) ((TTime&)Time).SetTime(ext.LocalTime / 1000);

	auto rs = msg.CreateReply();

	HelloMessage ext2;
	ext2.Reply = true;

	// 同一个控制器共用密码，如果还没有则新建一个
	auto& key = Control.Key;
	if (key.Length() == 0)
	{
		auto now = Sys.Ms();
		//auto crc = Crc::Hash(Buffer(&now, 8));
		//key = Buffer(&crc, 4);
		key = Buffer(&now, 4);
		key.Show(true);
		//通知其它内网，密码被修改了
	}
	ext2.Key = key;

	auto sock = Control._Socket;
	//ext2.EndPoint.Address = sock->Host->IP;
	ext2.EndPoint = sock->Local;

	//ext2.Cipher = "RC4";
	ext2.Name = Client.Cfg->User();

	// 使用当前时间
	ext2.LocalTime = DateTime::Now().TotalMs();
	ext2.WriteMessage(rs);

	//Client.Reply(rs, Control);
	Control.Reply(rs);
	Status = 1;

	return true;
}

bool TokenSession::OnLogin(TokenMessage& msg)
{
	if (msg.Reply) return false;

	TS("TokenSession::OnLogin");

	LoginMessage login;
	login.ReadMessage(msg);

	auto name = login.User;
	// 访问令牌
	auto vistoken = login.Pass;
	auto cfg = TokenConfig::Current;

	debug_printf("内网登录密文:");
	vistoken.Show(true);
	// 上位机以HEX字符传送
	auto pass = vistoken.ToHex();
	// 密钥
	auto key = cfg->Pass().GetBytes();
	// 解密
	RC4::Encrypt(pass, key);

	String str((char*)(pass.GetBuffer()), pass.Length());
	debug_printf("内网登录明文:");
	str.Show(true);
	auto rs = msg.CreateReply();

	if (!str.Contains(name))
	{
		login.Error = true;
		login.ErrorCode = 0x01;
		login.ErrorMessage = "访问令牌非法";
		login.WriteMessage(rs);

		return false;
	}
	else
	{
		Token = (uint)Sys.Ms();
		login.Key = Control.Key;
		login.Token = Token;
		login.Reply = true;
		login.WriteMessage(rs);

		Status = 2;
		Control.Token = Token;
	}

	Control.Reply(rs);

	return true;
}

bool TokenSession::OnPing(TokenMessage& msg)
{
	TS("TokenSession::OnPing");

	if (msg.Reply) return false;

	auto rs = msg.CreateReply();
	rs.SetData(Buffer(msg.Data, msg.Length));

	Control.Reply(rs);

	Status = 3;
	return true;
}

bool TokenSession::CheckExpired()
{
	auto now = Sys.Ms();
	// 5分钟不活跃超时	LastActive为0的为特殊Session 不删除
	if (LastActive + 5 * 60 * 1000 < now) return true;

	// 握手一分钟后还不登录的删掉
	if (LastActive + 60 * 1000 < now && Status == 1) return true;

	return false;
}

String& TokenSession::ToStr(String& str) const
{
	int sec = (int)(Sys.Ms() - LastActive) / 1000UL;
	str = str + Remote + "\t" + DeviceID + "\tLastActive\t" + sec + "s";

	return str;
}

void TokenSession::Show(IList& sessions)
{
#if DEBUG
	static uint _task = 0;
	if(!_task) _task = Sys.AddTask(
		[](void *param)
	{
		auto& list = *(IList*)param;
		debug_printf("Sessions: %d \r\n", list.Count());
		String str;
		for (int i = 0; i < list.Count(); i++)
		{
			auto ss = (TokenSession*)list[i];
			//ss->Stat.Show(true);
			//ss->ToString().Show(true);
			ss->ToStr(str);
			str.Show(true);
			str.SetLength(0);
		}
		debug_printf("\r\n");
	},
		&sessions, 5000, 15000, "会话状态");
#endif
}
