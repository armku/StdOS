#include "Sys.h"
#include "Bsp\Bsp.h"
#include "BspPlatform\BspPlatform.h"
#include "Device\DeviceConfigHelper.h"

int main(void)
{
	Sys.Init();
	DeviceConfigHelper::ConfigCom(COM1, 256000);
#if DEBUG
	Sys.ShowInfo();
#endif 	
	BspInit();
		
	Sys.Start();
}
