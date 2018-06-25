#include "IPipeline.h"

/// <summary>添加处理器到开头</summary>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::AddFirst(IHandler *handler)
{

}

/// <summary>添加处理器到末尾</summary>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::AddLast(IHandler *handler)
{

}

/// <summary>添加处理器到指定名称之前</summary>
/// <param name="baseHandler">基准处理器</param>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::AddBefore(IHandler *baseHandler, IHandler *handler)
{

}

/// <summary>添加处理器到指定名称之后</summary>
/// <param name="baseHandler">基准处理器</param>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::AddAfter(IHandler* baseHandler, IHandler* handler)
{

}

/// <summary>删除处理器</summary>
/// <param name="handler">处理器</param>
/// <returns></returns>
void IPipeline::Remove(IHandler* handler)
{

}


void IPipeline::Read(Buffer &bs)
{

}

/// <summary>
/// 写入数据
/// </summary>
/// <param name="buf"></param>
void IPipeline::Write(Buffer& bs)
{

}

