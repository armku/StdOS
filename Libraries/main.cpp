#include "Sys.h"
#include "Bsp\Bsp.h"
#include "Device\Configuration.h"
#include "BspPlatform\BspPlatform.h"

void OS_ComSendChk(void *param);
//´®¿Ú½ÓÊÕÅÐ¶Ï
void OS_ComRcvChk(void *param);
int main(void)
{
	Sys.Init();
	BspPlatformInit();
#if DEBUG	
	Sys.ShowInfo();
#endif 		
	Sys.AddTask(OS_ComSendChk, 0, 5, 1, "ComSendChk");
	Sys.AddTask(OS_ComRcvChk, 0, 100, 1, "ComRcvChk");
	BspInit();

	Sys.Start();
}
