#include "Sys.h"
#include "Bsp.h"
#include "../Bsp/Porting.h"

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
