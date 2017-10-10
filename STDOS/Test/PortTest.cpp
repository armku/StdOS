/*
F1:
野火ISO-V2 ISO-V3
K0:PA0
K1:PC13


F4
正点原子探索者
K0:PA0
K1:PE2
K2:PE3
K3:PE4

 */
#include "Port.h"
#include "Platform\Pin.h"
//void EXTIX_Init(void);
#ifdef DEBUG

    //测试代码
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
            //EXTIX_Init(); //初始化外部中断输入 
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
    //IO口地址映射
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
    //IO口操作,只对单一的IO口!
    //确保n的值小于16!
    #define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
    #define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

    #define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
    #define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

    #define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
    #define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

    #define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
    #define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

    #define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
    #define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

    #define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
    #define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

    #define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
    #define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

    #define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
    #define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

    #define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
    #define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入

    #define BEEP PFout(8)	// 蜂鸣器控制IO 
    #define LED0 PFout(9)	// DS0
    #define LED1 PFout(10)	// DS1	
    /*下面方式是通过位带操作方式读取IO*/
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

        //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE); //使能GPIOA,GPIOE时钟

//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; //KEY0 KEY1 KEY2对应引脚
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //普通输入模式
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
//        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
//        GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化GPIOE2,3,4


        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //WK_UP对应引脚PA0
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
        GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA0

    }

    //外部中断初始化程序
    //初始化PE2~4,PA0为中断输入.
    void EXTIX_Init()
    {
//        NVIC_InitTypeDef NVIC_InitStructure;
//        EXTI_InitTypeDef EXTI_InitStructure;

        KEY_Init(); //按键对应的IO口初始化

//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //使能SYSCFG时钟


//        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2); //PE2 连接到中断线2
//        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3); //PE3 连接到中断线3
//        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4); //PE4 连接到中断线4
//        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //PA0 连接到中断线0

        /* 配置EXTI_Line0 */
		#if 0
//        EXTI_InitStructure.EXTI_Line = EXTI_Line0; //LINE0
//        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断事件
//        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
//        EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能LINE0
//        EXTI_Init(&EXTI_InitStructure); //配置
		#endif
        /* 配置EXTI_Line2,3,4 */
//        EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
//        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断事件
//        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//        EXTI_InitStructure.EXTI_LineCmd = ENABLE; //中断线使能
//        EXTI_Init(&EXTI_InitStructure); //配置
#if 0
//        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //外部中断0
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级0
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级2
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//        NVIC_Init(&NVIC_InitStructure); //配置
#endif
//        NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn; //外部中断2
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03; //抢占优先级3
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级2
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//        NVIC_Init(&NVIC_InitStructure); //配置


//        NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //外部中断3
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级2
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级2
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//        NVIC_Init(&NVIC_InitStructure); //配置


//        NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; //外部中断4
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级2
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//        NVIC_Init(&NVIC_InitStructure); //配置

    }
#endif
