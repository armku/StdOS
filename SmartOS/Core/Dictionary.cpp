#include "Type.h"
#include "Buffer.h"
#include "SString.h"

#include "List.h"
#include "Dictionary.h"

IDictionary::IDictionary(IComparer comparer)
{
	_Keys.Comparer = comparer;
}

IDictionary::IDictionary(const IDictionary& dic)
{
	operator=(dic);
}

IDictionary::IDictionary(IDictionary&& dic)
{
	move(dic);
}

IDictionary& IDictionary::operator=(const IDictionary& dic)
{
	_Keys = dic._Keys;
	_Values = dic._Values;

	return *this;
}

IDictionary& IDictionary::operator=(IDictionary&& dic)
{
	move(dic);

	return *this;
}

void IDictionary::move(IDictionary& dic)
{
	_Keys = dic._Keys;
	_Values = dic._Values;
}

//int IDictionary::Count() const { return _Keys.Count(); }
//const IList& IDictionary::Keys()	const { return _Keys; }
//const IList& IDictionary::Values()	const { return _Values; }

// ��ӵ���Ԫ��
void IDictionary::Add(PKey key, void* value)
{
	// �ж�һ�£�����Ѵ��ڣ��򸲸�
	int idx = _Keys.FindIndex(key);
	if (idx >= 0)
	{
		_Values[idx] = value;
	}
	else
	{
		_Keys.Add((void*)key);
		_Values.Add(value);
	}
}

// ɾ��ָ��Ԫ��
void IDictionary::Remove(PKey key)
{
	int idx = _Keys.Remove(key);
	if (idx >= 0) _Values.RemoveAt(idx);
}

void IDictionary::Clear()
{
	_Keys.Clear();
	_Values.Clear();
}

// �Ƿ����ָ����
bool IDictionary::ContainKey(PKey key) const
{
	return _Keys.FindIndex(key) >= 0;
}

// ���Ի�ȡֵ
bool IDictionary::TryGetValue(PKey key, void*& value) const
{
	int idx = _Keys.FindIndex(key);
	if (idx < 0) return false;

	value = _Values[idx];

	return true;
}


// �������������[]������ָ��Ԫ�صĵ�һ��
void* IDictionary::operator[](PKey key) const
{
	int idx = _Keys.FindIndex(key);
	if (idx < 0) return nullptr;

	return _Values[idx];
}

void*& IDictionary::operator[](PKey key)
{
	int idx = _Keys.FindIndex(key);
	if (idx < 0)
	{
		static void* dummy;
		return dummy;
	}

	return _Values[idx];
}

const String IDictionary::GetString(PKey key) const
{
	void* p = nullptr;
	TryGetValue(key, p);

	return String((cstring)p);
}
