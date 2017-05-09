#include "SerialPort.h"
#include "TInterrupt.h"
#include "stm32f10x.h"

SerialPort *onSerialPortRcv[5];
TInterrupt Interrupt;

// 初始化中断向量表
void TInterrupt::Init()const{

}

void TInterrupt::Process(uint num)const{

}

// 注册中断函数（中断号，函数，参数）
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
    SerialPort *sp = (SerialPort*)param;
    switch (irq)
    {
        case EXTI0_IRQn:
            break;
        case EXTI1_IRQn:
            break;
        case EXTI2_IRQn:
            break;
        case EXTI3_IRQn:
            break;
        case EXTI4_IRQn:
            break;
        case EXTI9_5_IRQn:
            break;
        case EXTI15_10_IRQn:
            break;
        case USART1_IRQn:
            onSerialPortRcv[0] = sp;
            break;
        case USART2_IRQn:
            onSerialPortRcv[1] = sp;
            break;
        case USART3_IRQn:
            onSerialPortRcv[2] = sp;
            break;
            #ifdef STM32F10X_HD
            case UART4_IRQn:
                onSerialPortRcv[3] = sp;
                break;
            case UART5_IRQn:
                onSerialPortRcv[4] = sp;
                break;
            #endif 
        default:
            break;
    }
    return true;
}

//关闭中断
bool TInterrupt::Deactivate(short irq)
{
    return false;
}

// 设置优先级
void TInterrupt::SetPriority(short irq, uint priority)const
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    switch (irq)
    {
        case EXTI0_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = EXTI0_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case EXTI1_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = EXTI1_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case EXTI2_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = EXTI2_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case EXTI3_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = EXTI3_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case EXTI4_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = EXTI4_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case EXTI9_5_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case EXTI15_10_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case USART1_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART1_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case USART2_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART2_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
        case USART3_IRQn:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART3_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = priority;
            break;
            #ifdef STM32F10X_HD
            case UART4_IRQn:
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART4_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
                break;
            case UART5_IRQn:
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART5_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
                NVIC_Init(&nvic);
                break;
            #endif 
        default:
            break;
    }

    NVIC_Init(&nvic);
}

// 获取优先级
void TInterrupt::GetPriority(short irq)const{

}
// 编码优先级
uint TInterrupt::EncodePriority(uint priorityGroup, uint preemptPriority, uint subPriority)const
{
    return 0;
}

// 解码优先级
void TInterrupt::DecodePriority(uint priority, uint priorityGroup, uint *pPreemptPriority, uint *pSubPriority)const{

}
// 打开全局中断
void TInterrupt::GlobalEnable()
{
    __ASM volatile("cpsie i");
}

// 关闭全局中断
void TInterrupt::GlobalDisable()
{
    __ASM volatile("cpsid i");
}

// 全局中断开关状态
bool TInterrupt::GlobalState()
{
    return false;
}

// 是否在中断里面
bool TInterrupt::IsHandler()
{
    return false;
}

// 系统挂起
void TInterrupt::Halt(){}
void TInterrupt::OnInit()const{}
bool TInterrupt::OnActivate(short irq)
{
    return false;
}

bool TInterrupt::OnDeactivate(short irq)
{
    return false;
}

Lock::Lock(int &ref){}
Lock::~Lock(){}

bool Lock::Wait(int ms)
{
    return false;
}




//所有中断线处理
void EXTI_IRQHandler(ushort num, void *param);
extern SerialPort *_printf_sp;
void OnUsartReceive(ushort num, void *param);
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
		#if 0
		byte ch;
		ch = USART_ReceiveData(USART1);
	  	printf( "%c", ch );    //将接收到的数据直接返回打印
		#else
        if (onSerialPortRcv[0])
        {
            OnUsartReceive(0, onSerialPortRcv[0]);
        }
		#endif
    }
    void USART2_IRQHandler(void) //串口2中断服务程序
    {
        if (onSerialPortRcv[1])
        {
            //            SerialPort::OnUsartReceive(1, onSerialPortRcv[1]);
        }
    }
    void USART3_IRQHandler(void) //串口3中断服务程序
    {
        if (onSerialPortRcv[2])
        {
            OnUsartReceive(2, onSerialPortRcv[2]);
        }
        uint8_t ch;

        if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
        {
            //ch = USART1->DR;
            ch = USART_ReceiveData(USART3);
            printf("%c", ch); //将接受到的数据直接返回打印
        }
    }
    void USART4_IRQHandler(void) //串口4中断服务程序
    {
        if (onSerialPortRcv[3])
        {
            //            SerialPort::OnUsartReceive(3, onSerialPortRcv[3]);
        }
    }
    void USART5_IRQHandler(void) //串口5中断服务程序
    {
        if (onSerialPortRcv[4])
        {
            //           SerialPort::OnUsartReceive(4, onSerialPortRcv[4]);
        }
    }

    void EXTI0_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line0) != RESET)
        {
            EXTI_IRQHandler(EXTI0_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line0);
        }
    }

    void EXTI1_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            EXTI_IRQHandler(EXTI1_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line1);
        }
    }

    void EXTI2_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line2) != RESET)
        {
            EXTI_IRQHandler(EXTI2_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line2);
        }
    }

    void EXTI3_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            EXTI_IRQHandler(EXTI3_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line3);
        }
    }

    void EXTI4_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            EXTI_IRQHandler(EXTI4_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line4);
        }
    }

    void EXTI9_5_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line5) != RESET)
        {
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line5);
        }
        if (EXTI_GetITStatus(EXTI_Line6) != RESET)
        {
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line6);
        }
        if (EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line7);
        }
        if (EXTI_GetITStatus(EXTI_Line8) != RESET)
        {
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line8);
        }
        if (EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line9);
        }
    }
    /// IO 线中断，中断口为PC13
    void EXTI15_10_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line10) != RESET)
        {
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line10);
        }
        if (EXTI_GetITStatus(EXTI_Line11) != RESET)
        {
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line11);
        }
        if (EXTI_GetITStatus(EXTI_Line12) != RESET)
        {
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line12);
        }
        if (EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line13);
        }
        if (EXTI_GetITStatus(EXTI_Line14) != RESET)
        {
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line14);
        }
        if (EXTI_GetITStatus(EXTI_Line15) != RESET)
        {
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            EXTI_ClearITPendingBit(EXTI_Line15);
        }
    }
}
