#include "stm32f4xx.h"


/* �����ڶ�Ӧ��RCCʱ�� */
#define RCC_ALL_LED 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOI)

#define GPIO_PORT_LED1  GPIOI
#define GPIO_PIN_LED1	GPIO_Pin_10

#define GPIO_PORT_LED2  GPIOF
#define GPIO_PIN_LED2	GPIO_Pin_7

#define GPIO_PORT_LED3  GPIOF
#define GPIO_PIN_LED3	GPIO_Pin_8

#define GPIO_PORT_LED4  GPIOC
#define GPIO_PIN_LED4	GPIO_Pin_2
/*
 *********************************************************************************************************
 *	�� �� ��: bsp_LedOff
 *	����˵��: Ϩ��ָ����LEDָʾ�ơ�
 *	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void bsp_LedOff(uint8_t _no)
{
    _no--;

    if (_no == 0)
    {
        GPIO_PORT_LED1->BSRRL = GPIO_PIN_LED1;
    }
    else if (_no == 1)
    {
        GPIO_PORT_LED2->BSRRL = GPIO_PIN_LED2;
    }
    else if (_no == 2)
    {
        GPIO_PORT_LED3->BSRRL = GPIO_PIN_LED3;
    }
    else if (_no == 3)
    {
        GPIO_PORT_LED4->BSRRL = GPIO_PIN_LED4;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_InitLed
 *	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void bsp_InitLed(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ��GPIOʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_ALL_LED, ENABLE);

    /*
    �������е�LEDָʾ��GPIOΪ�������ģʽ
    ���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
    �����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
     */
    bsp_LedOff(1);
    bsp_LedOff(2);
    bsp_LedOff(3);
    bsp_LedOff(4);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; /* ��Ϊ����ģʽ */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; /* ���������費ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; /* IO������ٶ� */

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
    GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
    GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED3;
    GPIO_Init(GPIO_PORT_LED3, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED4;
    GPIO_Init(GPIO_PORT_LED4, &GPIO_InitStructure);
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_LedOn
 *	����˵��: ����ָ����LEDָʾ�ơ�
 *	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void bsp_LedOn(uint8_t _no)
{
    _no--;

    if (_no == 0)
    {
        GPIO_PORT_LED1->BSRRH = GPIO_PIN_LED1;
    }
    else if (_no == 1)
    {
        GPIO_PORT_LED2->BSRRH = GPIO_PIN_LED2;
    }
    else if (_no == 2)
    {
        GPIO_PORT_LED3->BSRRH = GPIO_PIN_LED3;
    }
    else if (_no == 3)
    {
        GPIO_PORT_LED4->BSRRH = GPIO_PIN_LED4;
    }
}
/*
 *********************************************************************************************************
 *	�� �� ��: bsp_LedToggle
 *	����˵��: ��תָ����LEDָʾ�ơ�
 *	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
 *	�� �� ֵ: ��������
 *********************************************************************************************************
 */
void bsp_LedToggle(uint8_t _no)
{
    if (_no == 1)
    {
        GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;
    }
    else if (_no == 2)
    {
        GPIO_PORT_LED2->ODR ^= GPIO_PIN_LED2;
    }
    else if (_no == 3)
    {
        GPIO_PORT_LED3->ODR ^= GPIO_PIN_LED3;
    }
    else if (_no == 4)
    {
        GPIO_PORT_LED4->ODR ^= GPIO_PIN_LED4;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_IsLedOn
 *	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
 *	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
 *	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
 *********************************************************************************************************
 */
uint8_t bsp_IsLedOn(uint8_t _no)
{
    if (_no == 1)
    {
        if ((GPIO_PORT_LED1->ODR &GPIO_PIN_LED1) == 0)
        {
            return 1;
        }
        return 0;
    }
    else if (_no == 2)
    {
        if ((GPIO_PORT_LED2->ODR &GPIO_PIN_LED2) == 0)
        {
            return 1;
        }
        return 0;
    }
    else if (_no == 3)
    {
        if ((GPIO_PORT_LED3->ODR &GPIO_PIN_LED3) == 0)
        {
            return 1;
        }
        return 0;
    }
    else if (_no == 4)
    {
        if ((GPIO_PORT_LED4->ODR &GPIO_PIN_LED4) == 0)
        {
            return 1;
        }
        return 0;
    }

    return 0;
}

void delay(int len)
{
    for (int i = 0; i < len; i++)
        ;
}

int main(void)
{
    bsp_InitLed();
    while (1)
    {
        delay(2000000);
        /* ÿ��500ms ����һ�� */
        bsp_LedToggle(4); /* ��תLED4��״̬ */
        bsp_LedToggle(3);
        bsp_LedToggle(2);
        bsp_LedToggle(1);
    }
}
