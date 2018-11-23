#include "DemoLink.h"

DemoLink::DemoLink(USART &uart) :com(uart)
{

}

bool DemoLink::CheckFrame()
{
	return this->CheckFrame(rxFrame);	
}
bool DemoLink::CheckFrame(DataFrame &df)
{
#if 0
	return com.mRxBuf.CheckFrame(df);
#else
	//at least 4 bytes: header  fnCode  length  checkSum
	while (com.mRxBuf._size > 0 && (com.mRxBuf._buf[com.mRxBuf._out_idx] != df.header)) //find frame header
	{
		if (++com.mRxBuf._out_idx >= com.mRxBuf._max_size)		com.mRxBuf._out_idx = 0;
		com.mRxBuf._size--;
	}
	if (com.mRxBuf._size < 4)                    //not enuogh data, or not find frame header
		return false;


	uint16_t idx = com.mRxBuf._out_idx + 1;
	if (idx >= com.mRxBuf._max_size) idx -= com.mRxBuf._max_size;   //function code index

	uint8_t fnCode = com.mRxBuf._buf[idx];                 //get function code

	if (fnCode > MAX_FN_CODE)                 //validate function code
	{
		Get(fnCode);     //out idx move forward one byte, 
		return false;    //function code error
	}
	idx = com.mRxBuf._out_idx + 2;
	if (idx >= com.mRxBuf._max_size) idx -= com.mRxBuf._max_size;   //data length index
	uint8_t dataLength = com.mRxBuf._buf[idx];             //get data length number
	if (dataLength != DATA_LENGTH[fnCode][DIRECTION_RECV]) //validate data length
	{
		Get(fnCode);    //out idx move forward one byte,  
		return false;   //data length error
	}

	if (com.mRxBuf._size < dataLength + 4)                 //not enuogh data
		return false;

	uint8_t checkSum = 0;
	for (uint8_t i = 0; i < dataLength + 3; i++)         //get checksum code
	{
		idx = com.mRxBuf._out_idx + i;

		if (idx >= com.mRxBuf._max_size)
			idx -= com.mRxBuf._max_size;

		checkSum += com.mRxBuf._buf[idx];
	}
	if (++idx >= com.mRxBuf._max_size)
		idx -= com.mRxBuf._max_size;
	if (checkSum != com.mRxBuf._buf[idx])     //和校验失败
	{
		Get(fnCode);
		return false;
	}
	//和校验成功，填充数据帧

	Get(fnCode);                 //frame header
	Get(df.fnCode);              //function code
	Get(df.dataLength);          //data length
	Gets(df.data, dataLength);    //data
	Get(df.checkSum);            //check sum
	df.isUpdated = true;
	return true;
#endif
}
bool DemoLink::Get(uint8_t & da)
{
	return true;
}
bool DemoLink::Gets(uint8_t * pData, uint16_t num)
{
	return true;
}

bool DemoLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;
	if (!com.SendByte(txFrame.header))  //send frame header
		return false;
	if (txFrame.fnCode>MAX_FN_CODE || !com.SendByte(txFrame.fnCode))  //send function code
		return false;
	txFrame.dataLength = DATA_LENGTH[txFrame.fnCode][DIRECTION_SEND];
	if (!com.SendByte(txFrame.dataLength))  //send data length
		return false;
	if (!com.SendBytes(txFrame.data, txFrame.dataLength)) //send data;
		return false;
	txFrame.CreateCheckCode();
	if (!com.SendByte(txFrame.checkSum))    //send check code
		return false;
	txFrame.isUpdated = false;
	return true;
}

