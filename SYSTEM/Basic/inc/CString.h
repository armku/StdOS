/*
字符串 String
字符串 String 继承自 Array，内部存储指针和长度。特别注意，内部会为
结尾的\0 保留存储空间，以确保取得的字符串指针是标准 C 格式字符串指针。
参考 C#版 String 类，所有基本类型都可以转为字符串，支持字符串赋值拷
贝、连接、相等比较、转数字、截取等。
1) 基本类型转为字符
3) 数字转为十进制字符串
5) 数字转为十六进制字符串
6) 字符串到字符串拷贝为数据拷贝，内容相等，指针不同
7) 字符串连接支持基本类型。数字默认以十进制方式连接。
***特别注意，为了避免产生过多碎片对象，字符串连接实质上是在源字符
串上累加，会改变源字符串。这一点与 C#/Java/C++的字符串类机制不同。
9) 字符串以十六进制连接数字
10) 十六进制字符串转为二进制数组
11) 字符串搜索
13) 字符串截取
14) 字符串分割 Split
受制于 C++语言特性，无法仿造 C#版本 String.Split，而采用枚举结构。

*/
#pragma once

#include "Array.h"
#include "DateTime.h"

//内部会为
//结尾的\0 保留存储空间，以确保取得的字符串指针是标准 C 格式字符串指针
//自带64字节的缓冲区。
class String:public Array
{
	public:
		String();
		String(const char * str,int length=-1);
		String(char ch);
		String(const String& str1);
		String(byte value,byte radix=10);//进制字符串
		String(short value,byte radix=10);//进制字符串		
		String(int value,byte radix=10);//进制字符串
		String(uint value,byte radix=10);//进制字符串
		String(Int64 value,byte radix=10);//进制字符串
		String(UInt64 value,byte radix=10);//进制字符串
		String(float value,byte dot=2);//浮点数
		String(double value,byte dot=2);//浮点数
		
		~String();
		
		String& operator=(const char* str) ;
		String& operator=(const String& str) ;
		bool operator==(const char* str) ;
		bool operator==(const String& str);
	
		String& operator+=(const DateTime& dt);
		String& operator+=(const String& str);
		String& operator+=(const float da);
		
		String& Concat(byte value,byte radix=10);
		String& Concat(short value,byte radix=10);
		String& Concat(int value,byte radix=10);
		String& Concat(uint value,byte radix=10);
		String& Concat(Int64 value,byte radix=10);
		String& Concat(UInt64 value,byte radix=10);
		String& Concat(float value,byte dot=2);
		String& Concat(double value,byte dot=2);
		
		String ToHex();
		virtual void Show(bool newLine=true) const;
		uint Capacity() const;//数组容量
	private:
		void initCapacity();//初始化容器
		void jsLength();//计算字符串长度
		short mcapacity;//字符串容量
		
		
};
