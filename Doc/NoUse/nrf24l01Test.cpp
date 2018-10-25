#include "Drivers\NRF24L01.h"

//#define _NRF24L01_TEST_H
#ifdef _NRF24L01_TEST_H

NRF24L01* nrf;

void OnSend(void* param)
{
	
}

void OnReceive(void* param)
{
    ByteArray bs;
	bs.Show(true);
}

uint32_t OnReceive(ITransport* transport, Buffer& bs, void* param, void* param2)
{
	bs.Show(true);

	return 0;
}

void TestNRF24L01()
{
    debug_printf("\r\n");
    debug_printf("TestNRF24L01 Start......\r\n");

    // 修改数据，加上系统ID
    //uint8_t* p = tx_buf + 5;
    //Sys.ToHex(p, (uint8_t*)Sys.ID, 6);

    //nrf->Timeout = 1000;
    nrf->Channel = 0;
    nrf->AutoAnswer = false;
    if(!nrf->Check())
        debug_printf("请检查线路\r\n");
    else
    {
        //nrf->Config();
        //nrf->SetMode(true);
        //Sys.AddTask(OnReceive, nullptr, 0, 1);
		//nrf->Register(OnReceive, nrf);
        Sys.AddTask(OnSend, nullptr, 0, 1000);
    }

    debug_printf("TestNRF24L01 Finish!\r\n");
}
#endif
