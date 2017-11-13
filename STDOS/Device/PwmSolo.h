#ifndef _PWMSOLO_H
    #define _PWMSOLO_H
    #include "Type.h"
    #include "Port.h"

    class PwmSolo: private AlternatePort
    {
        public:
            PwmSolo(Pin pin, uint frq = 10000, uint duty = 50);
            void Init();
            void SetOut(ushort pwmValue); //设置输出值
            void SetOutPercent(float per); //设置输出比例，0-100		
		protected:
			void OnInit();//初始化
        private:
            uint freq; //频率
            uint duty; //输出值
    };
#endif
