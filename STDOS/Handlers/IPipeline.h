#ifndef _IPIPELINE_H
#define _IPIPELINE_H
#include "IHandler.h"

class IPipeline
{
public:
	/// <summary>ͷ��������</summary>
	IHandler * Head;

		/// <summary>β��������</summary>
	IHandler *Tail;

private:

};

#endif // !_IPIPELINE_H
