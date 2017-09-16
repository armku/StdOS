#ifndef __Task_H__
#define __Task_H__

#include "Kernel\Sys.h"

class TaskScheduler;

// ����
class Task
{
public:
	TaskScheduler* Host;

	uint	ID;			// ���
	cstring	Name;		// ����

	Action	Callback;	// �ص�
	void*	Param;		// ����

	int		Period;		// ����ms
	UInt64	NextTime;	// ��һ��ִ��ʱ��ms

	int		Times;		// ִ�д���
	int		SleepTime;	// ��ǰ˯��ʱ��us
	int		Cost;		// ƽ��ִ��ʱ��us
	int		CostMs;		// ƽ��ִ��ʱ��ms
	int		MaxCost;	// ���ִ��ʱ��us

	bool	Enable;		// �Ƿ�����
	bool	Event;		// �Ƿ�ִֻ��һ�κ���ͣ���¼�������
	byte	Deepth;		// ��ǰ���
	byte	MaxDeepth;	// �����ȡ�Ĭ��1�㣬����������

	Task();
	Task(const Task& task)	= delete;
	~Task();

	// ִ�����񡣷����Ƿ�����ִ�С�
	bool Execute(UInt64 now);
	// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
	void Set(bool enable, int msNextTime = -1);
	// ��ʾ״̬
	void ShowStatus();

	// ȫ�����������
	static TaskScheduler* Scheduler();
	static Task* Get(int taskid);
	static Task& Current();

private:
	friend class TaskScheduler;

	bool CheckTime(UInt64 end, bool isSleep);
	void Init();
	
	
	
    private:
        Task(TaskScheduler *scheduler);
    public:        
        uint CpuTime; // �ܺķ�ʱ��        
		bool operator==(Task& tsk);
};

template<class T,int length> class FixedArray
{
	public:
		FixedArray();
		void Add(T&);
		void Add(T*);
		void Remove(T&);
		void RemoveAt(int pos=-1);
		bool MoveNext(int pos);//����һ���ڵ�
		T* Find(T&);
		T *operator[](int taskid);
		void PrintList();
		~FixedArray();
	protected:
		struct Node{
			Node* pNext;
			T* pT;
		};
		Node *pFirst;//���׽ڵ�ָ��
		Node *mpCur;//��ǰ�ڵ�
};


template <class T,int length> 
FixedArray<T,length>::FixedArray()
{
        this->pFirst = 0;
}
template <class T,int length> 
void FixedArray<T,length>::Add(T &t)
{
        Node *temp = new Node();
        temp->pT = &t;
        temp->pNext = pFirst;
        pFirst = temp;
		mpCur=temp;
}

template <class T,int length> 
void FixedArray<T,length>::Add(T *t)
{
        Node *temp = new Node();
        temp->pT = t;
        temp->pNext = pFirst;
        pFirst = temp;
}

template <class T,int length> void FixedArray<T,length>::Remove(T &t)
{
        Node *q = 0; //������λ��ɾ���Ľڵ�
        if (*(pFirst->pT) == t)
        {
                q = pFirst;
                pFirst = pFirst->pNext; //��ɾ���ڵ�������ʱ������
        }
        else
        {
                for (Node *p = pFirst; p->pNext; p = p->pNext)
                //˳������
                {
                        if (*(p->pNext->pT) == t)
                        {
                                q = p->pNext;
                                p->pNext = q->pNext;
                                break;
                        }
                }
        }
        if (q)
        {
                delete q->pT;
                delete q;
        }
}
template <class T,int length> void FixedArray<T,length>::RemoveAt(int pos)
{
	if(pos==-1)
	{
		this->Remove(*(mpCur->pT));
	}
}
template<class T,int length>
T* FixedArray<T,length>::Find(T& t)
{
	for(Node* p=pFirst;p;p=p->pNext)
	{
		if(*(p->pT)==t)
		{
			return p->pT;
		}
	}
	return 0;
}
template<class T,int length>
bool FixedArray<T,length>::MoveNext(int pos)
{
	if(pos<0)
	{	
		if(mpCur)
		{
			mpCur=mpCur->pNext;
			if(mpCur)
			{
				return true;
			}
			else
			{				
				return false;
			}
		}
		else
		{
			mpCur=pFirst;
			return true;
		}
	}
	
	
	int i=0;
	for(Node* p=pFirst;p;p=p->pNext,i++)
	{
		if(p&&(i==pos))
		{
			return true;
		}
	}
	return false;
}
template<class T,int length>
T* FixedArray<T,length>::operator[](int taskid)
{	
	if(taskid<0)
	{
		return this->mpCur->pT;
	}
	int i=0;
	for(Node* p=pFirst;p;p=p->pNext,i++)
	{
		if(p&&(i==taskid))
		{
			return p->pT;
		}
	}	
	return 0;
}
template<class T,int length>
void FixedArray<T,length>::PrintList()
{	
	for(Node* p=pFirst;p;p=p->pNext)
	{
			printf("%f\n",*(p->pT));		
	}
}

template<class T,int length>
FixedArray<T,length>::~FixedArray<T,length>()
{
	Node *p;
	p=pFirst;
	while(p)
	{
		pFirst=p->pNext;
		delete p->pT;
		delete p;
		p=pFirst;
	}
}
// ���������
class TaskScheduler
{
private:
	List<Task*>	_Tasks;	// �����б�
	bool	_SkipSleep;	// �������һ��˯�ߣ����Ͽ�ʼ��һ��ѭ��

	friend class Task;

public:
	cstring	Name;	// ϵͳ����
	int		Count;		// �������
	Task*	Current;	// ����ִ�е�����
	bool	Running;	// �Ƿ���������
	bool	Sleeping;	// �����ǰ����Sleep״̬������ֹͣ���˳�
	byte	Deepth;		// ��ǰ���
	byte	MaxDeepth;	// �����ȡ�Ĭ��5��

	int		Times;		// ִ�д���
	int		Cost;		// ƽ��ִ��ʱ��us
	UInt64	TotalSleep;	// �����������˯��ʱ��ms
	UInt64	LastTrace;	// ���ͳ�Ƹ���ʱ��ms

	typedef void (*SAction)(int ms);
	SAction	EnterSleep;	// ֪ͨ�ⲿ����Ҫ˯�����ɺ���
	Func	ExitSleep;	// ֪ͨ�ⲿ��Ҫ���˳�˯�ߣ��ָ�����

	TaskScheduler(cstring name = nullptr);

	// ʹ���ⲿ��������ʼ�������б�����Ƶ���Ķѷ���
	void Set(Task* tasks, int count);
	// �������� ����ʹ�ô˺������׸������ID
	uint FindID(Action func);
	// �������� ����ʹ�ô˺������׸�����
	Task* FindTask(Action func);
	// �������񣬷��������š�dueTime�״ε���ʱ��ms��-1��ʾ�¼�������period���ȼ��ms��-1��ʾ������һ��
	uint Add(Action func, void* param, int dueTime = 0, int period = 0, cstring name = nullptr);
	template<typename T>
	uint Add(void(T::*func)(), T* target, int dueTime = 0, int period = 0, cstring name = nullptr)
	{
		return Add(*(Action*)&func, target, dueTime, period, name);
	}
	void Remove(uint taskid);

	void Start();
	void Stop();
	// ִ��һ��ѭ����ָ��������ʱ��
	void Execute(uint msMax, bool& cancel);
	uint ExecuteForWait(uint msMax, bool& cancel);

	// �������һ��˯�ߣ����Ͽ�ʼ��һ��ѭ��
	void SkipSleep();

	void ShowStatus();	// ��ʾ״̬

    Task* operator[](int taskid);
	
	
	
    private:        
        FixedArray < Task, 32 > _TasksOld;    
};

#endif
