#include "SerialPort.h"
#include "Bsp\Bsp.h"

#define namee "StdOS"

const uint8_t vers[] = "yyyy-MM-dd HH:mm:ss";

int main()
{
	Sys.Name = (char*)namee;
	Sys.Init();
#if DEBUG
	Sys.MessagePort = COM1;
	Sys.ShowInfo();
	int aa = vers[0];
	aa++;
#endif 
	BspInit();
			
	Sys.Start();
}
