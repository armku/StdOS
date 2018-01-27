#include "I2C.h"
#include "Platform\stm32.h"

bool HardI2C::SendAddress(int addr, bool tx)
{
	return false;
}
void HardI2C::Ack(bool ack)
{
	
}
void HardI2C::Stop()
{
}
void HardI2C::Start()
{
}
void HardI2C::OnInit()
{
}
void HardI2C::OnOpen()
{
}
void HardI2C::OnClose()
{
}
// 等待Ack，默认0表示采用全局Retry
bool HardI2C::WaitAck(int retry)
{
	return false;
}
byte HardI2C::ReadByte()
{
	return 0;
}
void HardI2C::WriteByte(byte dat)
{
}
