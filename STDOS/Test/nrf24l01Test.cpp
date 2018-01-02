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
	
	NRF24L01 n2401;

    void n2404Routin(void *param)
    {
        debug_printf("\r\n ������ ������Ӧ����ģʽ\r\n");
        n2401.NRF_TX_Mode();

        /*��ʼ��������*/
        status = n2401.NRF_Tx_Dat(txbuf);

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
        n2401.NRF_RX_Mode();

        /*�ȴ���������*/
        status = n2401.NRF_Rx_Dat(rxbuf);

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
		n2401._CE.Set(PG8);
		n2401._CE.Invert=0;
		n2401._CE.OpenDrain=true;
		n2401._CE.Open();
		
		n2401._CSN.Set(PG15);
		n2401._CSN.Invert=0;
		n2401._CSN.OpenDrain=true;
		n2401._CSN.Open();
		
		n2401._spi=new Spi(Spi1,CPOL_Low,CPHA_1Edge,9000000);
		
        n2401.SPI_NRF_Init();
        debug_printf("\r\n ����һ�� NRF24L01 ���ߴ���ʵ�� \r\n");
        debug_printf("\r\n �������ߴ��� ������ �ķ�����Ϣ\r\n");
        debug_printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");

        /*���NRFģ����MCU������*/
        status = n2401.NRF_Check();

        /*�ж�����״̬*/
        if (status == SUCCESS)
            debug_printf("\r\n      NRF��MCU���ӳɹ���\r\n");
        else
            debug_printf("\r\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\r\n");
        Sys.AddTask(n2404Routin, 0, 0, 1000, "n2404Routin ");

    }

#endif
