#pragma once

/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 
 Port key2(PC13);
 btn2.attach(SINGLE_CLICK, BTN2_SINGLE_CLICK_Handler);
 void btn2ticks()
{
    btn2.ticks();
}
 */


#include "Type.h"
#include "Port.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    5	//ms //����ʱ����
#define DEBOUNCE_TICKS    3	//MAX 8 ˫������
#define SHORT_TICKS       (300 /TICKS_INTERVAL) //�̰�����ʱʱ��
#define LONG_TICKS        (1000 /TICKS_INTERVAL)//��������ʱʱ��

typedef void(*BtnCallback)(void*); //�ص���������

typedef enum
{
    PRESS_DOWN = 0, PRESS_UP, PRESS_REPEAT, SINGLE_CLICK, DOUBLE_CLICK, LONG_RRESS_START, LONG_PRESS_HOLD, number_of_event, NONE_PRESS
} PressEvent;

class CButton
{
    public:
        CButton(PinPort pin, byte active_level = 0,byte DownBit=0);         
        void attach(PressEvent event, BtnCallback cb);
        void ticks();
        PressEvent get_button_event(); //��ȡ��ǰ����״̬
    private:
        Port *pPin;    
        struct Button
        {
                ushort ticks;
                byte repeat: 4;
                byte event: 4; //��ǰ״̬
                byte state: 3; //��ǰ����״̬
                byte debounce_cnt: 3; //˫������
                byte active_level: 1; //�������µĵ�ƽ
                byte button_level: 1; //�����ɿ��ĵ�ƽ
                BtnCallback cb[number_of_event]; //�ص�����
                //struct Button *next;
        } btn;
		byte downBit;//������ʱ��״̬���ߵ͵�ƽ��1�ߵ�ƽ��0�͵�ƽ
		byte hal_button_Level(void); //��ȡ�Ƿ��м����¡�
};
