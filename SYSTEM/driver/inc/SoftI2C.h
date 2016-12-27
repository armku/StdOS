#pragma once 

#include "BasePort.h"

class CSoftI2C
{
    public:
        CSoftI2C(PinPort pinsck, PinPort pinsda, uint32_t nus = 1); //��ʱʱ��Ĭ��Ϊ10��Ƶ��Ϊ100kHz
        void SetPin(PinPort pinsck, PinPort pinsda);//���ö˿�
        void Init();
        void Start(void);
        void Stop(void);
        void Ack(void);
        void NAck(void);
        byte WaitAck(void); //�ȴ�Ӧ���źŵ��� ����ֵ��1������Ӧ��ʧ�� 0������Ӧ��ɹ�
        
		void WriteByte(byte _ucByte);
        byte ReadByte(void);
    private:
        BasePort *psck;
        BasePort *psda;
        uint32_t delayus; //��ʱʱ��
    private:
        void delay(void);
};
