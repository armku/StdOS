#include "SerialPortOld.h"
#include "stm32f10x.h"
#include "usart.h"
#include "InputPort.h"
#include "AlternatePort.h"

void uart_init(uint bound)
{
    //串口1设置
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1

    //AlternatePort tx1(PA9);
	InputPort tx1(PA9);
    InputPort rx1(PA10);
    tx1.SetModeAF_PP();
    rx1.SetModeIN_FLOATING();

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound; //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启串口接收中断
    USART_Cmd(USART1, ENABLE); //使能串口1
    USART_ClearFlag(USART1, USART_FLAG_TC);

    //串口2设置
    InputPort tx2(PA2);
    InputPort rx2(PA3);
    tx2.SetModeAF_PP();
    rx2.SetModeIN_FLOATING();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_Init(USART2, &USART_InitStructure); //初始化串口2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_Cmd(USART2, ENABLE); //使能串口2
    USART_ClearFlag(USART2, USART_FLAG_TC);

    //	 //串口3设置
    //	InputPort tx3(PB11);
    //    InputPort rx3(PB10);
    //	tx3.SetModeAF_PP();
    //    rx3.SetModeIN_FLOATING();
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    //    //Usart3 NVIC 配置
    //    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    //    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //    //USART 初始化设置

    //    USART_Init(USART3, &USART_InitStructure); //初始化串口3
    //    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
    //    USART_Cmd(USART3, ENABLE);                    //使能串口3
    //	USART_ClearFlag(USART3, USART_FLAG_TC);
    //
    //	//串口4设置
    //	InputPort tx4(PC10);
    //    InputPort rx4(PC11);
    //	tx4.SetModeAF_PP();
    //    rx4.SetModeIN_FLOATING();
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    //    //Usart4 NVIC 配置
    //    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    //    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //    //USART 初始化设置

    //    USART_Init(UART4, &USART_InitStructure); //初始化串口4
    //    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
    //    USART_Cmd(UART4, ENABLE);                    //使能串口4
    //USART_ClearFlag(UART4, USART_FLAG_TC);
    //
    //	 //串口5设置
    //	InputPort tx5(PC12);
    //    InputPort rx5(PD3);
    //	tx5.SetModeAF_PP();
    //    rx5.SetModeIN_FLOATING();
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

    //    //Usart5 NVIC 配置
    //    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    //    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //    //USART 初始化设置

    //    USART_Init(UART5, &USART_InitStructure); //初始化串口5
    //    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
    //    USART_Cmd(UART5, ENABLE);                    //使能串口5
    //USART_ClearFlag(UART5, USART_FLAG_TC);
}

#ifdef __cplusplus
    extern "C"
    {
    #endif 
    #if EN_USART1_RX   //如果使能了接收
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
    #endif 
    #if EN_USART2_RX   //如果使能了接收

        void USART2_IRQHandler(void) //串口1中断服务程序
        {
            if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
            //接收中断(接收到的数据必须是0x0d 0x0a结尾)
            {
                USART_ClearITPendingBit(USART2, USART_IT_RXNE);
                byte inch = USART_ReceiveData(USART2); //读取接收到的数据


            }
        }
    #endif 
    #ifdef __cplusplus
    }
#endif
