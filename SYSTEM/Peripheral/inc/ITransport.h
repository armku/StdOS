#pragma once

#include "Type.h"
#include "Buffer.h"

class ITransport
{
	public:
	 //virtual void Write(Buffer& bs);//д����
};

//����Ϊ��ʱ����
#define NULL   0
typedef void(*TransportHandler)(ITransport* transport,Buffer & bs);




//����Ϊ��ʱ����
