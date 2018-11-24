#ifndef _FIFOBUFF_H
#define _FIFOBUFF_H

template<typename T, uint16_t MAX_SIZE>
class FIFOBuffer
{
private:
public:
	T _buf[MAX_SIZE]; //fifo buffer data
	uint16_t _max_size;     //max buffer size
	uint16_t _size;         //current buffer nodes
	uint16_t _out_idx;      //output index of buffer
	uint16_t _in_idx;       //input index of buffer
public:
	FIFOBuffer(); //constructor
	bool Put(const T &data); //input one node to buffer
	bool Get(T &data);       //get one node from buffer
	bool Puts(T *pData, uint16_t num); //input multi nodes to buffer
	bool Gets(T *pData, uint16_t num); //get multi nodes from buffer
	uint16_t Size();   //get current size of buffer
	void Clear(); //clear buffer
	uint16_t MaxSize(); //the max byte size of buffer
	uint16_t ResSize(); //the unused bytes of buffer
};

template<typename T, uint16_t MAX_SIZE>
FIFOBuffer<T, MAX_SIZE>::FIFOBuffer()
{
	_max_size = MAX_SIZE;     //max buffer size
	_size = 0;         //current buffer nodes
	_out_idx = 0;      //output index of buffer
	_in_idx = 0;       //input index of buffer
}

//input one node to buffer
template<typename T, uint16_t MAX_SIZE>
bool FIFOBuffer<T, MAX_SIZE>::Put(const T &data)
{
	if (_size >= _max_size) //buffer overflow
		return false;

	_buf[_in_idx++] = data; //input data

	if (_in_idx >= _max_size) //input index validate
		_in_idx = 0;
	_size++;  //update buffer size 
	return true;
}


//input multi nodes to buffer
template<typename T, uint16_t MAX_SIZE>
bool FIFOBuffer<T, MAX_SIZE>::Puts(T *pData, uint16_t num)
{
	if ((_size + num) > _max_size) //buffer overflow
		return false;

	for (uint16_t i = 0; i < num; i++)
	{
		_buf[_in_idx++] = pData[i]; //input data
		if (_in_idx >= _max_size) //input index validate
			_in_idx = 0;
	}
	_size += num;  //update buffer size 
	return true;
}


//get one node from buffer
template<typename T, uint16_t MAX_SIZE>
bool FIFOBuffer<T, MAX_SIZE>::Get(T &data)
{
	if (_size <= 0)    //no data to get
		return false;

	data = _buf[_out_idx++]; //output data

	if (_out_idx >= _max_size)  //output index validate
		_out_idx = 0;

	_size--;//update buffer size
	return true;
}


//get multi nodes from buffer
template<typename T, uint16_t MAX_SIZE>
bool FIFOBuffer<T, MAX_SIZE>::Gets(T *pData, uint16_t num)
{
	if (_size < num) return false;  //not enough data to get

	for (uint16_t i = 0; i < num; i++)
	{
		pData[i] = _buf[_out_idx++]; //output data

		if (_out_idx >= _max_size)  //output index validate
			_out_idx = 0;
	}
	_size -= num;//update buffer size
	return true;
}

//Get the number of nodes in buffer (_size)
template<typename T, uint16_t MAX_SIZE>
uint16_t FIFOBuffer<T, MAX_SIZE>::Size()
{
	return _size;
}
//Get the number of max nodes in buffer (_max_size)
template<typename T, uint16_t MAX_SIZE>
uint16_t FIFOBuffer<T, MAX_SIZE>::MaxSize()
{
	return _max_size;
}

//Get the number of unused/free nodes in buffer (_max_size - _size)
template<typename T, uint16_t MAX_SIZE>
uint16_t FIFOBuffer<T, MAX_SIZE>::ResSize()
{
	return (_max_size - _size);
}

//clear all nodes in buffer
template<typename T, uint16_t MAX_SIZE>
void FIFOBuffer<T, MAX_SIZE>::Clear()
{
	_size = 0;         //current buffer nodes
	_out_idx = 0;      //output index of buffer
	_in_idx = 0;       //input index of buffer
}

#endif // !_FIFOBUFF_H

