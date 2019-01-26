#ifndef _Delegate_H_
#define _Delegate_H_

#include <stdint.h>

// û�в����ͷ���ֵ��ί��
typedef void (*Func)(void);
// һ������û�з���ֵ��ί�У�һ��param��������Ŀ����󣬵������þ�̬������װ��Ա����
typedef void (*Action)(void* param);
typedef void(*Action2)(void*, void*);
typedef void(*Action3)(void*, void*, void*);
// �¼�����ί�У�һ��sender��ʾ�¼������ߣ�param����Ŀ����󣬵������þ�̬������װ��Ա����
typedef void (*EventHandler)(void* sender, void* param);
// �������ݻ�������ַ�ͳ��ȣ����з�������ʹ�øû��������������ݳ���
typedef uint32_t (*DataHandler)(void* sender, uint8_t* buf, uint32_t size, void* param);

// ί�нӿ�
class IDelegate
{
public:
	void*	Method;	// ����ָ��
	void*	Target;	// ����

	IDelegate& operator=(const IDelegate& dlg)	{ Bind(dlg.Method, dlg.Target); return *this; }
    explicit operator bool() const { return Method != nullptr; }
    bool operator !() const { return Method == nullptr; }

protected:
	void Bind(void* method, void* target = nullptr)
	{
		Method	= method;
		Target	= target;
	}
};

// ί�С���һ����Ŀ�����ָ�룬�ڶ����Ͳ���
template <typename TArg>
class Delegate : public IDelegate
{
public:
	typedef void(*TAction)(TArg);
	typedef void(*VAction)(void*, TArg);

	using IDelegate::Bind;

	Delegate()	{ Bind(nullptr, nullptr); }
	Delegate(const Delegate& dlg)	= delete;

	// ȫ�ֺ������ྲ̬����
    Delegate(Action func)	{ Bind((void*)func); }
    Delegate(TAction func)	{ Bind((void*)func); }
    Delegate& operator=(Action func)	{ Bind((void*)func); return *this; }
    Delegate& operator=(TAction func)	{ Bind((void*)func); return *this; }

	// ��Ŀ���ȫ�ֺ���
	template<typename T>
	Delegate(void(*func)(T&, TArg), T* target)	{ Bind((void*)func, target); }

	// ���Ա����
	// func��һ�����󣬶���ֵΪ����ָ�룬���ǲ���ֱ��תΪvoid*��������Ҫͨ��ָ��תΪ������ͣ���ת�������ܸ�ֵ
	template<typename T>
	Delegate(void(T::*func)(TArg), T* target)	{ Bind((void*)*(int*)&func, target); }

	void Bind(Action func)	{ Bind((void*)func); }
	void Bind(TAction func)	{ Bind((void*)func); }
	template<typename T>
	void Bind(void(*func)(T&, TArg), T* target)	{ Bind((void*)func, target); }
	template<typename T>
	void Bind(void(T::*func)(TArg), T* target)	{ Bind((void*)*(int*)&func, target); }

	// ִ��ί��
	void operator()(TArg arg)
	{
		if(Method)
		{
			if(Target)
				((VAction)Method)(Target, arg);
			else
				((TAction)Method)(arg);
		}
	}
};

// ί�С���һ����Ŀ�����ָ�룬�ڶ��������Ͳ���
template <typename TArg, typename TArg2>
class Delegate2 : public IDelegate
{
public:
	typedef void(*TAction)(TArg, TArg2);
	typedef void(*VAction)(void*, TArg, TArg2);

	using IDelegate::Bind;

	Delegate2()	{ Bind(nullptr, nullptr); }
	Delegate2(const Delegate2& dlg)	= delete;

	// ȫ�ֺ������ྲ̬����
    Delegate2(Action2 func)	{ Bind((void*)func); }
    Delegate2(TAction func)	{ Bind((void*)func); }
    Delegate2& operator=(Action2 func)	{ Bind((void*)func); return *this; }
    Delegate2& operator=(TAction func)	{ Bind((void*)func); return *this; }

	using IDelegate::operator=;

	// ��Ŀ���ȫ�ֺ���
	template<typename T>
	Delegate2(void(*func)(T&, TArg, TArg2), T* target)	{ Bind((void*)func, target); }

	// ���Ա����
	template<typename T>
	Delegate2(void(T::*func)(TArg, TArg2), T* target)	{ Bind((void*)*(int*)&func, target); }

	void Bind(Action2 func)	{ Bind((void*)func); }
	void Bind(TAction func)	{ Bind((void*)func); }
	template<typename T>
	void Bind(void(*func)(T&, TArg, TArg2), T* target)	{ Bind((void*)func, target); }
	template<typename T>
	void Bind(void(T::*func)(TArg, TArg2), T* target)	{ Bind((void*)*(int*)&func, target); }

	// ִ��ί��
	void operator()(TArg arg, TArg2 arg2)
	{
		if(Method)
		{
			if(Target)
				((VAction)Method)(Target, arg, arg2);
			else
				((TAction)Method)(arg, arg2);
		}
	}
};

// ί�С���һ����Ŀ�����ָ�룬�ڶ����Ͳ���
template <typename TArg, typename TArg2, typename TArg3>
class Delegate3 : public IDelegate
{
public:
	typedef void(*TAction)(TArg, TArg2, TArg3);
	typedef void(*VAction)(void*, TArg, TArg2, TArg3);

	using IDelegate::Bind;

	Delegate3()	{ Bind(nullptr, nullptr); }
	Delegate3(const Delegate3& dlg)	= delete;

	// ȫ�ֺ������ྲ̬����
    Delegate3(Action3 func)	{ Bind((void*)func); }
    Delegate3(TAction func)	{ Bind((void*)func); }
    Delegate3& operator=(Action3 func)	{ Bind((void*)func); return *this; }
    Delegate3& operator=(TAction func)	{ Bind((void*)func); return *this; }

	// ��Ŀ���ȫ�ֺ���
	template<typename T>
	Delegate3(void(*func)(T&, TArg, TArg2, TArg3), T* target)	{ Bind((void*)func, target); }

	// ���Ա����
	template<typename T>
	Delegate3(void(T::*func)(TArg, TArg2, TArg3), T* target)	{ Bind((void*)*(int*)&func, target); }

	void Bind(Action3 func)	{ Bind((void*)func); }
	void Bind(TAction func)	{ Bind((void*)func); }
	template<typename T>
	void Bind(void(*func)(T&, TArg, TArg2, TArg3), T* target)	{ Bind((void*)func, target); }
	template<typename T>
	void Bind(void(T::*func)(TArg, TArg2, TArg3), T* target)	{ Bind((void*)*(int*)&func, target); }

	// ִ��ί��
	void operator()(TArg arg, TArg2 arg2, TArg3 arg3)
	{
		if(Method)
		{
			if(Target)
				((VAction)Method)(Target, arg, arg2, arg3);
			else
				((TAction)Method)(arg, arg2, arg3);
		}
	}
};

//***************************************************************************
// ����ģ��ӿ�
template <typename TParameter>
class ifunction
{
public:
	// ��������������
	typedef TParameter parameter_type;

	// �������صĺ�������
	virtual void operator ()(TParameter) const = 0;
};

// �޲κ���ģ��ӿ�
template <>
class ifunction<void>
{
public:
	typedef void parameter_type;

	virtual void operator ()() const = 0;
};

// ������ģ��
template <typename TObject, typename TParameter>
class function : public ifunction<TParameter>
{
public:
	typedef TObject    object_type;    // ��������
	typedef TParameter parameter_type; // ��������������

	function(TObject& object, void(TObject::* p_function)(TParameter))
		: p_object(&object),
		p_function(p_function)
	{
	}

	virtual void operator ()(TParameter data) const
	{
		// ���ö���ĳ�Ա����
		(p_object->*p_function)(data);
	}

private:
	TObject* p_object;                        // ����ָ��
	void (TObject::* p_function)(TParameter); // ��Ա����ָ��
};

// �����޲κ���ģ��
template <typename TObject>
class function<TObject, void> : public ifunction<void>
{
public:
	function(TObject& object, void(TObject::* p_function)(void))
		: p_object(&object),
		p_function(p_function)
	{
	}

	virtual void operator ()() const
	{
		(p_object->*p_function)();
	}

private:
	TObject* p_object;
	void (TObject::* p_function)();
};

// ȫ�ֺ���ģ��
template <typename TParameter>
class function<void, TParameter> : public ifunction<TParameter>
{
public:
	function(void(*p_function)(TParameter))
		: p_function(p_function)
	{
	}

	virtual void operator ()(TParameter data) const
	{
		(*p_function)(data);
	}

private:
	void (*p_function)(TParameter);
};

template <>
class function<void, void> : public ifunction<void>
{
public:
	function(void(*p_function)(void))
		: p_function(p_function)
	{
	}

	virtual void operator ()() const
	{
		(*p_function)();
	}

private:
	void (*p_function)();
};

/*
��Ա����ָ��Ĳ���
class A{
public:
    void Func(int){...}
};

Ҫȡ��Func����ָ�룬void (A::*pFunc)(int)=&A::Func;
::*��һ���������������ʾpFunc��һ��ָ�룬ָ��A�ĳ�Ա����ȡ��Ա�����ĵ�ַ����ͨ�����������ȡ������ͨ��������ȡ������Ҫ����ȡ��ַ������
��ô���ͨ��ָ�������øú�������Ա������������һ��this��������ʾ����Ҫ�����Ķ�������ֻ��ȡ�˺�����ָ�룬��ȱ��һ��������Ϊthis������
Ϊ�����Ŀ�ģ������ȴ���һ������Ȼ��ͨ���ö��������ó�Ա����ָ�룺
A a; ( a.*pFunc)(10);
A* pa=&a;
(pa->*pFunc)(11);
��һ�ַ�ʽͨ����������ã��ڶ��ַ�ʽͨ������ָ����ã�Ч��һ����

ʹ����ģ�壺
Ҫ����һ����Ա�����������г�Ա����ָ���ǲ����ģ�����Ҫһ������ָ�룬����Ҫ��һ���ཫ���߰�һ��
*/

#endif //_Delegate_H_
