#include "IHandler.h"

/// <summary>
/// ��ȡ���ݴ���
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer & IHandler::Read(Buffer &bs)
{
	return bs;
}
/// <summary>
/// д�����ݴ���
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer &  IHandler::Write(Buffer& bs)
{
	return bs;
}
