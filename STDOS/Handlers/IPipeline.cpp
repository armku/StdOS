#include "IPipeline.h"

/// <summary>添加处理器到开头</summary>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::AddFirst(IHandler *handler)
{
	if (Head == NULL)
	{
		handler->Next = NULL;
		handler->Prev = NULL;
		Head = handler;
		Tail = handler;
	}
	else
		AddBefore(Head, handler);
}

/// <summary>添加处理器到末尾</summary>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::AddLast(IHandler *handler)
{
	if (Tail == NULL)
	{
		handler->Next = NULL;
		handler->Prev = NULL;
		Head = handler;
		Tail = handler;
	}
	else
		AddAfter(Tail, handler);
}

/// <summary>添加处理器到指定名称之前</summary>
/// <param name="baseHandler">基准处理器</param>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::AddBefore(IHandler *baseHandler, IHandler *handler)
{
	handler->Next = baseHandler;
	handler->Prev = baseHandler->Prev;
	if (baseHandler->Prev != NULL) baseHandler->Prev->Next = handler;
	baseHandler->Prev = handler;

	if (baseHandler == Head) Head = handler;
}

/// <summary>添加处理器到指定名称之后</summary>
/// <param name="baseHandler">基准处理器</param>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::AddAfter(IHandler* baseHandler, IHandler* handler)
{
	handler->Next = baseHandler->Next;
	handler->Prev = baseHandler;
	if (baseHandler->Next != NULL) baseHandler->Next->Prev = handler;
	baseHandler->Next = handler;

	if (baseHandler == Tail) Tail = handler;
}

/// <summary>删除处理器</summary>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::Remove(IHandler* handler)
{
	if (handler->Prev != NULL)
		handler->Prev->Next = handler->Next;
	else
		Head = handler->Next;

	if (handler->Next != NULL)
		handler->Next->Prev = handler->Prev;
	else
		Tail = handler->Prev;
}


void IPipeline::Read(Buffer &bs)
{
	if (Head == NULL)
		return;
	/*auto rr= Head->Read(bs);
	if (rs != NULL)
	{
		auto next = Head->Next;
		while (next != NULL)
		{
			if (rs != NULL)
				rs = next->Read(rs);
			next = next->Next;
		}
	}*/
}

/// <summary>
/// 写入数据
/// </summary>
/// <param name="buf"></param>
void IPipeline::Write(Buffer& bs)
{
	if (Tail == NULL)
		return;
	/*auto rs = Tail->Write(bs);
	if (rs != NULL)
	{
		auto next = Tail ->Prev;
		while (next != NULL)
		{
			if (rs != NULL)
				rs = next->Write(rs);
			next = next->Next;
		}
	}*/
}

