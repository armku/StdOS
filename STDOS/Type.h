/*
基本类型表，在 C++基础上尽可能参考 C#/Net


 */
#pragma once

#include <stdint.h>

typedef unsigned char byte; //单字节 0~255
typedef unsigned short int ushort; //无符号短整型 0~65535
typedef unsigned int uint; //无符号整型
typedef int64_t Int64; //长整型
typedef uint64_t UInt64; //无符号长整型
typedef uint64_t ulong; //

#ifndef NULL
    #define NULL 0
#endif

#ifdef  DEBUG
void assert_failed(uint8_t* file, uint32_t line,char * errstr);
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert(expr,errstr) ((expr) ?(void)0:assert_failed((uint8_t *)__FILE__, __LINE__,errstr))
/* Exported functions ------------------------------------------------------- */  
#else
  #define assert(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus         

    class CType{

    }
    ;
#endif /* __cplusplus */
