#include "TinyIP.h"

#if 0
����������û����̫����ô���У�
����Enc28j60�������ŶӶ���ʵ������̫������Э��ջTinyIP��Ŀǰ֧��ARP/ICMP/TCP/UDP/DHCP����ȱһ��DNS�������ˡ�
TinyIP����һ�����ݻ�������Enc28j60�յ������Ժ󣬷��뻺������Ȼ��TinyIP�ڲ���ʼ����Ethernet/ARP/IP/ICMP/TCP/UDP/DHCPͷ���ṹ������ݽ��в�ֺ����飬ִ��ҵ���߼���
TinyIP�ٷְ��������ŶӶ�����ɣ�û�г�Ϯ���²ο��κ�һ��������̫��Э�飨����uip/lwip��������Э��ṹ��Ϊ���ʱ�׼�ǹ����ģ�����ֻ��Ҫ����Э��ȥʵ�ּ��ɡ�
������Щ�Լ���������˵���˳�Ϯ���ˣ�
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
оƬGD32F103VK/GD32F103VE/STM32F103VE
TinyIP���ڲ�����ϵͳID���������MAC��ַ�ͳ�ʼIP��ַ192.168.0.x��IP���һ�ֽھ���ϵͳID��һ�ֽڡ�
����DHCP�󣬽����Զ���ȡIP��ַ��
����ʵ��ARPЭ�飬��������Ҳ��������IP����Ӧ��MAC�������޷�ͨ�š�
֧��Ping/TCP/UDP���ֱ�Ҫ�¼����ء�

��̫�����й���ȫ����RTM�¹̼�ROM���9k��
#endif
