#pragma once 

#include "Port.h"

class CI2CSoft
{
    public:
        CI2CSoft(uint16_t pinsck, uint16_t pinsda, uint32_t nus = 1); //延时时间默认为10，频率为100kHz
        ~CI2CSoft();
        void Init();
    public:
        void Start(void);
        void Stop(void);
        void Ack(void);
        void NAck(void);
        uint8_t WaitAck(void); //等待应答信号到来 返回值：1，接收应答失败 0，接收应答成功
        
		void WriteByte(uint8_t _ucByte);
        uint8_t ReadByte(void);
    private:
        CPort *psck;
        CPort *psda;
        uint32_t delayus; //延时时间
    private:
        void delay(void);
};
