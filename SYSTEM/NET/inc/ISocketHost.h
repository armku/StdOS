#pragma once
#include "CString.h"

#if 1
    #define IPAddress byte
    #define MacAddress byte
    #define SocketMode byte
#endif 


class ISocketHost
{
    public:
        IPAddress IP; // 本地IP地址
        IPAddress Mask; // 子网掩码
        MacAddress Mac; // 本地Mac地址
        SocketMode Mode; // 无线模式。0不是

        IPAddress DHCPServer;
        IPAddress DNSServer;
        IPAddress DNAServer2;
        IPAddress Gateway;

        String *SSID; // 无线SSID
        String *Pass; // 无线密码

};
