#include "ByteArray.h"

#if 0
    ByteArray::ByteArray(int length){

    }
    ByteArray::ByteArray(byte item, int length){

    }
    // ��Ϊʹ���ⲿָ�룬�����ʼ��ʱû��Ҫ�����ڴ�����˷�
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

    // ���صȺ��������ʹ���ⲿָ�롢�ڲ����ȣ��û��Լ�ע�ⰲȫ
    //ByteArray& ByteArray::operator=(const void* data)
    //{

    //}

    // ���浽��ͨ�ֽ����飬���ֽ�Ϊ����
    int ByteArray::Load(const void *data, int maxsize){

    }
    // ����ͨ�ֽ���������أ����ֽ�Ϊ����
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
