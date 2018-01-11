#include "Type.h"
#include "Heap.h"

Heap::Heap(uint addr, int size)
{
	//SmartOS_printf("Heap::Init(0x%p, %d) Free=%d \r\n", pthis->start, v7);
}
// 已使用内存数
int Heap::Used() const
{
	return 0;
}
// 已使用内存块数
int Heap::Count() const
{
	return 0;
}
// 可用内存数
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