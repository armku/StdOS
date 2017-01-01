#pragma once
#include "Type.h"

#define NMAX 1000					//缓冲区大小，根据实际需要设置，但不能大于65535

class CFIFORing
{
public:
    CFIFORing();
private:
    byte  buffer[NMAX];      //缓冲区
    ushort writeIndex ;    //写索引，描述当前数据写入的位置
    ushort readIndex ;	//读索引，描述当前读出数据的位置
    byte  flagOverflow ;   //越界标志
    byte  flagFull ;		//缓冲区满标志
    byte  flagEmpty ;		//缓冲区空标志
public:
    void Push(byte data);		//数据写入FIFO
    byte Pop();			//从FIFO中读出数据
	ushort Pop(byte *buf,ushort pos,ushort len);//读出指定长度的数据
    ushort GetLength();			//获得FIFO中数据个数
void Reset();//复位
};
