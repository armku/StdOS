#include "wiring_private.h"

#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3)


// Constructors ////////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial() 
{
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial::_rx_complete_irq(void)
{
  
}

#endif // whole file
