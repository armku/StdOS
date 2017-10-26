#include "SerialPort.h"
#include "Timer.h"
#include "Spi.h"
#if 0
    OutputPort led1(PF9, false);
    OutputPort led2(PF10, false);

    void LedTask(void *param)
    {
        OutputPort *leds = (OutputPort*)param;
        *leds = ! * leds;
        //led2 = keyup;
    }

    uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
    {
        bs.Show(true);
        return 0;
    }
    uint time6cnt;
    void TimerTask(void *param)
    {
        //    static int i = 0;
        //    printf("\r\n%d: cnt:%d", i++, time6cnt);
    }
    void InterruptTest();
    void w25q128test();
#endif 
OutputPort led1(PI10, false);
OutputPort led2(PF7, false);
OutputPort led3(PF8, false);
OutputPort led4(PC2, false);
void delay(int len)
{
    for (int i = 0; i < len; i++)
        ;
}

void bsp_LedToggle(uint8_t _no)
{
    if (_no == 1)
    {
        led1 = !led1; //GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;
    }
    else if (_no == 2)
    {
        led2 = !led2; //GPIO_PORT_LED2->ODR ^= GPIO_PIN_LED2;
    }
    else if (_no == 3)
    {
        led3 = !led3; //GPIO_PORT_LED3->ODR ^= GPIO_PIN_LED3;
    }
    else if (_no == 4)
    {
        led4 = !led4; //GPIO_PORT_LED4->ODR ^= GPIO_PIN_LED4;
    }
}

int main(void)
{
	Sys.Init();
	#if DEBUG
            Sys.MessagePort = COM1;
            Sys.ShowInfo();
    #endif 
	//SerialPort::GetMessagePort()->Register(OnUsart1Read);
    while (1)
    {
        delay(2000000);
        /* 每隔500ms 进来一次 */
        bsp_LedToggle(4); /* 翻转LED4的状态 */
        bsp_LedToggle(3);
        bsp_LedToggle(2);
        bsp_LedToggle(1);
    }
    #if 0        
        
        

        Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
        Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
        InterruptTest();
        w25q128test();
        Sys.Start();
    #endif 
}
