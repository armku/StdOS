#pragma once
#include <stdint.h>
class CThread
{
	public:
		CThread();
		~CThread();
	public:
		uint32_t TickCur;//当前计数值	
		uint32_t intervalms;//回调间隔
		bool canRun;//可以执行
		uint32_t delaycnt;//延时时间		
	public:
		void (*callback)(void);//回调函数
		void checkRun();//判断是否运行
		void SetPara(void (*callback)(void),uint32_t intervalms,uint32_t delaycntms=0);//设置参数
};
