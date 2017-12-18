#include "SString.h"
#include <string.h>
#include <CType.h>
#include "Sys.h"

#define STRINGTEST

#ifdef STRINGTEST
	void TestNum10();
	void TestNumTrim();
	
	int convint;
	void Testconv()
	{
		char strch[]={"-1234.5 11.2 332"};
		String str(strch,ArrayLength(strch));
		convint=str.ToInt();
		debug_printf("\r\n int aa = %d \r\n",convint);
	}
	
	void StrintTest()
	{
		TestNum10();
		TestNumTrim();
		Testconv();
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
	String utohex(uint ch,int a2,char *buf,bool uppercase);
	char buftest[20];
	void TestNumTrim()
	{
		char buf[]={"  Hello world   "};
		String str(buf);
		debug_printf("\r\nString Test ToLower() ToUpper() Begin\r\n");
		str.Show(true);
		
		auto strtrim= str.Trim();
		auto ll=strtrim.Length();
		strtrim.Show(true);
		char cc=0x3a;
		strtrim.Concat(cc,1);
		strtrim.Show();
		
		utohex(0x12D4,2,buftest,true);
		
		debug_printf("\r\nString Test End\r\n");
	}
#endif
