
//#define _WATCHDOG_TEST_H

//���Դ���
#ifdef _WATCHDOG_TEST_H
	#if DEBUG
        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
#endif
