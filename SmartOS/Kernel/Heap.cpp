#include "Kernel\Sys.h"
#include "Interrupt.h"

#include "Heap.h"
#include "TTime.h"


#define MEMORY_ALIGN	4

// ��ǰ��
const Heap* Heap::Current = nullptr;


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
	struct MemoryBlock_*	Next;
} MemoryBlock;

