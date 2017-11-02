#include "bsp_key.h"
#include "Port.h"

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

void KEY_FIFO::Push(byte da)
{
    this->Buf[this->Write] = da;
    if (++this->Write >= this->_bufSize)
    {
        this->Write = 0;
    }
}

byte KEY_FIFO::Pop()
{
    byte ret;

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

byte KEY_FIFO::Pop2()
{
    byte ret;

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
void Key::PutKey(byte _KeyCode)
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
byte Key::GetKeyState(KEY_ID_E _ucKeyID)
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
void Key::SetKeyParam(byte _ucKeyID, ushort _LongTime, byte _RepeatSpeed)
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
    byte i;

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
    s_tBtn[KID_JOY_U].LongTime = 100;
    s_tBtn[KID_JOY_U].RepeatSpeed = 5; /* 每隔50ms自动发送键值 */
}

/*
 *********************************************************************************************************
 *	函 数 名: bsp_DetectKey
 *	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
 *	形    参:  按键结构变量指针
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Key::DetectKey(byte i)
{
    KEY_T *pBtn;
    
    //如果没有初始化按键函数，则报错
    if (s_tBtn[i].IsKeyDownFunc == 0)
    {
    printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
    }
     

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
                PutKey((byte)(3 *i + 1));
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* 发送按钮持续按下的消息 */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* 键值放入按键FIFO */
                        PutKey((byte)(3 *i + 3));
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
                            PutKey((byte)(3 *i + 1));
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
                PutKey((byte)(3 *i + 2));
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
    byte i;

    for (i = 0; i < KEY_COUNT; i++)
    {
        this->DetectKey(i);
    }
}
byte Key::GetKeyCode()
{
	return this->s_tKey.Pop();
}

#ifdef DEBUG
	
    void readkeyroutin(void *param)
    {
		Key * key=(Key*)param;
        key->KeyScan();
    }
    void keycoderoutin(void *param)
    {
		Key * key=(Key*)param;
        int ucKeyCode = key->GetKeyCode();
        if (ucKeyCode != KEY_NONE)
        {
            switch (ucKeyCode)
            {
                case KEY_1_DOWN:
                    /* 摇杆LEFT键按下, 控制LED显示流动 */
                    printf("键码：%d KEY_1_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_2_DOWN:
                    /* 摇杆RIGHT键按下 */
                    printf("键码：%d KEY_2_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_9_DOWN:
                    /* 摇杆UP键按下 */
                    printf("键码：%d KEY_9_DOWN\r\n", ucKeyCode);
                    break;
                case KEY_1_UP:
                    /* 摇杆DOWN键按下 */
                    printf("键码：%d KEY_1_UP\r\n", ucKeyCode);
                    break;
                case KEY_1_LONG:
                    /* 摇杆OK键按下 */
                    printf("键码：%d KEY_1_LONG\r\n", ucKeyCode);
                    break;
                default:
                    break;
            }
        }
    }
    OutputPort key11(PA0);
    OutputPort key22(PC13);
    /*
     *********************************************************************************************************
     *	函 数 名: IsKeyDownX
     *	功能说明: 判断按键是否按下
     *	形    参: 无
     *	返 回 值: 返回值1 表示按下，0表示未按下
     *********************************************************************************************************
     */
    byte IsKeyDown1()
    {
        if (key11 != 0 && key22 != 0)
            return 1;
        else
            return 0;
    }

    byte IsKeyDown2()
    {
        if (key11 == 0 && key22 == 0)
            return 1;
        else
            return 0;
    }

    byte IsKeyDown9() /* K1 K2组合键 */
    {
        if (key11 != 0 && key22 == 0)
            return 1;
        else
            return 0;
    }
	Key keytest;
    void keyTest()
    {
        key11.OpenDrain = false;
        key22.OpenDrain = false;

        key11.Invert = 0;
        key22.Invert = 0;

        key11.Open();
        key22.Open();
		
		keytest.InitKeyVar();

        /* 判断按键按下的函数 */
        keytest.s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
        keytest.s_tBtn[1].IsKeyDownFunc = IsKeyDown2;

        /* 组合键 */
        keytest.s_tBtn[2].IsKeyDownFunc = IsKeyDown9;
        
        Sys.AddTask(readkeyroutin, &keytest, 0, 10, "readkeyroutin");
        Sys.AddTask(keycoderoutin, &keytest, 6, 10, "keycoderoutin");
    }
#endif
