#include "Handlers\IHandler.h"
#include "Handlers\IPipeline.h"

#define _PIPELINE_TEST_CPP
#ifdef _PIPELINE_TEST_CPP

IPipeline pipeline;

class Handler1Test0 :public IHandler
{
public:
	virtual Buffer & Read(Buffer& bs);
	virtual Buffer& Write(Buffer&bs);
};
class Handler1Test1 :public IHandler
{
public:
	virtual Buffer & Read(Buffer& bs);
	virtual Buffer& Write(Buffer&bs);
};
class Handler1Test2 :public IHandler
{
public:
	virtual Buffer & Read(Buffer& bs);
	virtual Buffer& Write(Buffer&bs);
};


Handler1Test0 handler0;
Handler1Test1 handler1;
Handler1Test2 handler2;

char bufpipeline[20];
Buffer bspipeline(bufpipeline, ArrayLength(bufpipeline));

void PipelineTest()
{
	pipeline.AddFirst(&handler0);
	pipeline.AddLast(&handler1);
	pipeline.AddLast(&handler2);
	for (int i = 0; i < bspipeline.Length(); i++)
		bspipeline[i] = i + 1;
	debug_printf("\nPipeline ²âÊÔ¿ªÊ¼\n");
	pipeline.Read(bspipeline);
	debug_printf("Pipeline ²âÊÔ½áÊø\n\n");
}

Buffer& Handler1Test0::Read(Buffer& bs)
{
	for (int i = 0; i < bs.Length(); i++)
		bs[i] += 0x10;
	debug_printf("Handler1Test0\n");
	bs.ShowHex(true);
	return bs;
}
Buffer& Handler1Test0::Write(Buffer&bs)
{

	return bs;
}

Buffer& Handler1Test1::Read(Buffer& bs)
{
	for (int i = 0; i < bs.Length(); i++)
		bs[i] += 0x10;
	debug_printf("Handler1Test1\n");
	bs.SetLength(bs.Length() - 1);
	bs.ShowHex(true);
	return bs;
}
Buffer& Handler1Test1::Write(Buffer&bs)
{

	return bs;
}

Buffer& Handler1Test2::Read(Buffer& bs)
{
	for (int i = 0; i < bs.Length(); i++)
		bs[i] += 0x10;
	debug_printf("Handler1Test2\n");
	bs.SetLength(bs.Length()-1);
	bs.ShowHex(true);
	return bs;
}
Buffer& Handler1Test2::Write(Buffer&bs)
{

	return bs;
}
#endif
