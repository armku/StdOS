#pragma once 

#include "Sys.h"
#include "stm32f10x.h"

#ifdef STM32F4
    #define GPIO_MAX_SPEED 100
#else 
    #define GPIO_MAX_SPEED 50
#endif 



class Port
{
	public:
		Pin _Pin; //����
		ushort  PinBit; // ��������λ��ÿ������һ��λ
		Port& Set(Pin pin); // �������ţ���Ӧ�����á�
		bool Empty() const { return _Pin == P0; }
		virtual void Config(); // ȷ������,ȷ���ö����ڲ��Ĳ������г�ʼ��
		// ���ù��̣���Config���ã����GPIO_Init
		//virtual void OnConfig(GPIO_InitTypeDef& gpio);
	protected:
        Port();
        virtual ~Port();
};
// �����
class OutputPort: public Port
{
    public:
        bool OpenDrain; // �Ƿ�©���
        bool Invert; // �Ƿ����������
        uint Speed; // �ٶ�

        OutputPort()
        {
                Init();
        }
        // ��ͨ���һ����ÿ�©�������Ҫ����
        OutputPort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED)
        {
                Init(invert, openDrain, speed);
                Set(pin);
        }

        // ����д�����а���������
        void Write(bool value);
        void WriteGroup(ushort value); // ����д��
        void Up(uint ms); // ����һ��ʱ�������
        void Blink(uint times, uint ms); // ��˸���

        ushort ReadGroup(); // �����ȡ
        // ��ȡָ���������š��������մ�С����0xFF��ʾ�����Ϊtrue�򷵻�true
        bool Read(byte index);
        bool Read(); // Read() ReadReal() ��������  ǰ�߶����  һ��������    �ڿ�©�����ʱ���кܴ�����
        bool ReadInput();

        static bool Read(Pin pin);
        static void Write(Pin pin, bool value);

        OutputPort &operator = (bool value)
        {
                Write(value);
                return  *this;
        }
        OutputPort &operator = (OutputPort &port)
        {
                Write(port.Read());
                return  *this;
        }
        operator bool()
        {
                return Read();
        }

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);

        void Init(bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED)
        {
                OpenDrain = openDrain;
                Speed = speed;
                Invert = invert;
        }

        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 
};
