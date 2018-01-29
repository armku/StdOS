#include "Drivers\GPS_ATGM332D.h"
#include "Sys.h"
#include "Device\SerialPort.h"

#define _GPS_ATGM332DTest
#ifdef _GPS_ATGM332DTest
uint OnUsart2Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
		bs.Show(true);
	//gps1216.Analysis((byte*)bs.GetBuffer());//分析字符串
	//gps1216.Show();				//显示信息	
	return 0;
}

SerialPort *sp2;
byte com2rx[500], com2tx[500];
void GPS_ATGM332DTest()
{
	sp2 = new SerialPort(COM2);
	sp2->Rx.SetBuf(com2rx, ArrayLength(com2rx));
	sp2->Tx.SetBuf(com2tx, ArrayLength(com2tx));
	sp2->Register(OnUsart2Read);
	sp2->SetBaudRate(9600);
	sp2->Open();
}

#endif // _GPS_ATGM332DTest
