/*
���ļ���Ҫ�Ż���
 */
#include "TTime.h"
//#include "_Core.h"
#include "Platform\stm32.h"
#include "BspPlatform\BspPlatform.h"

extern int gTicks; //
void TTime::Init()
{
    // ��ʼ��Ϊ���
    this->Index = 5;

    gTicks = (Sys.Clock >> 3) / 0xF4240u;
    
    SysTick_Config(9000); //����SysTick tick is 9ms	9000
	SysTick->CTRL  = SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر�jtag������swd	
}

// ��ǰ�δ�ʱ��
uint32_t TTime::CurrentTicks()const
{
    return CurrentTicks1();
}

// ��ǰ������ ����ֵ2000
uint64_t TTime::Current()const
{    
    return this->Milliseconds + CurrentTick();
}

//us��ʱ��100us���¾�ȷ
void TTime::DelayUs(int nus) const
{
	/*
	F103 72MHz  750us->753us
	*/
	for(int i=0;i<nus;i++)
	{
		for(int j=0;j<10;j++);		
	}
}
#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //����Ϊ��ຯ��
    //THUMBָ�֧�ֻ������
    //�������·���ʵ��ִ�л��ָ��WFI  
    void WFI_SET(void)
    {
            __ASM volatile("wfi");
    }

    //����ջ����ַ
    //addr:ջ����ַ
    __asm void MSR_MSP(uint32_t addr)
    {
        MSR MSP, r0  //set Main Stack value
        BX r14
    }
    #ifdef __cplusplus
    }
#endif
