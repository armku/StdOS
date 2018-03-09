#include "SerialPort.h"
#include "Bsp\Bsp.h"

int main(void)
{
	Sys.Init();
#if DEBUG
	Sys.MessagePort = COM1;
	Sys.ShowInfo();
#endif 	
	BspInit();
		
	Sys.Start();
}
