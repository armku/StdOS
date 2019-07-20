#ifndef __String_H__
#define __String_H__

//#include "Array.h"
//#include "ByteArray.h"

// 字符串助手，主要用于字符串连接
class StringSplit;

// 字符串
class String : public Array
{
public:
	
};

#define R(str) String(str)

class StringSplit
{
public:
	cstring Sep;		// 分隔符。下一个要寻找的边界符
	int		Position;	// 当前段位置。负数表示到了结尾。
	int		Length;		// 当前段长度。

	StringSplit(const String& str, cstring sep);

	const String Next();

	// 是否已经到达末尾
	//bool End() const { return Position == -2; }

    explicit operator bool() const { return Position >= -1; }
    bool operator !() const { return Position < -1; }

private:
	const String& _Str;
};

#endif
