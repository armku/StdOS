#include "Type.h"
#include "Heap.h"

Heap::Heap(uint addr, int size)
{
	//SmartOS_printf("Heap::Init(0x%p, %d) Free=%d \r\n", pthis->start, v7);
}
// ��ʹ���ڴ���
int Heap::Used() const
{
	return 0;
}
// ��ʹ���ڴ����
int Heap::Count() const
{
	return 0;
}
// �����ڴ���
int Heap::FreeSize() const
{
	return 0;
}
void* Alloc(int size)
{
	return (void*)0;
}
void Free(void* ptr)
{
	
}