#include "List.h"
#include "Sys.h"
#include "Task.h"

//#define DEBUGIListTest

#ifdef DEBUGIListTest
	List<Task*>	_Tasks;	// �����б�	
	void IList::Test()
    {
//		TraceStack ts("TestList");
//		debug_printf("TestList......\r\n");		
//		IList list;
		
			
		debug_printf("\r\n\r\n TestList begin\r\n");	
		
		for(int i=0;i<34;i++)
		{					
			Task *k1=new Task();
			k1->ID=i;
			_Tasks.Add(k1);
		}
		Task *kk2=_Tasks[1];
		if(kk2)
		debug_printf("\r\n ID:%d",kk2->ID);
		
		
		debug_printf("\r\n\r\n TestList end\r\n\r\n");
    }
#endif 
