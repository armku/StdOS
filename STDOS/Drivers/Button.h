#pragma once

#include "Port.h"
#include "Delegate.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    3	//ms //����ʱ����
#define DEBOUNCE_TICKS    3	//MAX 8 ˫������
#define SHORT_TICKS       (300 /TICKS_INTERVAL) //�̰�����ʱʱ��
#define LONG_TICKS        (1000 /TICKS_INTERVAL)//��������ʱʱ��
typedef uint (*FuncRead)();//�з���ֵ��ί��
typedef enum
{
    PRESS_DOWN = 0, PRESS_UP, PRESS_REPEAT, SINGLE_CLICK, DOUBLE_CLICK, LONG_RRESS_START, LONG_PRESS_HOLD, number_of_event, NONE_PRESS
} PressEvent;

class CButton
{
    public:
        CButton();         
        void Attach(PressEvent event, Action action);
        void Ticks();
        PressEvent GetButtonEvent(); //��ȡ��ǰ����״̬
		FuncRead ReadKey;//������ȡ		
    private:		
        byte event: 4; //��ǰ״̬
		byte debounce_cnt: 3; //˫������
		byte repeat: 4;
		byte state: 3; //��ǰ����״̬
		Action actions[number_of_event]; //�ص�����
		ushort ticks;	
		ushort KeyCur;//��ǰ����
		ushort KeyOld;//��ʷ����
};
