/*
基本类型表，在 C++基础上尽可能参考 C#/Net


 */
#pragma once

#include <stdint.h>


typedef unsigned char byte; //单字节 0~255
//typedef  int16_t short;//短整型
typedef unsigned short int ushort; //无符号短整型 0~65535
//typedef  int32_t int;//整型
typedef unsigned int uint; //无符号整型
typedef int64_t Int64; //长整型
typedef uint64_t UInt64; //无符号长整型
typedef uint cstring; //C 格式字符串

#ifdef __cplusplus         

    class CType{

    }
    ;
#endif /* __cplusplus */
