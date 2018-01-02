#include "Drivers\nrf24l01.h"
#include "Sys.h"
#include "stm32f10x.h"

#define nrf24l01Test

#ifdef nrf24l01Test

    /*
     * PA2  -  PG8   ce使能
     * PA1  -  PG15  cs片选
     * PA3  -  PC4   irq中断
     */
    byte status; //用于判断接收/发送状态
    byte txbuf[4] = 
    {
        0, 1, 2, 3
    }; //发送缓冲
    byte rxbuf[4]; //接收缓冲
    int i = 0;
	
	

    void n2404Routin(void *param)
    {
		NRF24L01 *n24l01=(NRF24L01*)param;
		
        debug_printf("\r\n 主机端 进入自应答发送模式\r\n");
        n24l01->TX_Mode();

        /*开始发送数据*/
        status = n24l01->Tx_Dat(txbuf);

        /*判断发送状态*/
        switch (status)
        {
            case MAX_RT:
                debug_printf(
                    "\r\n 主机端 没接收到应答信号，发送次数超过限定值，发送失败。 \r\n");
                break;

            case ERROR:
                debug_printf("\r\n 未知原因导致发送失败。 \r\n");
                break;

            case TX_DS:
                debug_printf(
                    "\r\n 主机端 接收到 从机端 的应答信号，发送成功！ \r\n");
                break;
        }

        debug_printf("\r\n 主机端 进入接收模式。 \r\n");
        n24l01->RX_Mode();

        /*等待接收数据*/
        status = n24l01->Rx_Dat(rxbuf);

        /*判断接收状态*/
        switch (status)
        {
            case RX_DR:
                for (i = 0; i < 4; i++)
                {
                    debug_printf(
                        "\r\n 主机端 接收到 从机端 发送的数据为：%d \r\n",
                        rxbuf[i]);
                    txbuf[i] = rxbuf[i];
                }
                break;

            case ERROR:
                debug_printf("\r\n 主机端 接收出错。   \r\n");
                break;
        }
    }
	NRF24L01 n2401;
	
	Spi nspi(Spi1,CPOL_Low,CPHA_1Edge,9000000);
    void n24l01Test()
    {	
		n2401._CSN.Set(PG15);
		n2401._CSN.Invert=0;
		n2401._CSN.OpenDrain=true;
		n2401._CSN.Open();
	
        n2401.Init(&nspi,PG8,PC4);
        debug_printf("\r\n 这是一个 NRF24L01 无线传输实验 \r\n");
        debug_printf("\r\n 这是无线传输 主机端 的反馈信息\r\n");
        debug_printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");

        /*检测NRF模块与MCU的连接*/
        status = n2401.Check();

        /*判断连接状态*/
        if (status == SUCCESS)
            debug_printf("\r\n      NRF与MCU连接成功！\r\n");
        else
            debug_printf("\r\n  NRF与MCU连接失败，请重新检查接线。\r\n");
        Sys.AddTask(n2404Routin, &n2401, 0, 1000, "n2404Routin ");

    }

#endif
