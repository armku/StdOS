#include "SString.h"
#include <string.h>
#include <CType.h>
#include "Sys.h"

#define STRINGTEST

#ifdef STRINGTEST
	void TestNum10();
	
	void StrintTest()
	{
		TestNum10();
		
	}
	
	void TestNum10()
	{
		String str("Hello world");
		debug_printf("\r\nString Test Begin\r\n");
				
		str.Show();
		str.ToLower();
		str.Show();
		str.ToUpper();
		str.Show();
		
		debug_printf("\r\nString Test End\r\n");
	}
#endif
