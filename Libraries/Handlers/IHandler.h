#ifndef _HANDLER_H
#define _HANDLER_H

#include "Sys.h"
#include "Core\Buffer.h"

class IHandler
{
public:

	/// <summary>上一个处理器</summary>
	IHandler * Prev;

	/// <summary>下一个处理器</summary>
	IHandler *Next;
	/// <summary>
	/// 读取数据处理
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Read(Buffer &bs);
	/// <summary>
	/// 写入数据处理
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Write(Buffer& bs);

private:

};

class HandlerShow:public IHandler
{
public:
	/// <summary>
	/// 读取数据处理
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Read(Buffer &bs);
	/// <summary>
	/// 写入数据处理
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Write(Buffer& bs);

};
class HandlerShowHex :public IHandler
{
public:
	/// <summary>
	/// 读取数据处理
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Read(Buffer &bs);
	/// <summary>
	/// 写入数据处理
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Write(Buffer& bs);

};

#endif // !_IPIPELINE_H
