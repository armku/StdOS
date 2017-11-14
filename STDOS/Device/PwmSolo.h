#ifndef _PWMSOLO_H
    #define _PWMSOLO_H
    #include "Type.h"
    #include "Port.h"

    class PwmSolo: public AlternatePort
    {
        public:
            PwmSolo(uint frq = 10000, uint duty = 50);
			void Open();
            void SetOut(ushort pwmValue); //�������ֵ
            void SetOutPercent(float per); //�������������0-100		
		protected:
			void OnOpen();//��ʼ��
        private:
            uint freq; //Ƶ��
            uint duty; //���ֵ
    };
#endif
