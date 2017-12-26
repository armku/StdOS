#ifndef _DS18B20_H
    #define _DS18B20_H
    #include "Port.h"
	#if 0
    class DS18B20
    {
        public:
            byte Init();	
			void SetPin(Pin pin);
            float GetTemp();
		private:
			void Start();
			void WriteByte(byte dat);
			byte ReadByte();
			byte ReadBit();
			byte Presence();
			void Rest();
		private:
			OutputPort _dio;

    };
	#else
	class DS18B20
	  {
		public:
		  byte Init();
		  //			void SetPin(Pin pin);
		  float GetTemp();
		  uint8_t DS18B20_Init(void);

		  void DS18B20_ReadId(uint8_t *ds18b20_id);
		  float DS18B20_GetTemp_MatchRom(uint8_t *ds18b20_id);
		private:
		  void Start();
		  void WriteByte(byte dat);
		  byte ReadByte();
		  byte ReadBit();
		  byte Presence();
		  void Rest();
		  float DS18B20_GetTemp_SkipRom(void);
		private:
		  //			OutputPort _dio;
	  };
	#endif
#endif
