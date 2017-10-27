#ifndef _DS18B20_H
    #define _DS18B20_H
    #include "Port.h"
    class DS18B20
    {
        public:
            byte Init();	
			void SetPin(Pin pin);
            float GetTemp();
		private:
			void DS18B20_Start();
			void DS18B20_Write_Byte(byte dat);
			byte DS18B20_Read_Byte();
			byte DS18B20_Read_Bit();
			byte DS18B20_Presence();
			void DS18B20_Rst();
			void DS18B20_Mode_Out_PP();
		
			byte DS18B20_DATA_IN();
		private:
			OutputPort _dio;

    };
#endif
