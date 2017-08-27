#include "ByteArray.h"

#if 0
    ByteArray::ByteArray(int length){

    }
    ByteArray::ByteArray(byte item, int length){

    }
    // 因为使用外部指针，这里初始化时没必要分配内存造成浪费
    ByteArray::ByteArray(const void *data, int length, bool copy){

    }
    ByteArray::ByteArray(void *data, int length, bool copy){

    }
    explicit ByteArray::ByteArray(const Buffer &arr){

    }
    ByteArray::ByteArray(ByteArray && rval){

    }

    ByteArray &ByteArray::operator = (const Buffer &rhs){

    }
    ByteArray &ByteArray::operator = (const ByteArray &rhs){

    }
    ByteArray &ByteArray::operator = (const void *p){

    }
    ByteArray &ByteArray::operator = (ByteArray && rval){

    }

    // 重载等号运算符，使用外部指针、内部长度，用户自己注意安全
    //ByteArray& ByteArray::operator=(const void* data)
    //{

    //}

    // 保存到普通字节数组，首字节为长度
    int ByteArray::Load(const void *data, int maxsize){

    }
    // 从普通字节数据组加载，首字节为长度
    int ByteArray::Save(void *data, int maxsize)const{

    }

    //bool operator==(const ByteArray& bs1, const ByteArray& bs2)
    //{

    //}
    //bool operator!=(const ByteArray& bs1, const ByteArray& bs2)
    //{

    //}

    #if DEBUG
        void ByteArray::Test(){

        }
    #endif 

    void *ByteArray::Alloc(int len){

    }

    void ByteArray::move(ByteArray &rval){

    }
#endif
