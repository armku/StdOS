#include "Fifo.h"

FIFO::FIFO()
{
    this->pBuf = 0;
    this->BufSize = 0;
    this->Read = 0;
    this->Write = 0;
}

void FIFO::SetBuf(void *buf, int len)
{
    if (len >= 0)
    {
        this->pBuf = (byte*)buf;
        this->BufSize = len;
    }
    this->Clear();
}

void FIFO::Clear()
{
    this->Write = 0;
    this->Read = 0;
    this->Count = 0;
}

int FIFO::Push(byte da)
{
    if (this->pBuf == 0)
    {
        return  - 1;
    }
    this->pBuf[this->Write] = da;
    if (++this->Write >= this->BufSize)
    {
        this->Write = 0;
    }
    if (this->Count < this->BufSize)
    {
        this->Count++;
        return 0;
    }
    else
    {
        return  - 1;
    }
}

int FIFO::Pop(byte *da)
{
    *da = this->pBuf[this->Read];
    if (++this->Read >= this->BufSize)
    {
        this->Read = 0;
    }
    if (this->Count)
    {
        this->Count--;
        return 0;
    }
    else
    {
        return  - 1;
    }
}

bool FIFO::BufIsEmpty()
{
    return this->Count == 0;
}

bool FIFO::BufIsFull()
{
    return this->Count >= this->BufSize;
}
