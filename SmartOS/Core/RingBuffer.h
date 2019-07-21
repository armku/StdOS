#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

#include "Type.h"
//暂时未用
class RingBuffer
{
public:
	RingBuffer(void* buffer, int bufsize);
	void Init(void* buffer, int bufsize);
	int Put(char data);
	int Put(char *c, int len);
	int Get(char *c, int len);
	bool Empty();

	byte * buf;
	int length;//数据长度
	int head;//头
	int tail;//尾
	int BufLen;//缓冲区最大长度

private:

};

#endif // !_RING_BUFFER_H
