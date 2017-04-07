#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include "Type.h"
// �ж�ί�У��жϺţ�������
typedef void (*InterruptCallback)(ushort num, void* param);
//VectorySize 64 δ��֤
// �жϹ�����
class TInterrupt
{
public:
    void Init() const;    // ��ʼ���ж�������

	void Process(uint num) const;

    // ע���жϺ������жϺţ�������������
    bool Activate(short irq, InterruptCallback isr, void* param = nullptr);
    // ����ж�ע��
    bool Deactivate(short irq);
    // ���ж�
    //bool Enable(short irq) const;
    // ���ж�
    //bool Disable(short irq) const;

    // �Ƿ��ж�
    //bool EnableState(short irq) const;
    // �Ƿ����
    //bool PendingState(short irq) const;

    // �������ȼ�
    void SetPriority(short irq, uint priority = 1) const;
    // ��ȡ���ȼ�
    void GetPriority(short irq) const;

    // �������ȼ�
    uint EncodePriority (uint priorityGroup, uint preemptPriority, uint subPriority) const;
    // �������ȼ�
    void DecodePriority (uint priority, uint priorityGroup, uint* pPreemptPriority, uint* pSubPriority) const;

    static void GlobalEnable();	// ��ȫ���ж�
    static void GlobalDisable();	// �ر�ȫ���ж�
	static bool GlobalState();	// ȫ���жϿ���״̬

	static bool IsHandler();		// �Ƿ����ж�����

	static void Halt();	// ϵͳ����

private:
	void OnInit() const;
	bool OnActivate(short irq);
	bool OnDeactivate(short irq);
};

// ȫ���ж������
extern TInterrupt Interrupt;
// ����maim����  ֮ǰ���г�ʼ��   û�й��캯��...
// ��ʼ������  Interrupt.Init����  �� Sys.cpp �� TSys::TSys() �б�����
// <TSys::TSys()Ҳ�ǹ��캯��   Sys.TSys()���� ��main��������֮ǰ��ִ��>

// ����������ʼ��ʱ����һ������������ʱ����
class Lock
{
private:
	int* _ref;		// ����������������ָ��

public:
	bool Success;	// �Ƿ�ɹ�����

	Lock(int& ref);
	~Lock();

	bool Wait(int ms);
};

extern "C"
{
	void FaultHandler(void);
	void UserHandler(void);

#if defined(BOOT) || defined(APP)
	void RealHandler(void);
#endif
}

#endif

/*
��ȫ�ӹ��жϣ���RAM�п����ж�������������ʱ�ɻ���
�����ж�������Ҫ��128���룬��������128�ֽڣ��ҵ��������������ʹ��

Ϊ����ǿ�жϺ�����������ʹ��_Vectors��Ϊ�������ж�������ȫ��ʹ��OnHandler��Ϊ�жϴ�������
Ȼ����OnHandler�ڲ���ȡ�жϺţ��ٵ���Vectors�б�����û�ί�У������������жϺźͲ�����
*/
