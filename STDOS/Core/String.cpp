/*
���е��ַ�������
 */

#include "SString.h"
#include <string.h>
#include <CType.h>
#include "Sys.h"

static char buftmp[10];

String::String(cstring cstr): Array(cstr, ArrayLength(cstr))
{
	int len;
	this->init();
	if(cstr)
		len=strlen(cstr);
	else
		len=0;
	this->_Length=len;
	if(len)
	{
		this->_Arr=(char*)cstr;
		this->_Capacity=len+1;
		this->_needFree=false;
	}
	
//    this->_Length = ArrayLength(cstr);
//    this->Copy(0, cstr, this->_Length);
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

// ��װ��̬�ַ�����ֱ��ʹ�ã��޸�ʱ����
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

// �������鳤�ȡ��ı䳤�Ⱥ�ȷ�������0��β
bool String::SetLength(int length, bool bak)
{
    return false;
}

// �������ݣ�Ĭ��-1���ȱ�ʾ��ǰ����
int String::Copy(int destIndex, const void *src, int len)
{
    return 0;
}

// �����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
int String::CopyTo(int srcIndex, void *dest, int len)const
{
    return 0;
}

// Ϊ����ֵ������һ�����ݡ�
// ���ֵΪ�ջ���Ч�������ڴ����ʧ�ܣ��ַ������ᱻ���Ϊ��Ч
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

// �����ڽ����͡����������Ч����Ϊ����ʧ��
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
    return buftmp[0];
}

void String::GetBytes(byte *buf, int bufsize, int index)const{}
#if 0
ByteArray String::GetBytes()const
{
	
}
#endif
#if 0
ByteArray String::ToHex()const
{
	
}
#endif
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

// ���������ַ�����ʾ��ʽ
String &String::ToStr(String &str)const
{
    return str;
}

// ���������ַ�����ʾ��ʽ
String String::ToString()const
{
    return  *this;
}

// ��������ַ���
void String::Show(bool newLine)const
{
    for (int i = 0;; ++i)
    {
        if (this->_Length <= i)
            break;
        SmartOS_printf("%c", this->_Arr[i]);
    }
    if (newLine)
        printf("\r\n");
}

// ��ʽ���ַ���������������ַ������档��������������ʽ������ַ���
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
#if 0
StringSplit String::Split(const String &sep)const
{
	
}
#endif
#if 0
StringSplit String::Split(cstring sep)const
{
	
}
#endif
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

// ��̬�Ƚ������Ƚ������ַ���ָ��
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

const String StringSplit::Next()
{
	return NULL;
}
