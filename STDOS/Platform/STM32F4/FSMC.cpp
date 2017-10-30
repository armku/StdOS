#include "FSMC.h"
#include "stm32f4xx.h"

#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

//��ʼ��fsmc����
void FSMC::PinInit(Pin pin)
{
	GPIO_PinAFConfig(_GROUP(pin), _PORT(pin), GPIO_AF_FSMC);
}
/*
 *********************************************************************************************************
 *	�� �� ��: bsp_InitExtSRAM
 *	����˵��: ���������ⲿSRAM��GPIO��FSMC
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void FSMC::InitExtSRAM()
{
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef p;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ��GPIOʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

    /* ʹ�� FSMC ʱ�� */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

    /* SRAM �� GPIO ��
    PD0/FSMC_D2
    PD1/FSMC_D3
    PD4/FSMC_NOE
    PD5/FSMC_NWE
    PD8/FSMC_D13
    PD9/FSMC_D14
    PD10/FSMC_D15
    PD11/FSMC_A16
    PD12/FSMC_A17
    PD13/FSMC_A18
    PD14/FSMC_D0
    PD15/FSMC_D1

    PE0/FSMC_NBL0
    PE1/FSMC_NBL1
    PE3/FSMC_A19
    PE4/FSMC_A20	-- ����Ƭѡ������
    PE5/FSMC_A21	-- ����Ƭѡ������
    PE7/FSMC_D4
    PE8/FSMC_D5
    PE9/FSMC_D6
    PE10/FSMC_D7
    PE11/FSMC_D8
    PE12/FSMC_D9
    PE13/FSMC_D10
    PE14/FSMC_D11
    PE15/FSMC_D12

    PF0/FSMC_A0
    PF1/FSMC_A1
    PF2/FSMC_A2
    PF3/FSMC_A3
    PF4/FSMC_A4
    PF5/FSMC_A5
    PF12/FSMC_A6
    PF13/FSMC_A7
    PF14/FSMC_A8
    PF15/FSMC_A9

    PG0/FSMC_A10
    PG1/FSMC_A11
    PG2/FSMC_A12
    PG3/FSMC_A13
    PG4/FSMC_A14
    PG5/FSMC_A15
    PG10/FSMC_NE3	--- Ƭѡ���ź�
     */

    /* GPIOD configuration */
	#if 0
	FSMC::PinInit(PD0);
	FSMC::PinInit(PD1);
	FSMC::PinInit(PD4);
	FSMC::PinInit(PD5);
	FSMC::PinInit(PD8);
	FSMC::PinInit(PD9);
	FSMC::PinInit(PD10);
	FSMC::PinInit(PD11);
	FSMC::PinInit(PD12);
	FSMC::PinInit(PD13);
	FSMC::PinInit(PD14);
	FSMC::PinInit(PD15);
	#else
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
#endif

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
        | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOD, &GPIO_InitStructure);


    /* GPIOE configuration */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
        GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_Init(GPIOE, &GPIO_InitStructure);


    /* GPIOF configuration */
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_Init(GPIOF, &GPIO_InitStructure);


    /* GPIOG configuration */
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource2, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource3, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10;

    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /*-- FSMC Configuration ------------------------------------------------------*/
    p.FSMC_AddressSetupTime = 3; /* ����Ϊ2�����; 3���� */
    p.FSMC_AddressHoldTime = 0;
    p.FSMC_DataSetupTime = 2; /* ����Ϊ1����2���� */
    p.FSMC_BusTurnAroundDuration = 1;
    p.FSMC_CLKDivision = 0;
    p.FSMC_DataLatency = 0;
    p.FSMC_AccessMode = FSMC_AccessMode_A;

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM; // FSMC_MemoryType_PSRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    /*!< Enable FSMC Bank1_SRAM3 Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
}

