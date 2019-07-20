#include "key_driver.h"

/**
  * @brief  按键初始化操作，如端口配置、按键句柄结构体初始化等
  * @param  None
  * @retval None
  */
__weak void KEY_Init(void)
{
	/*
	hkey.KeyId = KEY_ID_1;
	hkey.KeyReadFuncPtr = KeyReadFunc;
	hkey.KeyState = KEY_RELEASED;
	hkey.KeyCnt = 0;
	hkey.KeyFsmState = KEY_FSM_FSTATE(KEY_FSM_WAIT_PRESS, KEY_FSM_WAIT_PRESS);
	*/
}

/**
  * @brief  读取指定按键对应的IO口电平值，不需要消抖，一次读取即可，该函数需要用户根据硬件自行实现
  * @param  按键的ID值，用户自行编码
  * @retval IO口电平值，本实现中1：按键按下，0：按键弹起，
  */
__weak uint8_t KEY_Read_Port(uint8_t KeyId)
{
	/*
	//根据KeyId读取指定IO口电平值
	switch(KeyId)
	{
		case KEY_ID_1:
			return (HAL_GPIO_ReadPin(USEKEY_GPIO_Port, USEKEY_Pin));
			break;
		default:
			return 0;
			break;
	}
	*/
	return 0;
}


/**
  * @brief  单个按键处理函数，需要被周期性调用，建议10ms扫描一次
  * @param  指定单个按键的句柄
  * @retval None
  */
void KEY_Scan(KEY_HandleTypeDef* keyHandle)
{
	//读取按键端口电平
	uint8_t IsKeyDown = KEY_Read_Port(keyHandle->KeyId);
	
	//获取状态机的当前状态
	switch(keyHandle->KeyFsmState) 
	{
		case KEY_FSM_WAIT_PRESS:
			if(IsKeyDown) //检测到按键按下
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(KEY_HIGH_CNT(keyHandle->KeyCnt)+1, 0);
				//按键按下消抖：连续几次检测到按键按下，可以确认按键按下
				if(KEY_HIGH_CNT(keyHandle->KeyCnt) >= KEY_PRESS_DB_CNT) 
				{
					keyHandle->KeyCnt = 0; //复位计数器，状态跳转
					keyHandle->KeyFsmState = KEY_FSM_SHORT_PRESS;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_RELEASED, KEY_SHORT_PRESSED);
				}
			}
			else 
				keyHandle->KeyCnt = 0; //检测到按键释放，复位计数器
			break;
		
		case KEY_FSM_SHORT_PRESS:
			keyHandle->KeyState = KEY_FULL_STATE(KEY_SHORT_PRESSED, KEY_LONG_PRESS_WAIT);
			if(!IsKeyDown)  //检测到按键释放
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(KEY_HIGH_CNT(keyHandle->KeyCnt)+1, 0);
				//按键释放消抖：连续几次检测到按键释放，可以确认按键释放
				if(KEY_HIGH_CNT(keyHandle->KeyCnt) >= KEY_RELEASE_DB_CNT)
				{
					keyHandle->KeyCnt = 0; //复位计数器，状态跳转
					keyHandle->KeyFsmState = KEY_FSM_RELEASE;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_SHORT_PRESSED, KEY_RELEASED);
				}
			}
			else
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(0, KEY_LOW_CNT(keyHandle->KeyCnt)+1);
				if(KEY_LOW_CNT(keyHandle->KeyCnt) >= KEY_LONG_PRESS_CNT) //可以确认按键长按
				{
					keyHandle->KeyCnt = 0; //复位计数器，状态跳转
					keyHandle->KeyFsmState = KEY_FSM_LONG_PRESS;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_LONG_PRESS_WAIT, KEY_LONG_PRESSED);
				}
			}
			break;
		
		case KEY_FSM_LONG_PRESS:
			keyHandle->KeyState = KEY_FULL_STATE(KEY_LONG_PRESSED, KEY_REPEAT_PRESS_WAIT);
			if(!IsKeyDown)  //检测到按键释放
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(KEY_HIGH_CNT(keyHandle->KeyCnt)+1, 0);
				//按键释放消抖：连续几次检测到按键释放，可以确认按键释放
				if(KEY_HIGH_CNT(keyHandle->KeyCnt) >= KEY_RELEASE_DB_CNT)
				{
					keyHandle->KeyCnt = 0; //复位计数器，状态跳转
					keyHandle->KeyFsmState = KEY_FSM_RELEASE;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_LONG_PRESSED, KEY_RELEASED);
				}
			}
			else
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(0, KEY_LOW_CNT(keyHandle->KeyCnt)+1);
				if(KEY_LOW_CNT(keyHandle->KeyCnt) >= KEY_REPEAT_PRESS_CNT) //可以确认按键连击
				{
					keyHandle->KeyCnt = 0; //复位计数器，状态跳转
					keyHandle->KeyFsmState = KEY_FSM_REPEAT_PRESS;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_REPEAT_PRESS_WAIT, KEY_REPEAT_PRESSED);
				}
			}
			break;
		
		case KEY_FSM_REPEAT_PRESS:
			keyHandle->KeyState = KEY_FULL_STATE(KEY_REPEAT_PRESSED, KEY_REPEAT_PRESS_WAIT);
			if(!IsKeyDown)  //检测到按键释放
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(KEY_HIGH_CNT(keyHandle->KeyCnt)+1, 0);
				//按键释放消抖：连续几次检测到按键释放，可以确认按键释放
				if(KEY_HIGH_CNT(keyHandle->KeyCnt) >= KEY_RELEASE_DB_CNT) 
				{
					keyHandle->KeyCnt = 0; //复位计数器，状态跳转
					keyHandle->KeyFsmState = KEY_FSM_RELEASE;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_REPEAT_PRESS_WAIT, KEY_RELEASED);
				}
			}
			else
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(0, KEY_LOW_CNT(keyHandle->KeyCnt)+1);
				if(KEY_LOW_CNT(keyHandle->KeyCnt) >= KEY_REPEAT_PRESS_CNT) //可以确认按键连击
				{
					keyHandle->KeyCnt = 0; //复位计数器，状态跳转
					keyHandle->KeyFsmState = KEY_FSM_REPEAT_PRESS;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_REPEAT_PRESS_WAIT, KEY_REPEAT_PRESSED);
				}
			}
			break;
		
		case KEY_FSM_RELEASE:
			keyHandle->KeyCnt = 0; //复位计数器，状态跳转
			keyHandle->KeyFsmState = KEY_FSM_WAIT_PRESS;
			keyHandle->KeyState = KEY_FULL_STATE(KEY_RELEASED, KEY_RELEASED);
			break;

		default:
			break;
	}
}


/**
  * @brief  获取指定单个按键的当前按键状态
  * @param  指定单个按键的句柄
  * @retval None
  */
KEY_StateTypeDef KEY_Get_State(KEY_HandleTypeDef* keyHandle)
{
	return (keyHandle->KeyState);
}


