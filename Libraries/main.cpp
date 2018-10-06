#include "Sys.h"
#include "Bsp\Bsp.h"
#include "OnChip\Configuration.h"
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
