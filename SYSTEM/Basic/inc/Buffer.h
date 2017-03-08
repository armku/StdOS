/*
������ Buffer
Buffer ��ʾһ�����������ڲ���һ��ָ���һ��������ɡ�
����ϵͳ��Ʋ������䵥�ֽ�ָ�룬 �Ա������������������ڴ�й©
��Ƿ��޸�������������ݡ�
���ò�����
1) ʹ��ָ��ͳ��ȹ���һ����������GetBuffer ����ָ�룬Length ���س��ȡ�
Buffer ��ֱ����ָ�������ȱȽϡ�
2) ��Դָ�뿽�����ݵ�����������ֵ���ǿ������ݶ������滻�ڲ�ָ�롣
�������̣�bs=bts ����� bts ���� 4 ���ֽڵ� bs��Ҳ���� buf �����ڴ�����
���Դָ���������ڻ��������ȣ� ��ô����Դָ�����ݺ�����ſ���������
�ӻ������������ݵ���һ�����������ر�ע��Դ���������ȴ���Ŀ�껺����
�������
�ı仺�������� SetLength��
SetLength ���԰ѻ������������õĸ�С��������ø������漰�Զ�����
���⡣
��Դ���������ȴ���Ŀ�껺����ʱ��Buffer ����ᱨ������ Buffer ������
Array/ByteArray/String �Ȼ���� SetLength �Զ����ݣ��Ӵ󻺳�����
5) �߼����� Copy/CopyTo��
֧�ֻ������ͻ�������������������ָ��֮��ָ��ƫ�ƺͳ��ȵĸ߼�������
�ر�ע�⣬Ŀ�껺�������Ȳ���ʱ���ο� SetLength ����ԭ��
6) ��ȡ�ӻ����� Sub
7) תΪʮ�������ַ��� ToHex
8) ���������������ݻ��� ToUInt32/Write
9) ����������ֱ����Ϊ�ַ��� AsString()
 */
#pragma once

#include "Type.h"
#include "Object.h"

//������ԭ���Ǿ�����Ϊ���ж��ٿ��ÿռ俽�����ٳ���
class Buffer:public Object
{
    public:
		//���һ��ָ��ͳ���ָ����������
		Buffer(void* ptr,int len);    
		//���ÿ������캯��
		Buffer(const Buffer& buf)=delete;
		//����Mov������ָ��ͳ��ȹ��ң���նԷ�
		Buffer(Buffer&& rval);
		
		//����һ�����󿽱����ݺͳ��ȣ����Ȳ���ʱ����ʧ��ʱ����
		Buffer& operator = (const Buffer&rhs);
		//��ָ�뿽����ʹ���ҵĳ���
		Buffer& operator = (const void * prt);
		//����Mov������ָ��ͳ��ȹ��ң���նԷ�
		Buffer& operator=(Buffer&& rval);
		
		//�ó�ָ�빩�ⲿʹ��
		inline byte* GetBuffer(){return (byte*)_Arr;}
		inline const byte* GetBuffer() const{return (byte*)_Arr;}
		inline int Length() const{return _Length;}
		
		virtual bool SetLength(int len);
		//����ָ��λ�õ�ֵ�����Ȳ���ʱ����
		bool SetAt(int index,byte value);
		
		//�������������[] ����ָ��Ԫ�صĵ�һ���ֽ�
		byte operator[](int i)const;
		//֧��buf[i]=0x33���﷨
		byte& operator[](int i);
		
		//ԭʼ���� ���� �����߽�
		static void Copy(void* dest,const void* source,int len);
		static void Zero(void * dest,int len);
		//�������� Ĭ��-1���ȱ�ʾ��ǰ����
		virtual void Copy(int destIndex, const void *src, int len=-1);//�������ݣ�Ĭ��-1���ȱ�ʾ��ǰ����   
		virtual void CopyTo(int destIndex, const void *dest, int len=-1);//const;//�����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
		virtual void Copy(int destIndex, const Buffer &src, int srcIndex, int len);//�������ݣ�Ĭ��-1���ȱ�ʾ������С���� 
		int Copy(const Buffer &src, int destIndex = 0);     
		//��ָ���ֽ����ó�ʼ��һ������
		int Set(byte item,int index,int len);
		void Clear(byte item=0);
		
		//��ȡһ���ӻ�������Ĭ��-1���ȱ�ʾʣ��ȫ��		
		Buffer Sub(int index,int length);//��ȡ�Ի�����  
		const Buffer Sub(int index,int len)const;
		
		//��ʾ16��ֹ���ݣ�ָ���ָ��ַ��ͻ��г���
		String& ToHex(String& str,char sep=0,int newLine=0)const;
		//��ʾ16��ֹ���ݣ�ָ���ָ��ַ��ͻ��г���
		String ToHex(char sep=0,int newLine=0);
		
		
		ushort ToUInt16() const;
		uint ToUInt32() const;
		UInt64 ToUInt64() const;
		
		void Write(ushort value,int index=0);
		void Write(short value,int index=0);
		void Write(uint value,int index=0);
		void Write(int value,int index=0);
		void Write(UInt64 value,int index=0);
		//���������ַ�����ʾ��ʽ
		virtual String& ToStr(String& str)const;
		//��װΪ�ַ�������
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
			byte Arr[0x40];//�ڲ�������
			virtual void* Alloc(int len);
		//����Ϊ�²�����
		int _Length=0x40;
		
	//��������Ϊ�°汾
		
		
		
		
	
		
		Buffer(char* buf,ushort len=-1);
		Buffer &operator = (byte* bufsrc);
		Buffer &operator = (Buffer bufsrc);
		byte &operator [] (int pos);
        byte* GetBuffer() const;//����ָ��        
        int Length() const;//����     		
        void SetLength(ushort len);//���ó��ȣ����Զ�����        
             
        
               
           
              
        #if 0
		String ToHex();//ת��Ϊ16�����ַ���        
		#endif
		#if 0
        void AsString();//����������ֱ����Ϊ�ַ���
		#endif
		virtual void Show(bool newLine=false) const;
	protected:
		byte* pbuf;//������
		ushort bufLength;//����������

};
