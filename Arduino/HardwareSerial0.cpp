#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

#if defined(HAVE_HWSERIAL0)

HardwareSerial0 Serial;

HardwareSerial0::HardwareSerial0()
{

}
void HardwareSerial0::begin(unsigned long baud, byte config)
{
	
}
#include "Sys.h"
size_t HardwareSerial0::write(uint8_t ch)
{
	debug_printf("%c",ch);
	return 1;
}
#endif // HAVE_HWSERIAL0
