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
