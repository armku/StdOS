#include "ByteArray.h"
#include "Stream.h"

void Stream::Init(void *buf, int len)
{
    if (!buf)
    {
        //assert_failed2("buf,%s",__FILE__);
    }

    this->_Buffer = (byte*)buf;
    this->_Capacity = len;
    this->_Position = 0;

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
            //SmartOS_printf("(%d - %d) = %d  %d \r\n",this);
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
        //SmartOS_printf((const char *)sub_2F4);
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
    return false;
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

int Stream::ReadByte(){}
ushort Stream::ReadUInt16()
{
    return 0;
}

uint Stream::ReadUInt32()
{
    return 0;
}

UInt64 Stream::ReadUInt64()
{
    return 0;
}

bool Stream::Write(byte value)
{
    return false;
}

bool Stream::Write(ushort value)
{
    return false;
}

bool Stream::Write(uint value)
{
    return false;
}

bool Stream::Write(UInt64 value)
{
    return false;
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