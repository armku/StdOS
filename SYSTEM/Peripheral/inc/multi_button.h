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
        CButton(PinPort pin, byte active_level = 0,byte DownBit=0);         
        void attach(PressEvent event, BtnCallback cb);
        void ticks();
        PressEvent get_button_event(); //获取当前按键状态
    private:
        Port *pPin;    
        struct Button
        {
                ushort ticks;
                byte repeat: 4;
                byte event: 4; //当前状态
                byte state: 3; //当前按键状态
                byte debounce_cnt: 3; //双击次数
                byte active_level: 1; //按键按下的电平
                byte button_level: 1; //按键松开的电平
                BtnCallback cb[number_of_event]; //回调函数
                //struct Button *next;
        } btn;
		byte downBit;//键按下时的状态，高低电平，1高电平，0低电平
		byte hal_button_Level(void); //读取是否有键按下。
};
