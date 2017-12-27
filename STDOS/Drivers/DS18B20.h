#ifndef _DS18B20_H
    #define _DS18B20_H
    #include "Port.h"
	class DS18B20
    {
        public:
            byte Init();	
			void SetPin(Pin pin);
            float GetTemp();
			void ReadId();
			bool Presence();
		private:
			void Start();
			void WriteByte(byte dat);
			byte ReadByte();
			byte ReadBit();			
			void Rest();
		private:
			OutputPort _dio;
		public:
			char id[8];//…Ë±∏ID

    };	
#endif
