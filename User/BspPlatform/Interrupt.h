#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "Configuration.h"

#ifdef USE_USART1
extern USART *pCOM1;
#endif
#ifdef USE_USART1_DMA
extern USART *pCOM1;
#endif

#endif // !_INTERRUPT_H
