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
		debug_printf("ID:%d\r\n",k1->ID);
		_Tasks.Add(k1);
		Task *kk1;
		kk1=_Tasks[0];
		debug_printf("\r\n ID:%d",kk1->ID);
		
		
		debug_printf("\r\n\r\n TestList end\r\n\r\n");
    }
#endif 
