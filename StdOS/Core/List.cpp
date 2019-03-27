#include "List.h"
#include "Sys.h"

IList::IList()
{
	this->Init();
}

// ���ӵ���Ԫ��
void IList::Add(void *item)
{
	if (this->_Count < this->_Capacity)
	{
		this->_tmpbuf[this->_Count++] = (uint32_t)item;
	}
	else
	{
		debug_printf("�б��������� %d��������\n",this->_Capacity);
	}
}

// ɾ��ָ��λ��Ԫ��
void IList::RemoveAt(int index)
{
	return;
}

// ɾ��ָ��Ԫ��
int IList::Remove(const void *item)
{
	int idx=this->FindIndex(item);
	if(idx>=0)
	this->RemoveAt(idx);
    return idx;
}

// ����ָ���������ʱ����-1
int IList::FindIndex(const void *item)const
{
    for (int i = 0; this->_Count > i; ++i)
    {
        if ((const void*)(this->_tmpbuf[i]) == item)
            return i;
        if (this->Comparer((void*)this->_tmpbuf[i], item))
        {
            return i;
        }
    }
    return  - 1;
}

// �������������[]������ָ��Ԫ�صĵ�һ��
void *IList::operator[](int i)const
{
	if(i>=0&&this->_Count>i)
	{
		return (void *)(this->_tmpbuf[i]);
	}
	else
	{
		return NULL;
	}
}

void IList::Init()
{
	this->_Count=0;
	this->_Capacity = ArrayLength(this->_tmpbuf);
}