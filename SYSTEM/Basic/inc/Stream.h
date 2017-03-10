#pragma once

#include "ByteArray.h"

class Stream: public Object
{
    public:
        //数据流容量
        uint Capacity()const;
        void SetCapacity(uint len);
        //当前位置
        uint Position()const;
        //设置位置
        bool SetPosition(int p);
        //余下的有效数据流长度，0表示已经到达终点
        uint Remain()const;
        //尝试前后移动一段距离，返回成功或者失败。如果失败，不移动游标
        bool Seek(int offset);

        //数据流指针，注意：扩容后指针会改变
        byte *GetBuffer()const;
        //数据流udagnqian位置指针。注意：扩容后指针会改变
        byte *Current()const;

        //读取7为压缩编码整数
        uint ReadEncodeInt();
        //读取数据到字节数组，由于字节数组指定大小，不包含长度前缀
        uint Read(Buffer &bs);

        //写入7位压缩编码整数
        uint WriteEncodeInt(uint value);
        //把字节数组的数据写入到数据流，不包含长度前缀
        bool Write(const Buffer &bs);

        //从数据流读取变长数据到字节数组。以压缩整数开头表示长度
        uint ReadArray(Buffer &bs);
        ByteArray ReadArray(int count);
        //把字节数组作为变长数据写入到数据流。以压缩整数开头表示长度
        bool WriteArray(const Buffer &bs);
};
