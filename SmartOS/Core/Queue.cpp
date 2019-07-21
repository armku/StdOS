#include "_Core.h"
#include "Queue.h"

extern void EnterCritical();
extern void ExitCritical();

Queue::Queue() : _s((void*)nullptr, 0)
{
	Clear();
}

void Queue::SetCapacity(int len)
{
	_s.SetLength(len);
}

void Queue::Clear()
{
	_s.SetLength(_s.Capacity());
	_head	= 0;
	_tail	= 0;
	_size	= 0;
}

// �ؼ��Դ��룬�ŵ���ͷ
INROOT void Queue::Enqueue(byte dat)
{
	int total	= _s.Capacity();
	if(!total) _s.SetLength(64);
	// ������ٽ���
	if(_size >= total) return;

	_s[_head++] = dat;
	//_head %= _s.Capacity();
	// ����������һ����������������˷��˴���ʱ�䣬���´����жϽ��ն�����
	if(_head >= total) _head -= total;

	EnterCritical();
	_size++;
	ExitCritical();
}

INROOT byte Queue::Dequeue()
{
	if(_size == 0) return 0;

	EnterCritical();
	_size--;
	ExitCritical();

	/*
	�����ִ���Ƶ���շ�һ�����ݺ���ֶ���������Ҳ����sizeΪ0��Ȼ��tail��headС���տ�ʼСһ���ֽڣ�Ȼ���������
	����������֪��ARMָ��û�еݼӵݼ�ָ���û��ԭ�Ӳ�����
	size�ó�����һ��Ȼ���ٱ����ȥ�����Ǳ����ȥ֮ǰ�����ڽ����ж�д�룬�õ��˾ɵ�size����������size��ԭ�ƻ�ҪС1��
	������ֻ��ͨ���ر��ж��������Ϊ�˼��ٹ��ж�ʱ�����������ܣ�������ר�ŵ�Read������
	*/

	int total	= _s.Capacity();
	byte dat	= _s[_tail++];
	//_tail		%= _s.Capacity();
	if(_tail >= total) _tail -= total;

	return dat;
}

int Queue::Write(const Buffer& bs)
{
	int total	= _s.Capacity();
	if(!total) _s.SetLength(64);
	// ������ٽ���
	if(_size >= total) return 0;

	/*
	1������д�����ĩβ
	2�������ʣ�����ݣ���ӿ�ͷ��ʼд��
	3��ѭ������2
	4��������й�С�����п��ܺ������ݻḲ��ǰ������
	*/

	int	len	= bs.Length();

	// ����������ˣ�����Ҫ����
	if(_size + len > total)
		len	= total - _size;

	int rs = 0;
	while(true)
	{
		// ������һ��ѭ��ʣ�µ�λ��
		int remain	= _s.Capacity() - _head;
		// ���Ҫд��������㹻���
		if(len <= remain)
		{
			_s.Copy(_head, bs, rs, len);
			rs		+= len;
			_head	+= len;
			if(_head >= _s.Length()) _head -= _s.Length();

			break;
		}

		// ������дһ�Σ�ָ��ص���ͷ
		_s.Copy(_head, bs, rs, remain);
		len		-= remain;
		rs		+= remain;
		_head	= 0;
	}

	EnterCritical();
	_size += rs;
	ExitCritical();

	return rs;
}

int Queue::Read(Buffer& bs)
{
	if(_size == 0) return 0;

	/*
	1����ȡ��ǰ���ݵ�ĩβ
	2�������ʣ�����ݣ����ͷ��ʼ��ȡ
	3��ѭ������2
	4��������й�С�����п��ܺ������ݻḲ��ǰ������
	*/

	int	len	= bs.Length();
	if(!len) return 0;

	if(len > _size) len = _size;

	int rs = 0;
	while(true)
	{
		int total	= _s.Capacity();
		// ������һ��ѭ��ʣ�µ�λ��
		int remain = total - _tail;
		// ���Ҫ��ȡ�����ݶ�������
		if(len <= remain)
		{
			bs.Copy(rs, _s, _tail, len);
			rs		+= len;
			_tail	+= len;
			if(_tail >= total) _tail -= total;

			break;
		}

		// �����ȶ�һ�Σ�ָ��ص���ͷ
		bs.Copy(rs, _s, _tail, remain);
		len		-= remain;
		rs		+= remain;
		_tail	= 0;
	}

	bs.SetLength(rs);

	EnterCritical();
	_size -= rs;
	ExitCritical();

	return rs;
}

WEAK void EnterCritical() { }
WEAK void ExitCritical() { }
