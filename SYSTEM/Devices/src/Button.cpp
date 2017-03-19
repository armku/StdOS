#include "Button.h"
#include "string.h"

///*Ĭ�ϰ���ȥ����ʱ   70ms*/
//static byte shake_time = 70;

CButton::CButton()
{
    this->event = (byte)NONE_PRESS;
}

void CButton::Attach(PressEvent event, Action action)
{
    this->actions[event] = action;
}

void CButton::Ticks()
{
    if (this->ReadKey)
    {
        this->KeyCur = this->ReadKey();
    }
    else
    {
        this->KeyOld = 0;
        this->KeyCur = 0;
        return ;
    }
    switch (this->state)
    {
        case 0:
            //û�м�����
            if (this->KeyCur)
            {
                this->state++;
				break;
            }
			else
			{
				this->KeyOld=0;
			}
            break;
        case 1:
            //û�м����£�����״̬0
            if (!this->KeyCur)
            {
                this->state = 0;
                break;
            }
            //�м�����
            if (this->KeyOld != this->KeyCur)
            {
                this->KeyOld = this->KeyCur;
                break;
            }
            this->state++;
            break;
        case 2:
            //ͳ�ư���ʱ��
            if (this->KeyCur == this->KeyOld)
            {
                this->ticks++;
                if (this->ticks > 2)
                {
                    //�����м�����״̬
                    this->state++;
                }
            }
            else
            {
                //����������
                this->state = 0;
            }
            break;
        case 3:
            //�������¼�
            if (this->KeyCur == this->KeyOld)
            {
                this->event = (byte)PRESS_DOWN;
                if (this->actions[PRESS_DOWN])
                {
                    this->actions[PRESS_DOWN](this);
                }
                this->state++;
            }
            else
            {
                this->state = 0;
            }
            break;
        case 4:
            //���ɿ��¼�
            if (this->KeyCur == this->KeyOld)
            {
                //һֱ����
            }
            else
            {
                //�ɿ�
                this->event = (byte)PRESS_UP;
                if (this->actions[PRESS_UP])
                {
                    this->actions[PRESS_UP](this);
                }
                if(this->KeyCur==0)
				{
					//��ȫ�ɿ�
					this->state=0;
				}
				else
				{
					//�����ɿ�
					this->state++;
					this->state=0;
				}
            }
            break;
		case 5:
			//���������ɿ�
			if(!this->KeyCur)
			{
				//������ȫ�ɿ����ع�����״̬
				this->state=0;
			}
			break;
        default:
            this->state = 0;
            break;
    }
    return ;
    
    #if 0
        /*------------button debounce handle---------------*/
        //not equal to prev one
        //continue read 3 times same new level change
        if (++(this->btn.debounce_cnt) >= DEBOUNCE_TICKS)
        {
            this->btn.debounce_cnt = 0;
        }
        else
        {
            //leved not change ,counter reset.
            this->btn.debounce_cnt = 0;
        }

        /*-----------------State machine-------------------*/
        switch (this->btn.state)
        {
            //
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
    #endif 
}

//��ȡ��ǰ����״̬
PressEvent CButton::GetButtonEvent()
{
    return (PressEvent)(this->event);
}
