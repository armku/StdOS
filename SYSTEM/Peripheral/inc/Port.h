/*
�˿ڻ���
���ڹ���һ���˿ڣ�ͨ��PinBit��ʶ�������Щ���š�
�����ʼ��ʱ��ͨ��SetPort���ö˿ڣ���������״̬��Ȼ��Configͨ��gpio�ṹ�����ö˿ڣ��˿�����ʱ�ָ�����״̬
 */
#pragma once 

#include "Sys.h"
#include "stm32f10x.h"

#define _force_inline __inline
#ifdef STM32F4
    #define GPIO_MAX_SPEED 100
#else 
    #define GPIO_MAX_SPEED 50
#endif 

class Port
{
    public:
        GPIO_TypeDef *Group; // �����
        Pin _Pin; // ���
        ushort PinBit; // ��������λ��ÿ������һ��λ

        Port &Set(Pin pin); // �������ţ���Ӧ�����á�
        bool Empty()const;
        virtual void Config(); // ȷ������,ȷ���ö����ڲ��Ĳ������г�ʼ��
        // ��������
        _force_inline static GPIO_TypeDef *IndexToGroup(byte index);
        _force_inline static byte GroupToIndex(GPIO_TypeDef *group);

    protected:
        Port();
        virtual ~Port();
        // ���ù��̣���Config���ã����GPIO_Init
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
    private:
        #if defined(STM32F1)
            ulong InitState; // �������ų�ʼ״̬��������ʱ��ԭ
        #endif 
};
/*
�����
 */
class OutputPort: public Port
{
    public:
        bool OpenDrain; // �Ƿ�©���
        bool Invert; // �Ƿ����������
        uint Speed; // �ٶ�

        OutputPort();
        // ��ͨ���һ����ÿ�©�������Ҫ����
        OutputPort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);

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

        OutputPort &operator = (bool value);
        OutputPort &operator = (OutputPort &port);
        operator bool();

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
        void Init(bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);
};
/*
����˿ڻỰ�ࡣ��ʼ��ʱ�򿪶˿ڣ���������������ʱ�رա�������������ö˿�Ϊ����
 */
class PortScope
{
    private:
        OutputPort *_port;
        bool _value;
    public:
        PortScope(OutputPort *port, bool value = true);

        ~PortScope();
};
/*
���������
 */
class AlternatePort: public OutputPort
{
    public:
        AlternatePort();
        // �������һ������������������Ҫ����
        AlternatePort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);
    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};
/*
ģ�����������
 */
class AnalogInPort: public Port
{
    public:
        AnalogInPort(Pin pin);

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};
