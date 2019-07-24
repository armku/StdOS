#ifndef _Dictionary_H_
#define _Dictionary_H_

// �ֵ䡣�����ڴ洢ָ�롣
// ��������List���ڴ洢��ֵ���ϣ����ɾ��ʱ�����ǽ���ͬ������
class IDictionary
{
	typedef const void* PKey;
	typedef void* PValue;
public:
	IDictionary(IComparer comparer = nullptr);
	IDictionary(const IDictionary& dic);
	IDictionary(IDictionary&& dic);

	IDictionary& operator=(const IDictionary& dic);
	IDictionary& operator=(IDictionary&& dic);

	inline int Count()				const { return _Keys.Count(); }
	inline const IList& Keys()		const { return _Keys; }
	inline const IList& Values()	const { return _Values; }

	// ��ӵ���Ԫ��
	void Add(PKey key, PValue value);

	// ɾ��ָ��Ԫ��
	void Remove(PKey key);

	void Clear();

	// �Ƿ����ָ����
	bool ContainKey(PKey key) const;

	// ���Ի�ȡֵ
	bool TryGetValue(PKey key, PValue& value) const;

	// �������������[]������ָ��Ԫ�صĵ�һ��
	PValue operator[](PKey key) const;
	PValue& operator[](PKey key);

	const String GetString(PKey key) const;

#if DEBUG
	static void Test();
#endif

private:
	IList	_Keys;
	IList	_Values;

	void move(IDictionary& dic);
};

template<typename TKey, typename TValue>
class Dictionary : public IDictionary
{
	static_assert(sizeof(TKey) <= 4, "Dictionary only support pointer or int");
	static_assert(sizeof(TValue) <= 4, "Dictionary only support pointer or int");

	typedef const TKey	PKey;
	typedef TValue		PValue;
public:
	Dictionary(IComparer comparer = nullptr) : IDictionary(comparer) { }

	const List<TKey>& Keys() const { return (List<TKey>&)	 IDictionary::Keys(); };
	const List<TValue>& Values() const { return (List<TValue>&) IDictionary::Values(); };

	// ��ӵ���Ԫ��
	void Add(PKey key, PValue value) { IDictionary::Add((const void*)key, (void*)value); }

	// ɾ��ָ��Ԫ��
	void Remove(PKey key) { IDictionary::Remove((const void*)key); }

	// �Ƿ����ָ����
	bool ContainKey(PKey key) const { return IDictionary::ContainKey((const void*)key); }

	// ���Ի�ȡֵ
	bool TryGetValue(PKey key, PValue& value) const
	{
		void* val = nullptr;
		bool rs = IDictionary::TryGetValue((const void*)key, val);
		value = (PValue)val;

		return rs;
	}

	// �������������[]������ָ��Ԫ�صĵ�һ��
	PValue operator[](PKey key) const { return (PValue)IDictionary::operator[]((const void*)key); }
	PValue& operator[](PKey key) { return (PValue&)IDictionary::operator[]((const void*)key); }
};

#endif
