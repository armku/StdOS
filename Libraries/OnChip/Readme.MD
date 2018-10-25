#include "USART.h"
#include "BspPlatform\Interrupt.h"
#include "Configuration.h"

//#define _USARTTEST_CPP
#ifdef _USARTTEST_CPP

//需要打开 Configuration.h 行号12 USE_USART2

USART usart222(USART2, 115200);
uint8_t buf485[20];

void USARTTask12(void *param)
{
#if 0
	buf485[0] = 'A';
	buf485[1] = 'B';
	buf485[2] = 'C';
	buf485[3] = 'D';
	buf485[4] = 'E';
	buf485[5] = 0;
	pCOM2->SendBytes(buf485, 5);
#endif
	if (pCOM2->RxSize() > 0)
	{
		int len = pCOM2->RxSize();
		pCOM2->GetBytes(buf485, len);
		//pCOM2->ClearRxBuf();
		pCOM2->SendBytes(buf485, len);
	}
}
OutputPort r485(PC0, false);

void USARTTestInit()
{
	r485 = 0;//接收	
	pCOM2->SetBaudRate(115200);

	Sys.AddTask(USARTTask12, 0, 0, 500, "USARTTask12");
}
#endif


#include "WatchDog.h"
#include "Sys.h"

//#define _WATCHDOGTEST_CPP
#ifdef _WATCHDOGTEST_CPP

void WatchDogTestInit()
{
#ifdef DEBUG
	WatchDog::Start(20000, 10000);
#else 
	WatchDog::Start();
#endif 
}
#endif

#include "OnChip\PwmSolo.h"

#define _PWMSOLO_TEST_CPP
#ifdef _PWMSOLO_TEST_CPP

PwmSolo pwm(10000, 2700);

void PwmTestRoutin123(void *param)
{
	static int i = 0;
	if (i++ > 100)
		i = 0;
	pwm.SetOutPercent(i);
}

void PwmSoloTestInit()
{
	pwm.Set(PC6);
	pwm.SetOutPercent(0);
	pwm.Open();
	
	Sys.AddTask(PwmTestRoutin123,0,0,10,"pwmtest");
}
#endif // _PWMSOLO_TEST_CPP

#include "Port.h"
#include "Sys.h"

//#define _LEDTEST_CPP
#ifdef _LEDTEST_CPP
OutputPort led12(PC6, true);

void LedTask12(void *param)
{
	auto leds = (OutputPort*)param;
	*leds = !* leds;
}

void LedTestInit()
{
	Sys.AddTask(LedTask12, &led12, 0, 500, "LedTask");
}
#endif

#include "OnChip\Configuration.h"
#include "Sys.h"

//#define _SERIALPORT485TEST_CPP
#ifdef _SERIALPORT485TEST_CPP

uint8_t buf485[20];

void SerialPort485Task12(void *param)
{
	DeviceConfigHelper::comSend(COM1, Buffer(buf485, ArrayLength(buf485)));
}
uint8_t bufrcv485[100];
void R485Rcv()
{
	Buffer bsrcv(bufrcv485, ArrayLength(bufrcv485));
	Rxx1.Read(bsrcv);
	DeviceConfigHelper::com1send(bsrcv);
}

OutputPort r485(PC0,false);

void SerialPort485TestInit()
{
	r485 = 0;//接收
	DeviceConfigHelper::pCOM1Rx485 = &r485;
	DeviceConfigHelper::PRcvCOM1 = R485Rcv;
	DeviceConfigHelper::ConfigCom(COM1, 115200);
	DeviceConfigHelper::ComChgBaudRate(COM1, 115200);

	Sys.AddTask(SerialPort485Task12, 0, 0, 500, "R485Task");
}
#endif
