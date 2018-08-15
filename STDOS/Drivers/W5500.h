#ifndef _W5500_H
#define _W5500_H

#include "Spi.h"

class W5500
{
public:
	uint16_t		RetryTime;
	uint8_t		RetryCount;
	bool		PingACK;

	uint32_t	TaskID;

	//IDataPort*	Led;	// Ö¸Ê¾µÆ
private:

};

#endif // !_W5500_H
