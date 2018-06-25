#ifndef _HANDLER_H
#define _HANDLER_H

#include "Sys.h"

class IHandler
{
public:
	
	/// <summary>上一个处理器</summary>
	IHandler *Prev;

		/// <summary>下一个处理器</summary>
	IHandler *Next;
		/// <summary>
		/// 读取数据处理
		/// </summary>
		/// <param name="buf"></param>
		/// <returns></returns>
	Buffer& Read(Buffer &bs);
	/// <summary>
	/// 写入数据处理
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	Buffer& Write(Buffer& bs);

private:

};

#endif // !_IPIPELINE_H
