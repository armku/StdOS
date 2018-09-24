#include "IHandler.h"

/// <summary>
/// 读取数据处理
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer & IHandler::Read(Buffer &bs)
{
	return bs;
}
/// <summary>
/// 写入数据处理
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer &  IHandler::Write(Buffer& bs)
{
	return bs;
}

/// <summary>
/// 读取数据处理
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer & HandlerShow::Read(Buffer &bs)
{
	bs.Show(true);
	return bs;
}
/// <summary>
/// 写入数据处理
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer &HandlerShow:: Write(Buffer& bs)
{
	bs.Show(true);
	return bs;
}

/// <summary>
/// 读取数据处理
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer & HandlerShowHex::Read(Buffer &bs)
{
	bs.ShowHex(true);
	return bs;
}
/// <summary>
/// 写入数据处理
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
Buffer & HandlerShowHex::Write(Buffer& bs)
{
	bs.ShowHex(true);
	return bs;
}
