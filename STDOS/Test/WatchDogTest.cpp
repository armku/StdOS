
//#define DEBUGWatchDogTest

//���Դ���
#ifdef DEBUGWatchDogTest
	#if DEBUG
        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
#endif
