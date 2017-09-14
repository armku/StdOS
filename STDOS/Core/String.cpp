/*
进行到字符串搜索
 */

#include "SString.h"
#include <string.h>
#include <CType.h>
#include "Sys.h"

static byte buftmp[10];

String::String(cstring cstr): Array(cstr, ArrayLength(cstr))
{
    this->_Length = ArrayLength(cstr);
    this->Copy(0, cstr, this->_Length);
}

String::String(const String &str): Array(str.GetBuffer(), str.Length()){

}
String::String(String && rval): Array(rval.GetBuffer(), rval.Length()){

}
String::String(char *str, int length): Array(str, length){

}
String::String(char *str, int length, bool expand): Array(str, length)
{
    this->Expand = expand;
}

// 包装静态字符串，直接使用，修改时扩容
String::String(cstring str, int length): Array(str, length){}
String::String(bool value): Array(value ? "true" : "false", value ? 5 : 6){

}
String::String(char c): Array(buftmp, 10){}
String::String(byte value, int radix): Array(buftmp, 10){}
String::String(short value, int radix): Array(buftmp, 10){}
String::String(ushort value, int radix): Array(buftmp, 10){}
String::String(int value, int radix): Array(buftmp, 10){}
String::String(uint value, int radix): Array(buftmp, 10){}
String::String(Int64 value, int radix): Array(buftmp, 10){}
String::String(UInt64 value, int radix): Array(buftmp, 10){}
String::String(float value, int decimalPlaces): Array(buftmp, 10){}
String::String(double value, int decimalPlaces): Array(buftmp, 10){}

// 设置数组长度。改变长度后，确保最后以0结尾
bool String::SetLength(int length, bool bak)
{
    return false;
}

// 拷贝数据，默认-1长度表示当前长度
int String::Copy(int destIndex, const void *src, int len)
{
    return 0;
}

// 把数据复制到目标缓冲区，默认-1长度表示当前长度
int String::CopyTo(int srcIndex, void *dest, int len)const
{
    return 0;
}

// 为被赋值对象建立一个备份。
// 如果值为空或无效，或者内存分配失败，字符串将会被标记为无效
String &String::operator = (const String &rhs)
{
    return  *this;
}

String &String::operator = (cstring cstr)
{
    return  *this;
}

String &String::operator = (String && rval)
{
    return  *this;
}

// 连接内建类型。如果参数无效则认为连接失败
bool String::Concat(const Object &obj)
{
    return false;
}

bool String::Concat(const String &str)
{
    return false;
}

bool String::Concat(cstring cstr)
{
    return false;
}

bool String::Concat(bool value)
{
    return false;
}

bool String::Concat(char c)
{
    return false;
}

bool String::Concat(byte c, int radix)
{
    return false;
}

bool String::Concat(short num, int radix)
{
    return false;
}

bool String::Concat(ushort num, int radix)
{
    return false;
}

bool String::Concat(int num, int radix)
{
    return false;
}

bool String::Concat(uint num, int radix)
{
    return false;
}

bool String::Concat(Int64 num, int radix)
{
    return false;
}

bool String::Concat(UInt64 num, int radix)
{
    return false;
}

bool String::Concat(float num, int decimalPlaces)
{
    return false;
}

bool String::Concat(double num, int decimalPlaces)
{
    return false;
}

int String::CompareTo(const String &s)const
{
    return 0;
}

int String::CompareTo(cstring cstr, int len, bool ignoreCase)const
{
    return 0;
}

bool String::Equals(const String &s)const
{
    return false;
}

bool String::Equals(cstring cstr)const
{
    return false;
}

bool String::EqualsIgnoreCase(const String &s)const
{
    return false;
}

bool String::EqualsIgnoreCase(cstring cstr)const
{
    return false;
}

bool String::operator < (const String &rhs)const
{
    return false;
}

bool String::operator > (const String &rhs)const
{
    return false;
}

bool String::operator <= (const String &rhs)const
{
    return false;
}

bool String::operator >= (const String &rhs)const
{
    return false;
}

char String::operator[](int index)const
{
    return '0';
}

char &String::operator[](int index)
{
    char ret = '0';
    return ret;
}

void String::GetBytes(byte *buf, int bufsize, int index)const{}
ByteArray String::GetBytes()const{}
ByteArray String::ToHex()const{}
int String::ToInt()const
{
    return 0;
}

float String::ToFloat()const
{
    return 0;
}

double String::ToDouble()const
{
    return 0;
}

// 输出对象的字符串表示方式
String &String::ToStr(String &str)const
{
    return str;
}

// 输出对象的字符串表示方式
String String::ToString()const
{
    return  *this;
}

// 调试输出字符串
void String::Show(bool newLine)const
{
	if(newLine)
	printf("\n\r");//"0a0d"
}

// 格式化字符串，输出到现有字符串后面。方便我们连续格式化多个字符串
String &String::Format(cstring format, ...)
{
    return  *this;
}

int String::IndexOf(const char ch, int startIndex)const
{
    return 0;
}

int String::IndexOf(const String &str, int startIndex)const
{
    return 0;
}

int String::IndexOf(cstring str, int startIndex)const
{
    return 0;
}

int String::LastIndexOf(const char ch, int startIndex)const
{
    return 0;
}

int String::LastIndexOf(const String &str, int startIndex)const
{
    return 0;
}

int String::LastIndexOf(cstring str, int startIndex)const
{
    return 0;
}

bool String::Contains(const String &str)const
{
    return false;
}

bool String::Contains(cstring str)const
{
    return false;
}

bool String::StartsWith(const String &str, int startIndex)const
{
    return false;
}

bool String::StartsWith(cstring str, int startIndex)const
{
    return false;
}

bool String::EndsWith(const String &str)const
{
    return false;
}

bool String::EndsWith(cstring str)const
{
    return false;
}

StringSplit String::Split(const String &sep)const{}
StringSplit String::Split(cstring sep)const{}

String String::Substring(int start, int len)const
{
    return  *this;
}

String String::TrimStart()const
{
    return  *this;
}

String String::TrimEnd()const
{
    return  *this;
}

String String::Trim()const
{
    return  *this;
}

String String::Replace(char find, char replace)const
{
    return  *this;
}

String String::Replace(const String &find, const String &replace)const
{
    return  *this;
}

String String::Remove(int index)const
{
    return  *this;
}

String String::Remove(int index, int count)const
{
    return  *this;
}

String String::ToLower()const
{
    return  *this;
}

String String::ToUpper()const
{
    return  *this;
}

// 静态比较器。比较两个字符串指针
int String::Compare(const void *v1, const void *v2)
{
    return 0;
}

#if DEBUG
    void String::Test(){}
#endif 
void String::init(){}
void String::release(){}
bool String::Concat(cstring cstr, int length)
{
    return false;
}

String &String::copy(cstring cstr, int length)
{
    return  *this;
}

void String::move(String &rhs){}
bool String::CopyOrWrite()
{
    return false;
}


void *String::Alloc(int len)
{
    return this;
}

int String::Search(cstring str, int len, int startIndex, bool rev)const
{
    return 0;
}

StringSplit::StringSplit(const String &str, cstring sep): _Str(str){

}

const String StringSplit::Next(){}
