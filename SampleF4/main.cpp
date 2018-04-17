#include "Sys.h"
#include "Bsp\Bsp.h"
#include "BspPlatform\BspPlatform.h"

int main(void)
{
	Sys.Init();
	SerialPrintInit();
#if DEBUG
	Sys.ShowInfo();
#endif 	
	BspInit();
		
	Sys.Start();
}
