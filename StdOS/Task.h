#ifndef __Task_H__
#define __Task_H__

#include "../component/Type.h"
#include "Sys.h"

class TaskScheduler;

// ����
class Task
{
public:
	TaskScheduler* Host;

	uint32_t	ID;			// ���
	const char*	Name;		// ����

	Action	Callback;	// �ص�
	void*	Param;		// ����

	int		Period;		// ����ms
	uint64_t	NextTime;	// ��һ��ִ��ʱ��ms

	int		Times;		// ִ�д���
	int		SleepTime;	// ��ǰ˯��ʱ��us
	int		Cost;		// ƽ��ִ��ʱ��us
	int		CostMs;		// ƽ��ִ��ʱ��ms
	int		MaxCost;	// ���ִ��ʱ��us

	bool	Enable;		// �Ƿ�����
	bool	Event;		// �Ƿ�ִֻ��һ�κ���ͣ���¼�������
	uint8_t	Deepth;		// ��ǰ���
	uint8_t	MaxDeepth;	// �����ȡ�Ĭ��1�㣬����������

	Task();
	Task(const Task& task)	= delete;
	~Task();

	// ִ�����񡣷����Ƿ�����ִ�С�
	bool Execute(uint64_t now);
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

	bool CheckTime(uint64_t end, bool isSleep);
	void Init();
};
// ���������
class TaskScheduler
{
private:
	List<Task*>	_Tasks;	// �����б�
	bool	_SkipSleep;	// �������һ��˯�ߣ����Ͽ�ʼ��һ��ѭ��

	friend class Task;

public:
	const char*	Name;	// ϵͳ����
	int		Count;		// �������
	Task*	Current;	// ����ִ�е�����
	bool	Running;	// �Ƿ���������
	bool	Sleeping;	// �����ǰ����Sleep״̬������ֹͣ���˳�
	uint8_t	Deepth;		// ��ǰ���
	uint8_t	MaxDeepth;	// �����ȡ�Ĭ��5��

	int		Times;		// ִ�д���
	int		Cost;		// ƽ��ִ��ʱ��us
	uint64_t	TotalSleep;	// �����������˯��ʱ��ms
	uint64_t	LastTrace;	// ���ͳ�Ƹ���ʱ��ms

	typedef void (*SAction)(int ms);
	SAction	EnterSleep;	// ֪ͨ�ⲿ����Ҫ˯�����ɺ���
	Func	ExitSleep;	// ֪ͨ�ⲿ��Ҫ���˳�˯�ߣ��ָ�����

	TaskScheduler(const char* name = nullptr);

	// ʹ���ⲿ��������ʼ�������б�����Ƶ���Ķѷ���
	void Set(Task* tasks, int count);
	// �������񣬷��������š�dueTime�״ε���ʱ��ms��-1��ʾ�¼�������period���ȼ��ms��-1��ʾ������һ��
	uint32_t Add(Action func, void* param, int dueTime = 0, int period = 0, const char* name = nullptr);
	template<typename T>
	uint32_t Add(void(T::*func)(), T* target, int dueTime = 0, int period = 0, const char* name = nullptr)
	{
		return Add(*(Action*)&func, target, dueTime, period, name);
	}
	void Remove(uint32_t taskid);

	void Start();
	void Stop();
	// ִ��һ��ѭ����ָ��������ʱ��
	void Execute(uint32_t msMax, bool& cancel);
	uint32_t ExecuteForWait(uint32_t msMax, bool& cancel);

	// �������һ��˯�ߣ����Ͽ�ʼ��һ��ѭ��
	void SkipSleep();

	void ShowStatus();	// ��ʾ״̬

    Task* operator[](int taskid);
};

#endif
