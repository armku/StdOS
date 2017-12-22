#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"

//#define DEBUGSerialPort

#ifdef DEBUGSerialPort
    void SerialPort::Test(){

    }
    void Rs485Test();
    void SerialPortTest()
    {
        Rs485Test();
    }
    //#define ISOV2
    #define ISOV3

    uint OnUsart485Read(ITransport *transport, Buffer &bs, void *para, void *para2)
        {
            //transport->Write(bs);
            bs.ShowHex();
            return 0;
        }

    OutputPort port485;
		
	uint OnUsart2Read(ITransport *transport, Buffer &bs, void *para, void *para2)
	{
		time6cnt++;
		transport->Write(bs);
		bs.Show(true);
		return 0;
	}
	char tx2[500],rx2[500];

    SerialPort com485(COM2);
    void USART2_Config();
    void Rs485Test()
    {
		#ifdef ISOV2
			port485.Set(PB5);
		#elif defined ISOV3
			port485.Set(PC2);
		#endif 
		port485.OpenDrain = false;
		port485.Invert = 0;
		port485.Open();
		port485=0;

        com485.RS485=&port485;
        com485.SetBaudRate(115200);
        com485.Register(OnUsart485Read);
        com485.Open();
		
		sp2=new SerialPort(COM2);	
		sp2->Register(OnUsart2Read);
		sp2->Open();
		sp2->Tx.SetBuf(tx2,100);
		sp2->Rx.SetBuf(rx2,100);
    }
#endif 
