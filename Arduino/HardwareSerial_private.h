#include "wiring_private.h"

#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3)


// Constructors ////////////////////////////////////////////////////////////////

HardwareSerial_T::HardwareSerial_T() 
{
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial_T::_rx_complete_irq(void)
{
  
}

#endif // whole file
