#ifndef _Stream_H_
#define _Stream_H_
#include "Buffer.h"

// ������
// ����������һ�����������α�λ�ã����ݳ��ȡ�ʵ����Ч���ݽ�ռ�û������м䲿�֣�ͷβ��������ʣ��
class Stream
{
protected:
	byte* _Buffer;	// ���ݻ����������ݺ�����·��仺����
	int _Capacity;	// ����������
    int _Position;	// �α�λ��

	void Init(void* buf, int len);
	virtual bool CheckRemain(int count);
public:
	int	Length;	// ���ݳ���
	bool Little;	// Ĭ��С�ֽ��򡣽�Ӱ�����ݶ�д����
	bool CanWrite;	// �Ƿ��д
	bool CanResize;	// �Ƿ�����Զ�����

	// ʹ�û�������ʼ����������ע�⣬��ʱָ��λ��0�������ݳ���Ϊ����������
	Stream(void* buf, int len);
	Stream(const void* buf, int len);
	// ʹ���ֽ������ʼ����������ע�⣬��ʱָ��λ��0�������ݳ���Ϊ����������
	explicit Stream(Buffer& bs);
	explicit Stream(const Buffer& bs);
	virtual ~Stream();

	// ����������
	int Capacity() const;
	void SetCapacity(int len);
	// ��ǰλ��
	int Position() const;
	// ����λ��
	bool SetPosition(int p);
	// ���µ���Ч���������ȡ�0��ʾ�Ѿ������յ�
	int Remain() const;
	// ����ǰ���ƶ�һ�ξ��룬���سɹ�����ʧ�ܡ����ʧ�ܣ����ƶ��α�
	bool Seek(int offset);

	// ������ָ�롣ע�⣺���ݺ�ָ���ı䣡
    byte* GetBuffer() const;
	// ��������ǰλ��ָ�롣ע�⣺���ݺ�ָ���ı䣡
    byte* Current() const;

	// ��ȡ7λѹ����������
	int ReadEncodeInt();
	// ��ȡ���ݵ��ֽ����飬���ֽ�����ָ����С������������ǰ׺
	int Read(Buffer& bs);

	// д��7λѹ����������
	int WriteEncodeInt(int value);
	// ���ֽ����������д�뵽������������������ǰ׺
	bool Write(const Buffer& bs);

	// ����������ȡ�䳤���ݵ��ֽ����顣��ѹ��������ͷ��ʾ����
	int ReadArray(Buffer& bs);
	ByteArray ReadArray(int count);
	// ���ֽ�������Ϊ�䳤����д�뵽����������ѹ��������ͷ��ʾ����
	bool WriteArray(const Buffer& bs);

	ByteArray ReadArray();
	String ReadString();

	int		ReadByte();
	ushort	ReadUInt16();
	uint	ReadUInt32();
	UInt64	ReadUInt64();

	bool Write(byte value);
	bool Write(ushort value);
	bool Write(uint value);
	bool Write(UInt64 value);
	//bool Write(sbyte value)	{ return Write((byte)value); }
	bool Write(short value)	{ return Write((ushort)value); }
	bool Write(int value)	{ return Write((uint)value); }
	bool Write(Int64 value)	{ return Write((UInt64)value); }

	// ȡ��ָ���ṹ��ָ�룬���ƶ��α�λ��
	template<typename T>
	T* Retrieve(bool move = true)
	{
		int p = sizeof(T) + _Position;
		// �����ƶ������һ���ֽ�֮��Ҳ����Length
		if(p < 0 || p > Length) return nullptr;

		T* pt = (T*)Current();
		if(move && !Seek(sizeof(T))) return nullptr;

		return pt;
	}

	// ��ȡָ�����ȵ����ݲ��������ֽ�ָ�룬�ƶ�������λ��
	byte* ReadBytes(int count = -1);

	// ��ȡһ���ֽڣ����ƶ��αꡣ���û�п������ݣ��򷵻�-1
	int Peek() const;
};

// �ڴ���������Ԥ����ռ䣬�Զ�����
class MemoryStream : public Stream
{
private:
	byte _Arr[0x40];	// �ڲ�����������С�ڴ���Ҫʱ��ֱ��ʹ��ջ���䣬������ܡ�
	bool _needFree;		// �Ƿ��Զ��ͷ�
	//bool _resize;		// �Ƿ�����Զ�����

	virtual bool CheckRemain(int count);

public:
	// ����ָ����С��������
	MemoryStream(int len = 0);
	// ʹ�û�������ʼ����������֧���Զ�����
	MemoryStream(void* buf, int len);
	// ����������
	virtual ~MemoryStream();
};

#endif
#if 0
#pragma once

#include "ByteArray.h"

class Stream: public Object
{
    public:
        //����������
        uint Capacity()const;
        void SetCapacity(uint len);
        //��ǰλ��
        uint Position()const;
        //����λ��
        bool SetPosition(int p);
        //���µ���Ч���������ȣ�0��ʾ�Ѿ������յ�
        uint Remain()const;
        //����ǰ���ƶ�һ�ξ��룬���سɹ�����ʧ�ܡ����ʧ�ܣ����ƶ��α�
        bool Seek(int offset);

        //������ָ�룬ע�⣺���ݺ�ָ���ı�
        byte *GetBuffer()const;
        //������udagnqianλ��ָ�롣ע�⣺���ݺ�ָ���ı�
        byte *Current()const;

        //��ȡ7Ϊѹ����������
        uint ReadEncodeInt();
        //��ȡ���ݵ��ֽ����飬�����ֽ�����ָ����С������������ǰ׺
        uint Read(Buffer &bs);

        //д��7λѹ����������
        uint WriteEncodeInt(uint value);
        //���ֽ����������д�뵽������������������ǰ׺
        bool Write(const Buffer &bs);
        //����������ȡ�䳤���ݵ��ֽ����顣��ѹ��������ͷ��ʾ����
        uint ReadArray(Buffer &bs);
        ByteArray ReadArray(int count);
        //���ֽ�������Ϊ�䳤����д�뵽����������ѹ��������ͷ��ʾ����
        bool WriteArray(const Buffer &bs);
    private:
        byte *pbuf; //������
        int pos; //��ǰָ��
		int capcity;//����

};
#endif
