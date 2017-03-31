/*
�������ͱ��� C++�����Ͼ����ܲο� C#/Net


 */
#pragma once

#include <stdint.h>

typedef unsigned char byte; //���ֽ� 0~255
typedef unsigned short int ushort; //�޷��Ŷ����� 0~65535
typedef unsigned int uint; //�޷�������
typedef int64_t Int64; //������
typedef uint64_t UInt64; //�޷��ų�����
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
