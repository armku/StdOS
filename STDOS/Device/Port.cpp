#include "Port.h"

Port::Port()
{
    this->_Pin = P0;
    this->State = NULL;
    //this->PinBit = 0;
}

#ifndef TINY	
    Port::~Port()
    {
        #ifdef STM32F0
        #elif defined STM32F1
            // �ָ�Ϊ��ʼ��״̬
            //            ushort bits = PinBit;
            //            int config = InitState &0xFFFFFFFF;
            //            for (int i = 0; i < 16 && bits; i++, bits >>= 1)
            //            {
            //                if (i == 7)
            //                    config = InitState >> 32;
            //                if (bits &1)
            //                {
            //                    uint shift = (i &7) << 2; // ÿ����4λ
            //                    uint mask = 0xF << shift; // ���ε�����λ

            //                    GPIO_TypeDef *port = (GPIO_TypeDef *)this->State;
            //                    if (i &0x08)
            //                    {
            //                        // bit 8 - 15
            //                        port->CRH = port->CRH &~mask | (config &mask);
            //                    }
            //                    else
            //                    {
            //                        // bit 0-7
            //                        port->CRL = port->CRL &~mask | (config &mask);
            //                    }
            //                }
            //            }
        #elif defined STM32F4
        #endif 
    }
#endif 

/*
��һ���ų�ʼ��
 */
Port &Port::Set(Pin pin)
{
    //assert_param(pin != P0);

    this->_Pin = pin;
    //    if (_Pin != P0)
    //    {
    //        this->State = IndexToGroup(pin >> 4);
    //        //        PinBit = 1 << (pin &0x0F);
    //    }
    //    else
    //    {
    //        this->State = NULL;
    //        //        PinBit = 0;
    //    }
    #ifdef STM32F0
    #elif defined STM32F1
        // �������ŵĳ�ʼ״̬������ʱ��ѡ��ָ�
        //        if (_Pin != P0)
        //            InitState = ((UInt64)((GPIO_TypeDef *)this->State)->CRH << 32) + ((GPIO_TypeDef *)this->State)->CRL;
    #elif defined STM32F4
    #endif 


    this->Open();
    return  *this;
}

bool Port::Empty()const
{
    return _Pin == P0;
}

void Port::Close()
{
    this->OnClose();
    this->Opened = false;
}

void Port::Clear(){}

OutputPort::OutputPort(){}
OutputPort::OutputPort(Pin pin)
{
    this->Opened = false;
    Set(pin);
    this->Write(0);
}

OutputPort::OutputPort(Pin pin, byte invert, bool openDrain, byte speed)
{
    this->Opened = false;
    this->Invert = invert;
    this->OpenDrain = openDrain;
    Set(pin);
    this->Write(0);
}

bool OutputPort::ReadInput()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}

void OutputPort::Up(int ms)const
{
    Write(true);
    Sys.Sleep(ms);
    Write(false);
}

void OutputPort::Down(int ms)const
{
    Write(false);
    Sys.Sleep(ms);
    Write(true);
}

void OutputPort::Blink(int times, int ms)const
{
    bool flag = true;
    for (int i = 0; i < times; i++)
    {
        Write(flag);
        flag = !flag;
        Sys.Sleep(ms);
    }
    Write(false);
}

void OutputPort::OnOpen(void *param)
{
    Port::OnOpen(param);
    this->OpenPin(param);
}

AlternatePort::AlternatePort(): OutputPort()
{
    this->Opened = false;
}

AlternatePort::AlternatePort(Pin pin): OutputPort(pin)
{
    this->Opened = false;
    this->OpenDrain = false;
}

AlternatePort::AlternatePort(Pin pin, byte invert, bool openDrain, byte speed)
{
    //�ֲ������У�����ֵ��ȷ��
    this->Opened = false;
    this->Invert = invert;
    this->OpenDrain = openDrain;
    Set(pin);
}

InputPort::InputPort(Pin pin, bool floating, PuPd pupd)
{
    this->Opened = false;
    this->Floating = floating;
    this->Pull = pupd;
    Set(pin);
}

InputPort::InputPort(){

}

bool InputPort::UsePress()
{
    return false;
}

void InputPort::OnClose(){

}

/* �ж�״̬�ṹ�� */
/* һ��16���ж��ߣ���ζ��ͬһ����ÿһ��ֻ����һ������ʹ���ж� */
typedef struct TIntState
{
    Pin Pin;
    InputPort inputport;
    InputPort::IOReadHandler Handler; // ί���¼�
    void *Param; // �¼�������һ��������Ϊ�¼������ߵĶ���Ȼ�������̬�������ó�Ա����	
    bool OldValue;

    uint ShakeTime; // ����ʱ��	
    int Used; // ��ʹ�ô���������ǰ5���ж���˵�����ֻ����1�����ں�����ж�����˵�����ܶ��
} IntState;

// 16���ж���
static IntState InterruptState[16];
static bool hasInitState = false;

InputPort::~InputPort()
{
    // ȡ�������ж�
    //    if (_Registed)
    {
        //       Register(NULL);
    }
}

// ��ȡ�����������ţ������Ϊtrue�򷵻�true����ҪΪ��һ���ŷ���
bool InputPort::Read()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}

void GPIO_ISR(int num) // 0 <= num <= 15
{
    if (!hasInitState)
    {
        return ;
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
    if (!state3->Handler)
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
    if (state3->Handler)
    {
        // ��ֵvalueΪtrue��˵�����������ڶ���������down������ȡ��
        state3->Handler(&(state3->inputport), value, state3->Param);
    }
}
