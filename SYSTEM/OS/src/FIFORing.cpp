/*
参考：http://www.amobbs.com/thread-5604139-1-1.html

*/
#include "FIFORing.h"


CFIFORing::CFIFORing()
{
    this->Reset();
}
CFIFORing::~CFIFORing()
{

}

//将数据写入缓冲区
//当缓冲区满时，置位 flagFull 标志，表示缓冲区满
//缓冲区已满，但又向其中写入数据，则最先写入的数据被冲掉
//data: 写入缓冲区的数据
//返回值固定为0
void CFIFORing::Push(byte data)
{
    //writeIndex &= (NMAX - 1);
    this->buffer[writeIndex] = data;
    this->writeIndex++;

    //确保指针一直在有效范围内，写指针越界时，重新从0开始写，并且设置越界标志
    if(this->writeIndex == NMAX)
    {
        this->writeIndex = 0;
        this->flagOverflow = 1;
    }

    //FIFO已满，又写入一个数据，则最早写入的数据要被冲掉
    //所以读指针要保持与写指针相等
    if(this->flagFull)
    {
        this->readIndex = this->writeIndex;
    }

    //如果写指针越界，且与读指针相等，则说明写满
    if(this->flagOverflow & (this->writeIndex == this->readIndex))
    {
        this->flagFull = 1;
    }
    else
    {
        this->flagFull = 0;
    }

    //写入一个数据，则缓冲区就一定是不空的
    this->flagEmpty = 0;

}

//从缓冲区中取出1字节数据
//当缓冲区中最后一个数据被取出的时候，置位 flagEmpty 标志，表示缓冲区空
//缓冲区已空，又进行读数据操作，则读出的数据是无效的
//返回值为读出的数据
byte CFIFORing::Pop()
{
    byte ucData;

    //readIndex &= (NMAX - 1);
    ucData = this->buffer[readIndex];
    this->readIndex++;

    //确保指针一直在有效范围内，读指针越界时，重新从0开始读，并且清除越界标志
    if(this->readIndex == NMAX)
    {
        this->readIndex = 0;
        this->flagOverflow = 0;
    }

    //FIFO已空，又读一个数据，则读出的数据是无效的
    //并且写指针要跟着读指针变化，否则下次写入的数据位置是不正确的
    if(this->flagEmpty)
    {
        this->writeIndex = this->readIndex;
    }

    //若越界标志为0，且读写指针相等，则说明已经读空了
    if((!this->flagOverflow) & (this->writeIndex == this->readIndex))
    {
        this->flagEmpty = 1;
    }
    else
    {
        this->flagEmpty = 0;
    }

    //读出一个数据，则缓冲区就一定是不满的
    this->flagFull = 0;

    return ucData;
}
ushort CFIFORing::Pop(byte *buf,ushort pos,ushort len)//读出指定长度的数据
{
	ushort readlen=0;
	
	if(len>this->GetLength())
	{
		readlen=GetLength();		
	}
	else
	{
		readlen=len;
	}
	for(int i=0;i<readlen;i++)
	{
		buf[pos+i]=this->Pop();
	}

	return readlen;	
}

//获取缓冲区中数据个数
ushort CFIFORing::GetLength()
{
    ushort ucDataNum;

    if(this->flagOverflow)
    {
        ucDataNum = NMAX + this->writeIndex - this->readIndex;
    }
    else
    {
        ucDataNum = this->writeIndex - this->readIndex;
    }

    return 	ucDataNum;
}
void CFIFORing::Reset()//复位
{
	this->writeIndex = 0;    //写索引，描述当前数据写入的位置
    this->readIndex = 0;	//读索引，描述当前读出数据的位置
    this->flagOverflow = 0;   //越界标志
    this->flagFull = 0;		//缓冲区满标志
    this->flagEmpty = 1;		//缓冲区空标志
}
