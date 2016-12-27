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
#define TICKS_INTERVAL    5	//ms //按键时间间隔
#define DEBOUNCE_TICKS    3	//MAX 8 双击次数
#define SHORT_TICKS       (300 /TICKS_INTERVAL) //短按键延时时间
#define LONG_TICKS        (1000 /TICKS_INTERVAL)//长按键延时时间

typedef void(*BtnCallback)(void*); //回调函数定义

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
        PressEvent get_button_event(); //获取当前按键状态
    private:
        BasePort *pPin;    
        struct Button
        {
                uint16_t ticks;
                uint8_t repeat: 4;
                uint8_t event: 4; //当前状态
                uint8_t state: 3; //当前按键状态
                uint8_t debounce_cnt: 3; //双击次数
                uint8_t active_level: 1; //按键按下的电平
                uint8_t button_level: 1; //按键松开的电平
                BtnCallback cb[number_of_event]; //回调函数
                //struct Button *next;
        } btn;
		uint8_t downBit;//键按下时的状态，高低电平，1高电平，0低电平
		uint8_t hal_button_Level(void); //读取是否有键按下。
};
