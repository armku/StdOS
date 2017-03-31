#pragma once

#include "Type.h"

// �䳤�б�ģ��
template <typename T> class List
{
    public:
        List()
        {
                _count = _total = 0;
        }
        List(int size = 4)
        {
                _count = 0;
                _total = size;
                arr = new T[size];
        }
        List(T *items, uint count)
        {
                arr = new T[count];

                _count = 0;
                _total = count;
                for (int i = 0; i < count; i++)
                {
                        arr[_count++] =  *items++;
                }
        }

        ~List()
        {
                if (arr)
                    delete [] arr;
                arr = NULL;
        }

        void Add(T item)
        {
                // ����С
                CheckSize();

                arr[_count++] = item;
        }

        void Add(T *items, int count)
        {
                int size = _count + count;
                if (size >= _total)
                    ChangeSize(size *2);

                for (int i = 0; i < count; i++)
                {
                        arr[_count++] =  *items++;
                }
        }

        T *ToArray()
        {
                // ����պ�������ֱ�ӷ��أ��������µ����ռ�
                if (_count != _total)
                {
                        T *arr2 = new T[_count];
                        memcpy(arr, arr2, _count);
                        delete [] arr;
                        arr = arr2;
                }
                return arr;
        }
        int Count()
        {
                return _count;
        }

        // �������������[]����������������һ��ʹ���±��������ڲ�������±�Խ�磬�ⲿ����Ϊ֮
        T operator[](int i)
        {
                return arr;
        }
        T *operator = (List list)
        {
                return list.ToArray();
        }

    private:
        T *arr;
        uint _count;
        uint _total;

        void ChangeSize(int newSize)
        {
                if (_total == newSize)
                    return ;

                T *arr2 = new T[newSize];
                if (arr)
                {
                        // ����������С����ֱ�Ӹ��ƣ����������ϴ����ȸ��ƣ���������²���
                        if (newSize < _total)
                            memcpy(arr, arr2, newSize);
                        else
                        {
                                memcpy(arr, arr2, _total);
                                memset(arr2 + _total, newSize - _total);
                        }
                        delete [] arr;
                }
                arr = arr2;
        }

        void CheckSize()
        {
                // �������ռ������꣬����������
                if (_count >= _total)
                    ChangeSize(_count *2);
        }
};
