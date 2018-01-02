#include "Drivers\nrf24l01.h"
#include "Sys.h"
#include "stm32f10x.h"

#define nrf24l01Test

#ifdef nrf24l01Test

    /*
     * PA2  -  PG8   ceʹ��
     * PA1  -  PG15  csƬѡ
     * PA3  -  PC4   irq�ж�
     */
    byte status; //�����жϽ���/����״̬
    byte txbuf[4] = 
    {
        0, 1, 2, 3
    }; //���ͻ���
    byte rxbuf[4]; //���ջ���
    int i = 0;

    void n2404Routin(void *param)
    {
        debug_printf("\r\n ������ ������Ӧ����ģʽ\r\n");
        NRF_TX_Mode();

        /*��ʼ��������*/
        status = NRF_Tx_Dat(txbuf);

        /*�жϷ���״̬*/
        switch (status)
        {
            case MAX_RT:
                debug_printf(
                    "\r\n ������ û���յ�Ӧ���źţ����ʹ��������޶�ֵ������ʧ�ܡ� \r\n");
                break;

            case ERROR:
                debug_printf("\r\n δ֪ԭ���·���ʧ�ܡ� \r\n");
                break;

            case TX_DS:
                debug_printf(
                    "\r\n ������ ���յ� �ӻ��� ��Ӧ���źţ����ͳɹ��� \r\n");
                break;
        }

        debug_printf("\r\n ������ �������ģʽ�� \r\n");
        NRF_RX_Mode();

        /*�ȴ���������*/
        status = NRF_Rx_Dat(rxbuf);

        /*�жϽ���״̬*/
        switch (status)
        {
            case RX_DR:
                for (i = 0; i < 4; i++)
                {
                    debug_printf(
                        "\r\n ������ ���յ� �ӻ��� ���͵�����Ϊ��%d \r\n",
                        rxbuf[i]);
                    txbuf[i] = rxbuf[i];
                }
                break;

            case ERROR:
                debug_printf("\r\n ������ ���ճ���   \r\n");
                break;
        }
    }
    void n24l01Test()
    {

        SPI_NRF_Init();
        debug_printf("\r\n ����һ�� NRF24L01 ���ߴ���ʵ�� \r\n");
        debug_printf("\r\n �������ߴ��� ������ �ķ�����Ϣ\r\n");
        debug_printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");

        /*���NRFģ����MCU������*/
        status = NRF_Check();

        /*�ж�����״̬*/
        if (status == SUCCESS)
            debug_printf("\r\n      NRF��MCU���ӳɹ���\r\n");
        else
            debug_printf("\r\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\r\n");
        Sys.AddTask(n2404Routin, 0, 0, 1000, "n2404Routin ");

    }

#endif
