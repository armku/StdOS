#include "ByteArray.h"
#include "Stream.h"

ushort _REV16(ushort a1)
{
  return (ushort)(a1 << 8) | (a1 >> 8);
}
uint _REV(uint a1)
{
  ushort v1;
  uint v2;

  v1 = a1;
  v2 = _REV16(a1 >> 16);
  return v2 | (_REV16(v1) << 16);
}

void Stream::Init(void *buf, int len)
{
    if (!buf)
    {
        //assert_failed2("buf,%s",__FILE__);
    }

    this->_Buffer = (byte*)buf;
    this->_Capacity = len;
    this->_Position = 0;
	this->Length=len;
	this->Little=true;
	this->CanWrite=true;
	this->CanResize=true;
}

bool Stream::CheckRemain(int count)
{
    if (count <= this->_Capacity)
    {
        return true;
    }
    else
    {
        if (this->_Buffer)
        {
            //debug_printf("(%d - %d) = %d  %d \r\n",this);
        }
        else
        {
            //assert_failed2((const char *)dword_2AC, __FILE__, 0x3Fu);
            return false;
        }
        return false;
    }
}

// ʹ�û�������ʼ����������ע�⣬��ʱָ��λ��0�������ݳ���Ϊ����������
Stream::Stream(void *buf, int len)
{
    this->Init(buf, len);
}

Stream::Stream(const void *buf, int len)
{
    this->Init((void*)buf, len);
}

// ʹ���ֽ������ʼ����������ע�⣬��ʱָ��λ��0�������ݳ���Ϊ����������
Stream::Stream(Buffer &bs)
{
    this->Init(bs.GetBuffer(), bs.Length());
}

Stream::Stream(const Buffer &bs)
{
    this->Init((void*)bs.GetBuffer(), bs.Length());
}

Stream::~Stream(){}

// ����������
int Stream::Capacity()const
{
    return this->_Capacity;
}

void Stream::SetCapacity(int len)
{
    this->CheckRemain(len);
}

// ��ǰλ��
int Stream::Position()const
{
    return this->_Position;
}

// ����λ��
bool Stream::SetPosition(int p)
{
    if (p >= 0 || this->_Capacity >= p)
    {
        this->_Position = p;
        return true;
    }
    else
    {
        //debug_printf((const char *)sub_2F4);
        return false;
    }
}

// ���µ���Ч���������ȡ�0��ʾ�Ѿ������յ�
int Stream::Remain()const
{
    return this->_Capacity - this->_Position;
}

// ����ǰ���ƶ�һ�ξ��룬���سɹ�����ʧ�ܡ����ʧ�ܣ����ƶ��α�
bool Stream::Seek(int offset)
{
    if (offset)
    {
        return this->SetPosition(this->_Position + offset);
    }
    else
    {
        return true;
    }
}

// ������ָ�롣ע�⣺���ݺ�ָ���ı䣡
byte *Stream::GetBuffer()const
{
    return this->_Buffer;
}

// ��������ǰλ��ָ�롣ע�⣺���ݺ�ָ���ı䣡
byte *Stream::Current()const
{
    return this->_Buffer + this->_Position;
}

// ��ȡ7λѹ����������
int Stream::ReadEncodeInt()
{
    int ret = 0;
    int v3 = 0;
    for (int i = 0; v3 < 4; i += 7)
    {
        int v5 = this->ReadByte();
        if (v5 < 0)
            break;
        ret |= (v5 &0x7F) << i;
        if (!(v5 &0x80))
            break;
        ++v3;
    }
    return ret;
}

// ��ȡ���ݵ��ֽ����飬���ֽ�����ָ����С������������ǰ׺
int Stream::Read(Buffer &bs)
{
	if(bs.Length())
	{
		if(this->_Position+bs.Length()>=this->Length)
		{
			return 0;
		}
		for(int i=0;i<bs.Length();i++)
		{
			bs[i]=*this->Current();
			this->_Position++;
		}
		return bs.Length();
	}
	else
		return 0;
}

// д��7λѹ����������
int Stream::WriteEncodeInt(int value)
{
    return 0;
}

// ���ֽ����������д�뵽������������������ǰ׺
bool Stream::Write(const Buffer &bs)
{
	int bslen=bs.Length();
	if(bslen)
	{
		if(this->CanWrite)
		{
			if(bslen+this->_Position<this->Length)
			{
				int i;
				for(i=0;(i<bslen)&&(this->_Position<this->_Capacity);i++)
				{
					this->_Buffer[this->_Position]=bs[i];
					this->_Position++;
				}
			}
			else
				return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

// ����������ȡ�䳤���ݵ��ֽ����顣��ѹ��������ͷ��ʾ����
int Stream::ReadArray(Buffer &bs)
{
    return 0;
}

ByteArray Stream::ReadArray(int count){}
// ���ֽ�������Ϊ�䳤����д�뵽����������ѹ��������ͷ��ʾ����
bool Stream::WriteArray(const Buffer &bs)
{
    return false;
}

ByteArray Stream::ReadArray(){}
//String Stream::ReadString()
//{
//	return nullptr;
//}

int Stream::ReadByte()
{
	if(this->_Position+1>=this->Length)
		return -1;
	else
	{
		byte v3=*this->Current();
		if(this->Seek(1))
			return v3;
		else
			return 0;
	}
}
ushort Stream::ReadUInt16()
{	
	ushort buf[1];
	Buffer v3(buf,2);
	if(this->Read(v3))	
	{
		if(!this->Little)
			buf[0]=_REV16(buf[0]);
		return buf[0];
	}
	else
		return 0;
}

uint Stream::ReadUInt32()
{
    uint buf[1];
	Buffer v3(buf,4);
	if(this->Read(v3))	
	{
		if(!this->Little)
			buf[0]=_REV(buf[0]);
		return buf[0];
	}
	else
		return 0;
}

UInt64 Stream::ReadUInt64()
{
    UInt64 buf[1];
	Buffer v3(buf,8);
	if(this->Read(v3))	
	{
		if(!this->Little)
		{
			uint hi=_REV(buf[0]);
			uint lo=_REV(buf[0]>>32);
			buf[0]=hi;
			buf[0]<<=32;
			buf[0]|=lo;
		}
		return buf[0];
	}
	else
		return 0;
}

bool Stream::Write(byte value)
{
	byte buf[1];
	buf[0]=value;
	const Buffer v3(buf,1);
	return this->Write(v3);
}

bool Stream::Write(ushort value)
{
    ushort buf[1];
	buf[0]=value;
	const Buffer v3(buf,2);
	if(!this->Little)
		buf[0]= _REV16(buf[0]);
	return this->Write(v3);
}

bool Stream::Write(uint value)
{
    uint buf[1];
	buf[0]=value;
	const Buffer v3(buf,4);
	if(!this->Little)
		buf[0]=_REV(buf[0]);
	return this->Write(v3);
}

bool Stream::Write(UInt64 value)
{
	UInt64 buf[1];
	buf[0]=value;
	const Buffer v3(buf,8);
	if(!this->Little)
	{
		uint hi=_REV(buf[0]>>32);
		uint lo=_REV(buf[0]);
		buf[0]=lo;
		buf[0]<<=32;
		buf[0]|=hi;
	}
	return this->Write(v3);
}

// ��ȡָ�����ȵ����ݲ��������ֽ�ָ�룬�ƶ�������λ��
byte *Stream::ReadBytes(int count)
{
	int v3;
	if(count<0)
		v3=this->Remain();	
	if(this->Seek(v3))
		return this->Current();
	else
		return nullptr;	
	
}

// ��ȡһ���ֽڣ����ƶ��αꡣ���û�п������ݣ��򷵻�-1
int Stream::Peek()const
{
	if(this->Remain())
	{
		return (int)*this->Current();
	}
	else
	{
		return -1;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool MemoryStream::CheckRemain(int count)
{
    return false;
}

// ����ָ����С��������
//MemoryStream::MemoryStream(int len )
//{
//}
// ʹ�û�������ʼ����������֧���Զ�����
//MemoryStream::MemoryStream(void* buf, int len)
//{
//}

// ����������
MemoryStream::~MemoryStream(){}
