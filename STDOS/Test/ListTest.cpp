#include "List.h"
#include <stdio.h>
#include "Sys.h"
#include "Task.h"

#ifdef DEBUG
	List<Task*>	_Tasks;	// 任务列表	
	void IList::Test()
    {
//		TraceStack ts("TestList");
//		SmartOS_printf("TestList......\r\n");		
//		IList list;
		
			
		debug_printf("\r\n\r\n TestList begin\r\n");	
		
		for(int i=0;i<16;i++)
		{					
			Task *k1=new Task();
			k1->ID=i+10;
			_Tasks.Add(k1);
		}
		Task *kk2=_Tasks[1];
		if(kk2)
		debug_printf("\r\n ID:%d",kk2->ID);
		
		
		debug_printf("\r\n\r\n TestList end\r\n\r\n");
    }
#endif 
