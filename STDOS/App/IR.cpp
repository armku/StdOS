#include "IR.h"

IR::IR(Pwm * pwm)
{
}

bool IR::Open()
{
    return false;
}

bool IR::Close()
{
    return false;
}

bool IR::Send(const Buffer &bs)
{
    return false;
}

int IR::Receive(Buffer &bs, int sTimeout)
{
    return  - 1;
}

void IR::OnSend(void *sender, void *param)
{
	
}

