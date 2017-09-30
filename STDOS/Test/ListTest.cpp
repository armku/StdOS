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
		Task *k1=new Task();
		k1->ID=1;
		_Tasks.Add(k1);
		Task *kk1;
		kk1=_Tasks[0];
		if(kk1)
		debug_printf("\r\n ID:%d %d",k1->ID,kk1->ID);
		
		Task *k2=new Task();
		k2->ID=2;
		_Tasks.Add(k2);
		Task *kk2=_Tasks[1];
		if(kk2)
		debug_printf("\r\n ID:%d %d",k2->ID,kk2->ID);
		
		
		debug_printf("\r\n\r\n TestList end\r\n\r\n");
    }
#endif 
