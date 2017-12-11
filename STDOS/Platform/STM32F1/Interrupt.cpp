#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"
#include "stm32f10x.h"

#define NVIC_OFFSET	((uint)0x1000)
#define ISRADDR (NVIC_VectTab_RAM+NVIC_OFFSET)
#define ISRLENGTH	100	//中断数量

TInterrupt Interrupt;

class CInterrupt
{
    public:
        //通用
        static void Reset_Handler(); // Reset Handler
        static void NMI_Handler(); // NMI Handler
        static void HardFault_Handler(); // Hard Fault Handler
        static void MemManage_Handler(); // MPU Fault Handler
        static void BusFault_Handler(); // Bus Fault Handler
        static void UsageFault_Handler(); // Usage Fault Handler
        static void SVC_Handler(); // SVCall Handler
        static void DebugMon_Handler(); // Debug Monitor Handler
        static void PendSV_Handler(); // PendSV Handler
        static void SysTick_Handler(); // SysTick Handler

        //F0        
        static void WWDG_IRQHandler(); // Window Watchdog
        static void PVD_VDDIO2_IRQHandler(); // PVD and VDDIO2 through EXTI Line detect
        static void RTC_IRQHandler(); // RTC through EXTI Line
        static void FLASH_IRQHandler(); // FLASH
        static void RCC_CRS_IRQHandler(); // RCC and CRS
        static void EXTI0_1_IRQHandler(); // EXTI Line 0 and 1
        static void EXTI2_3_IRQHandler(); // EXTI Line 2 and 3
        static void EXTI4_15_IRQHandler(); // EXTI Line 4 to 15
        static void TSC_IRQHandler(); // TS
        static void DMA1_Channel1_IRQHandler(); // DMA1 Channel 1
        static void DMA1_Channel2_3_IRQHandler(); // DMA1 Channel 2 and Channel 3
        static void DMA1_Channel4_5_6_7_IRQHandler(); // DMA1 Channel 4, Channel 5, Channel 6 and Channel 7
        static void ADC1_COMP_IRQHandler(); // ADC1, COMP1 and COMP2 
        static void TIM1_BRK_UP_TRG_COM_IRQHandler(); // TIM1 Break, Update, Trigger and Commutation
        static void TIM1_CC_IRQHandler(); // TIM1 Capture Compare
        static void TIM2_IRQHandler(); // TIM2
        static void TIM3_IRQHandler(); // TIM3
        static void TIM6_DAC_IRQHandler(); // TIM6 and DAC
        static void TIM7_IRQHandler(); // TIM7
        static void TIM14_IRQHandler(); // TIM14
        static void TIM15_IRQHandler(); // TIM15
        static void TIM16_IRQHandler(); // TIM16
        static void TIM17_IRQHandler(); // TIM17
        static void I2C1_IRQHandler(); // I2C1
        static void I2C2_IRQHandler(); // I2C2
        static void SPI1_IRQHandler(); // SPI1
        static void SPI2_IRQHandler(); // SPI2
        static void USART1_IRQHandler(); // USART1
        static void USART2_IRQHandler(); // USART2
        static void USART3_4_IRQHandler(); // USART3 and USART4
        static void CEC_CAN_IRQHandler(); // CEC and CAN
        static void USB_IRQHandler(); // USB

        //F1       
        //static void WWDG_IRQHandler(); // Window Watchdog
        //static void PVD_IRQHandler(); // PVD through EXTI Line detect
        static void TAMPER_IRQHandler(); // Tamper
        //static void RTC_IRQHandler(); // RTC
        //static void FLASH_IRQHandler(); // Flash
        //static void RCC_IRQHandler(); // RCC
        static void EXTI0_IRQHandler(); // EXTI Line 0
        static void EXTI1_IRQHandler(); // EXTI Line 1
        static void EXTI2_IRQHandler(); // EXTI Line 2
        static void EXTI3_IRQHandler(); // EXTI Line 3
        static void EXTI4_IRQHandler(); // EXTI Line 4
        //static void DMA1_Channel1_IRQHandler(); // DMA1 Channel 1
        static void DMA1_Channel2_IRQHandler(); // DMA1 Channel 2
        static void DMA1_Channel3_IRQHandler(); // DMA1 Channel 3
        static void DMA1_Channel4_IRQHandler(); // DMA1 Channel 4
        static void DMA1_Channel5_IRQHandler(); // DMA1 Channel 5
        static void DMA1_Channel6_IRQHandler(); // DMA1 Channel 6
        static void DMA1_Channel7_IRQHandler(); // DMA1 Channel 7
        static void ADC1_2_IRQHandler(); // ADC1 & ADC2
        static void USB_HP_CAN1_TX_IRQHandler(); // USB High Priority or CAN1 TX
        static void USB_LP_CAN1_RX0_IRQHandler(); // USB Low  Priority or CAN1 RX0
        static void CAN1_RX1_IRQHandler(); // CAN1 RX1
        static void CAN1_SCE_IRQHandler(); // CAN1 SCE
        static void EXTI9_5_IRQHandler(); // EXTI Line 9..5
        static void TIM1_BRK_IRQHandler(); // TIM1 Break
        static void TIM1_UP_IRQHandler(); // TIM1 Update
        static void TIM1_TRG_COM_IRQHandler(); // TIM1 Trigger and Commutation
        //static void TIM1_CC_IRQHandler(); // TIM1 Capture Compare
        //static void TIM2_IRQHandler(); // TIM2
        //static void TIM3_IRQHandler(); // TIM3
        static void TIM4_IRQHandler(); // TIM4
        static void I2C1_EV_IRQHandler(); // I2C1 Event
        static void I2C1_ER_IRQHandler(); // I2C1 Error
        static void I2C2_EV_IRQHandler(); // I2C2 Event
        static void I2C2_ER_IRQHandler(); // I2C2 Error
        //static void SPI1_IRQHandler(); // SPI1
        //static void SPI2_IRQHandler(); // SPI2
        //static void USART1_IRQHandler(); // USART1
        //static void USART2_IRQHandler(); // USART2
        static void USART3_IRQHandler(); // USART3
        static void EXTI15_10_IRQHandler(); // EXTI Line 15..10
        static void RTCAlarm_IRQHandler(); // RTC Alarm through EXTI Line
        static void USBWakeUp_IRQHandler(); // USB Wakeup from suspend
        static void TIM8_BRK_IRQHandler(); // TIM8 Break
        static void TIM8_UP_IRQHandler(); // TIM8 Update
        static void TIM8_TRG_COM_IRQHandler(); // TIM8 Trigger and Commutation
        static void TIM8_CC_IRQHandler(); // TIM8 Capture Compare
        static void ADC3_IRQHandler(); // ADC3
        static void FSMC_IRQHandler(); // FSMC
        static void SDIO_IRQHandler(); // SDIO
        static void TIM5_IRQHandler(); // TIM5
        static void SPI3_IRQHandler(); // SPI3
        static void UART4_IRQHandler(); // UART4
        static void UART5_IRQHandler(); // UART5
        static void TIM6_IRQHandler(); // TIM6
        //static void TIM7_IRQHandler(); // TIM7
        static void DMA2_Channel1_IRQHandler(); // DMA2 Channel1
        static void DMA2_Channel2_IRQHandler(); // DMA2 Channel2
        static void DMA2_Channel3_IRQHandler(); // DMA2 Channel3
        static void DMA2_Channel4_5_IRQHandler(); // DMA2 Channel4 & Channel5

        //F4
        //static void WWDG_IRQHandler(); // Window WatchDog                                        
        //static void PVD_IRQHandler(); // PVD through EXTI Line detection                        
        static void TAMP_STAMP_IRQHandler(); // Tamper and TimeStamps through the EXTI line            
        static void RTC_WKUP_IRQHandler(); // RTC Wakeup through the EXTI line                       
        //static void FLASH_IRQHandler(); // FLASH                                           
        //static void RCC_IRQHandler(); // RCC                                             
        //static void EXTI0_IRQHandler(); // EXTI Line0                                             
        //static void EXTI1_IRQHandler(); // EXTI Line1                                             
        //static void EXTI2_IRQHandler(); // EXTI Line2                                             
        //static void EXTI3_IRQHandler(); // EXTI Line3                                             
        //static void EXTI4_IRQHandler(); // EXTI Line4                                             
        static void DMA1_Stream0_IRQHandler(); // DMA1 Stream 0                                   
        static void DMA1_Stream1_IRQHandler(); // DMA1 Stream 1                                   
        static void DMA1_Stream2_IRQHandler(); // DMA1 Stream 2                                   
        static void DMA1_Stream3_IRQHandler(); // DMA1 Stream 3                                   
        static void DMA1_Stream4_IRQHandler(); // DMA1 Stream 4                                   
        static void DMA1_Stream5_IRQHandler(); // DMA1 Stream 5                                   
        static void DMA1_Stream6_IRQHandler(); // DMA1 Stream 6                                   
        static void ADC_IRQHandler(); // ADC1, ADC2 and ADC3s                            
        static void CAN1_TX_IRQHandler(); // CAN1 TX                                                
        static void CAN1_RX0_IRQHandler(); // CAN1 RX0                                               
        //static void CAN1_RX1_IRQHandler(); // CAN1 RX1                                               
        //static void CAN1_SCE_IRQHandler(); // CAN1 SCE                                               
        //static void EXTI9_5_IRQHandler(); // External Line[9:5]s                                    
        static void TIM1_BRK_TIM9_IRQHandler(); // TIM1 Break and TIM9                   
        static void TIM1_UP_TIM10_IRQHandler(); // TIM1 Update and TIM10                 
        static void TIM1_TRG_COM_TIM11_IRQHandler(); // TIM1 Trigger and Commutation and TIM11
        //static void TIM1_CC_IRQHandler(); // TIM1 Capture Compare                                   
        //static void TIM2_IRQHandler(); // TIM2                                            
        //static void TIM3_IRQHandler(); // TIM3                                            
        //static void TIM4_IRQHandler(); // TIM4                                            
        //static void I2C1_EV_IRQHandler(); // I2C1 Event                                             
        //static void I2C1_ER_IRQHandler(); // I2C1 Error                                             
        //static void I2C2_EV_IRQHandler(); // I2C2 Event                                             
        //static void I2C2_ER_IRQHandler(); // I2C2 Error                                               
        //static void SPI1_IRQHandler(); // SPI1                                            
        //static void SPI2_IRQHandler(); // SPI2                                            
        //static void USART1_IRQHandler(); // USART1                                          
        //static void USART2_IRQHandler(); // USART2                                          
        //static void USART3_IRQHandler(); // USART3                                          
        //static void EXTI15_10_IRQHandler(); // External Line[15:10]s                                  
        static void RTC_Alarm_IRQHandler(); // RTC Alarm (A and B) through EXTI Line                  
        static void OTG_FS_WKUP_IRQHandler(); // USB OTG FS Wakeup through EXTI line                        
        static void TIM8_BRK_TIM12_IRQHandler(); // TIM8 Break and TIM12                  
        static void TIM8_UP_TIM13_IRQHandler(); // TIM8 Update and TIM13                 
        static void TIM8_TRG_COM_TIM14_IRQHandler(); // TIM8 Trigger and Commutation and TIM14
        //static void TIM8_CC_IRQHandler(); // TIM8 Capture Compare                                   
        static void DMA1_Stream7_IRQHandler(); // DMA1 Stream7                                           
        //static void FSMC_IRQHandler(); // FSMC                                            
        //static void SDIO_IRQHandler(); // SDIO                                            
        //static void TIM5_IRQHandler(); // TIM5                                            
        //static void SPI3_IRQHandler(); // SPI3                                            
        //static void UART4_IRQHandler(); // UART4                                           
        //static void UART5_IRQHandler(); // // UART5                                           
        //static void TIM6_DAC_IRQHandler(); // TIM6 and DAC1&2 underrun errors                   
        //static void TIM7_IRQHandler(); // TIM7                   
        static void DMA2_Stream0_IRQHandler(); // DMA2 Stream 0                                   
        static void DMA2_Stream1_IRQHandler(); // DMA2 Stream 1                                   
        static void DMA2_Stream2_IRQHandler(); // DMA2 Stream 2                                   
        static void DMA2_Stream3_IRQHandler(); // DMA2 Stream 3                                   
        static void DMA2_Stream4_IRQHandler(); // DMA2 Stream 4                                   
        static void ETH_IRQHandler(); // Ethernet                                        
        static void ETH_WKUP_IRQHandler(); // Ethernet Wakeup through EXTI line                      
        static void CAN2_TX_IRQHandler(); // CAN2 TX                                                
        static void CAN2_RX0_IRQHandler(); // CAN2 RX0                                               
        static void CAN2_RX1_IRQHandler(); // CAN2 RX1                                               
        static void CAN2_SCE_IRQHandler(); // CAN2 SCE                                               
        static void OTG_FS_IRQHandler(); // USB OTG FS                                      
        static void DMA2_Stream5_IRQHandler(); // DMA2 Stream 5                                   
        static void DMA2_Stream6_IRQHandler(); // DMA2 Stream 6                                   
        static void DMA2_Stream7_IRQHandler(); // DMA2 Stream 7                                   
        static void USART6_IRQHandler(); // USART6                                           
        static void I2C3_EV_IRQHandler(); // I2C3 event                                             
        static void I2C3_ER_IRQHandler(); // I2C3 error                                             
        static void OTG_HS_EP1_OUT_IRQHandler(); // USB OTG HS End Point 1 Out                      
        static void OTG_HS_EP1_IN_IRQHandler(); // USB OTG HS End Point 1 In                       
        static void OTG_HS_WKUP_IRQHandler(); // USB OTG HS Wakeup through EXTI                         
        static void OTG_HS_IRQHandler(); // USB OTG HS                                      
        static void DCMI_IRQHandler(); // DCMI                                            
        static void CRYP_IRQHandler(); // CRYP crypto                                     
        static void HASH_RNG_IRQHandler(); // Hash and Rng
        static void FPU_IRQHandler(); // FPU

};
#include "TTime.h"
void *onIsr[ISRLENGTH]; //中断

extern "C"
{
    uint VectorTable[ISRLENGTH] __attribute__((at(ISRADDR)));
    uint *vsrom = (uint*)NVIC_VectTab_FLASH;
}


void FaultHandler(){}
void FAULT_SubHandler(){}

// 解码优先级
void TInterrupt::DecodePriority(uint priority, uint priorityGroup, uint *pPreemptPriority, uint *pSubPriority)const{

}
// 编码优先级
uint TInterrupt::EncodePriority(uint priorityGroup, uint preemptPriority, uint subPriority)const
{
    return 0;
}

void TInterrupt::OnInit()const
{
    //复制中断向量表
    for (int i = 0; i < ISRLENGTH; i++)
    {
        VectorTable[i] = vsrom[i];
    }
    //中断向量表重映射
    NVIC_SetVectorTable(NVIC_VectTab_RAM, NVIC_OFFSET);

    VectorTable[15] = (uint) &(CInterrupt::SysTick_Handler);

    //        VectorTable[16] = (uint) &(CInterrupt::WWDG_IRQHandler); // Window Watchdog
    //        VectorTable[17] = (uint) &(CInterrupt::PVD_IRQHandler); // PVD through EXTI Line detect
    //        VectorTable[18] = (uint) &(CInterrupt::TAMPER_IRQHandler); // Tamper
    //        VectorTable[19] = (uint) &(CInterrupt::RTC_IRQHandler); // RTC
    //        VectorTable[20] = (uint) &(CInterrupt::FLASH_IRQHandler); // Flash
    //        VectorTable[21] = (uint) &(CInterrupt::RCC_IRQHandler); // RCC
    VectorTable[22] = (uint) &(CInterrupt::EXTI0_IRQHandler); // EXTI Line 0
    VectorTable[23] = (uint) &(CInterrupt::EXTI1_IRQHandler); // EXTI Line 1
    VectorTable[24] = (uint) &(CInterrupt::EXTI2_IRQHandler); // EXTI Line 2
    VectorTable[25] = (uint) &(CInterrupt::EXTI3_IRQHandler); // EXTI Line 3
    VectorTable[26] = (uint) &(CInterrupt::EXTI4_IRQHandler); // EXTI Line 4
    //        VectorTable[27] = (uint) &(CInterrupt::DMA1_Channel1_IRQHandler); // DMA1 Channel 1
    //        VectorTable[28] = (uint) &(CInterrupt::DMA1_Channel2_IRQHandler); // DMA1 Channel 2
    //        VectorTable[29] = (uint) &(CInterrupt::DMA1_Channel3_IRQHandler); // DMA1 Channel 3
    //        VectorTable[30] = (uint) &(CInterrupt::DMA1_Channel4_IRQHandler); // DMA1 Channel 4
    //        VectorTable[31] = (uint) &(CInterrupt::DMA1_Channel5_IRQHandler); // DMA1 Channel 5
    //        VectorTable[32] = (uint) &(CInterrupt::DMA1_Channel6_IRQHandler); // DMA1 Channel 6
    //        VectorTable[33] = (uint) &(CInterrupt::DMA1_Channel7_IRQHandler); // DMA1 Channel 7
    //        VectorTable[34] = (uint) &(CInterrupt::ADC1_2_IRQHandler); // ADC1 & ADC2
    //        VectorTable[35] = (uint) &(CInterrupt::USB_HP_CAN1_TX_IRQHandler); // USB High Priority or CAN1 TX
    //        VectorTable[36] = (uint) &(CInterrupt::USB_LP_CAN1_RX0_IRQHandler); // USB Low  Priority or CAN1 RX0
    //        VectorTable[37] = (uint) &(CInterrupt::CAN1_RX1_IRQHandler); // CAN1 RX1
    //        VectorTable[38] = (uint) &(CInterrupt::CAN1_SCE_IRQHandler); // CAN1 SCE
    VectorTable[39] = (uint) &(CInterrupt::EXTI9_5_IRQHandler); // EXTI Line 9..5
    //        VectorTable[40] = (uint) &(CInterrupt::TIM1_BRK_IRQHandler); // TIM1 Break
    //        VectorTable[41] = (uint) &(CInterrupt::TIM1_UP_IRQHandler); // TIM1 Update
    //        VectorTable[42] = (uint) &(CInterrupt::TIM1_TRG_COM_IRQHandler); // TIM1 Trigger and Commutation
    //        VectorTable[43] = (uint) &(CInterrupt::TIM1_CC_IRQHandler); // TIM1 Capture Compare
    VectorTable[44] = (uint) &(CInterrupt::TIM2_IRQHandler); // TIM2
    VectorTable[45] = (uint) &(CInterrupt::TIM3_IRQHandler); // TIM3
    VectorTable[46] = (uint) &(CInterrupt::TIM4_IRQHandler); // TIM4
    //        VectorTable[47] = (uint) &(CInterrupt::I2C1_EV_IRQHandler); // I2C1 Event
    //        VectorTable[48] = (uint) &(CInterrupt::I2C1_ER_IRQHandler); // I2C1 Error
    //        VectorTable[49] = (uint) &(CInterrupt::I2C2_EV_IRQHandler); // I2C2 Event
    //        VectorTable[50] = (uint) &(CInterrupt::I2C2_ER_IRQHandler); // I2C2 Error
    //        VectorTable[51] = (uint) &(CInterrupt::SPI1_IRQHandler); // SPI1
    //        VectorTable[52] = (uint) &(CInterrupt::SPI2_IRQHandler); // SPI2
    VectorTable[53] = (uint) &(CInterrupt::USART1_IRQHandler); // USART1
    VectorTable[54] = (uint) &(CInterrupt::USART2_IRQHandler); // USART2
    VectorTable[55] = (uint) &(CInterrupt::USART3_IRQHandler); // USART3
    VectorTable[56] = (uint) &(CInterrupt::EXTI15_10_IRQHandler); // EXTI Line 15..10
    //        VectorTable[57] = (uint) &(CInterrupt::RTCAlarm_IRQHandler); // RTC Alarm through EXTI Line
    //        VectorTable[58] = (uint) &(CInterrupt::USBWakeUp_IRQHandler); // USB Wakeup from suspend
    //        VectorTable[59] = (uint) &(CInterrupt::TIM8_BRK_IRQHandler); // TIM8 Break
    //        VectorTable[60] = (uint) &(CInterrupt::TIM8_UP_IRQHandler); // TIM8 Update
    //        VectorTable[61] = (uint) &(CInterrupt::TIM8_TRG_COM_IRQHandler); // TIM8 Trigger and Commutation
    //        VectorTable[62] = (uint) &(CInterrupt::TIM8_CC_IRQHandler); // TIM8 Capture Compare
    //        VectorTable[63] = (uint) &(CInterrupt::ADC3_IRQHandler); // ADC3
    //        VectorTable[64] = (uint) &(CInterrupt::FSMC_IRQHandler); // FSMC
    //        VectorTable[65] = (uint) &(CInterrupt::SDIO_IRQHandler); // SDIO
    VectorTable[66] = (uint) &(CInterrupt::TIM5_IRQHandler); // TIM5
    //        VectorTable[67] = (uint) &(CInterrupt::SPI3_IRQHandler); // SPI3
    VectorTable[68] = (uint) &(CInterrupt::UART4_IRQHandler); // UART4
    VectorTable[69] = (uint) &(CInterrupt::UART5_IRQHandler); // UART5
    VectorTable[70] = (uint) &(CInterrupt::TIM6_IRQHandler); // TIM6
    VectorTable[71] = (uint) &(CInterrupt::TIM7_IRQHandler); // TIM7
    //        VectorTable[72] = (uint) &(CInterrupt::DMA2_Channel1_IRQHandler); // DMA2 Channel1
    //        VectorTable[73] = (uint) &(CInterrupt::DMA2_Channel2_IRQHandler); // DMA2 Channel2
    //        VectorTable[74] = (uint) &(CInterrupt::DMA2_Channel3_IRQHandler); // DMA2 Channel3
    //        VectorTable[75] = (uint) &(CInterrupt::DMA2_Channel4_5_IRQHandler); // DMA2 Channel4 & Channel5  
}

// 打开全局中断
void TInterrupt::GlobalEnable()
{
    __ASM volatile("cpsie i");
}

bool TInterrupt::OnDeactivate(short irq)
{
    return false;
}

// 关闭全局中断
void TInterrupt::GlobalDisable()
{
    __ASM volatile("cpsid i");
}

void TInterrupt::SetPriority(short irq, uint priority)const
{
    NVIC_InitTypeDef nvic;

    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannel = irq;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = priority;

    NVIC_Init(&nvic);
    NVIC_SetPriority((IRQn_Type)irq, priority);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void OnUsartReceive(ushort num, void *param);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void CInterrupt::SysTick_Handler()
{
    static uint tickCnt = 0;
    tickCnt++;
    if (tickCnt > 1000)
    {
        tickCnt = 0;
    }
}

//注意,读取USARTx->SR能避免莫名其妙的错误
void CInterrupt::USART1_IRQHandler()
{
    if (onIsr[USART1_IRQn])
    {
        OnUsartReceive(0, onIsr[USART1_IRQn]);
    }
}

void CInterrupt::USART2_IRQHandler()
{
    if (onIsr[USART2_IRQn])
    {
        OnUsartReceive(1, onIsr[USART2_IRQn]);
    }
}

void CInterrupt::USART3_IRQHandler()
{
	if (onIsr[USART3_IRQn])
	{
		OnUsartReceive(2, onIsr[USART3_IRQn]);
	}
}

void CInterrupt::USART3_4_IRQHandler(){

}

void CInterrupt::UART4_IRQHandler()
{
    if (onIsr[UART4_IRQn])
    {
        OnUsartReceive(3, onIsr[UART4_IRQn]);
    }
}

void CInterrupt::UART5_IRQHandler()
{
    if (onIsr[UART5_IRQn])
    {
        OnUsartReceive(4, onIsr[UART5_IRQn]);
    }
}

void CInterrupt::TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM2_IRQn])
        {
            ((Timer*)onIsr[TIM2_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM3_IRQn])
        {
            ((Timer*)onIsr[TIM3_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM4_IRQn])
        {
            ((Timer*)onIsr[TIM4_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM5_IRQHandler()
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM5_IRQn])
        {
            ((Timer*)onIsr[TIM5_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM6_IRQHandler()
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM6_IRQn])
        {
            ((Timer*)onIsr[TIM6_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM6_DAC_IRQHandler(){

}

void CInterrupt::TIM7_IRQHandler()
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM7_IRQn])
        {
            ((Timer*)onIsr[TIM7_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
    }
}

void GPIO_ISR(int num);
void CInterrupt::EXTI0_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        GPIO_ISR(0);
    }
}

void CInterrupt::EXTI1_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line1);
        GPIO_ISR(1);
    }
}

void CInterrupt::EXTI2_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line2);
        GPIO_ISR(2);
    }
}

void CInterrupt::EXTI3_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line3);
        GPIO_ISR(3);
    }
}

void CInterrupt::EXTI4_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line4);
        GPIO_ISR(4);
    }
}

void CInterrupt::EXTI9_5_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
        GPIO_ISR(5);
    }
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        GPIO_ISR(6);
    }
    if (EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
        GPIO_ISR(7);
    }
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
        GPIO_ISR(8);
    }
    if (EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line9);
        GPIO_ISR(9);
    }
}

void CInterrupt::EXTI15_10_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line10);
        GPIO_ISR(10);
    }
    if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line11);
        GPIO_ISR(11);
    }
    if (EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line12);
        GPIO_ISR(12);
    }
    if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line13);
        GPIO_ISR(13);
    }
    if (EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line14);
        GPIO_ISR(14);
    }
    if (EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line15);
        GPIO_ISR(15);
    }
}
