#include "Queue.h"
void EnterCritical()
{
}
void ExitCritical()
{
}
Queue::Queue()
{
#if _USEARRAY
	this->Clear();
#else
	this->pBuf = 0;
	this->_capacity = 0;
	this->_tail = 0;
	this->_head = 0;
#endif
}
void Queue::Clear()
{
#if _USEARRAY
	this->_s.SetLength(this->_s.Capacity());
	this->_head = 0;
	this->_tail = 0;
	this->_size = 0;
#else
	this->_head = 0;
	this->_tail = 0;
	this->_size = 0;
#endif
}
void Queue::Enqueue(byte da)
{
#if _USEARRAY	
	int capacity;
	int v5;
		
	capacity = this->Capacity();
	if (!capacity)
		this->_s.SetLength(64);
	if (this->_size < capacity)
	{
		v5 = this->_head++;
		this->_s[v5] = da;
		if (this->_head >= capacity)
			this->_head -= capacity;
		EnterCritical();
		++this->_size;
		ExitCritical();
	}
#else
	if (this->pBuf == 0)
	{
		return;
	}
	this->pBuf[this->_head] = da;
	if (++this->_head >= this->_capacity)
	{
		this->_head = 0;
	}
	if (this->_size < this->_capacity)
	{
		this->_size++;
		return;
	}
	else
	{
		return;
	}
#endif
}

byte Queue::Dequeue()
{
#if _USEARRAY	
	int result;
	int v3; 
	int v4; 
	int ret; 
		
	if (this->_size)
	{
		EnterCritical();
		--this->_size;
		ExitCritical();
		v3 =this->_s.Capacity();
		v4 = this->_tail++;
		ret =this->_s[v4];
		if (this->_tail >= v3)
			this->_tail -= v3;
		result = ret;
	}
	else
	{
		result = 0;
	}
	return result;
#else
	byte ret = this->pBuf[this->_tail];
	if (++this->_tail >= this->_capacity)
	{
		this->_tail = 0;
	}
	if (this->_size)
	{
		this->_size--;
		//return 0;
	}
	else
	{
		//return  - 1;
	}
	return ret;
#endif
}

// 批量写入
int Queue::Write(const Buffer &bs)
{
#if _USEARRAY	
	int capacity;
	int result;
	int bslen; 
	int v7;
	int pcapacity;
	int v9;
	int psize; 

	capacity = this->_s.Capacity();
	if (!capacity)
		this->_s.SetLength(64);
	if (this->_size < capacity)
	{
		bslen = this->_s.Length();
		if (this->_size + bslen > capacity)
			bslen = capacity - this->_size;
		v7 = 0;
		while (1)
		{
			pcapacity = this->Capacity();
			v9 = pcapacity - this->_head;
			if (bslen <= v9)
				break;
			this->_s.Copy(this->_head, bs, v7, pcapacity - this->_head);
			bslen -= v9;
			v7 += v9;
			this->_head = 0;
		}
		this->_s.Copy(this->_head, bs, v7, bslen);
		psize = v7 + bslen;
		this->_head += bslen;
		if (this->_s.Length() <= this->_head)
			this->_head -= this->_s.Length();
		EnterCritical();
		this->_size += psize;
		ExitCritical();
		result = psize;
	}
	else
	{
		result = 0;
	}
	return result;
#else
	for (int i = 0; i < bs.Length(); i++)
	{
		this->Enqueue(bs[i]);
	}
	return bs.Length();
#endif
}

// 批量读取
int Queue::Read(Buffer &bs)
{
#if _USEARRAY	
	int result; // r0@2
	int buflen; // r5@4
	int v6; // r6@8
	int v7; // r9@9
	int v8; // r8@9
	int v9; // r6@10

	if (this->_size)
	{
		buflen = bs.Length();
		if (buflen)
		{
			if (this->_size < buflen)
				buflen = this->_size;
			v6 = 0;
			while (1)
			{
				v7 = this->_s.Capacity();
				v8 = v7 - this->_tail;
				if (buflen <= v8)
					break;
				/*(*(void(__fastcall **)(Buffer *, int, Queue *, _DWORD))(*(_DWORD *)bs + 24))(bs,v6,this,this->_tail);*/
				buflen -= v8;
				v6 += v8;
				this->_tail = 0;
			}
			/*(*(void(__fastcall **)(Buffer *, int, Queue *, _DWORD))(*(_DWORD *)bs + 24))(bs,v6,this,this->_tail);*/
			v9 = v6 + buflen;
			this->_tail += buflen;
			if (this->_tail >= v7)
				this->_tail -= v7;
			/*(*(void(__fastcall **)(Buffer *, int))(*(_DWORD *)bs + 12))(bs, v9);*/
			EnterCritical();
			this->_size -= v9;
			ExitCritical();
			result = v9;
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 0;
	}
	return result;
#else
	int len = this->Length();
	for (int i = 0; i < len; i++)
	{
		bs[i] = this->Dequeue();
	}
	bs.SetLength(len);
	return len;
#endif
}

#if _USEARRAY
void Queue::SetCapacity(int len)
{
	this->_s.SetLength(len);
}
#else
void Queue::SetBuf(void *buf, int len)
{
	if (len >= 0)
	{
		this->pBuf = (byte*)buf;
		this->_capacity = len;
	}
	this->Clear();
}
#endif

