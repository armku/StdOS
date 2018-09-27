#include "Interrupt.h"

#ifdef USE_USART1
USART *pCOM1 = 0;
#endif
#ifdef USE_USART1_DMA
USART *pCOM1 = 0;
#endif


extern "C"
{
	//~~~~~~~~~~~~~~~~~~~~~ USART IQR  ~~~~~~~~~~~~~~~~~//
	void USART1_IRQHandler(void)    //----USART1 IRQ----//
	{
#ifdef USE_GPS
		if (pGPS1) {
			pGPS1->GPS_IRQHandler();
			return;
		}
#endif
#ifdef USE_USART1
		pCOM1->IRQ();
#endif
#ifdef USE_USART1_DMA
		pCOM1->IRQ();
#endif
	}
}
