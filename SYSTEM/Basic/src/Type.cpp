#include "stdio.h"
#include "Type.h"

void assert_failed(uint8_t* file, uint32_t line)
{
	printf("在文件%s的第%d行检测到错误！ 自毁中.....\n", file, line);
}
