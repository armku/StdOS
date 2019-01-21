#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"
#include "port/std_port_gpio.h"

// Êä³ö¿Ú
class OutputPort : public mcuGpio
{
public:
    
    OutputPort();
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false);
	    
    OutputPort& operator=(bool value) { Write(value); return *this; }
    OutputPort& operator=(OutputPort& port) { Write(port.Read()); return *this; }
    
protected:
    virtual void OnOpen(void* param);
};

#endif //_Port_H_
