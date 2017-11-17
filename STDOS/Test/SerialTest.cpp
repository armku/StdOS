#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"
#include "Fifo.h"

#define DEBUGSerialPort

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
    }
#endif 
