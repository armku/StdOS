#ifndef _FIFO_H
#define _FIFO_H
	#include "Type.h"
	class FIFO
	{
		public:
			FIFO();
			void SetBuf(void *buf, int len);
			int Push(byte da);
			int Pop(byte *da);
			bool BufIsEmpty();
			bool BufIsFull();
			void Clear();					
		private:			
			byte *pBuf; /* 缓冲区 */		
			int Write; /* 缓冲区写指针 */
			int BufSize; /* 缓冲区大小 */
			int Read; /* 缓冲区读指针 */
			int Count; /* 数据个数 */	
	};
#endif
