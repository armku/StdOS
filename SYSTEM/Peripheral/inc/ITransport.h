#pragma once

#include "Type.h"
#include "Buffer.h"

class ITransport
{
	public:
	 //virtual void Write(Buffer& bs);//写数据
};

//以下为临时区域
#define NULL   0
typedef void(*TransportHandler)(ITransport* transport,Buffer & bs);




//以上为临时区域
