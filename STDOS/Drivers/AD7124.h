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
			int Read(void); //返回-1,表示转换未完成
			bool GetFlag(void); //读取AD转换是否正常
		private:
			int flagOK; //ad转换正常
			int readCnt;//读取次数
			int readCntCheck;//上次检查的读取周期

			#if AD7124SPISOFT
				SpiSoft *pspi; //SPI接口
			#else
				Spi *pspi; //SPI接口
			#endif
	};
#endif
