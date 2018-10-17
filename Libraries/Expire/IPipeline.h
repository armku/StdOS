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

	/// <summary>��Ӵ���������ͷ</summary>
	/// <param name="handler">������</param>
	/// <returns></returns>
	void AddFirst(IHandler *handler);

	/// <summary>��Ӵ�������ĩβ</summary>
	/// <param name="handler">������</param>
	/// <returns></returns>
	void AddLast(IHandler *handler);

	/// <summary>��Ӵ�������ָ������֮ǰ</summary>
	/// <param name="baseHandler">��׼������</param>
	/// <param name="handler">������</param>
	/// <returns></returns>
	void AddBefore(IHandler *baseHandler, IHandler *handler);

	/// <summary>��Ӵ�������ָ������֮��</summary>
	/// <param name="baseHandler">��׼������</param>
	/// <param name="handler">������</param>
	/// <returns></returns>
	void AddAfter(IHandler* baseHandler, IHandler* handler);

	/// <summary>ɾ��������</summary>
	/// <param name="handler">������</param>
	/// <returns></returns>
	void Remove(IHandler* handler);


	void Read(Buffer &bs);

	/// <summary>
	/// д������
	/// </summary>
	/// <param name="buf"></param>
	void Write(Buffer& bs);

private:

};

#endif // !_IPIPELINE_H
