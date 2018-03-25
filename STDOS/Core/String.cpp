#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "SString.h"

static char buftmp[10];

int ltoa(int64_t value,char *buf,int radix)
{
	if(buf)
	{
		if(radix<= 36 && radix > 1)
		{
			bool v6 = radix == 10 && value < 0;
			uint64_t v7;
			if(v6)
			{
				v7 = -value;
				v7++;
			}
			else
			{
				v7 = value;
			}
			while(v7)
			{
				uint64_t v8 = v7%radix;
				v7/=(uint64_t)radix;
				if(v8 >= 10)
					*buf++=v8+'W';
				else
					*buf++=v8+'0';
			}
		}
		else
			return false;
	}
	else
		return false;
}
int itoa(int value,char *buf,int radix)
{
	return ltoa(value,buf,radix);
}

String utohex(uint32_t ch,int a2,char *buf,bool uppercase)
{
	if(buf)
	{
		int v6 = 2*a2&0xff;
		buf[v6]=0;
		char * v7=&buf[v6-1];
		int Aa;
		if(uppercase)
			Aa='A';
		else
			Aa='a';
		for(int i=0;i<v6;i++)
		{
			int v11=ch&0x0f;
			ch>>=4;
			if(v11 >=10)
				*v7--=v11+Aa-10;
			else
				*v7-- = v11+'0';
			
		}
		return buf;
	}
	else
		return NULL;
}


String::String(cstring cstr): Array(cstr, ArrayLength(cstr))
{
	int len;
	this->init();
	if(cstr)
		len=strlen(cstr);
	else
		len=0;
	this->_Length=len;
	if(this->_Length)
	{
		this->_Arr=(char*)cstr;
		this->_Capacity=this->_Length+1;
		this->_canWrite=false;
	}
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
String::String(char c): Array(buftmp, 0)
{
	this->init();
	this->_Arr[0]=c;
	this->_Length=1;
	
}
String::String(uint8_t value, int radix): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,radix);
}
String::String(short value, int radix): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,radix);
}
String::String(uint16_t value, int radix): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,radix);
}
String::String(int value, int radix): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,radix);
}
String::String(uint32_t value, int radix): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,radix);
}
String::String(int64_t value, int radix): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,radix);
}
String::String(uint64_t value, int radix): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,radix);
}
String::String(float value, int decimalPlaces): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,decimalPlaces);
}
String::String(double value, int decimalPlaces): Array(buftmp, 0)
{
	this->init();
	this->Concat(value,decimalPlaces);
}

// 设置数组长度。改变长度后，确保最后以0结尾
bool String::SetLength(int length, bool bak)
{
	String str;
	if(this->_Length<=length)
	{		
		if(!this->CheckCapacity(length+1,bak))
			return false;
		if(this->_Length < length)
			this->_Length=length;		
	}
	else
	{
		this->_Length=length;
	}
	this->_Arr[this->_Length]=0;
    return true;
}

// 拷贝数据，默认-1长度表示当前长度
int String::Copy(int destIndex, const void *src, int len)
{
	int ret=Buffer::Copy(destIndex,src,len);
	if(ret)
		((char*)src)[ret]=0;
	else
		ret = 0;
	return ret;
}

// 把数据复制到目标缓冲区，默认-1长度表示当前长度
int String::CopyTo(int srcIndex, void *dest, int len)const
{
	int ret = this->CopyTo(srcIndex,dest,len);
	if(ret)
		((char*)dest)[ret]=0;
	else
		ret=0;
    return ret;
}

// 为被赋值对象建立一个备份。
// 如果值为空或无效，或者内存分配失败，字符串将会被标记为无效
String &String::operator = (const String &rhs)
{
	//未验证
	if(*this!=rhs)
	{
		if(this->_Arr)
			this->copy(rhs._Arr,rhs._Length);
		else
			this->release();
	}
    return  *this;
}

String &String::operator = (cstring cstr)
{
	if(cstr)
	{
		this->copy(cstr,strlen(cstr));
	}
	else
	{
		this->release();
	}
    return  *this;
}

String &String::operator = (String && rval)
{
	if(*this !=rval)
		this->move(rval);
    return  *this;
}

// 连接内建类型。如果参数无效则认为连接失败
bool String::Concat(const Object &obj)
{
    return false;
}

bool String::Concat(const String &str)
{
	return this->Concat(str._Arr,str._Length);
}

bool String::Concat(cstring cstr)
{
	if(cstr)
	{
		return this->Concat(cstr,strlen(cstr));
	}
	else
		return false;
}

bool String::Concat(bool value)
{
	const char * cstr;
	if(value)
		cstr= "true";
	else
		cstr = "false";
    return this->Concat(cstr);
}

bool String::Concat(char c)
{
	if(this->CheckCapacity(this->_Length+1))
	{
		this->_Length++;
		this->_Arr[this->_Length-1]=c;
		return true;
	}
	else
		return false;
}

bool String::Concat(uint8_t c, int radix)
{
	char buf[20];
	for(int i=0;i<20;i++)
	{
		buf[i]=0;
	}
	if(radix !=16 && radix != -16)
	{
		itoa(c,buf,radix);
		this->Concat(buf,strlen(buf));
		
	}
	else if(this->CheckCapacity(this->_Length+2))
	{
		utohex(c,1,this->_Arr+this->_Length,true);
		this->_Length+=2;		
	}
	else
		return false;
}

bool String::Concat(short num, int radix)
{
    char buf[20];
	for(int i=0;i<20;i++)
	{
		buf[i]=0;
	}
	if(radix !=16 && radix != -16)
	{
		itoa(num,buf,radix);
		this->Concat(buf,strlen(buf));
		
	}
	else if(this->CheckCapacity(this->_Length+2))
	{
		utohex(num,1,this->_Arr+this->_Length,true);
		this->_Length+=2;		
	}
	else
		return false;
}

bool String::Concat(uint16_t num, int radix)
{
    char buf[20];
	for(int i=0;i<20;i++)
	{
		buf[i]=0;
	}
	if(radix !=16 && radix != -16)
	{
		itoa(num,buf,radix);
		this->Concat(buf,strlen(buf));
		
	}
	else if(this->CheckCapacity(this->_Length+2))
	{
		utohex(num,2,this->_Arr+this->_Length,true);
		this->_Length+=4;
	}
	else
		return false;
}

bool String::Concat(int num, int radix)
{
    char buf[20];
	for(int i=0;i<20;i++)
	{
		buf[i]=0;
	}
	if(radix !=16 && radix != -16)
	{
		itoa(num,buf,radix);
		this->Concat(buf,strlen(buf));
		
	}
	else if(this->CheckCapacity(this->_Length+2))
	{
		utohex(num,8,this->_Arr+this->_Length,true);
		this->_Length+=16;
	}
	else
		return false;
}

bool String::Concat(uint32_t num, int radix)
{
    char buf[20];
	for(int i=0;i<20;i++)
	{
		buf[i]=0;
	}
	if(radix !=16 && radix != -16)
	{
		itoa(num,buf,radix);
		this->Concat(buf,strlen(buf));
		
	}
	else if(this->CheckCapacity(this->_Length+2))
	{
		utohex(num,4,this->_Arr+this->_Length,true);
		this->_Length+=8;
	}
	else
		return false;
}

bool String::Concat(int64_t num, int radix)
{
    char buf[20];
	for(int i=0;i<20;i++)
	{
		buf[i]=0;
	}
	if(radix !=16 && radix != -16)
	{
		itoa(num,buf,radix);
		this->Concat(buf,strlen(buf));
		
	}
	else if(this->CheckCapacity(this->_Length+2))
	{
		utohex(num,8,this->_Arr+this->_Length,true);
		this->_Length+=16;
	}
	else
		return false;
}

bool String::Concat(uint64_t num, int radix)
{
    char buf[20];
	for(int i=0;i<20;i++)
	{
		buf[i]=0;
	}
	if(radix !=16 && radix != -16)
	{
		itoa(num,buf,radix);
		this->Concat(buf,strlen(buf));
		
	}
	else if(this->CheckCapacity(this->_Length+2))
	{
		utohex(num,8,this->_Arr+this->_Length,true);
		this->_Length+=16;
	}
	else
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
	return this->CompareTo(rhs)<0;
}

bool String::operator > (const String &rhs)const
{
    return this->CompareTo(rhs)>0;
}

bool String::operator <= (const String &rhs)const
{
   return this->CompareTo(rhs)<=0;
}

bool String::operator >= (const String &rhs)const
{
    return this->CompareTo(rhs)>=0;
}

char String::operator[](int index)const
{
	if(this->_Length > index && this->_Arr)
		return this->_Arr[index];
	else
		return 0;
}

char &String::operator[](int index)
{
	char aa;
	if(this->_Length> index && this->_Arr)
		return this->_Arr[index];
	else
		return aa;
}

void String::GetBytes(uint8_t *buf, int bufsize, int index)const
{
	if(bufsize && buf)
	{
		if(this->_Length > index)
		{
			if(this->_Length - index < bufsize)
				bufsize= this->_Length- index;
			Buffer* buft=new Buffer(buf,bufsize);
			buft->Copy(0,this->_Arr+index,bufsize);
		}
		else
		{
			buf[0]=0;
		}
	}
}

ByteArray String::GetBytes()const
{
//	ByteArray ret(0);
//	ret.SetLength(this->_Length);
//	this->GetBytes(ret._Arr,this->_Length,0);
//	return ret;
}

ByteArray String::ToHex()const
{
//	ByteArray ret;
//	
//	ret.SetLength(this->_Length/2);
//	for(int i=0;i<this->_Length;i+=2)
//	{
//		
//	}
//	
//	return ret;
}

int String::ToInt()const
{
	if(this->_Arr)
	{
		if(isdigit(this->_Arr[0])||this->_Arr[0]=='-')
		{
			return atoi(this->_Arr);
		}
		else
			return 0;
	}
	else
      return 0;
}

float String::ToFloat()const
{
	if(this->_Arr)
	{
		if(isdigit(this->_Arr[0])||this->_Arr[0]=='-')
		{
			return atof(this->_Arr);
		}
		else
			return 0;
	}
	else 
		return 0;
}

double String::ToDouble()const
{
    if(this->_Arr)
	{
		if(isdigit(this->_Arr[0])||this->_Arr[0]=='-')
		{
			return atof(this->_Arr);
		}
		else
			return 0;
	}
	else 
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
    for (int i = 0;; ++i)
    {
        if (this->_Length <= i)
            break;
        StdPrintf("%c", this->_Arr[i]);
    }
    if (newLine)
        StdPrintf("\r\n");
}

// 格式化字符串，输出到现有字符串后面。方便我们连续格式化多个字符串
String &String::Format(cstring format, ...)
{
    return  *this;
}

int String::IndexOf(const char ch, int startIndex)const
{
	if(startIndex >=0)
	{
		if(this->Length() > startIndex)
		{
			for(int i=startIndex;i<this->Length();i++)
			{
				if(this->_Arr[i]==ch)
					return i;
			}
			return -1;
		}
		else
			return -1;
	}
	else
		return -1;
}

int String::IndexOf(const String &str, int startIndex)const
{
    return this->Search(this->Arr,this->Length(),startIndex,0);
}

int String::IndexOf(cstring str, int startIndex)const
{
	return this->Search(str,strlen(str),startIndex,0);
}

int String::LastIndexOf(const char ch, int startIndex)const
{
	if(this->_Length > startIndex)
	{
		for(int i=this->_Length-1;i>startIndex;i--)
		{
			if(this->_Arr[i]==ch)
				return i;
		}
		return -1;
	}
	else
		return -1;
}

int String::LastIndexOf(const String &str, int startIndex)const
{
    return this->Search(str._Arr,str._Length,startIndex,1);
}

int String::LastIndexOf(cstring str, int startIndex)const
{
	return this->Search(str,strlen(str),startIndex,1);
}

bool String::Contains(const String &str)const
{
    return this->IndexOf(str,0)>0;
}

bool String::Contains(cstring str)const
{
	return this->IndexOf(str,0)>0;
}

bool String::StartsWith(const String &str, int startIndex)const
{
	if(this->_Arr && str)
	{
		int len=str._Length;
		if(len)
		{
			if((len+startIndex) <= this->_Length)
				return strncmp(this->_Arr+startIndex,str._Arr,len)==0;
			else
				return false;
		}
		return false;
	}
	else
		return false;
}

bool String::StartsWith(cstring str, int startIndex)const
{
	if(this->_Arr && str)
	{
		int len=strlen(str);
		if(len)
		{
			if((len+startIndex) <= this->_Length)
				return strncmp(this->_Arr+startIndex,str,len)==0;
			else
				return false;
		}
		return false;
	}
	else
		return false;
}

bool String::EndsWith(const String &str)const
{
    if(this->_Arr && str)
	{
		int len=str.Length();
		if(len)
		{
			if(this->_Length>=len)
			{
				return (strncmp(this->_Arr+this->_Length - len,str._Arr,_Length)==0);
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

bool String::EndsWith(cstring str)const
{
	if(this->_Arr && str)
	{
		int len=strlen(str);
		if(len)
		{
			if(this->_Length>=len)
			{
				return (strncmp(this->_Arr+this->_Length - len,str,_Length)==0);
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

StringSplit String::Split(const String &sep)const
{
	return StringSplit(*this,sep._Arr);
}


StringSplit String::Split(cstring sep)const
{
	return StringSplit(*this,sep);
}

String String::Substring(int start, int len)const
{
//	char * ret;
//	if(len < 0 )
//		ret=this->_Arr-start;
//	if(this->_Arr-start < len)
//		ret=this->_Arr-start;
	return String(this->_Arr+start,len);
}

void trim(char * &buf,int& len,bool Start=true,bool End=true)
{
	int posstart=0;
	int posend = len-1;
	if(buf)
	{
		if(Start)
		{
			while(isspace(buf[posstart]))
				++posstart;
		}
		
		
		if(End)
		{
			while(isspace(buf[posend])&&posend>posstart)
				--posend;
		}
		buf=buf+posstart; 
		len=posend-posstart+1;		
	}
	
}
String String::TrimStart()const
{
    char *buf = this->_Arr;
    int len = this->_Length;
    trim(buf, len, true, false);

    return String(buf, len);
}

String String::TrimEnd()const
{
    char *buf = this->_Arr;
    int len = this->_Length;
    trim(buf, len, false, true);
    return String(buf, len);
}

String String::Trim()const
{
    char *buf = this->_Arr;
    int len = this->_Length;
    trim(buf, len, true, true);

    return String(buf, len);
}

String String::Replace(char find, char replace)const
{
	for(int i=0;i<this->_Length;i++)
	{
		if(this->_Arr[i]==find)
			this->_Arr[i]=replace;
	}
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
	if(!this->_canWrite)
		return *this;
	for(int i=0;i<this->Length();i++)
	{
		this->_Arr[i]=tolower(this->_Arr[i]);
	}
    return  *this;
}

String String::ToUpper()const
{
	if(!this->_canWrite)
		return *this;
	for(int i=0;i<this->Length();i++)
	{
		this->_Arr[i]=toupper(this->_Arr[i]);
	}
    return  *this;
}

// 静态比较器。比较两个字符串指针
int String::Compare(const void *v1, const void *v2)
{
    if(v2)
	{
		if(v1)
		{
			return strncmp((const char *)v2,(const char *)v1,strlen((const char *)v2));
		}
		else 
			return 1;
	}
	else if(v2 == v1)
	{
		return 0;
	}
	else
		return -1;
}

void String::init()
{
	this->_Arr=this->Arr;
	this->_Capacity = 63;
	this->_canWrite=true;
	this->_needFree = false;
}
void String::release()
{
	this->Array::Release();
	this->init();
}
bool String::Concat(cstring cstr, int length)
{
	if(cstr)
	{
		if(length)
		{
			if(this->CheckCapacity(this->_Length+length))
			{
				Buffer bs((void*)cstr,length);
				this->Buffer::Copy(bs);
				return true;
			}
			else
				return false;
		}
		else
			return true;
	}
	else	
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
	if(len>64)
	{
		this->_Size=1;
		this->_Arr=new char(len);
	}
	else
	{
		this->_Size=0;
		
	}
    return this;
}
bool String::CheckCapacity(int size)
{
	void *src=this->_Arr;
	this->Array::CheckCapacity(size+1,true);
	if(src == this->_Arr)
		return true;
	else
	{
		return true;
	}
}
int String::Search(cstring str, int len, int startIndex, bool rev)const
{
	if(str)
	{
		if(startIndex >= 0)
		{
			if(startIndex <= this->_Length)
			{
				for(int i=0;i<this->_Length;i++)
				{
					if(!strncmp(str,this->_Arr+startIndex+i,len))
						return i;
				}
				return -1;
			}
			else
				return -1;
		}
		else
			return -1;
	}
	else
		return -1;
}

StringSplit::StringSplit(const String &str, cstring sep): _Str(str){

}

const String StringSplit::Next()
{
	return NULL;
}

