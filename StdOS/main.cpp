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
	sys.Init();
	BspPlatformInit();
#if DEBUG	
	sys.ShowInfo();
#endif 		
	setup();

	sys.Start();
	sys.AddTask(arduinoloop, 0, 13, 1, "loop");
}
