#pragma once
#include <stdint.h>

#define NMAX 1000					//缓冲区大小，根据实际需要设置，但不能大于65535

class CFIFORing
{
public:
    CFIFORing();
    ~CFIFORing();
private:
    uint8_t  buffer[NMAX];      //缓冲区
    uint16_t writeIndex ;    //写索引，描述当前数据写入的位置
    uint16_t readIndex ;	//读索引，描述当前读出数据的位置
    uint8_t  flagOverflow ;   //越界标志
    uint8_t  flagFull ;		//缓冲区满标志
    uint8_t  flagEmpty ;		//缓冲区空标志
public:
    void Push(uint8_t data);		//数据写入FIFO
    uint8_t Pop();			//从FIFO中读出数据
	uint16_t Pop(uint8_t *buf,uint16_t pos,uint16_t len);//读出指定长度的数据
    uint16_t GetLength();			//获得FIFO中数据个数
void Reset();//复位
};
