#include "SString.h"
#include <string.h>
#include <CType.h>
#include "Sys.h"

#define STRINGTEST

#ifdef STRINGTEST
	String str("Hello world");
	void StrintTest()
	{
		
		debug_printf("\r\nString Test Begin\r\n");
				
		str.Show();
		str.ToLower();
		str.Show();
		str.ToUpper();
		str.Show();
		
		debug_printf("\r\nString Test End\r\n");
	}
	void TestNum10()
	{
		
	}
#endif
