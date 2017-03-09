#pragma once

#include "Type.h"
#include "Object.h"

//拷贝的原则是尽力而为，有多少可用空间拷贝多少长度
class Buffer:public Object
{
    public:
		//打包一个指针和长度指定的数据区
		Buffer(void* ptr,int len);    
		//禁用拷贝构造函数
		Buffer(const Buffer& buf)=delete;
		//对象Mov操作，指针和长度归我，清空对方
		Buffer(Buffer& rval);
		
		//从另一个对象拷贝数据和长度，长度不足时扩容失败时报错
		Buffer& operator = (const Buffer&rhs);
		//从指针拷贝，使用我的长度
		Buffer& operator = (const void * prt);
		//对象Mov操作，指针和长度归我，清空对方
		Buffer& operator=(Buffer&& rval);
		
		//拿出指针供外部使用
		inline byte* GetBuffer(){return (byte*)Arr;}
		inline const byte* GetBuffer() const{return (byte*)Arr;}
		inline int Length() const{return _Length;}
		
		virtual bool SetLength(int len);
		//设置指定位置的值，长度不足时报错
		bool SetAt(int index,byte value);
		
		//自我索引运算符[] 返回指定元素的第一个字节
		byte operator[](int i)const;
		//支持buf[i]=0x33的语法
		byte& operator[](int i);
		
		//原始拷贝 清零 不检查边界
		static void Copy(void* dest,const void* source,int len);
		static void Zero(void * dest,int len);
		//拷贝数据 默认-1长度表示当前长度
		virtual void Copy(int destIndex, const void *src, int len=-1);//拷贝数据，默认-1长度表示当前长度   
		virtual void CopyTo(int destIndex, const void *dest, int len=-1);//const;//把数据复制到目标缓冲区，默认-1长度表示当前长度
		virtual void Copy(int destIndex, const Buffer &src, int srcIndex, int len);//拷贝数据，默认-1长度表示两者最小长度 
		int Copy(const Buffer &src, int destIndex = 0);     
		//用指定字节设置初始化一个区域
		int Set(byte item,int index,int len);
		void Clear(byte item=0);
		
		//截取一个子缓冲区，默认-1长度表示剩余全部		
		Buffer Sub(int index,int length);//截取自缓冲区  
		const Buffer Sub(int index,int len)const;
		
		//显示16禁止数据，指定分割字符和换行长度
		String& ToHex(String& str,char sep=0,int newLine=0)const;
		//显示16禁止数据，指定分割字符和换行长度
		String ToHex(char sep=0,int newLine=0);
		
		
		ushort ToUInt16() const;
		uint ToUInt32() const;
		UInt64 ToUInt64() const;
		
		void Write(ushort value,int index=0);
		void Write(short value,int index=0);
		void Write(uint value,int index=0);
		void Write(int value,int index=0);
		void Write(UInt64 value,int index=0);
		//输出对象的字符串表示方式
		virtual String& ToStr(String& str)const;
		//包装为字符串对象
		String AsString() const;
		
		explicit operator bool() const{return _Length>0;}
		bool operator!()const {return _Length==0;}
		
		int CompareTo(const Buffer& bs)const;
		int CompareTo(const void *ptr,int len=-1)const;
		
		friend bool operator==(const Buffer& bs1,const Buffer& bs2);
		friend bool operator==(const Buffer& bs1,const void* ptr);
		friend bool operator!=(const Buffer& bs1,const Buffer& bs2);
		friend bool operator!=(const Buffer& bs1,const void* ptr);
		
		#if DEBUG
		 static void Test();
		#endif
		protected:
			byte Arr[0x40];//内部缓冲区
			virtual void* Alloc(int len);
		//以下为猜测内容
		int _Length;
		
	//上面内容为新版本
		Buffer &operator = (byte* bufsrc);
		Buffer &operator = (Buffer bufsrc);
                            
        
		virtual void Show(bool newLine=false) const;
	protected:
		byte* pbuf;//缓冲区
		ushort bufLength;//缓冲区长度

};
