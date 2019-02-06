#include "Sys.h"
#include "../User/Bsp.h"
#include "../Bsp/Porting.h"

void loop();
void arduinoloop(void* param)
{
	loop();
}

int main(void)
{
	Sys.Init();
	BspPlatformInit();
#if DEBUG	
	Sys.ShowInfo();
#endif 		
	setup();

	Sys.Start();
	Sys.AddTask(arduinoloop, 0, 13, 1, "loop");
}
