#ifndef _W5500_H
#define _W5500_H

#include "../../../../Bsp/Porting.h"

class W5500
{
public:
	uint16_t		RetryTime;
	uint8_t		RetryCount;
	bool		PingACK;

	uint32_t	TaskID;

	W5500();
private:

};

#endif // !_W5500_H
