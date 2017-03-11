#pragma once

#include "Sys.h"
#include "stm32f10x.h"

// ��ʱ��
class Timer
{
    private:
        TIM_TypeDef *_port;
        byte _index; // �ڼ�����ʱ������0��ʼ
        volatile bool _started; // �������ж���ر��Լ�

        void ClockCmd(bool state);
    public:
        Timer(TIM_TypeDef *timer);
        ~Timer();

        ushort Prescaler; // Ԥ��Ƶ��ʵ��ֵ����ʱ�����һ��Ĭ��Ԥ���䵽1MHz
        uint Period; // ���ڡ�ʵ��ֵ����ʱ�����һ��Ĭ��1000������

        void Start(); // ��ʼ��ʱ��
        void Stop(); // ֹͣ��ʱ��
        //void SetScaler(uint scaler);        // ����Ԥ��ƵĿ�꣬����1MHz
        void SetFrequency(uint frequency); // ����Ƶ�ʣ��Զ�����Ԥ��Ƶ
        #if 0
            void Register(EventHandler handler, void *param = NULL);
        #endif 

    private:
        void OnInterrupt();
        static void OnHandler(ushort num, void *param);
        #if 0
            EventHandler _Handler;
        #endif 
        void *_Param;

    public:
        static Timer **Timers; // �Ѿ�ʵ�����Ķ�ʱ������
        static const byte TimerCount; // ��ʱ������

        static Timer *Create(byte index = 0xFF); // ����ָ�������Ķ�ʱ�������������ֱ�ӷ��أ�Ĭ��0xFF��ʾ�������
};
