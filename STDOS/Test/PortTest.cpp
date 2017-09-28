#include "Port.h"
#include "Platform\Pin.h"

#ifdef DEBUG
    //���Դ���
    extern OutputPort led2;
    InputPort exti(PC13); //PA1 PB3     PA0 PC13
    InputPort exti1(PA0);
    void OnKeyPress(InputPort *pin, bool down, void *param)
    {
        //led1.Write(onoff);
        led2 = !led2;
        printf("�ж����ţ�P%c%d ֵ%d \r\n", _PIN_NAME(pin->_Pin), down);
    }


    InputPort key0(PA0);
    //�����¼�
    void OnPress(InputPort &port, bool down)
    {
        debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
    }
    InputPort key(PC13);
	void led2flash(void *param)
	{
		led2 = !led2;
	}
    void InterruptTest()
    {		
        exti1.UsePress();

        key.Press = OnPress;
        key.UsePress();
        key.Open();
        //        exti.Register(OnKeyPress);
        //        exti1.Register(OnKeyPress);
		Sys.AddTask(led2flash,&led2,0,200,"led2");
    }
#endif
