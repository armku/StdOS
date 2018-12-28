#include "key_driver.h"

/**
  * @brief  ������ʼ����������˿����á���������ṹ���ʼ����
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
  * @brief  ��ȡָ��������Ӧ��IO�ڵ�ƽֵ������Ҫ������һ�ζ�ȡ���ɣ��ú�����Ҫ�û�����Ӳ������ʵ��
  * @param  ������IDֵ���û����б���
  * @retval IO�ڵ�ƽֵ����ʵ����1���������£�0����������
  */
__weak uint8_t KEY_Read_Port(uint8_t KeyId)
{
	/*
	//����KeyId��ȡָ��IO�ڵ�ƽֵ
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
  * @brief  ������������������Ҫ�������Ե��ã�����10msɨ��һ��
  * @param  ָ�����������ľ��
  * @retval None
  */
void KEY_Scan(KEY_HandleTypeDef* keyHandle)
{
	//��ȡ�����˿ڵ�ƽ
	uint8_t IsKeyDown = KEY_Read_Port(keyHandle->KeyId);
	
	//��ȡ״̬���ĵ�ǰ״̬
	switch(keyHandle->KeyFsmState) 
	{
		case KEY_FSM_WAIT_PRESS:
			if(IsKeyDown) //��⵽��������
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(KEY_HIGH_CNT(keyHandle->KeyCnt)+1, 0);
				//���������������������μ�⵽�������£�����ȷ�ϰ�������
				if(KEY_HIGH_CNT(keyHandle->KeyCnt) >= KEY_PRESS_DB_CNT) 
				{
					keyHandle->KeyCnt = 0; //��λ��������״̬��ת
					keyHandle->KeyFsmState = KEY_FSM_SHORT_PRESS;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_RELEASED, KEY_SHORT_PRESSED);
				}
			}
			else 
				keyHandle->KeyCnt = 0; //��⵽�����ͷţ���λ������
			break;
		
		case KEY_FSM_SHORT_PRESS:
			keyHandle->KeyState = KEY_FULL_STATE(KEY_SHORT_PRESSED, KEY_LONG_PRESS_WAIT);
			if(!IsKeyDown)  //��⵽�����ͷ�
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(KEY_HIGH_CNT(keyHandle->KeyCnt)+1, 0);
				//�����ͷ��������������μ�⵽�����ͷţ�����ȷ�ϰ����ͷ�
				if(KEY_HIGH_CNT(keyHandle->KeyCnt) >= KEY_RELEASE_DB_CNT)
				{
					keyHandle->KeyCnt = 0; //��λ��������״̬��ת
					keyHandle->KeyFsmState = KEY_FSM_RELEASE;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_SHORT_PRESSED, KEY_RELEASED);
				}
			}
			else
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(0, KEY_LOW_CNT(keyHandle->KeyCnt)+1);
				if(KEY_LOW_CNT(keyHandle->KeyCnt) >= KEY_LONG_PRESS_CNT) //����ȷ�ϰ�������
				{
					keyHandle->KeyCnt = 0; //��λ��������״̬��ת
					keyHandle->KeyFsmState = KEY_FSM_LONG_PRESS;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_LONG_PRESS_WAIT, KEY_LONG_PRESSED);
				}
			}
			break;
		
		case KEY_FSM_LONG_PRESS:
			keyHandle->KeyState = KEY_FULL_STATE(KEY_LONG_PRESSED, KEY_REPEAT_PRESS_WAIT);
			if(!IsKeyDown)  //��⵽�����ͷ�
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(KEY_HIGH_CNT(keyHandle->KeyCnt)+1, 0);
				//�����ͷ��������������μ�⵽�����ͷţ�����ȷ�ϰ����ͷ�
				if(KEY_HIGH_CNT(keyHandle->KeyCnt) >= KEY_RELEASE_DB_CNT)
				{
					keyHandle->KeyCnt = 0; //��λ��������״̬��ת
					keyHandle->KeyFsmState = KEY_FSM_RELEASE;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_LONG_PRESSED, KEY_RELEASED);
				}
			}
			else
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(0, KEY_LOW_CNT(keyHandle->KeyCnt)+1);
				if(KEY_LOW_CNT(keyHandle->KeyCnt) >= KEY_REPEAT_PRESS_CNT) //����ȷ�ϰ�������
				{
					keyHandle->KeyCnt = 0; //��λ��������״̬��ת
					keyHandle->KeyFsmState = KEY_FSM_REPEAT_PRESS;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_REPEAT_PRESS_WAIT, KEY_REPEAT_PRESSED);
				}
			}
			break;
		
		case KEY_FSM_REPEAT_PRESS:
			keyHandle->KeyState = KEY_FULL_STATE(KEY_REPEAT_PRESSED, KEY_REPEAT_PRESS_WAIT);
			if(!IsKeyDown)  //��⵽�����ͷ�
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(KEY_HIGH_CNT(keyHandle->KeyCnt)+1, 0);
				//�����ͷ��������������μ�⵽�����ͷţ�����ȷ�ϰ����ͷ�
				if(KEY_HIGH_CNT(keyHandle->KeyCnt) >= KEY_RELEASE_DB_CNT) 
				{
					keyHandle->KeyCnt = 0; //��λ��������״̬��ת
					keyHandle->KeyFsmState = KEY_FSM_RELEASE;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_REPEAT_PRESS_WAIT, KEY_RELEASED);
				}
			}
			else
			{
				keyHandle->KeyCnt = KEY_FULL_CNT(0, KEY_LOW_CNT(keyHandle->KeyCnt)+1);
				if(KEY_LOW_CNT(keyHandle->KeyCnt) >= KEY_REPEAT_PRESS_CNT) //����ȷ�ϰ�������
				{
					keyHandle->KeyCnt = 0; //��λ��������״̬��ת
					keyHandle->KeyFsmState = KEY_FSM_REPEAT_PRESS;
					keyHandle->KeyState = KEY_FULL_STATE(KEY_REPEAT_PRESS_WAIT, KEY_REPEAT_PRESSED);
				}
			}
			break;
		
		case KEY_FSM_RELEASE:
			keyHandle->KeyCnt = 0; //��λ��������״̬��ת
			keyHandle->KeyFsmState = KEY_FSM_WAIT_PRESS;
			keyHandle->KeyState = KEY_FULL_STATE(KEY_RELEASED, KEY_RELEASED);
			break;

		default:
			break;
	}
}


/**
  * @brief  ��ȡָ�����������ĵ�ǰ����״̬
  * @param  ָ�����������ľ��
  * @retval None
  */
KEY_StateTypeDef KEY_Get_State(KEY_HandleTypeDef* keyHandle)
{
	return (keyHandle->KeyState);
}


