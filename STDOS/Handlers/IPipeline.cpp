#include "IPipeline.h"

/// <summary>��Ӵ���������ͷ</summary>
/// <param name="handler">������</param>
/// <returns></returns>
void IPipeline::AddFirst(IHandler *handler)
{

}

/// <summary>��Ӵ�������ĩβ</summary>
/// <param name="handler">������</param>
/// <returns></returns>
void IPipeline::AddLast(IHandler *handler)
{

}

/// <summary>��Ӵ�������ָ������֮ǰ</summary>
/// <param name="baseHandler">��׼������</param>
/// <param name="handler">������</param>
/// <returns></returns>
void IPipeline::AddBefore(IHandler *baseHandler, IHandler *handler)
{

}

/// <summary>��Ӵ�������ָ������֮��</summary>
/// <param name="baseHandler">��׼������</param>
/// <param name="handler">������</param>
/// <returns></returns>
void IPipeline::AddAfter(IHandler* baseHandler, IHandler* handler)
{

}

/// <summary>ɾ��������</summary>
/// <param name="handler">������</param>
/// <returns></returns>
void IPipeline::Remove(IHandler* handler)
{

}


void IPipeline::Read(Buffer &bs)
{

}

/// <summary>
/// д������
/// </summary>
/// <param name="buf"></param>
void IPipeline::Write(Buffer& bs)
{

}

