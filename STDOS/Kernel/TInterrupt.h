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
	
    // ע���жϺ������жϺţ�������������
    bool Activate(short irq, InterruptCallback isr, void* param = nullptr);

    // �������ȼ�
    void SetPriority(short irq, uint32_t priority = 1) const;

    static void GlobalEnable();	// ��ȫ���ж�
    static void GlobalDisable();	// �ر�ȫ���ж�
private:
	void OnInit() const;
};

// ȫ���ж������
extern TInterrupt Interrupt;
// ����maim����  ֮ǰ���г�ʼ��   û�й��캯��...
// ��ʼ������  Interrupt.Init����  �� Sys.cpp �� TSys::TSys() �б�����
// <TSys::TSys()Ҳ�ǹ��캯��   Sys.TSys()���� ��main��������֮ǰ��ִ��>

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
