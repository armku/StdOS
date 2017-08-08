#include "TinyIP.h"

#if 0
做物联网，没有以太网怎么能行！
基于Enc28j60，我们团队独立实现了以太网精简协议栈TinyIP，目前支持ARP/ICMP/TCP/UDP/DHCP，还缺一个DNS就完整了。
TinyIP内置一个数据缓冲区，Enc28j60收到数据以后，放入缓冲区，然后TinyIP内部开始根据Ethernet/ARP/IP/ICMP/TCP/UDP/DHCP头部结构体对数据进行拆分和重组，执行业务逻辑。
TinyIP百分百是我们团队独立完成，没有抄袭那怕参考任何一款已有以太网协议（例如uip/lwip），各种协议结构作为国际标准是公开的，我们只需要按照协议去实现即可。
鄙视那些自己做不到就说别人抄袭的人！
#include "Sys.h"
#include "Enc28j60.h"
#include "SerialPort.h"
#include "TinyIP/TinyIP.h"
#include "conf.h"

Spi* spi;
Enc28j60* enc;
TinyIP* tip;

void OnPing(TinyIP* tip, ICMP_HEADER* icmp, byte* buf, uint len)
{
    debug_printf("Ping From ");
    TinyIP::ShowIP(tip->RemoteIP);
    debug_printf(" with Payload=%d\r\n", len);
}

void OnUdpReceived(TinyIP* tip, UDP_HEADER* udp, byte* buf, uint len)
{
    debug_printf("Udp From ");
    TinyIP::ShowIP(tip->RemoteIP);
    debug_printf(":%d with Payload=%d  ", tip->RemotePort, len);
    TinyIP::ShowData(buf, len);
    debug_printf(" \r\n");
}

void OnTcpAccepted(TinyIP* tip, TCP_HEADER* tcp, byte* buf, uint len)
{
    debug_printf("TcpAccepted From ");
    TinyIP::ShowIP(tip->RemoteIP);
    debug_printf(":%d with Payload=%d\r\n", tip->RemotePort, len);
}

void OnTcpDisconnected(TinyIP* tip, TCP_HEADER* tcp, byte* buf, uint len)
{
    debug_printf("TcpDisconnected From ");
    TinyIP::ShowIP(tip->RemoteIP);
    debug_printf(":%d with Payload=%d\r\n", tip->RemotePort, len);
}

void OnTcpReceived(TinyIP* tip, TCP_HEADER* tcp, byte* buf, uint len)
{
    debug_printf("TcpReceived From ");
    TinyIP::ShowIP(tip->RemoteIP);
    debug_printf(":%d with Payload=%d  ", tip->RemotePort, len);
    TinyIP::ShowData(buf, len);
    debug_printf(" \r\n");
}

void TestEthernet()
{
    debug_printf("\r\n\r\n");
    debug_printf("TestEthernet Start......\r\n");

    spi = new Spi(SPI_3);
    enc = new Enc28j60(spi);

    tip = new TinyIP(enc);
    tip->UseDHCP = true;
    tip->Init();
    
    tip->OnPing = OnPing;
    tip->OnUdpReceived = OnUdpReceived;
    tip->OnTcpAccepted = OnTcpAccepted;
    tip->OnTcpDisconnected = OnTcpDisconnected;
    tip->OnTcpReceived = OnTcpReceived;

    debug_printf("\r\n TestEthernet Finish!\r\n");
}
芯片GD32F103VK/GD32F103VE/STM32F103VE
TinyIP类内部根据系统ID生成随机的MAC地址和初始IP地址192.168.0.x，IP最后一字节就是系统ID第一字节。
开启DHCP后，将会自动获取IP地址。
必须实现ARP协议，否则别人找不到你这个IP所对应的MAC，进而无法通信。
支持Ping/TCP/UDP多种必要事件挂载。

以太网所有功能全开，RTM下固件ROM大概9k。
#endif
