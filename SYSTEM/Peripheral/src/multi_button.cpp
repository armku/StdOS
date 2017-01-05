/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

#include "multi_button.h"
#include "string.h"

///*默认按键去抖延时   70ms*/
//static byte shake_time = 70;

CButton::CButton(Pin pin, byte active_level,byte DownBit)
{	
    this->pPin.Set(pin);

    this->btn.event = (byte)NONE_PRESS;
    //this->btn.hal_button_Level = pin_level;
    this->btn.button_level = this->hal_button_Level();
    this->btn.active_level = 0;
	this->downBit=DownBit;
}

void CButton::attach(PressEvent event, BtnCallback cb)
{
    this->btn.cb[event] = cb;
}
//读取是否有键按下。
byte CButton::hal_button_Level(void)
{
	byte ret=0;
	ret=this->pPin.Read();
	if(!this->downBit)
	{
		ret=!ret;
	}
	return  ret;
}
void CButton::ticks()
{
    //当前按键状态
    byte read_gpio_level = !this->hal_button_Level();

    //ticks counter working..
    if (this->btn.state > 0)
    {
        this->btn.ticks++;
    }
    /*------------button debounce handle---------------*/
    if (read_gpio_level != this->btn.button_level)
    {
        //not equal to prev one
        //continue read 3 times same new level change
        if (++(this->btn.debounce_cnt) >= DEBOUNCE_TICKS)
        {
            this->btn.button_level = read_gpio_level;
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
            if (this->btn.button_level == this->btn.active_level)
            {
                //start press down
                this->btn.event = (byte)PRESS_DOWN;
                if (this->btn.cb[PRESS_DOWN])
                {
                    this->btn.cb[PRESS_DOWN]((Button*)(&(this->btn)));
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
            if (this->btn.button_level != this->btn.active_level)
            {
                //released press up
                this->btn.event = (byte)PRESS_UP;
                if (this->btn.cb[PRESS_UP])
                {
                    this->btn.cb[PRESS_UP]((Button*)(&(this->btn)));
                }
                this->btn.ticks = 0;
                this->btn.state = 2;

            }
            else if (this->btn.ticks > LONG_TICKS)
            {
                this->btn.event = (byte)LONG_RRESS_START;
                if (this->btn.cb[LONG_RRESS_START])
                {
                    this->btn.cb[LONG_RRESS_START]((Button*)(&(this->btn)));
                }
                this->btn.state = 5;
            }
            break;

        case 2:
            if (this->btn.button_level == this->btn.active_level)
            {
                //press down again
                this->btn.event = (byte)PRESS_DOWN;
                if (this->btn.cb[PRESS_DOWN])
                {
                    this->btn.cb[PRESS_DOWN]((Button*)(&(this->btn)));
                }
                this->btn.repeat++;
                if (this->btn.repeat == 2)
                {
                    if (this->btn.cb[DOUBLE_CLICK])
                    {
                        this->btn.cb[DOUBLE_CLICK]((Button*)(&(this->btn)));
                    }

                }
                if (this->btn.cb[PRESS_REPEAT])
                {
                    this->btn.cb[PRESS_REPEAT]((Button*)(&(this->btn)));
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
                    if (this->btn.cb[SINGLE_CLICK])
                    {
                        this->btn.cb[SINGLE_CLICK]((Button*)(&(this->btn)));
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
            if (this->btn.button_level != this->btn.active_level)
            {
                //released press up
                this->btn.event = (byte)PRESS_UP;
                if (this->btn.cb[PRESS_UP])
                {
                    this->btn.cb[PRESS_UP]((Button*)(&(this->btn)));
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
            if (this->btn.button_level == this->btn.active_level)
            {
                //continue hold trigger
                this->btn.event = (byte)LONG_PRESS_HOLD;
                if (this->btn.cb[LONG_PRESS_HOLD])
                {
                    this->btn.cb[LONG_PRESS_HOLD]((Button*)(&(this->btn)));
                }

            }
            else
            {
                //releasd
                this->btn.event = (byte)PRESS_UP;
                if (this->btn.cb[PRESS_UP])
                {
                    this->btn.cb[PRESS_UP]((Button*)(&(this->btn)));
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
