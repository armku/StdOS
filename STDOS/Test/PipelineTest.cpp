#include "Handlers\IHandler.h"
#include "Handlers\IPipeline.h"

#define _PIPELINE_TEST_CPP
#ifdef _PIPELINE_TEST_CPP

IHandler handler0;
IPipeline pipeline;

char bufpipeline[20];
Buffer bspipeline(bufpipeline, ArrayLength(bufpipeline));

void PipelineTest()
{
	pipeline.AddFirst(&handler0);
}
#endif
