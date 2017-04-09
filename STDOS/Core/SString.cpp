/*
进行到字符串搜索
 */

#include "SString.h"
#include <stdio.h>
#include <string.h>
#include <CType.h>
#include "Sys.h"

byte buftmp[10];

String::String(cstring cstr): Array(cstr, ArrayLength(cstr))
{
    this->_Length = ArrayLength(cstr);
    this->Copy(0, cstr, this->_Length);
}

String::String(const String &str): Array(str.GetBuffer(), str.Length())
{

}
String::String(String&& rval):Array(rval.GetBuffer(), rval.Length())
{
	
}
String::String(char* str, int length):Array(str,length)
{
	
}
String::String(char* str, int length, bool expand):Array(str,length)
	{
		this->Expand=expand;
	}
	// 包装静态字符串，直接使用，修改时扩容
	String::String(cstring str, int length):Array(str,length)
	{
	}
String::String(bool value):Array(value?"true":"false",value?5:6)
{
	
}
String::String(char c):Array(buftmp,10)
{
}
	String::String(byte value, int radix):Array(buftmp,10)
	{
	}
	String::String(short value, int radix):Array(buftmp,10)
	{
	}
	String::String(ushort value, int radix):Array(buftmp,10)
	{
	}
	String::String(int value, int radix ):Array(buftmp,10)
	{
	}
	String::String(uint value, int radix):Array(buftmp,10)
	{
	}
	String::String(Int64 value, int radix):Array(buftmp,10)
	{
	}
	String::String(UInt64 value, int radix):Array(buftmp,10)
	{
	}
	String::String(float value, int decimalPlaces):Array(buftmp,10)
	{
	}
	String::String(double value, int decimalPlaces):Array(buftmp,10)
	{
	}

// 拷贝数据，默认-1长度表示当前长度
	int String::Copy(int destIndex, const void* src, int len)
	{
		return 0;
	}
	
	// 把数据复制到目标缓冲区，默认-1长度表示当前长度
	int String::CopyTo(int srcIndex, void* dest, int len) const
	{
		return 0;
	}

	// 为被赋值对象建立一个备份。
	// 如果值为空或无效，或者内存分配失败，字符串将会被标记为无效
	String& String::operator = (const String& rhs)
	{
		return *this;
	}
	String& String::operator = (cstring cstr)
	{
		return *this;
	}
	String& String::operator = (String&& rval)
	{
		return *this;
	}

	// 连接内建类型。如果参数无效则认为连接失败
	bool String::Concat(const Object& obj)
	{
		return false;
	}
	bool String::Concat(const String& str)
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
	bool String::Concat(byte c, int radix )
	{
		return false;
	}
	bool String::Concat(short num, int radix )
	{
		return false;
	}
	bool String::Concat(ushort num, int radix )
	{
		return false;
	}
	bool String::Concat(int num, int radix )
	{
		return false;
	}
	bool String::Concat(uint num, int radix )
	{
		return false;
	}
	bool String::Concat(Int64 num, int radix )
	{
		return false;
	}
	bool String::Concat(UInt64 num, int radix )
	{
		return false;
	}
	bool String::Concat(float num, int decimalPlaces )
	{
		return false;
	}
	bool String::Concat(double num, int decimalPlaces )
	{
		return false;
	}
	int String::CompareTo(const String& s) const
	{
		return 0;
	}
	int String::CompareTo(cstring cstr, int len , bool ignoreCase ) const
	{
		return 0;
	}
	bool String::Equals(const String& s) const
	{
		return false;
	}
	bool String::Equals(cstring cstr) const
	{
		return false;
	}
	bool String::EqualsIgnoreCase(const String& s) const
	{
		return false;
	}
	bool String::EqualsIgnoreCase(cstring cstr) const
	{
		return false;
	}
	bool String::operator <  (const String& rhs) const
	{
		return false;
	}
	bool String::operator >  (const String& rhs) const
	{
		return false;
	}
	bool String::operator <= (const String& rhs) const
	{
		return false;
	}
	bool String::operator >= (const String& rhs) const
	{
		return false;
	}
	
	char String::operator [] (int index) const
	{
		return '0';
	}
	char& String::operator [] (int index)
	{
		char ret='0';
		return ret;
	}
	void String:: GetBytes(byte* buf, int bufsize, int index) const
	{
	}
	ByteArray String::GetBytes() const
	{
	}
	ByteArray String::ToHex() const
	{
	}
	int String::ToInt() const
	{
		return 0;
	}
	float String::ToFloat() const
	{
		return 0;
	}
	double String::ToDouble() const
	{
		return 0;
	}

	// 输出对象的字符串表示方式
	String& String::ToStr(String& str) const
	{
		return str;
	}
	// 输出对象的字符串表示方式
	String String::ToString() const
	{
		return *this;
	}

	// 调试输出字符串
	void String::Show(bool newLine) const
	{
	}

	// 格式化字符串，输出到现有字符串后面。方便我们连续格式化多个字符串
	String& String::Format(cstring format, ...)
	{
		return *this;
	}

	int String::IndexOf(const char ch, int startIndex ) const
	{
		return 0;
	}
	int String::IndexOf(const String& str, int startIndex ) const
	{
		return 0;
	}
	int String::IndexOf(cstring str, int startIndex ) const
	{
		return 0;
	}
	int String::LastIndexOf(const char ch, int startIndex ) const
	{
		return 0;
	}
	int String::LastIndexOf(const String& str, int startIndex ) const
	{
		return 0;
	}
	int String::LastIndexOf(cstring str, int startIndex ) const
	{
		return 0;
	}
	bool String::Contains(const String& str) const
	{
		return false;
	}
	bool String::Contains(cstring str) const
	{
		return false;
	}
	bool String::StartsWith(const String& str, int startIndex ) const
	{
		return false;
	}
	bool String::StartsWith(cstring str, int startIndex ) const
	{
		return false;
	}
	bool String::EndsWith(const String& str) const
	{
		return false;
	}
	bool String::EndsWith(cstring str) const
	{
		return false;
	}

	StringSplit String::Split(const String& sep) const
	{
	}
	StringSplit String::Split(cstring sep) const
	{
	}

	String String::Substring(int start, int len) const
	{
		return *this;
	}
	String String::TrimStart() const
	{
		return *this;
	}
	String String::TrimEnd() const
	{
		return *this;
	}
	String String::Trim() const
	{
		return *this;
	}
	String String::Replace(char find, char replace) const
	{
		return *this;
	}
	String String::Replace(const String& find, const String& replace) const
	{
		return *this;
	}
	String String::Remove(int index) const
	{
		return *this;
	}
	String String::Remove(int index, int count) const
	{
		return *this;
	}
	String String::ToLower() const
	{
		return *this;
	}
	String String::ToUpper() const
	{
		return *this;
	}

	// 静态比较器。比较两个字符串指针
	int String::Compare(const void* v1, const void* v2)
	{
		return 0;
	}

#if DEBUG
	void String::Test()
	{
	}
#endif
void String::init()
	{
	}
	void String::release()
	{
	}
	bool String::Concat(cstring cstr, int length)
	{
		return false;
	}

	String& String::copy(cstring cstr, int length)
	{
		return *this;
	}
	void String::move(String& rhs)
	{
	}
	bool String::CopyOrWrite()
	{
		return false;
	}

	
	void* String::Alloc(int len)
	{
		return this;
	}

int String::Search(cstring str, int len, int startIndex, bool rev) const
	{
		return 0;
	}
	
	StringSplit::StringSplit(const String& str, cstring sep):_Str(str)
	{
		
	}

const String StringSplit::Next()
{
}






















//String::String(char ch)
//{
////    this->initCapacity();
//    ((byte*)(this->_Arr))[0] = ch;
////    this->jsLength();
//}

//进制字符串
//String::String(byte value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
//            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
//            break;
//        case 16:
//            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
//            break;
//    }
// //   this->jsLength();
//}

//进制字符串
//String::String(short value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
//            break;
//        case 16:
//    //        snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
//            break;
//    }
////    this->jsLength();
//}

//进制字符串
//String::String(int value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
//            break;
//        case 16:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
//            break;
//    }
// //   this->jsLength();
//}

//进制字符串
//String::String(uint value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%u", value);
//            break;
//        case 16:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
//            break;
//    }
////    this->jsLength();
//}

//进制字符串
//String::String(Int64 value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%lld", value);
//            break;
//        case 16:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llX", value);
//            break;
//    }
////    this->jsLength();
//}

//进制字符串
//String::String(UInt64 value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llu", value);
//            break;
//        case 16:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llX", value);
//            break;
//    }
////    this->jsLength();
//}

//浮点数
//String::String(float value, int dot)
//{
////    this->initCapacity();
//    switch (dot)
//    {
//        case 0:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.0f", value);
//            break;
//        case 1:
//   //         snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.1f", value);
//            break;
//        case 2:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.2f", value);
//            break;
//        case 3:
// //           snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.3f", value);
//            break;
//        case 4:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.4f", value);
//            break;
//        case 5:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.5f", value);
//            break;
//        default:
//            break;
//    }
////    this->jsLength();
//}

//浮点数
//String::String(double value, int dot)
//{
////    this->initCapacity();
//    switch (dot)
//    {
//        case 0:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.0lf", value);
//            break;
//        case 1:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.1lf", value);
//            break;
//        case 2:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.2lf", value);
//            break;
//        case 3:
// //           snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.3lf", value);
//            break;
//        case 4:
// //           snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.4lf", value);
//            break;
//        case 5:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.5lf", value);
//            break;
//        default:
//            break;
//    }
////    this->jsLength();
//}

//String::String(const String &str1)
//{
//    #if 1
//  //      this->initCapacity();
//    #endif 
//    #if 1
//        for (int i = 0; i < str1.Length(); i++)
//        {
//            ((byte*)(this->_Arr))[i] = str1.GetBuffer()[i];
//        }
//  //      this->jsLength();
//    #endif 
//}
#if 0
String &String::operator = (const char *str)
{
    //    if (strlen(str) <= this->mcapacity)
    {
        //        this->_Length = strlen(str);
    }
    //   else
    {
        //        this->_Length = this->mcapacity;
    }
    for (int i = 0; i < this->_Length; i++)
    {
        ((byte*)(this->_Arr))[i] = str[i];
    }
    ((byte*)(this->_Arr))[this->_Length] = 0;
    return  *this;
}
#endif
#if 0
String &String::operator = (const String &str)
{
    //    if (str.Length() <= this->mcapacity)
    {
        //        this->_Length = str.Length();
    }
    //   else
    {
        //        this->_Length = this->mcapacity;
    }
    for (int i = 0; i < this->_Length; i++)
    {
        ((byte*)(this->_Arr))[i] = str.GetBuffer()[i];
    }
    ((byte*)(this->_Arr))[this->_Length] = 0;
    return  *this;
}
#endif
#if 0
    bool String::operator == (const char *str)
    {
        if (strlen(str) != this->_Length)
        {
            return false;
        }
        for (int i = 0; i < this->_Length; i++)
        {
            if (((byte*)(this->_Arr))[i] != str[i])
            {
                return false;
            }
        }
        return true;
    }
#endif
#if 0	
int String::Copy(int destIndex, const void *src, int len)
{
    return len;
}
#endif
#if 0
bool String::Equals(const String &str1)const
{
    if (this->Length() != str1._Length)
    {
        return false;
    }
    for (int i = 0; i < this->_Length; i++)
    {
        if (((byte*)(this->_Arr))[i] != ((byte*)str1._Arr)[i])
        {
            return false;
        }
    }
    return true;
}
#endif
#if 0
bool String::Equals(cstring cstr)const
{
    return true;
}
#endif
#if 0
    String &String::operator += (const DateTime &dt)
    {
        if ((this->_Length + 19) < this->Capacity())
        {
            this->_Length += 19;
            snprintf((char*) &((byte*)(this->_Arr))[this->_Length - 1-18], 20, "%04d-%02d-%02d %02d:%02d:%02d", dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second);
        }
        return  *this;
    }
#endif 
#if 0
    String &String::operator += (const String &str)
    {
        if ((str.Length() + this->_Length) < this->mcapacity)
        {
            this->_Length += str.Length();
            this->Copy(this->_Length - str.Length(), str.GetBuffer(), str.Length());
        }

        return  *this;
    }
#endif 
#if 0
    String &String::operator += (const float da)
    {
        byte buf[100] = 
        {
            0
        };
        snprintf((char*)buf, 100, "%2.2f", da);
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(byte value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%d", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%X", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(short value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%d", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%X", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(int value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%d", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%X", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(uint value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%u", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%X", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(Int64 value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%lld", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%llX", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(UInt64 value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%llu", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%llX", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(float value, byte dot)
    {
        byte buf[100] = 
        {
            0
        };
        switch (dot)
        {
            case 0:
                snprintf((char*)buf, 100, "%0.0f", value);
                break;
            case 1:
                snprintf((char*)buf, 100, "%1.1f", value);
                break;
            case 2:
                snprintf((char*)buf, 100, "%2.2f", value);
                break;
            case 3:
                snprintf((char*)buf, 100, "%3.3f", value);
                break;
            case 4:
                snprintf((char*)buf, 100, "%4.4f", value);
                break;
            default:
                break;
        }

        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 

#if 0

    String &String::Concat(double value, byte dot)
    {
        byte buf[100] = 
        {
            0
        };
        switch (dot)
        {
            case 0:
                snprintf((char*)buf, 100, "%0.0f", value);
                break;
            case 1:
                snprintf((char*)buf, 100, "%1.1f", value);
                break;
            case 2:
                snprintf((char*)buf, 100, "%2.2f", value);
                break;
            case 3:
                snprintf((char*)buf, 100, "%3.3f", value);
                break;
            case 4:
                snprintf((char*)buf, 100, "%4.4f", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
byte ToInt(char ch)
{
    ch = toupper(ch);
    if (ch <= '9')
    {
        return ch - '0';
    }
    return ch - 'A' + 10;
}
#endif
#if 0
    String String::ToHex()
    {
        byte buf[100];
        byte bi = 0;
        byte da;
        int j = 0;
        for (int i = 0; i < 100; i++)
        {
            buf[i] = 0;
        }
        for (int i = 0; i < this->_Length; i++)
        {

            if (isxdigit(((byte*)(this->_Arr))[i]))
            {
                if (bi == 0)
                {
                    da = ToInt(((byte*)(this->_Arr))[i]);
                    da <<= 4;
                    bi = 1;
                }
                else
                {
                    da |= ToInt(((byte*)(this->_Arr))[i]);
                    buf[j] = da;
                    j++;
                    bi = 0;
                }
            }
            else
            {
                bi = 0;
            }
        }

        return String((const char*)buf);
    }
#endif 
#if 0
    //数组容量
    uint String::Capacity()const
    {
        return this->mcapacity;
    }
#endif 
#if 0
void String::Show(bool newLine)const
{
    printf("%s", ((byte*)(this->_Arr)));
    newLine ? printf("\n"): 0;
}
#endif
//初始化容器
//void String::initCapacity()
//{
//    this->_Arr = new char[0x40];
////    this->mcapacity = 0x40 - 1;
//    memset(((byte*)(this->_Arr)), 0x40, 0);
//}

//计算字符串长度
//void String::jsLength()
//{
//    this->_Length = strlen((char*)((byte*)(this->_Arr)));
//}

#ifdef DEBUG
#if 0
    void String::Test()
    {
        char err[] = "String Err\r\n";
        //默认空字符串，使用内部数据区
        String str;
        assert(str.Length() == 0, err);
        assert(str.Capacity() == 0x40 - 1, err);

        String str1("456");
        assert(str1 == "456", err);
        assert(str1.GetBuffer() == "456", err);

        //		char err1[] = "String(const String& str)";


    }
	#endif
#endif
