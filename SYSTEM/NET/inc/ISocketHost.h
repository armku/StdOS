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
        IPAddress IP; // ����IP��ַ
        IPAddress Mask; // ��������
        MacAddress Mac; // ����Mac��ַ
        SocketMode Mode; // ����ģʽ��0����

        IPAddress DHCPServer;
        IPAddress DNSServer;
        IPAddress DNAServer2;
        IPAddress Gateway;

        String *SSID; // ����SSID
        String *Pass; // ��������

};
