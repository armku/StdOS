#include <stdio.h>
#include "SerialPortOld.h"
#include "stm32f10x.h"
#include "InputPortOld.h"
#include "AlternatePort.h"

SerialPortOld::SerialPortOld(COM_Def index, int baudRate, byte parity, byte dataBits, byte stopBits)
{
    this->_index = index;
    this->_baudRate = baudRate;
    this->_parity = parity;
    this->_dataBits = dataBits;
    this->_stopBits = stopBits;

    this->Name[0] = 'C';
    this->Name[1] = 'O';
    this->Name[2] = 'M';
    this->Name[3] = '1' + index;
    this->Name[4] = 0;

    USART_InitTypeDef usart;
    NVIC_InitTypeDef nvic;

    usart.USART_BaudRate = this->_baudRate;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_Parity = this->_parity;
    usart.USART_StopBits =this->_stopBits;
    usart.USART_WordLength = this->_dataBits;

    nvic.NVIC_IRQChannelCmd = ENABLE;
    InputPortOld *tx;
    InputPortOld *rx;
    //初始化端口引脚
    switch (this->_index)
    {
        case COM1:
            tx = new InputPortOld(PA9);
            rx = new InputPortOld(PA10);
            break;
        case COM2:
            tx = new InputPortOld(PA2);
            rx = new InputPortOld(PA3);
            break;
        case COM3:
            tx = new InputPortOld(PB10);
            rx = new InputPortOld(PB11);
            break;
        case COM4:
            tx = new InputPortOld(PC10);
            rx = new InputPortOld(PC11);
            break;
        case COM5:
            tx = new InputPortOld(PC12);
            rx = new InputPortOld(PD3);
            break;
        default:
            break;
    }
    tx->SetModeAF_PP();
    rx->SetModeIN_FLOATING();

    switch (this->_index)
    {
        case COM1:
            //串口1初始化
            //初始化时钟信号
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            //初始化USART
            USART_Init(USART1, &usart);
            USART_Cmd(USART1, ENABLE);
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
            //初始化NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART1_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 0;
            NVIC_Init(&nvic);
            break;
        case COM2:
            //串口2初始化
            //初始化时钟信号
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            //初始化USART            
            USART_Init(USART2, &usart);
            USART_Cmd(USART2, ENABLE);
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
            //初始化NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART2_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 1;
            NVIC_Init(&nvic);
            break;
        case COM3:
            //串口3初始化
            //初始化时钟信号   
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            //初始化USART            
            USART_Init(USART3, &usart);
            USART_Cmd(USART3, ENABLE);
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
            //初始化NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART3_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 2;
            NVIC_Init(&nvic);
            break;
            #if 0
            case COM4:
                //串口4初始化
                //初始化时钟信号   
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
                //初始化USART            
                USART_Init(UART4, &usart);
                USART_Cmd(UART4, ENABLE);
                USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
                //初始化NVIC
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART4_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
                NVIC_Init(&nvic);
                break;
            case COM5:
                //串口5初始化
                //初始化时钟信号   
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
                //初始化USART            
                USART_Init(UART5, &usart);
                USART_Cmd(UART5, ENABLE);
                USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
                //初始化NVIC
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART5_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 2;
                NVIC_Init(&nvic);
                break;
            #endif 
        default:
            break;
    }
    tx->SetModeAF_PP();
    rx->SetModeIN_FLOATING();

}
void SerialPortOld::Open()
{
	
}

CFIFORing com1buf; //串口1接收缓冲区
uint com1timeidle; //串口1空闲时间
#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //串口1中断服务程序
    //注意,读取USARTx->SR能避免莫名其妙的错误
    /*
     */
    void USART1_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
        //接收到一字节
        {
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
            byte inch = USART1->DR; //读取接收到的数据

            com1buf.Push(inch);
            com1timeidle = 0; //空闲计时器清零
        }
    }

    void USART2_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
        //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        {
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART2); //读取接收到的数据
        }
    }
    void USART3_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
        //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        {
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART3); //读取接收到的数据
        }
    }
    void USART4_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
        //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        {
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
            byte inch = USART_ReceiveData(UART4); //读取接收到的数据
        }
    }
    void USART5_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
        //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        {
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
            byte inch = USART_ReceiveData(UART5); //读取接收到的数据
        }
    }


    //向串口1发送数据
    void sendToUsart1(byte ch)
    {
        #if 0
            while ((USART1->SR &0X40) == 0){}
            //循环发送,直到发送完毕
            USART1->DR = (byte)ch;
        #else 
            USART_SendData(USART1, ch);

            /* 等待发送完毕 */
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
        #endif 
    }

    //向串口2发送数据
    void sendToUsart2(byte ch)
    {
        USART_SendData(USART2, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
    }

    //向串口3发送数据
    void sendToUsart3(byte ch)
    {
        USART_SendData(USART3, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
    }

    //向串口4发送数据
    void sendToUsart4(byte ch)
    {
        USART_SendData(UART4, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){}
    }

    //向串口5发送数据
    void sendToUsart5(byte ch)
    {
        USART_SendData(UART5, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET){}
    }
    void sendch(int ch)
    {
        switch (Sys.MessagePort)
        {
            case COM1:
                sendToUsart1(ch);
                break;
            case COM2:
                sendToUsart2(ch);
                break;
            case COM3:
                sendToUsart3(ch);
                break;
            case COM4:
                sendToUsart4(ch);
                break;
            case COM5:
                sendToUsart5(ch);
                break;
            default:
                break;
        }
    }

    //重定义fputc函数
    int fputc(int ch, FILE *f)
    {
        sendch(ch);
        return ch;
    }
    #ifdef __cplusplus
    }
#endif 
void SerialPortOld::SendData(byte data)
{
    switch (this->_index)
    {
        case COM1:
            sendToUsart1(data);
            break;
        case COM2:
            sendToUsart2(data);
            break;
        case COM3:
            sendToUsart3(data);
            break;
        case COM4:
            sendToUsart4(data);
            break;
        case COM5:
            sendToUsart5(data);
            break;
        default:
            break;
    }
}
//发送数据
void SerialPortOld::SendBuffer(byte *buff,int length)
{
	if(length==-1)
	{
		while(*buff)
		{
			this->SendData(*buff);
			buff++;
		}
		return;
	}
	if(length<=0)
	{
		return;
	}
	for(int i=0;i<length;i++)
	{
		this->SendData(buff[i]);
	}	
}
//发送数据
void SerialPortOld::SendBuffer(char *buff,int length)
{
	this->SendBuffer((byte*)buff,length);
}
// 注册数据到达事件
void SerialPortOld::Register(IOnUsartRead handler,SerialPortOld *sp)
{
	this->OnRcv=&handler;
}	
byte buftmp[20];
//从串口收到数据
void SerialPortOld::OnUsartReceive(byte * buf,ushort length)
{	
	Buffer bs(buftmp,20);
	if(this->OnRcv)
	{
		//*(this->OnRcv)(this,&bs,this);
		this->SendBuffer(this->Name);
	this->SendBuffer(" 收到: ");
	this->SendBuffer(buf,length);	
	this->SendBuffer("\n");
	}
}
