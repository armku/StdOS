#pragma once 

#include "InputPort.h"

class CSoftI2C
{
    public:
        CSoftI2C(Pin pinsck, Pin pinsda, uint nus = 1); //延时时间默认为10，频率为100kHz
        void SetPin(Pin pinsck, Pin pinsda);//设置端口
        void Init();
        void Start(void);
        void Stop(void);
        void Ack(void);
        void NAck(void);
        byte WaitAck(void); //等待应答信号到来 返回值：1，接收应答失败 0，接收应答成功
        
		void WriteByte(byte _ucByte);
        byte ReadByte(void);
    private:
        InputPort *psck;
        InputPort *psda;
        uint delayus; //延时时间
    private:
        void delay(void);
};
