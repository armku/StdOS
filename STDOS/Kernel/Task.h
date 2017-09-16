#ifndef __Task_H__
#define __Task_H__

#include "Kernel\Sys.h"

class TaskScheduler;

// 任务
class Task
{
public:
	TaskScheduler* Host;

	uint	ID;			// 编号
	cstring	Name;		// 名称

	Action	Callback;	// 回调
	void*	Param;		// 参数

	int		Period;		// 周期ms
	UInt64	NextTime;	// 下一次执行时间ms

	int		Times;		// 执行次数
	int		SleepTime;	// 当前睡眠时间us
	int		Cost;		// 平均执行时间us
	int		CostMs;		// 平均执行时间ms
	int		MaxCost;	// 最大执行时间us

	bool	Enable;		// 是否启用
	bool	Event;		// 是否只执行一次后暂停的事件型任务
	byte	Deepth;		// 当前深度
	byte	MaxDeepth;	// 最大深度。默认1层，不允许重入

	Task();
	Task(const Task& task)	= delete;
	~Task();

	// 执行任务。返回是否正常执行。
	bool Execute(UInt64 now);
	// 设置任务的开关状态，同时运行指定任务最近一次调度的时间，0表示马上调度
	void Set(bool enable, int msNextTime = -1);
	// 显示状态
	void ShowStatus();

	// 全局任务调度器
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
        uint CpuTime; // 总耗费时间        
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
		bool MoveNext(int pos);//到下一个节点
		T* Find(T&);
		T *operator[](int taskid);
		void PrintList();
		~FixedArray();
	protected:
		struct Node{
			Node* pNext;
			T* pT;
		};
		Node *pFirst;//链首节点指针
		Node *mpCur;//当前节点
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
        Node *q = 0; //用来定位待删除的节点
        if (*(pFirst->pT) == t)
        {
                q = pFirst;
                pFirst = pFirst->pNext; //待删除节点在链首时的脱链
        }
        else
        {
                for (Node *p = pFirst; p->pNext; p = p->pNext)
                //顺链查找
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
// 任务调度器
class TaskScheduler
{
private:
	List<Task*>	_Tasks;	// 任务列表
	bool	_SkipSleep;	// 跳过最近一次睡眠，马上开始下一轮循环

	friend class Task;

public:
	cstring	Name;	// 系统名称
	int		Count;		// 任务个数
	Task*	Current;	// 正在执行的任务
	bool	Running;	// 是否正在运行
	bool	Sleeping;	// 如果当前处于Sleep状态，马上停止并退出
	byte	Deepth;		// 当前深度
	byte	MaxDeepth;	// 最大深度。默认5层

	int		Times;		// 执行次数
	int		Cost;		// 平均执行时间us
	UInt64	TotalSleep;	// 所有任务的总睡眠时间ms
	UInt64	LastTrace;	// 最后统计跟踪时间ms

	typedef void (*SAction)(int ms);
	SAction	EnterSleep;	// 通知外部，需要睡眠若干毫秒
	Func	ExitSleep;	// 通知外部，要求退出睡眠，恢复调度

	TaskScheduler(cstring name = nullptr);

	// 使用外部缓冲区初始化任务列表，避免频繁的堆分配
	void Set(Task* tasks, int count);
	// 查找任务 返回使用此函数的首个任务的ID
	uint FindID(Action func);
	// 查找任务 返回使用此函数的首个任务
	Task* FindTask(Action func);
	// 创建任务，返回任务编号。dueTime首次调度时间ms，-1表示事件型任务，period调度间隔ms，-1表示仅处理一次
	uint Add(Action func, void* param, int dueTime = 0, int period = 0, cstring name = nullptr);
	template<typename T>
	uint Add(void(T::*func)(), T* target, int dueTime = 0, int period = 0, cstring name = nullptr)
	{
		return Add(*(Action*)&func, target, dueTime, period, name);
	}
	void Remove(uint taskid);

	void Start();
	void Stop();
	// 执行一次循环。指定最大可用时间
	void Execute(uint msMax, bool& cancel);
	uint ExecuteForWait(uint msMax, bool& cancel);

	// 跳过最近一次睡眠，马上开始下一轮循环
	void SkipSleep();

	void ShowStatus();	// 显示状态

    Task* operator[](int taskid);
	
	
	
    private:        
        FixedArray < Task, 32 > _TasksOld;    
};

#endif
