#ifndef __Type_H__
#define __Type_H__
#include <stdint.h>

typedef const char*		cstring;

#define UInt64_Max 0xFFFFFFFFFFFFFFFFull

// 逐步使用char替代byte，在返回类型中使用char*替代void*
// 因为格式化输出时，可以用%c输出char，用%s输出char*
// 数组长度
#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))

#endif
