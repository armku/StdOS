#ifndef _PWMSOLO_H
    #define _PWMSOLO_H
    #include "Type.h"
    #include "Port.h"

    class PwmSolo: private AlternatePort
    {
        public:
            PwmSolo(Pin pin, uint frq = 10000, uint duty = 50);
            void Init();
            void SetOut(ushort pwmValue); //�������ֵ
            void SetOutPercent(float per); //�������������0-100		
		protected:
			void OnInit();//��ʼ��
        private:
            uint freq; //Ƶ��
            uint duty; //���ֵ
    };
#endif
