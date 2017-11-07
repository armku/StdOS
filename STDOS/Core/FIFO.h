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
			byte *pBuf; /* ������ */		
			int Write; /* ������дָ�� */
			int BufSize; /* ��������С */
			int Read; /* ��������ָ�� */
			int Count; /* ���ݸ��� */	
	};
#endif
