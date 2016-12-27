#pragma once

/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 
 BasePort key2(PC13);
 btn2.attach(SINGLE_CLICK, BTN2_SINGLE_CLICK_Handler);
 Scheduling.ThreadAdd(btn2ticks, 5);
 void btn2ticks()
{
    btn2.ticks();
}
 */


#include "stdint.h"
#include "BasePort.h"

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
        CButton(PinPort pin, uint8_t active_level = 0,uint8_t DownBit=0);
        ~CButton();
    public:        
        void attach(PressEvent event, BtnCallback cb);
        void ticks();
        PressEvent get_button_event(); //��ȡ��ǰ����״̬
    private:
        BasePort *pPin;    
        struct Button
        {
                uint16_t ticks;
                uint8_t repeat: 4;
                uint8_t event: 4; //��ǰ״̬
                uint8_t state: 3; //��ǰ����״̬
                uint8_t debounce_cnt: 3; //˫������
                uint8_t active_level: 1; //�������µĵ�ƽ
                uint8_t button_level: 1; //�����ɿ��ĵ�ƽ
                BtnCallback cb[number_of_event]; //�ص�����
                //struct Button *next;
        } btn;
		uint8_t downBit;//������ʱ��״̬���ߵ͵�ƽ��1�ߵ�ƽ��0�͵�ƽ
		uint8_t hal_button_Level(void); //��ȡ�Ƿ��м����¡�
};
