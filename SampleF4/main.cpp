#include "Sys.h"
#include "Bsp\Bsp.h"
#include "Device\DeviceConfigHelper.h"
#include "BspPlatform\BspPlatform.h"

int main(void)
{
	Sys.Init();
	BspPlatformInit();
#if DEBUG	
	Sys.ShowInfo();
#endif 		
	BspInit();

	Sys.Start();
}
