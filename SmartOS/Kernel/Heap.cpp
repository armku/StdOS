#include "Kernel\Sys.h"
#include "Interrupt.h"

#include "Heap.h"
#include "TTime.h"

#define MEMORY_ALIGN	4

// ��ǰ��
const Heap * Heap::Current = nullptr;

/*
�ѷ���ԭ��
1��ʹ���ڴ�飬��¼��ǰ����ʹ�ô�С�Լ���һ���ַ���γ��ڴ������
2��ÿ���ڴ��ʣ��ռ�����һ��ָ���ȥ��ǰ��ָ���ټ�ȥ���ô�С
3����ʼ����һ���ڴ�飬��ʹ��0
4�������ڴ�ʱ���ӵ�һ�鿪ʼ�ҿ���ռ���ڵ���Ŀ���С���ڴ�飬��������Ϊ�����½ڵ�
5���ͷ��ڴ�ʱ���ҵ����ڿ飬Ȼ��ǰ����һ���Nextָ��ֱ��ָ��ǰ�����һ���ַ���Ƴ���ǰ�ڵ�
*/

/******************************** MemoryBlock ********************************/
// �ڴ�顣��С����һ���ַ
typedef struct MemoryBlock_
{
	int	Used;
	struct MemoryBlock_* Next;
} MemoryBlock;

/******************************** Heap ********************************/
Heap::Heap(uint addr, int size)
{
	Current = this;

	// ��ַ����
	uint end = addr + size;
	addr = (addr + MEMORY_ALIGN - 1) & (~(MEMORY_ALIGN - 1));
	end = end & (~(MEMORY_ALIGN - 1));

	Address = addr;
	Size = end - addr;

	// ��һ���ڴ��
	auto mb = (MemoryBlock*)addr;
	mb->Used = sizeof(MemoryBlock);
	mb->Next = (MemoryBlock*)(end - sizeof(MemoryBlock));
	mb->Next->Used = sizeof(MemoryBlock);
	mb->Next->Next = nullptr;

	_Used = sizeof(MemoryBlock) << 1;
	_Count = 0;

	// ��¼��һ���п����ڴ�Ŀ飬�����ڴ����ʱ�Ĳ��Ҵ���
	_First = mb;

	debug_printf("Heap::Init(0x%p, %d) Free=%d \r\n", Address, Size, FreeSize());
}

int Heap::Used()	const { return _Used; }
int Heap::Count()	const { return _Count; }
int Heap::FreeSize()	const { return Size - _Used; }

void* Heap::Alloc(int size)
{
	// Ҫ������ڴ��С��Ҫ����
	size = (size + MEMORY_ALIGN - 1) & (~(MEMORY_ALIGN - 1));

	//debug_printf("Address=%p Size=%d ", Address, Size);
	int remain = Size - _Used;
	if (size > remain)
	{
		debug_printf("Heap::Alloc %d > %d (0x%p) ʧ�ܣ�Size=%d Used=%d First=%p \r\n", size, remain, remain, Size, _Used, _First);
		return nullptr;
	}

#if DEBUG
	// ���ͷ��������
	auto head = (MemoryBlock*)Address;
	//assert(_First >= head && head->Used <= Size && (byte*)head + head->Used <= (byte*)head->Next, "��ͷ���ƻ���");
	assert(head->Used <= Size && (byte*)head + head->Used <= (byte*)head->Next, "��ͷ���ƻ���");
	assert(_Used <= Size, "Heap::Used�쳣��");
#endif

	void* ret = nullptr;
	int need = size + sizeof(MemoryBlock);

	SmartIRQ irq;
	for (auto mcb = (MemoryBlock*)_First; mcb->Next != nullptr; mcb = mcb->Next)
	{
		// �ҵ�һ�������С���ڴ�顣���㵱ǰ��ʣ�೤��
		int free = (byte*)mcb->Next - (byte*)mcb - mcb->Used;
		if (free >= need)
		{
			// ��һ�����
			auto tmp = (MemoryBlock*)((byte*)mcb + mcb->Used);
			tmp->Next = mcb->Next;
			tmp->Used = need;
			mcb->Next = tmp;

			// ��¼��һ���п����ڴ�Ŀ飬�����ڴ����ʱ�Ĳ��Ҵ���
			_First = tmp;

			ret = (byte*)(tmp + 1);

			_Used += need;
			_Count++;

			//debug_printf("Heap::Alloc (%p, %d) First=%p Used=%d Count=%d \r\n", ret, need, _First, _Used, _Count);

			return ret;
		}
	}

	if (!ret) debug_printf("Heap::Alloc %d ʧ�ܣ�Count=%d Used=%d Free=%d First=%p \r\n", size, _Count, _Used, FreeSize(), _First);

	return ret;
}

void Heap::Free(void* ptr)
{
	auto prev = (MemoryBlock*)Address;
	auto cur = (MemoryBlock*)ptr - 1;

	SmartIRQ irq;
	for (auto mcb = prev->Next; mcb->Next != nullptr; mcb = mcb->Next)
	{
		// �ҵ��ڴ��
		if (mcb == cur)
		{
			_Used -= cur->Used;
			_Count--;

			// ǰ���п���λ��
			if (cur <= _First) _First = prev;

			//debug_printf("Heap::Free  (%p, %d) First=%p Used=%d Count=%d \r\n", ptr, cur->Used, _First, _Used, _Count);

			prev->Next = cur->Next;

			return;
		}
		prev = mcb;
	}
	debug_printf("�����ͷŲ��Ǳ�ϵͳ������ڴ� 0x%p \r\n", ptr);
}
