#include "Kernel\Sys.h"

#include "Interrupt.h"

extern InterruptCallback Vectors[];      // 对外的中断向量表
extern void* VectorParams[];       // 每一个中断向量对应的参数

