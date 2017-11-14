#ifndef _PWMSOLO_H
    #define _PWMSOLO_H
    #include "Type.h"
    #include "Port.h"

    class PwmSolo: public AlternatePort
    {
        public:
            PwmSolo(uint frq = 10000, uint duty = 50);
			void Open();
            void SetOut(ushort pwmValue); //设置输出值
            void SetOutPercent(float per); //设置输出比例，0-100		
		protected:
			void OnOpen();//初始化
        private:
            uint freq; //频率
            uint duty; //输出值
    };
#endif
