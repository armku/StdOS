#include "../StdOS/Kernel/Sys.h"
#include "../User/Bsp.h"
#include "../Bsp/Porting.h"

int main(void)
{
	Sys.Init();
	BspPlatformInit();
#if DEBUG	
	Sys.ShowInfo();
#endif 		
	setup();

	Sys.Start();
}
