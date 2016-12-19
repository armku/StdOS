#pragma once 

#include "Port.h"

class CI2CSoft
{
    public:
        CI2CSoft(uint16_t pinsck, uint16_t pinsda, uint32_t nus = 1); //��ʱʱ��Ĭ��Ϊ10��Ƶ��Ϊ100kHz
        ~CI2CSoft();
        void Init();
    public:
        void Start(void);
        void Stop(void);
        void Ack(void);
        void NAck(void);
        uint8_t WaitAck(void); //�ȴ�Ӧ���źŵ��� ����ֵ��1������Ӧ��ʧ�� 0������Ӧ��ɹ�
        
		void WriteByte(uint8_t _ucByte);
        uint8_t ReadByte(void);
    private:
        CPort *psck;
        CPort *psda;
        uint32_t delayus; //��ʱʱ��
    private:
        void delay(void);
};
