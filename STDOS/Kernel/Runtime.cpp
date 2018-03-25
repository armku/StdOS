/*
ջ����������
�ѣ������������ڴ����Ӵ˴�����

 */
#include <stdlib.h>
#include "Sys.h"

#ifdef DEBUG

    void *operator new(uint32_t size);
    void *operator new[](uint32_t size);
    void operator delete (void *p);
    void operator delete [](void *p);

#endif 

#ifdef DEBUG
extern "C"
{
    extern uint32_t __heap_base;
    extern uint32_t __heap_limit;
}
    void *operator new(uint32_t size)
    {
        //debug_printf(" new size: %d ", size);
        void *p = malloc(size);
        if (!p)
            debug_printf("malloc failed! size=%d ", size);
        else
        {
            //debug_printf("0x%08x ", p);
            // �����ֻʣ��64�ֽڣ��򱨸�ʧ�ܣ�Ҫ���û�����ѿռ����ⲻ��
            uint32_t end = (uint32_t) &__heap_limit;
            if ((uint32_t)p + size + 0x40 >= end)
                debug_printf(" + %d near HeapEnd=0x%08x", size, end);
        }
        //    assert_param(p);
        return p;
    }

    void *operator new[](uint32_t size)
    {
        //debug_printf(" new size[]: %d ", size);
        void *p = malloc(size);
        if (!p)
            debug_printf("malloc failed! size=%d ", size);
        else
        {
            //debug_printf("0x%08x ", p);
            // �����ֻʣ��64�ֽڣ��򱨸�ʧ�ܣ�Ҫ���û�����ѿռ����ⲻ��
            uint32_t end = (uint32_t) &__heap_limit;
            if ((uint32_t)p + size + 0x40 >= end)
                debug_printf(" + %d near HeapEnd=0x%08x", size, end);
        }
        //    assert_param(p);
        return p;
    }

    void operator delete (void *p)
    {
        debug_printf(" delete 0x%08x ", p);
        if (p)
            free(p);
    }

    void operator delete [](void *p)
    {
        debug_printf(" delete[] 0x%08x ", p);
        if (p)
            free(p);
    }
#endif