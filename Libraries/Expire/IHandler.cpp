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

/// <summary>
/// ��ȡ���ݴ���
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer & HandlerShow::Read(Buffer &bs)
{
	bs.Show(true);
	return bs;
}
/// <summary>
/// д�����ݴ���
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer &HandlerShow:: Write(Buffer& bs)
{
	bs.Show(true);
	return bs;
}

/// <summary>
/// ��ȡ���ݴ���
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer & HandlerShowHex::Read(Buffer &bs)
{
	bs.ShowHex(true);
	return bs;
}
/// <summary>
/// д�����ݴ���
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer & HandlerShowHex::Write(Buffer& bs)
{
	bs.ShowHex(true);
	return bs;
}
