#ifndef _HANDLER_H
#define _HANDLER_H

#include "Sys.h"
#include "Core\Buffer.h"

class IHandler
{
public:

	/// <summary>��һ��������</summary>
	IHandler * Prev;

	/// <summary>��һ��������</summary>
	IHandler *Next;
	/// <summary>
	/// ��ȡ���ݴ���
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Read(Buffer &bs);
	/// <summary>
	/// д�����ݴ���
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
	/// ��ȡ���ݴ���
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Read(Buffer &bs);
	/// <summary>
	/// д�����ݴ���
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Write(Buffer& bs);

};
class HandlerShowHex :public IHandler
{
public:
	/// <summary>
	/// ��ȡ���ݴ���
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Read(Buffer &bs);
	/// <summary>
	/// д�����ݴ���
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Write(Buffer& bs);

};

#endif // !_IPIPELINE_H
