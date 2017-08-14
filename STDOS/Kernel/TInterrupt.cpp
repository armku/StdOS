#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

SerialPort *onSerialPortRcv[5];
Timer *onTimerPortRcv[18];
TInterrupt Interrupt;

// 初始化中断向量表
void TInterrupt::Init()const
{

}

void TInterrupt::Process(uint num)const
{

}
// 注册中断函数（中断号，函数，参数）
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
    switch (irq)
    {
        case  - 14: 
        //NonMaskableInt_IRQn
        break;
        case  - 13: 
        //
        break;
        case  - 12: 
        //MemoryManagement_IRQn
        break;
        case  - 11: 
        //BusFault_IRQn
        break;
        case  - 10: 
        //UsageFault_IRQn
        break;
        case  - 9: 
        //
        break;
        case  - 8: 
        //
        break;
        case  - 7: 
        //
        break;
        case  - 6: 
        //
        break;
        case  - 5: 
        //SVCall_IRQn
        break;
        case  - 4: 
        //DebugMonitor_IRQn
        break;
        case  - 3: 
        //
        break;
        case  - 2: 
        //PendSV_IRQn
        break;
        case  - 1: 
        //SysTick_IRQn
        break;
        case 0:
            //WWDG_IRQn
            break;
        case 1:
            //PVD_IRQn
            break;
        case 2:
            //TAMPER_IRQn
            break;
        case 3:
            //RTC_IRQn
            break;
        case 4:
            //FLASH_IRQn
            break;
        case 5:
            //RCC_IRQn
            break;
        case 6:
            //EXTI0_IRQn
            break;
        case 7:
            //EXTI1_IRQn
            break;
        case 8:
            //EXTI2_IRQn
            break;
        case 9:
            //EXTI3_IRQn
            break;
        case 10:
            //EXTI4_IRQn
            break;
        case 11:
            //DMA1_Channel1_IRQn
            break;
        case 12:
            //DMA1_Channel2_IRQn
            break;
        case 13:
            //DMA1_Channel3_IRQn
            break;
        case 14:
            //DMA1_Channel4_IRQn
            break;
        case 15:
            //DMA1_Channel5_IRQn
            break;
        case 16:
            //DMA1_Channel6_IRQn
            break;
        case 17:
            //DMA1_Channel7_IRQn
            break;
        case 18:
            //ADC1_2_IRQn
            break;
        case 19:
            //CAN1_TX_IRQn
            break;
        case 20:
            //CAN1_RX0_IRQn
            break;
        case 21:
            //CAN1_RX1_IRQn
            break;
        case 22:
            //CAN1_SCE_IRQn
            break;
        case 23:
            //EXTI9_5_IRQn
            break;
        case 24:
            //TIM1_BRK_IRQn
            break;
        case 25:
            //TIM1_UP_IRQn
            onTimerPortRcv[0] = (Timer*)param;
            break;
        case 26:
            //TIM1_TRG_COM_IRQn
            break;
        case 27:
            //TIM1_CC_IRQn
            break;
        case 28:
            //TIM2_IRQn
            onTimerPortRcv[1] = (Timer*)param;
            break;
        case 29:
            //TIM3_IRQn
            onTimerPortRcv[2] = (Timer*)param;
            break;
        case 30:
            //TIM4_IRQn
            onTimerPortRcv[3] = (Timer*)param;
            break;
        case 31:
            //I2C1_EV_IRQn
            break;
        case 32:
            //I2C1_ER_IRQn
            break;
        case 33:
            //I2C2_EV_IRQn
            break;
        case 34:
            //I2C2_ER_IRQn
            break;
        case 35:
            //SPI1_IRQn
            break;
        case 36:
            //SPI2_IRQn
            break;
        case 37:
            //USART1_IRQn
            onSerialPortRcv[0] = (SerialPort*)param;
            break;
        case 38:
            //USART2_IRQn
            onSerialPortRcv[1] = (SerialPort*)param;
            break;
        case 39:
            //USART3_IRQn
            onSerialPortRcv[2] = (SerialPort*)param;
            break;
        case 40:
            //EXTI15_10_IRQn
            break;
        case 41:
            //RTCAlarm_IRQn
            break;
        case 42:
            //OTG_FS_WKUP_IRQn
            break;
        case 43:
            //TIM8_BRK_IRQn
            break;
        case 44:
            //TIM8_UP_IRQn
            break;
        case 45:
            //TIM8_TRG_COM_IRQn
            break;
        case 46:
            //TIM8_CC_IRQn
            break;
        case 47:
            //ADC3_IRQn
            break;
        case 48:
            //FSMC_IRQn
            break;
        case 49:
            //SDIO_IRQn
            break;
        case 50:
            //TIM5_IRQn
            onTimerPortRcv[4] = (Timer*)param;
            break;
        case 51:
            //SPI3_IRQn
            break;
        case 52:
            //UART4_IRQn
            onSerialPortRcv[3] = (SerialPort*)param;
            break;
        case 53:
            //UART5_IRQn
            onSerialPortRcv[4] = (SerialPort*)param;
            break;
        case 54:
            //TIM6_IRQn
            onTimerPortRcv[5] = (Timer*)param;
            break;
        case 55:
            //TIM7_IRQn
            onTimerPortRcv[6] = (Timer*)param;
            break;
        case 56:
            //DMA2_Channel1_IRQn
            break;
        case 57:
            //DMA2_Channel2_IRQn
            break;
        case 58:
            //DMA2_Channel3_IRQn
            break;
        case 59:
            //DMA2_Channel4_IRQn
            break;
        case 60:
            //DMA2_Channel5_IRQn
            break;
        case 61:
            //ETH_IRQn
            break;
        case 62:
            //ETH_WKUP_IRQn
            break;
        case 63:
            //CAN2_TX_IRQn
            break;
        case 64:
            //CAN2_RX0_IRQn
            break;
        case 65:
            //CAN2_RX1_IRQn
            break;
        case 66:
            //CAN2_SCE_IRQn
            break;
        case 67:
            //OTG_FS_IRQn
            break;
        default:
            break;
    }
    return true;
}
// 解除中断注册
bool TInterrupt::Deactivate(short irq)
{
    return false;
}
// 开中断
//bool TInterrupt::Enable(short irq) const{return false;}
// 关中断
//bool TInterrupt::Disable(short irq) const{return false;}

// 是否开中断
//bool TInterrupt::EnableState(short irq) const{return false;}
// 是否挂起
//bool TInterrupt::PendingState(short irq) const{return false;}
// 设置优先级
void TInterrupt::SetPriority(short irq, uint priority)const
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannel = irq;
    switch (irq)
    {
        case  - 14: 
        //NonMaskableInt_IRQn
        break;
        case  - 13: 
        //
        break;
        case  - 12: 
        //MemoryManagement_IRQn
        break;
        case  - 11: 
        //BusFault_IRQn
        break;
        case  - 10: 
        //UsageFault_IRQn
        break;
        case  - 9: 
        //
        break;
        case  - 8: 
        //
        break;
        case  - 7: 
        //
        break;
        case  - 6: 
        //
        break;
        case  - 5: 
        //SVCall_IRQn
        break;
        case  - 4: 
        //DebugMonitor_IRQn
        break;
        case  - 3: 
        //
        break;
        case  - 2: 
        //PendSV_IRQn
        break;
        case  - 1: 
        //SysTick_IRQn
        break;
        case 0:
            //WWDG_IRQn
            break;
        case 1:
            //PVD_IRQn
            break;
        case 2:
            //TAMPER_IRQn
            break;
        case 3:
            //RTC_IRQn
            break;
        case 4:
            //FLASH_IRQn
            break;
        case 5:
            //RCC_IRQn
            break;
        case 6:
            //EXTI0_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 7:
            //EXTI1_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 8:
            //EXTI2_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 9:
            //EXTI3_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 10:
            //EXTI4_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 11:
            //DMA1_Channel1_IRQn
            break;
        case 12:
            //DMA1_Channel2_IRQn
            break;
        case 13:
            //DMA1_Channel3_IRQn
            break;
        case 14:
            //DMA1_Channel4_IRQn
            break;
        case 15:
            //DMA1_Channel5_IRQn
            break;
        case 16:
            //DMA1_Channel6_IRQn
            break;
        case 17:
            //DMA1_Channel7_IRQn
            //F0 TIM6
            #ifdef STM32F0
                nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
                nvic.NVIC_IRQChannelPriority = 2;
                nvic.NVIC_IRQChannelCmd = ENABLE;
            #endif 
            break;
        case 18:
            //ADC1_2_IRQn
            //F0 TIM7
            #ifdef STM32F0
                nvic.NVIC_IRQChannel = TIM7_IRQn;
                nvic.NVIC_IRQChannelPriority = 2;
                nvic.NVIC_IRQChannelCmd = ENABLE;
            #endif 
            break;
        case 19:
            //CAN1_TX_IRQn
            break;
        case 20:
            //CAN1_RX0_IRQn
            break;
        case 21:
            //CAN1_RX1_IRQn
            break;
        case 22:
            //CAN1_SCE_IRQn
            break;
        case 23:
            //EXTI9_5_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 24:
            //TIM1_BRK_IRQn
            break;
        case 25:
            //TIM1_UP_IRQn
            break;
        case 26:
            //TIM1_TRG_COM_IRQn
            break;
        case 27:
            //TIM1_CC_IRQn
            break;
        case 28:
            //TIM2_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
            #elif defined STM32F0
            #endif 
            nvic.NVIC_IRQChannelCmd = ENABLE;
            break;
            #if defined(STM32F1) || defined(STM32F4)
            case 29:
                //TIM3_IRQn
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
                nvic.NVIC_IRQChannelCmd = ENABLE;
                break;
            case 30:
                //TIM4_IRQn
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
                nvic.NVIC_IRQChannelCmd = ENABLE;
                break;
            #elif defined STM32F0
            #endif 
        case 31:
            //I2C1_EV_IRQn
            break;
        case 32:
            //I2C1_ER_IRQn
            break;
        case 33:
            //I2C2_EV_IRQn
            break;
        case 34:
            //I2C2_ER_IRQn
            break;
        case 35:
            //SPI1_IRQn
            break;
        case 36:
            //SPI2_IRQn
            break;
        case 37:
            //USART1_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 38:
            //USART2_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 39:
            //USART3_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 40:
            //EXTI15_10_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 41:
            //RTCAlarm_IRQn
            break;
        case 42:
            //OTG_FS_WKUP_IRQn
            break;
        case 43:
            //TIM8_BRK_IRQn
            break;
        case 44:
            //TIM8_UP_IRQn
            break;
        case 45:
            //TIM8_TRG_COM_IRQn
            break;
        case 46:
            //TIM8_CC_IRQn
            break;
        case 47:
            //ADC3_IRQn
            break;
        case 48:
            //FSMC_IRQn
            break;
        case 49:
            //SDIO_IRQn
            break;
        case 50:
            //TIM5_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
            #elif defined STM32F0
            #endif 
            nvic.NVIC_IRQChannelCmd = ENABLE;
            break;
        case 51:
            //SPI3_IRQn
            break;
        case 52:
            //UART4_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            break;
        case 53:
            //UART5_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = priority;
            #elif defined STM32F0
            #endif 
            NVIC_Init(&nvic);
            break;
        case 54:
            //TIM6_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
            #elif defined STM32F0
            #endif 
            nvic.NVIC_IRQChannelCmd = ENABLE;
            break;
        case 55:
            //TIM7_IRQn
            #if defined(STM32F1) || defined(STM32F4)
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
            #elif defined STM32F0
            #endif 
            nvic.NVIC_IRQChannelCmd = ENABLE;
            break;
        case 56:
            //DMA2_Channel1_IRQn
            break;
        case 57:
            //DMA2_Channel2_IRQn
            break;
        case 58:
            //DMA2_Channel3_IRQn
            break;
        case 59:
            //DMA2_Channel4_IRQn
            break;
        case 60:
            //DMA2_Channel5_IRQn
            break;
        case 61:
            //ETH_IRQn
            break;
        case 62:
            //ETH_WKUP_IRQn
            break;
        case 63:
            //CAN2_TX_IRQn
            break;
        case 64:
            //CAN2_RX0_IRQn
            break;
        case 65:
            //CAN2_RX1_IRQn
            break;
        case 66:
            //CAN2_SCE_IRQn
            break;
        case 67:
            //OTG_FS_IRQn
            break;
        default:
            break;
    }

    NVIC_Init(&nvic);
}
// 获取优先级
void TInterrupt::GetPriority(short irq)const
{

}
// 编码优先级
uint TInterrupt::EncodePriority(uint priorityGroup, uint preemptPriority, uint subPriority)const
{
    return 0;
}

// 解码优先级
void TInterrupt::DecodePriority(uint priority, uint priorityGroup, uint *pPreemptPriority, uint *pSubPriority)const{

}
// 全局中断开关状态
bool TInterrupt::GlobalState()
{
    return false;
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
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Lock::Lock(int &ref){}
Lock::~Lock(){}

bool Lock::Wait(int ms)
{
    return false;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
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
        if (onSerialPortRcv[0])
        {
            OnUsartReceive(0, onSerialPortRcv[0]);
        }
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
    int timer2testcnt123 = 0;
    int time2, time3, time4, time5, time6, time7;
    void TIM2_IRQHandler(void)
    {
        if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
        {
            if (onTimerPortRcv[1])
            {
                onTimerPortRcv[1]->OnInterrupt();
            }
            TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
        }
        timer2testcnt123++;
        time2++;
    }

    void TIM3_IRQHandler()
    {
        if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
        {
            time3++;
            if (onTimerPortRcv[2])
            {
                onTimerPortRcv[2]->OnInterrupt();
            }
            TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
        }
    }
    #if defined(STM32F1) || defined(STM32F4)
        void TIM4_IRQHandler()
        {
            if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
            {
                time4++;
                if (onTimerPortRcv[3])
                {
                    onTimerPortRcv[3]->OnInterrupt();
                }
                TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
            }
        }
        void TIM5_IRQHandler()
        {
            if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
            {
                time5++;
                if (onTimerPortRcv[4])
                {
                    onTimerPortRcv[4]->OnInterrupt();
                }
                TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
            }
        }
        void TIM6_IRQHandler()
        {
            if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
            {
                time6++;
                if (onTimerPortRcv[5])
                {
                    onTimerPortRcv[5]->OnInterrupt();
                }
                TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
            }
        }
        void TIM7_IRQHandler()
        {
            if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
            {
                time7++;
                if (onTimerPortRcv[6])
                {
                    onTimerPortRcv[6]->OnInterrupt();
                }
                TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
            }
        }
    #elif defined STM32F0
        void TIM6_IRQHandler(void)
        {
            TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //先清空中断标志位，以备下次使用。
            if (onTimerPortRcv[5])
            {
                onTimerPortRcv[5]->OnInterrupt();
            }
        }
		void TIM7_IRQHandler(void)
        {
            TIM_ClearITPendingBit(TIM7, TIM_IT_Update); //先清空中断标志位，以备下次使用。
            if (onTimerPortRcv[6])
            {
                onTimerPortRcv[6]->OnInterrupt();
            }
        }
    #endif 
    void EXTI0_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line0) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI0_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI0_1_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line0);
        }
    }

    void EXTI1_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI0_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI0_1_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line1);
        }
    }

    void EXTI2_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line2) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI2_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI2_3_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line2);
        }
    }

    void EXTI3_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI3_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI2_3_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line3);
        }
    }

    void EXTI4_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI4_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line4);
        }
    }

    void EXTI9_5_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line5) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line5);
        }
        if (EXTI_GetITStatus(EXTI_Line6) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line6);
        }
        if (EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line7);
        }
        if (EXTI_GetITStatus(EXTI_Line8) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line8);
        }
        if (EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI9_5_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line9);
        }
    }
    /// IO 线中断，中断口为PC13
    void EXTI15_10_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line10) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line10);
        }
        if (EXTI_GetITStatus(EXTI_Line11) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line11);
        }
        if (EXTI_GetITStatus(EXTI_Line12) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line12);
        }
        if (EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line13);
        }
        if (EXTI_GetITStatus(EXTI_Line14) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line14);
        }
        if (EXTI_GetITStatus(EXTI_Line15) != RESET)
        {
            #if defined(STM32F1) || defined(STM32F4)			
                EXTI_IRQHandler(EXTI15_10_IRQn, 0);
            #elif defined STM32F0
                EXTI_IRQHandler(EXTI4_15_IRQn, 0);
            #endif 
            EXTI_ClearITPendingBit(EXTI_Line15);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
class CInterrupt
{
    public:
        static void USB_LP_CAN1_RX0_IRQHandler();
        static void TIM3_IRQHandler();
};
void CInterrupt::USB_LP_CAN1_RX0_IRQHandler()
{
    //        if(!g_bInterruptPause)        AndyUSB.USB_Istr();
}

void CInterrupt::TIM3_IRQHandler()
{
    //        if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    //        {
    //                TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    //                
    //                g_nNowTime++;
    //                
    ////                if(0==(g_nNowTime%2))
    //                {
    //                        AndyHW.HwCallBackKeyScan();
    //                }
    //                AndyHW.HwCallBackOverCurrent();
    //                AndyOS.sysCallBackTimer10ms();   //10mS
    //        }
}

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    void __initial_sp_ex(void);
    void Reset_Handler(void);

    #ifdef __cplusplus
    }
#endif 

void Default_Handler(void);

typedef void(*const ISR_t)(void);
#if 0
#define FLASH_SAVE_ADDR  0x0800DC00 				//设置FLASH 保存地址(必须为偶数) 保存在55k位置
const ushort a[10] __attribute__((at(FLASH_SAVE_ADDR))) = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};
ISR_t IsrVector[] __attribute__ ((section("RESET"))) =
{
	__initial_sp_ex,
	Reset_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	0,
	0,
	0,
	0,
	Default_Handler,
	Default_Handler,
	0,
	Default_Handler,
	Default_Handler,


	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	CInterrupt::USB_LP_CAN1_RX0_IRQHandler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	CInterrupt::TIM3_IRQHandler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler,
	Default_Handler
};

void Default_Handler(void)
{
    while (1);
}
#endif
