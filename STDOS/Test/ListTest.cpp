#include "List.h"
#include <stdio.h>
#include "Sys.h"

class TaskTest
{
	public:
		int aa;
		int bb;
	void show()
	{
		debug_printf("aa:%d,bb:%d \r\n",aa,bb);
	}
};


#if DEBUG
	#define SmartOS_printf debug_printf
	List<TaskTest*>	_Tasks;
    void IList::Test()
    {
//		TraceStack ts("TestList");
//		SmartOS_printf("TestList......\r\n");		
//		IList list;
		
		
		
		debug_printf("\r\n\r\n TestList begin");
			
		
		TaskTest *t1=new TaskTest();
		t1->aa=1;
		t1->bb=2;
		debug_printf("\r\n 原始t1:\r\n");
		t1->show();
		
		
		_Tasks.Add(t1);
		debug_printf("\r\n 取出t1:\r\n");
		_Tasks[0]->show();
		
		debug_printf("\r\n\r\n TestList end\r\n\r\n");
    }
#endif 
