#pragma once

#include "Port.h"
#include "Delegate.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    3	//ms //按键时间间隔
#define DEBOUNCE_TICKS    3	//MAX 8 双击次数
#define SHORT_TICKS       (300 /TICKS_INTERVAL) //短按键延时时间
#define LONG_TICKS        (1000 /TICKS_INTERVAL)//长按键延时时间

typedef enum
{
    PRESS_DOWN = 0, PRESS_UP, PRESS_REPEAT, SINGLE_CLICK, DOUBLE_CLICK, LONG_RRESS_START, LONG_PRESS_HOLD, number_of_event, NONE_PRESS
} PressEvent;

class CButton
{
    public:
        CButton();         
        void attach(PressEvent event, Action action);
        void ticks();
        PressEvent get_button_event(); //获取当前按键状态
		FuncRead ReadKey;//按键读取
		ushort keycur;//当前按键
		ushort keyold;//历史按键
    private:		
        struct Button
        {
                Action actions[number_of_event]; //回调函数
				ushort ticks;
                byte repeat: 4;
                byte event: 4; //当前状态
                byte state: 3; //当前按键状态
                byte debounce_cnt: 3; //双击次数 
        } btn;	
};
