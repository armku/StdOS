#ifndef _HANDLER_H
#define _HANDLER_H

#include "Sys.h"

class IHandler
{
public:
	
	/// <summary>��һ��������</summary>
	IHandler *Prev;

		/// <summary>��һ��������</summary>
	IHandler *Next;
		/// <summary>
		/// ��ȡ���ݴ���
		/// </summary>
		/// <param name="buf"></param>
		/// <returns></returns>
	Buffer& Read(Buffer &bs);
	/// <summary>
	/// д�����ݴ���
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	Buffer& Write(Buffer& bs);

private:

};

#endif // !_IPIPELINE_H
