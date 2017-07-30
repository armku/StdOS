#ifndef _AD7124_H
#define _AD7124_H

	#include "Spi.h"
	
	#define AD7124SPISOFT 0
	
	class AD7124
	{
		public:
			#if AD7124SPISOFT
			AD7124(SpiSoft *spi);
			#else
			AD7124(Spi *spi);
			#endif
			int Read(void); //����-1,��ʾת��δ���
			bool GetFlag(void); //��ȡADת���Ƿ�����
		private:
			int flagOK; //adת������
			int readCnt;//��ȡ����
			int readCntCheck;//�ϴμ��Ķ�ȡ����

			#if AD7124SPISOFT
				SpiSoft *pspi; //SPI�ӿ�
			#else
				Spi *pspi; //SPI�ӿ�
			#endif
	};
#endif
