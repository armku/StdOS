#include "BrLink.h"
#include "Buffer.h"

BrLink::BrLink(USART &uart) :com(uart)
{

}

bool BrLink::CheckFrame()
{
	int rxlen = com.RxSize();

	if (rxlen == 0)
		return false;

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
	return rxFrame.CheckFrame();
}

bool BrLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;
	txFrame.data[0] = txFrame.frameLength;
	txFrame.data[1] = txFrame.fnCode;
	txFrame.data[txFrame.frameLength - 1] = CheckSum::XorCheckSum8(txFrame.data, txFrame.frameLength - 1);
	
	com.SendBytes(txFrame.data, txFrame.frameLength);
	txFrame.isUpdated = false;
	rxFrame.RemoveOneFrame();//移除处理完的接收数据帧
	return true;
}
