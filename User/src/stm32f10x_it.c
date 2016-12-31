/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "Type.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
//外部中断0
extern uint flagbtn;//按键
extern uint exticnt;//中断次数

void EXTI0_IRQHandler()
{    
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {

        EXTI_ClearITPendingBit(EXTI_Line0); //清除中断标志位		
		exticnt++;
    }
	flagbtn=!flagbtn;
}

void EXTI1_IRQHandler()
{   
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line1); //清除中断标志位
		exticnt++;
    }
	flagbtn=!flagbtn;
}

void EXTI2_IRQHandler()
{    
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line2); //清除中断标志位
		exticnt++;
    }
	flagbtn=!flagbtn;
}

void EXTI3_IRQHandler()
{    
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line3); //清除中断标志位
		exticnt++;
    }
	flagbtn=!flagbtn;
}

void EXTI4_IRQHandler()
{   
    if (EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line4); //清除中断标志位
		exticnt++;
    }
	flagbtn=!flagbtn;
}

void EXTI9_5_IRQHandler()
{    
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5); //清除中断标志位		
    }	 
	if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line6); //清除中断标志位		
    }	 
	if (EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line7); //清除中断标志位		
    }	 
	if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line8); //清除中断标志位		
    }	 
	if (EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line9); //清除中断标志位		
    }	 
	exticnt++;
}
/// IO 线中断，中断口为PC13
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line10); //清除中断标志位		
    }  
	if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line11); //清除中断标志位		
    }  
	if (EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line12); //清除中断标志位		
    }  
	if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line13); //清除中断标志位		
    }  
	if (EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line14); //清除中断标志位		
    }  
	if (EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line15); //清除中断标志位		
    }  
	exticnt++;
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
