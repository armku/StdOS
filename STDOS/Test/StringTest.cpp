#include "SString.h"
#include <string.h>
#include <CType.h>
#include "Sys.h"

#define STRINGTEST

#ifdef STRINGTEST
	void TestNum10();
	void TestNumTrim();
	
	void StrintTest()
	{
		TestNum10();
		TestNumTrim();
	}
	
	void TestNum10()
	{
		String str("Hello world");
		debug_printf("\r\nString Test ToLower() ToUpper() Begin\r\n");
				
		str.Show(true);
		str.ToLower();
		str.Show(true);
		str.ToUpper();
		str.Show(true);
		
		debug_printf("\r\nString Test End\r\n");
	}
	void TestNumTrim()
	{
		char buf[]={"  Hello world   "};
		String str(buf);
		debug_printf("\r\nString Test ToLower() ToUpper() Begin\r\n");
		str.Show(true);
		
		str.Trim();
		str.Show(true);
		
		debug_printf("\r\nString Test End\r\n");
	}
#endif
