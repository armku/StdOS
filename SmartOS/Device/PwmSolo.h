#ifndef _PWMSOLO_H
    #define _PWMSOLO_H
    #include "Type.h"
    #include "Port.h"

    class PwmSolo: public Port
    {
        public:
            PwmSolo(uint frq = 10000, uint duty = 50,bool invert=false);
			virtual Port& SetPin(Pin pin);	// ��������
			void Open();
            void SetOut(int pwmValue); //�������ֵ
            void SetOutPercent(float per); //�������������0-100		
		protected:
			void OnOpen2();//��ʼ��
        private:
            uint _freq; //Ƶ��
            uint _duty; //���ֵ
			bool _invert;
    };
#endif
