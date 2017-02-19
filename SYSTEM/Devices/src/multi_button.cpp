/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

#include "multi_button.h"
#include "string.h"

///*默认按键去抖延时   70ms*/
//static byte shake_time = 70;

CButton::CButton()
{
    this->btn.event = (byte)NONE_PRESS;
}

void CButton::attach(PressEvent event, Action action)
{
    this->btn.actions[event] = action;
}

void CButton::ticks()
{    
	if (this->ReadKey)
    {
        this->keycur = this->ReadKey();
    }
	else
	{
		this->keyold=0;
		return;
	}
	
	if(this->keycur==0)
	{
		return;
	}
	
	if(this->keyold!=this->keycur)
	{
		this->keyold=this->keycur;
		this->btn.ticks=0;
	}
    
    this->btn.ticks++;
    
    /*------------button debounce handle---------------*/
    if (this->keycur)
    {
        //not equal to prev one
        //continue read 3 times same new level change
        if (++(this->btn.debounce_cnt) >= DEBOUNCE_TICKS)
        {
            this->btn.debounce_cnt = 0;
        }
    }
    else
    {
        //leved not change ,counter reset.
        this->btn.debounce_cnt = 0;
    }

    /*-----------------State machine-------------------*/
    switch (this->btn.state)
    {
        case 0:
            if (this->keycur)
            {
                //start press down
                this->btn.event = (byte)PRESS_DOWN;
                if (this->btn.actions[PRESS_DOWN])
                {
                    this->btn.actions[PRESS_DOWN]((Button*)(&(this->btn)));
                }
                this->btn.ticks = 0;
                this->btn.repeat = 1;
                this->btn.state = 1;
            }
            else
            {
                this->btn.event = (byte)NONE_PRESS;
            }
            break;

        case 1:
            if (!this->keycur)
            {
                //released press up
                this->btn.event = (byte)PRESS_UP;
                if (this->btn.actions[PRESS_UP])
                {
                    this->btn.actions[PRESS_UP]((Button*)(&(this->btn)));
                }
                this->btn.ticks = 0;
                this->btn.state = 2;

            }
            else if (this->btn.ticks > LONG_TICKS)
            {
                this->btn.event = (byte)LONG_RRESS_START;
                if (this->btn.actions[LONG_RRESS_START])
                {
                    this->btn.actions[LONG_RRESS_START]((Button*)(&(this->btn)));
                }
                this->btn.state = 5;
            }
            break;

        case 2:
            if (this->keycur)
            {
                //press down again
                this->btn.event = (byte)PRESS_DOWN;
                if (this->btn.actions[PRESS_DOWN])
                {
                    this->btn.actions[PRESS_DOWN]((Button*)(&(this->btn)));
                }
                this->btn.repeat++;
                if (this->btn.repeat == 2)
                {
                    if (this->btn.actions[DOUBLE_CLICK])
                    {
                        this->btn.actions[DOUBLE_CLICK]((Button*)(&(this->btn)));
                    }

                }
                if (this->btn.actions[PRESS_REPEAT])
                {
                    this->btn.actions[PRESS_REPEAT]((Button*)(&(this->btn)));
                }
                this->btn.ticks = 0;
                this->btn.state = 3;
            }
            else if (this->btn.ticks > SHORT_TICKS)
            {
                //released timeout
                if (this->btn.repeat == 1)
                {
                    this->btn.event = (byte)SINGLE_CLICK;
                    if (this->btn.actions[SINGLE_CLICK])
                    {
                        this->btn.actions[SINGLE_CLICK]((Button*)(&(this->btn)));
                    }
                }
                else if (this->btn.repeat == 2)
                {
                    this->btn.event = (byte)DOUBLE_CLICK;
                }
                this->btn.state = 0;
            }
            break;

        case 3:
            if (this->keycur)
            {
                //released press up
                this->btn.event = (byte)PRESS_UP;
                if (this->btn.actions[PRESS_UP])
                {
                    this->btn.actions[PRESS_UP]((Button*)(&(this->btn)));
                }
                if (this->btn.ticks < SHORT_TICKS)
                {
                    this->btn.ticks = 0;
                    this->btn.state = 2; //repeat press
                }
                else
                {
                    this->btn.state = 0;
                }
            }
            break;

        case 5:
            if (this->keycur)
            {
                //continue hold trigger
                this->btn.event = (byte)LONG_PRESS_HOLD;
                if (this->btn.actions[LONG_PRESS_HOLD])
                {
                    this->btn.actions[LONG_PRESS_HOLD]((Button*)(&(this->btn)));
                }

            }
            else
            {
                //releasd
                this->btn.event = (byte)PRESS_UP;
                if (this->btn.actions[PRESS_UP])
                {
                    this->btn.actions[PRESS_UP]((Button*)(&(this->btn)));
                }
                this->btn.state = 0; //reset
            }
            break;
    }

}

//获取当前按键状态
PressEvent CButton::get_button_event()
{
    return (PressEvent)(this->btn.event);
}
