/*
F1:
野火ISO-V2 ISO-V3
K0:PA0
K1:PC13

F4
正点原子探索者
K0:PA0
K1:PE2
K2:PE3
K3:PE4
 */
 
#include "Port.h"
#include "Platform\Pin.h"

//#define _PORT_TEST_H

#ifdef _PORT_TEST_H

    //测试代码
    extern OutputPort led2;
	#ifdef STM32F1
		InputPort key0(PA0, true, InputPort::DOWN);
        InputPort key(PC13);
    #elif defined STM32F4
        InputPort key0(PA0, true, InputPort::DOWN);
        InputPort key1(PE2, true, InputPort::UP);
        InputPort key2(PE3, true, InputPort::UP);
        InputPort key3(PE4, true, InputPort::UP);
    #endif 
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
        #if defined STM32F1
            key0.Press = OnPress;
            key0.UsePress();
            key0.Open();

            key.Press = OnPress;
            key.UsePress();
            key.Open();

            Sys.AddTask(led2flash, &led2, 0, 200, "led2");
        #elif defined STM32F4
            key0.Press = OnPress;
            key0.UsePress();
            key0.Open();

            key1.Press = OnPress;
            key1.UsePress();
            key1.Open();

            key2.Press = OnPress;
            key2.UsePress();
            key2.Open();

            key3.Press = OnPress;
            key3.UsePress();
            key3.Open();

        #endif 
    }
#endif
