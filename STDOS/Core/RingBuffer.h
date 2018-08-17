#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

#include "Type.h"

class RingBuffer
{
public:
	void Init(void* buffer, int bufsize);
	int Put(char data);
	int Put(char *c, int len);
	int Get(char *c, int len);
	bool Empty();

	uint8_t * buf;
	int length;
	int head;
	int tail;
	int fillcnt;

private:

};

#endif // !_RING_BUFFER_H
