#include "HardwareSerial.h"

/**
  * @brief  ���ڶ����캯��
  * @param  ���������ַ
  * @retval ��
  */
HardwareSerial::HardwareSerial(USART_TypeDef *_USARTx)
{
    this->USARTx = _USARTx;
    USART_Function = 0;
    _rx_buffer_head = _rx_buffer_tail = 0;
}

/**
  * @brief  �����ж����
  * @param  ��
  * @retval ��
  */
void HardwareSerial::IRQHandler()
{
    if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
    {
        uint8_t c = USART_ReceiveData(USARTx);
        uint16_t i = (uint16_t)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;
        if (i != _rx_buffer_tail)
        {
            _rx_buffer[_rx_buffer_head] = c;
            _rx_buffer_head = i;
        }

        if(USART_Function)
            USART_Function();

        USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
    }
}

/**
  * @brief  ���ڳ�ʼ��
  * @param  BaudRate: ������
  * @retval ��
  */
void HardwareSerial::begin(uint32_t BaudRate)
{
    begin(BaudRate, SERIAL_Config_Default);
}

/**
  * @brief  ���ڳ�ʼ��
  * @param  BaudRate: ������
  * @param  Config: ���ò���
  * @retval ��
  */
void HardwareSerial::begin(uint32_t BaudRate, SERIAL_Config Config)
{
    begin(BaudRate, Config, USART_PreemptionPriority_Default, USART_SubPriority_Default);
}

/**
  * @brief  ���ڳ�ʼ��
  * @param  BaudRate: ������
  * @param  Config: ���ò���
  * @param  PreemptionPriority: ��ռ���ȼ�
  * @param  SubPriority: �����ȼ�
  * @retval ��
  */
void HardwareSerial::begin(uint32_t BaudRate, SERIAL_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_TypeDef *GPIOx;
    uint16_t Tx_Pin, Rx_Pin;

    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    uint8_t USARTx_IRQn;

    if(USARTx == USART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
        Tx_Pin = GPIO_Pin_9;
        Rx_Pin = GPIO_Pin_10;
        GPIOx = GPIOA;
        USARTx_IRQn = USART1_IRQn;
    }
    else if(USARTx == USART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        Tx_Pin = GPIO_Pin_2;
        Rx_Pin = GPIO_Pin_3;
        GPIOx = GPIOA;
        USARTx_IRQn = USART2_IRQn;
    }
    else if(USARTx == USART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        Tx_Pin = GPIO_Pin_10;
        Rx_Pin = GPIO_Pin_11;
        GPIOx = GPIOB;
        USARTx_IRQn = USART3_IRQn;
    }

    //USART_TX
    GPIO_InitStructure.GPIO_Pin = Tx_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOx, &GPIO_InitStructure);//��ʼ��

    //USART_RX
    GPIO_InitStructure.GPIO_Pin = Rx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOx, &GPIO_InitStructure);//��ʼ��

    //USART NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority ;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;		//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = BaudRate;//���ڲ�����
    USART_InitStructure.USART_WordLength = Get_USART_WordLength_x(Config);//�ֳ����ݸ�ʽ
    USART_InitStructure.USART_Parity = Get_USART_Parity_x(Config);//��żУ��λ
    USART_InitStructure.USART_StopBits = Get_USART_StopBits_x(Config);//ֹͣλ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USARTx, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USARTx, ENABLE);                    //ʹ�ܴ���
}

/**
  * @brief  �رմ���
  * @param  ��
  * @retval ��
  */
void HardwareSerial::end(void)
{
    USART_Cmd(USARTx, DISABLE);
}

/**
  * @brief  �����жϻص�
  * @param  Function: �ص�����
  * @retval ��
  */
void HardwareSerial::attachInterrupt(USART_CallbackFunction_t Function)
{
    USART_Function = Function;
}

/**
  * @brief  ��ȡ�ɴӴ��ж˿ڶ�ȡ���ֽ���
  * @param  ��
  * @retval �ɶ�ȡ���ֽ���
  */
int HardwareSerial::available(void)
{
    return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

/**
  * @brief  ��ȡ����Ĵ�������(�ַ�)
  * @param  ��
  * @retval ���õĴ��봮�����ݵĵ�һ���ֽ� (���û�п��õ�����, ��Ϊ-1)
  */
int HardwareSerial::read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer_head == _rx_buffer_tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = _rx_buffer[_rx_buffer_tail];
        _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
        return c;
    }
}

/**
  * @brief  ���ش��봮�����ݵ���һ���ֽ�(�ַ�), ����������ڲ����л�������ɾ��
  * @param  ��
  * @retval ���õĴ��봮�����ݵĵ�һ���ֽ� (���û�п��õ�����, ��Ϊ-1)
  */
int HardwareSerial::peek(void)
{
    if (_rx_buffer_head == _rx_buffer_tail)
    {
        return -1;
    }
    else
    {
        return _rx_buffer[_rx_buffer_tail];
    }
}

/**
  * @brief  ��մ��ڻ���
  * @param  ��
  * @retval ��
  */
void HardwareSerial::flush(void)
{
    _rx_buffer_head = _rx_buffer_tail;
}

/**
  * @brief  ����д��һ���ֽ�
  * @param  д����ֽ�
  * @retval �ֽ�
  */
size_t HardwareSerial::write(uint8_t n)
{
    while(!IS_USARTx_SendDone(USARTx)) {}; //ѭ������,ֱ���������
    USART_SendData(USARTx, n);
    return n;
}

//Creat object for user
HardwareSerial Serial(USART1);//TX-PA9 RX-PA10
HardwareSerial Serial2(USART2);//TX-PA2 RX-PA3
HardwareSerial Serial3(USART3);//TX-PB10 RX-PB11

//USARTx_IRQHandler
extern "C" {
    void USART11_IRQHandler(void)
    {
        Serial.IRQHandler();
    }

    void USART22_IRQHandler(void)
    {
        Serial2.IRQHandler();
    }

    void USART33_IRQHandler(void)
    {
        Serial3.IRQHandler();
    }
}
