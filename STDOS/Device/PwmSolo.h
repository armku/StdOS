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
            void SetOut(uint16_t pwmValue); //�������ֵ
            void SetOutPercent(float per); //�������������0-100		
		protected:
			void OnOpen2();//��ʼ��
        private:
            uint32_t _freq; //Ƶ��
            uint32_t _duty; //���ֵ
			bool _invert;
    };
#endif
