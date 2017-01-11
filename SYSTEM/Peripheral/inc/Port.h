#pragma once 

#include "Sys.h"
#include "stm32f10x.h"

#define _force_inline __inline
#ifdef STM32F4
    #define GPIO_MAX_SPEED 100
#else 
    #define GPIO_MAX_SPEED 50
#endif 

// �˿ڻ���
// ���ڹ���һ���˿ڣ�ͨ��PinBit��ʶ�������Щ���š�
// �����ʼ��ʱ��ͨ��SetPort���ö˿ڣ���������״̬��Ȼ��Configͨ��gpio�ṹ�����ö˿ڣ��˿�����ʱ�ָ�����״̬
class Port
{
    public:
        GPIO_TypeDef *Group; // �����
        Pin _Pin; // ���
        ushort PinBit; // ��������λ��ÿ������һ��λ

        Port &Set(Pin pin); // �������ţ���Ӧ�����á�
        bool Empty()const
        {
                return _Pin == P0;
        }

        virtual void Config(); // ȷ������,ȷ���ö����ڲ��Ĳ������г�ʼ��

        // ��������
        _force_inline static GPIO_TypeDef* IndexToGroup(byte index);
        _force_inline static byte GroupToIndex(GPIO_TypeDef* group);

        #if DEBUG
            static bool Reserve(Pin pin, bool flag); // �������ţ���Ĺ���Ҫʹ��ʱ���ᱨ�������Ƿ񱣻��ɹ�
            static bool IsBusy(Pin pin); // �����Ƿ񱻱���
        #endif 		
    protected:
        Port();
        virtual ~Port();

        // ���ù��̣���Config���ã����GPIO_Init
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 

    private:
        #if defined(STM32F1)
            ulong InitState; // �������ų�ʼ״̬��������ʱ��ԭ
        #endif 
};
