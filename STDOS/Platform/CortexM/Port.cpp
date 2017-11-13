#include "Port.h"

int IndexToGroup(int a1)
{
  return (a1 << 10) + 1073809408;
}

/* �ж�״̬�ṹ�� */
/* һ��16���ж��ߣ���ζ��ͬһ����ÿһ��ֻ����һ������ʹ���ж� */
typedef struct TIntState
{
    Pin Pin;
    InputPort inputport;
    //InputPort::IOReadHandler Handler; // ί���¼�
	Delegate2<InputPort&, bool>	Press;	// �����¼�
    void *Param; // �¼�������һ��������Ϊ�¼������ߵĶ���Ȼ�������̬�������ó�Ա����	
    bool OldValue;

    uint ShakeTime; // ����ʱ��	
    int Used; // ��ʹ�ô���������ǰ5���ж���˵�����ֻ����1�����ں�����ж�����˵�����ܶ��
} IntState;


// 16���ж���
static IntState InterruptState[16];
static bool hasInitState = false;
//InputPort *
void GPIO_ISR(int num) // 0 <= num <= 15
{
	
	if (!hasInitState)
    {
//        return ;
    }
    IntState *state3 = InterruptState + num;
    if (!state3)
    {
        return ;
    }
	
    #if 0
        uint bit = 1 << num;
    #endif 
    bool value;
    //    value = InputPort::Read(state3->Pin);
    //byte line = EXTI_Line0 << num;
    // ���δָ��ί�У��򲻴���
    if (!state3->Press)
    {
        return ;
    }	
    #if 0
        // Ĭ��20us����ʱ��
        uint shakeTime = state->ShakeTime;
    #endif 
    #if 0
        do
        {
            EXTI->PR = bit; // ���ù���λ
            value = InputPort::Read(state->Pin); // ��ȡ����״̬
            if (shakeTime > 0)
            {
                // ֵ�����б䶯�Ŵ���
                if (value == state->OldValue)
                {
                    return ;
                }
                Time.Sleep(shakeTime); // ���ⶶ��
            }
        }
        while (EXTI->PR &bit); // ����ٴι������ظ�
    #endif 
    //EXTI_ClearITPendingBit(line);
    #if 0
        // ֵ�����б䶯�Ŵ���
        if (shakeTime > 0 && value == state->OldValue)
            return ;
        state->OldValue = value;
    #endif 
    
	if (state3->Press)
    {
        // ��ֵvalueΪtrue��˵�����������ڶ���������down������ȡ��
        state3->Press((state3->inputport), value);
    }
}

void Port::OnClose(){}

void Port::Opening()
{
	//GPIO_StructInit();
}
bool InputPort::OnRegister()
{
	if(this->Press)
	{
		IntState *state=new IntState();
		state->Press=this->Press;
		state->inputport._Pin=this->_Pin;
		InterruptState[this->_Pin&0x0f]=*state;
	}
	return true;
}
void InputPort::ClosePin()
{
	
}
bool OutputPort::Read()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}
