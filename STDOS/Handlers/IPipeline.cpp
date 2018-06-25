#include "IPipeline.h"

/// <summary>��Ӵ���������ͷ</summary>
/// <param name="handler">������</param>
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

/// <summary>��Ӵ�������ĩβ</summary>
/// <param name="handler">������</param>
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

/// <summary>��Ӵ�������ָ������֮ǰ</summary>
/// <param name="baseHandler">��׼������</param>
/// <param name="handler">������</param>
/// <returns></returns>
void IPipeline::AddBefore(IHandler *baseHandler, IHandler *handler)
{
	handler->Next = baseHandler;
	handler->Prev = baseHandler->Prev;
	if (baseHandler->Prev != NULL) baseHandler->Prev->Next = handler;
	baseHandler->Prev = handler;

	if (baseHandler == Head) Head = handler;
}

/// <summary>��Ӵ�������ָ������֮��</summary>
/// <param name="baseHandler">��׼������</param>
/// <param name="handler">������</param>
/// <returns></returns>
void IPipeline::AddAfter(IHandler* baseHandler, IHandler* handler)
{
	handler->Next = baseHandler->Next;
	handler->Prev = baseHandler;
	if (baseHandler->Next != NULL) baseHandler->Next->Prev = handler;
	baseHandler->Next = handler;

	if (baseHandler == Tail) Tail = handler;
}

/// <summary>ɾ��������</summary>
/// <param name="handler">������</param>
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
/// д������
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

