#pragma once 

#include "BasePort.h"

class CSoftI2C
{
    public:
        CSoftI2C(PinPort pinsck, PinPort pinsda, uint32_t nus = 1); //延时时间默认为10，频率为100kHz
        void SetPin(PinPort pinsck, PinPort pinsda);//设置端口
        void Init();
        void Start(void);
        void Stop(void);
        void Ack(void);
        void NAck(void);
        byte WaitAck(void); //等待应答信号到来 返回值：1，接收应答失败 0，接收应答成功
        
		void WriteByte(byte _ucByte);
        byte ReadByte(void);
    private:
        BasePort *psck;
        BasePort *psda;
        uint32_t delayus; //延时时间
    private:
        void delay(void);
};
