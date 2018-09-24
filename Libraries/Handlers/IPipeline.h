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

	/// <summary>添加处理器到开头</summary>
	/// <param name="handler">处理器</param>
	/// <returns></returns>
	void AddFirst(IHandler *handler);

	/// <summary>添加处理器到末尾</summary>
	/// <param name="handler">处理器</param>
	/// <returns></returns>
	void AddLast(IHandler *handler);

	/// <summary>添加处理器到指定名称之前</summary>
	/// <param name="baseHandler">基准处理器</param>
	/// <param name="handler">处理器</param>
	/// <returns></returns>
	void AddBefore(IHandler *baseHandler, IHandler *handler);

	/// <summary>添加处理器到指定名称之后</summary>
	/// <param name="baseHandler">基准处理器</param>
	/// <param name="handler">处理器</param>
	/// <returns></returns>
	void AddAfter(IHandler* baseHandler, IHandler* handler);

	/// <summary>删除处理器</summary>
	/// <param name="handler">处理器</param>
	/// <returns></returns>
	void Remove(IHandler* handler);


	void Read(Buffer &bs);

	/// <summary>
	/// 写入数据
	/// </summary>
	/// <param name="buf"></param>
	void Write(Buffer& bs);

private:

};

#endif // !_IPIPELINE_H
