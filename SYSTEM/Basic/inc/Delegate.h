#pragma once

#include "Type.h"
/*
回调函数，分号之前必须有空格，否则可能编译异常
*/
typedef void (*Func)(void) ;//没有参数和返回值的委托
typedef void(*Action)(void* param) ;  //一个参数没有返回值的委托，一般param参数用作目标对象，调用者用静态函数包装
typedef void(*Action2)(void*,void*) ;
typedef void(*Action3)(void*,void*,void*) ;
typedef void(*EnventHandler)(void* sender,void* param) ;//事件处理委托，一般sender表示事件发出者，param用作目标对象，调用者用静态函数包装
typedef uint (*DataHandler)(void* sender,byte* buf,uint size_t,void* param) ;//传入数据缓冲区地址和长度，如有反馈，仍使用该缓冲区，返回数据长度
typedef uint (*FuncRead)();//有返回值的委托
