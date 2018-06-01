#ifndef _PWMSOLO_H
    #define _PWMSOLO_H
    #include "Type.h"
    #include "Port.h"

    class PwmSolo: public AlternatePort
    {
        public:
            PwmSolo(uint32_t frq = 10000, uint32_t duty = 50,bool invert=false);
			void Open();
			void SetPin(Pin pin=P0);
            void SetOut(uint16_t pwmValue); //设置输出值
            void SetOutPercent(float per); //设置输出比例，0-100		
		protected:
			void OnOpen2();//初始化
        private:
            uint32_t _freq; //频率
            uint32_t _duty; //输出值
			bool _invert;
    };
#endif
