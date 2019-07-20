#include "bspkey.h"
#include "Sys.h"
/*
按键滤波时间50ms, 单位10ms。
只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
 */
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* 单位10ms， 持续1秒，认为长按事件 */ 

void KEY_FIFO::Init()
{
    this->Write = 0;
    this->Read = 0;
    this->Read2 = 0;
}

void KEY_FIFO::Clear()
{
    this->Read = this->Write;
}

void KEY_FIFO::Push(uint8_t da)
{
    this->Buf[this->Write] = da;
    if (++this->Write >= this->_bufSize)
    {
        this->Write = 0;
    }
}

uint8_t KEY_FIFO::Pop()
{
    uint8_t ret;

    if (this->Read == this->Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = this->Buf[this->Read];

        if (++this->Read >= this->_bufSize)
        {
            this->Read = 0;
        }
        return ret;
    }
}

uint8_t KEY_FIFO::Pop2()
{
    uint8_t ret;

    if (this->Read2 == this->Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = this->Buf[this->Read2];

        if (++this->Read2 >= this->_bufSize)
        {
            this->Read2 = 0;
        }
        return ret;
    }
}

/*
 *********************************************************************************************************
 *	函 数 名: bsp_PutKey
 *	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
 *	形    参:  _KeyCode : 按键代码
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Key::PutKey(uint8_t _KeyCode)
{
    s_tKey.Push(_KeyCode);
}

/*
 *********************************************************************************************************
 *	函 数 名: bsp_GetKeyState
 *	功能说明: 读取按键的状态
 *	形    参:  _ucKeyID : 按键ID，从0开始
 *	返 回 值: 1 表示按下， 0 表示未按下
 *********************************************************************************************************
 */
uint8_t Key::GetKeyState(KEY_ID_E _ucKeyID)
{
    return s_tBtn[_ucKeyID].State;
}

/*
 *********************************************************************************************************
 *	函 数 名: bsp_SetKeyParam
 *	功能说明: 设置按键参数
 *	形    参：_ucKeyID : 按键ID，从0开始
 *			_LongTime : 长按事件时间
 *			 _RepeatSpeed : 连发速度
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Key::SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t _RepeatSpeed)
{
    s_tBtn[_ucKeyID].LongTime = _LongTime; /* 长按时间 0 表示不检测长按键事件 */
    s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed; /* 按键连发的速度，0表示不支持连发 */
    s_tBtn[_ucKeyID].RepeatCount = 0; /* 连发计数器 */
}

/*
 *********************************************************************************************************
 *	函 数 名: bsp_InitKeyVar
 *	功能说明: 初始化按键变量
 *	形    参:  无
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Key::InitKeyVar()
{
    uint8_t i;

    /* 对按键FIFO读写指针清零 */
    s_tKey.Init();

    /* 给每个按键结构体成员变量赋一组缺省值 */
    for (i = 0; i < KEY_COUNT; i++)
    {
        s_tBtn[i].LongTime = KEY_LONG_TIME; /* 长按时间 0 表示不检测长按键事件 */
        s_tBtn[i].Count = KEY_FILTER_TIME / 2; /* 计数器设置为滤波时间的一半 */
        s_tBtn[i].State = 0; /* 按键缺省状态，0为未按下 */
        //s_tBtn[i].KeyCodeDown = 3 * i + 1;				/* 按键按下的键值代码 */
        //s_tBtn[i].KeyCodeUp   = 3 * i + 2;				/* 按键弹起的键值代码 */
        //s_tBtn[i].KeyCodeLong = 3 * i + 3;				/* 按键被持续按下的键值代码 */
        s_tBtn[i].RepeatSpeed = 0; /* 按键连发的速度，0表示不支持连发 */
        s_tBtn[i].RepeatCount = 0; /* 连发计数器 */
    }

    /* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
    /* 比如，我们希望按键1按下超过1秒后，自动重发相同键值 */
    s_tBtn[KID_KOK].LongTime = 100;
    s_tBtn[KID_KOK].RepeatSpeed = 5; /* 每隔50ms自动发送键值 */
	
	s_tBtn[KID_KLEFT].LongTime = 100;
    s_tBtn[KID_KLEFT].RepeatSpeed = 5; /* 每隔50ms自动发送键值 */
	
	s_tBtn[KID_KDOWN].LongTime = 100;
    s_tBtn[KID_KDOWN].RepeatSpeed = 5; /* 每隔50ms自动发送键值 */
	
	s_tBtn[KID_KUP].LongTime = 100;
    s_tBtn[KID_KUP].RepeatSpeed = 5; /* 每隔50ms自动发送键值 */
}

/*
 *********************************************************************************************************
 *	函 数 名: bsp_DetectKey
 *	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
 *	形    参:  按键结构变量指针
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Key::DetectKey(uint8_t i)
{
    KEY_T *pBtn;

    //如果没有初始化按键函数，则报错
	#ifdef DEBUG
    if (s_tBtn[i].IsKeyDownFunc == 0)
    {
        debug_printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
    }
	#endif

    pBtn = &s_tBtn[i];
    if (pBtn->IsKeyDownFunc())
    {
        if (pBtn->Count < KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if (pBtn->Count < 2 *KEY_FILTER_TIME)
        {
            pBtn->Count++;
        }
        else
        {
            if (pBtn->State == 0)
            {
                pBtn->State = 1;

                /* 发送按钮按下的消息 */
                PutKey((uint8_t)(3 *i + 1));
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* 发送按钮持续按下的消息 */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* 键值放入按键FIFO */
                        PutKey((uint8_t)(3 *i + 3));
						this->flagLongkey = 1;
                    }
                }
                else
                {
                    if (pBtn->RepeatSpeed > 0)
                    {
                        if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
                        {
                            pBtn->RepeatCount = 0;
                            /* 常按键后，每隔10ms发送1个按键 */
                            PutKey((uint8_t)(3 *i + 1));
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (pBtn->Count > KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if (pBtn->Count != 0)
        {
            pBtn->Count--;
        }
        else
        {
            if (pBtn->State == 1)
            {
                pBtn->State = 0;

                /* 发送按钮弹起的消息 */
				if (!this->flagLongkey)
				{
					PutKey((uint8_t)(3 * i + 2));
				}
				this->flagLongkey = 0;
            }
        }

        pBtn->LongCount = 0;
        pBtn->RepeatCount = 0;
    }
}

/*
 *********************************************************************************************************
 *	函 数 名: bsp_KeyScan
 *	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用
 *	形    参:  无
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Key::KeyScan()
{
    uint8_t i;

    for (i = 0; i < KEY_COUNT; i++)
    {
        this->DetectKey(i);
    }
}

uint8_t Key::GetKeyCode()
{
    return this->s_tKey.Pop();
}

void Key::SetKeyDetectFunc(uint8_t(*func)(), uint8_t pos)
{
    this->s_tBtn[pos].IsKeyDownFunc = func;
}
