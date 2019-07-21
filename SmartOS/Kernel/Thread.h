#ifndef __Thread_H__
#define __Thread_H__

#include "Kernel\Sys.h"
#include "Core\LinkedList.h"

class Thread;

// �߳�
class Thread : public LinkedNode<Thread>
{
private:
	bool CheckExpire();	// ���Sleep�Ƿ����
	void CheckStack();	// ���ջ�Ƿ����

public:
	uint ID;		// ���
	cstring Name;	// ����

	uint* Stack;	// ջ��
	uint* StackTop;	// ջ��
	uint StackSize;	// ջ��С

	typedef enum
	{
		Ready = 0,	// ����״̬���ȴ�����ִ��
		Running,	// ����ִ��״̬
		Suspended,	// ����״̬
		Stopped		// ֹͣ״̬
	} States;
	States State;	// ״̬
	bool IsReady;	// �Ƿ�λ�ھ�������

	typedef enum
	{
		Lowest,		// ������ȼ�
		BelowNormal,// �Ե�
		Normal,		// ��ͨ���ȼ�
		AboveNormal,// �Ը�
		Highest		// ������ȼ�
	} Priorities;
	Priorities Priority;	// ���ȼ�

	Thread(Action callback, void* state = nullptr, uint stackSize = 0x200);
	virtual ~Thread();

	void Start();
	void Stop();
	void Suspend();
	void Resume();

	UInt64 DelayExpire;		// ����ʱ�䣬��λ΢�롣˯�ߵ��̴߳ﵽ��ʱ��󽫻ָ�����
	void Sleep(uint ms);	// ˯��ָ����������

	// ��̬����
private:
	static bool Inited;		// �Ƿ��ѳ�ʼ��
	static uint g_ID;		// ȫ���߳�ID
	static byte	_running;	// ���������߳���
	static byte _sleeps;	// ����˯�ߵ��߳���

	static Thread* Free;	// �����̶߳��У�δ�õ�ʱ��Ƭ
	static Thread* Busy;	// ��������ʱ��Ƭ�ĸ����ȼ���������

	static void Add(Thread* thread);
	static void Remove(Thread* thread);

	static void OnTick();	// ϵͳ�δ�ʱ�Ӷ�ʱ���ø÷���

	static void Init();
	static void OnInit();
	static void OnEnd();	// ÿ���߳̽���ʱִ�и÷����������߳�

	static byte BuildReady();// ׼����������

	static void Schedule();	// ϵͳ�̵߳��ȿ�ʼ
	static void OnSchedule();

public:
	static Thread* Current;	// ����ִ�е��߳�
	static Thread* Idle;	// �����̡߳�������ȼ�
	static Thread* Main;	// ���̡߳��Ե����ȼ�
	static byte Count;		// �̸߳���
	static void Switch();	// �л��̣߳������л�ʱ��Ƭ����һ���߳�
	
private:
	static bool CheckPend();
	static void OnSwitch();
	
// �̳߳�
public:
	//static void QueueUserWorkItem(Func func);
	static void QueueUserWorkItem(Action func, void* param);
};

#endif

/*
SmartOS�������ȼ�����ռʽ���̵߳���
���ԣ�
1��֧�����޶���̡߳��̵߳����ಢ��Ӱ���л�Ч�ʣ���Ӱ�촴����ֹͣ��Ч��
2��֧�����޶����̬���ȼ�������Խ�����ȼ�Խ�ߣ�0Ϊ������ȼ���֧������ʱ��̬�޸�
3��֧�ֵ����趨ÿ���̵߳�ջ��С��������Ҫ����ʹ���ڴ�
4���Զ�����߳�ջ�ռ���������ʱ���ջ���������ʱ��������������
5�������㷨��������˫����������һ��������ȼ��������У�����ʱ��������Ҫ����
6��֧��ʱ��Ƭ���ȡ�ͬ��������ȼ��̹߳�ͬ����ʱ��Ƭ
7���߳���ɺ��������١�

���˼·��
1���������У�һ����������Busy����������������ȼ������̣߳�һ���󱸶���Free���������з�������ȼ��̺߳�������ȼ��Ǿ����̡߳�
2�����̵߳��Ƚ��ھ�������Busy֮��ִ�У������ȼ����߳�Freeû�л��ᣬ���Ǿ�������BusyΪ�գ��ؽ��������С��л��߳�ʱֻ��ҪȡBusy����һ���ڵ㼴�ɣ����Ϊ�����ͷ��ʼ���ǳ���Ч
3���̵߳Ĵ�����ֹͣ�����𡢻ָ���˯�߶����ܻᵼ���ؽ��������У���ȷ��������ȼ��ľ����̵߳õ����ȵ���
4��û���������ھ���״̬���߳�ʱ��ϵͳ��CPU��Դ����������߳�Idle
5��ÿ���߳����Լ���ջ�ռ䣬���̵߳��ȵĹؼ�������PendSV�ж������л���Ҫ���ȵ��߳�ջ��A�̱߳�PendSV�жϴ�ϣ�Ȼ����PendSV�ж������ջ����B�̣߳����������˳�PendSV�ж�ʱ�����ܵ�B�߳�ȥִ�С�
6���л��߳�Switch�������û����ã�Ҳ������ϵͳ�δ�ʱ�ӵ���
*/
