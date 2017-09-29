#include "Port.h"
#include "Platform\Pin.h"

#ifdef DEBUG1
    //≤‚ ‘¥˙¬Î
    extern OutputPort led2;    
    InputPort key(PC13);
	InputPort key0(PA0);
	    
    void OnPress(InputPort &port, bool down)
    {
        debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
    }
	
    void led2flash(void *param)
	{
		led2 = !led2;
	}
    void InterruptTest()
    {		
        key0.Press = OnPress;
        key0.UsePress();
        key0.Open();

        key.Press = OnPress;
        key.UsePress();
        key.Open();
		
		Sys.AddTask(led2flash,&led2,0,200,"led2");
    }
#endif
