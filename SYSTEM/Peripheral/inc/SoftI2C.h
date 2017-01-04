#pragma once 

#include "InputPort.h"

class CSoftI2C
{
    public:
        CSoftI2C(Pin pinsck, Pin pinsda, uint nus = 1); //��ʱʱ��Ĭ��Ϊ10��Ƶ��Ϊ100kHz
        void SetPin(Pin pinsck, Pin pinsda);//���ö˿�
        void Init();
        void Start(void);
        void Stop(void);
        void Ack(void);
        void NAck(void);
        byte WaitAck(void); //�ȴ�Ӧ���źŵ��� ����ֵ��1������Ӧ��ʧ�� 0������Ӧ��ɹ�
        
		void WriteByte(byte _ucByte);
        byte ReadByte(void);
    private:
        InputPort *psck;
        InputPort *psda;
        uint delayus; //��ʱʱ��
    private:
        void delay(void);
};
