#include "RX8025T.h"

//#define _RX8025T_TEST_CPP

#ifdef _RX8025T_TEST_CPP
    //RX8025T rx8025(PB6, PB7);
    RX8025T rx8025(PC1, PC2);
    DateTime now;
    void Rx8025Refresh(void *param)
    {
        RX8025T *rx = (RX8025T*)param;
        rx->LoadTime(now);
        now.Show();
        debug_printf("\r\n");
    }
    void RX8025Test()
    {
        rx8025.Init();
        now.Year = 2017;
        now.Month = 8;
        now.Day = 6;
        now.Hour = 18;
        now.Minute = 26;
        now.Second = 2;
        //rx8025.SaveTime(now);
        Sys.AddTask(Rx8025Refresh, &rx8025, 100, 1000, "TimeUp");
    }

#endif 


