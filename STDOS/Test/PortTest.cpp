/*
F1:
Ұ��ISO-V2 ISO-V3
K0:PA0
K1:PC13


F4
����ԭ��̽����
K0:PA0
K1:PE2
K2:PE3
K3:PE4

 */
#include "Port.h"
#include "Platform\Pin.h"
//void EXTIX_Init(void);
#ifdef DEBUG

    //���Դ���
    extern OutputPort led2;
    InputPort key(PC13);
    InputPort key0(PA0);

    void OnPress(InputPort &port, bool down)
    {
        debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
    }

    void led2flash(void *param)
    {
        led2 = !led2;
    }
    void EXTIX_Init();
    void InterruptTest()
    {
        #if defined STM32F1
            key0.Press = OnPress;
            key0.UsePress();
            key0.Open();

            key.Press = OnPress;
            key.UsePress();
            key.Open();
            //EXTIX_Init();

            Sys.AddTask(led2flash, &led2, 0, 200, "led2");
        #elif defined STM32F4
            //EXTIX_Init(); //��ʼ���ⲿ�ж����� 
			key0.Pull=InputPort::DOWN;
			key0.Press = OnPress;
            key0.UsePress();
			key0.Pull=InputPort::DOWN;
            key0.Open();
        #endif 
    }
#endif 
#ifdef STM32F4
    #include "stm32f4xx.h" 

    #define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
    #define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
    #define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
    //IO�ڵ�ַӳ��
    #define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
    #define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
    #define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
    #define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
    #define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
    #define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
    #define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
    #define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
    #define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

    #define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
    #define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
    #define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
    #define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
    #define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
    #define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
    #define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
    #define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
    #define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
    #define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
    #define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
    #define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
    #define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
    //IO�ڲ���,ֻ�Ե�һ��IO��!
    //ȷ��n��ֵС��16!
    #define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
    #define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

    #define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
    #define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

    #define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
    #define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

    #define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
    #define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

    #define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
    #define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

    #define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
    #define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

    #define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
    #define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

    #define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
    #define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����

    #define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
    #define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����

    #define BEEP PFout(8)	// ����������IO 
    #define LED0 PFout(9)	// DS0
    #define LED1 PFout(10)	// DS1	
    /*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
    /*
    #define KEY0 		PEin(4)   	//PE4
    #define KEY1 		PEin(3)		//PE3 
    #define KEY2 		PEin(2)		//P32
    #define WK_UP 	PAin(0)		//PA0
     */


    #define KEY0_PRES 	1
    #define KEY1_PRES	2
    #define KEY2_PRES	3
    #define WKUP_PRES   4
    void delay_ms(int ms){}
    void KEY_Init()
    {

        GPIO_InitTypeDef GPIO_InitStructure;

        //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��GPIOA,GPIOEʱ��

//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //��ͨ����ģʽ
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
//        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
//        GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��GPIOE2,3,4


        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //WK_UP��Ӧ����PA0
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
        GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIOA0

    }

    //�ⲿ�жϳ�ʼ������
    //��ʼ��PE2~4,PA0Ϊ�ж�����.
    void EXTIX_Init()
    {
//        NVIC_InitTypeDef NVIC_InitStructure;
//        EXTI_InitTypeDef EXTI_InitStructure;

        KEY_Init(); //������Ӧ��IO�ڳ�ʼ��

//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //ʹ��SYSCFGʱ��


//        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2); //PE2 ���ӵ��ж���2
//        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3); //PE3 ���ӵ��ж���3
//        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4); //PE4 ���ӵ��ж���4
//        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //PA0 ���ӵ��ж���0

        /* ����EXTI_Line0 */
		#if 0
//        EXTI_InitStructure.EXTI_Line = EXTI_Line0; //LINE0
//        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж��¼�
//        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
//        EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ��LINE0
//        EXTI_Init(&EXTI_InitStructure); //����
		#endif
        /* ����EXTI_Line2,3,4 */
//        EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
//        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж��¼�
//        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
//        EXTI_InitStructure.EXTI_LineCmd = ENABLE; //�ж���ʹ��
//        EXTI_Init(&EXTI_InitStructure); //����
#if 0
//        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //�ⲿ�ж�0
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ�0
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
//        NVIC_Init(&NVIC_InitStructure); //����
#endif
//        NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn; //�ⲿ�ж�2
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03; //��ռ���ȼ�3
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
//        NVIC_Init(&NVIC_InitStructure); //����


//        NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //�ⲿ�ж�3
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ�2
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
//        NVIC_Init(&NVIC_InitStructure); //����


//        NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; //�ⲿ�ж�4
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�1
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
//        NVIC_Init(&NVIC_InitStructure); //����

    }
#endif
