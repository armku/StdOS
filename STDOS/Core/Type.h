#ifndef __Type_H__
#define __Type_H__

/* 类型定义 */
typedef char			sbyte;
typedef unsigned char	byte;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long long	UInt64;
typedef long long		Int64;
typedef const char*		cstring;

#define UInt64_Max 0xFFFFFFFFFFFFFFFFull

// 逐步使用char替代byte，在返回类型中使用char*替代void*
// 因为格式化输出时，可以用%c输出char，用%s输出char*

class String;
class Type;

// 根对象
class Object
{
public:
	// 输出对象的字符串表示方式
	//virtual String& ToStr(String& str) const;
	// 输出对象的字符串表示方式。支持RVO优化
	//virtual String ToString() const;
	// 显示对象。默认显示ToString
	virtual void Show(bool newLine = false) const;

	//const Type GetType() const;
};
// 类型
class Type
{
private:
	const void* _info;

	friend class Object;

	Type();

public:
	int		Size;	// 大小
	//String	Name;	// 名称

	const String Name() const;	// 名称
};
// 数组长度
//#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))
// 数组清零，固定长度
//#define ArrayZero(arr) memset(arr, 0, sizeof(arr))

#include <stdint.h>

#ifndef NULL
    #define NULL 0
#endif

#ifdef  DEBUG
void assert_failed(byte* file, uint line,char * errstr);
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


#endif
