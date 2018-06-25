#ifndef _IPIPELINE_H
#define _IPIPELINE_H
#include "IHandler.h"

class IPipeline
{
public:
	/// <summary>头部处理器</summary>
	IHandler * Head;

		/// <summary>尾部处理器</summary>
	IHandler *Tail;

private:

};

#endif // !_IPIPELINE_H
