#include "Array.h"

Array::Array(void* data, int len):Buffer(data,len)
{
	
}
Array::Array(const void* data, int len):Buffer((void*)data ,len)
{
	
}
bool Array::Release()
{
	return true;
}
void* Array::Alloc(int len)
{
	return 0;
}
void Array::Clear()
{
}
void Array::SetItemAt(int i, const void* item)
{
	
}
int Array::Copy(int destIndex, const Buffer& src, int srcIndex, int len)
{
	return true;
}
bool Array::SetLength(int len, bool bak)
{
	return true;
}
bool Array::SetLength(int len)
{
	return true;
}
Array::~Array()
{
}
