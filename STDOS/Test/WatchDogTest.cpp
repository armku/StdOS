
//#define _WATCHDOGTEST_H

//���Դ���
#ifdef _WATCHDOGTEST_H
	#if DEBUG
        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
#endif
