#include "Sys.h"
#include "SmartOSPor.h"
#include "stm32f10x_rcc.h"

static __I uint8_t APBAHBPrescTable[16] = { 0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9 };
static __I uint8_t ADCPrescTable[4] = { 2, 4, 6, 8 };

#ifdef __cplusplus
extern "C" {
#endif
	/******************************** 增加专有函数 ********************************/
	uint32_t RCC_GetSysClock(void)
	{
		uint32_t tmp = 0, pllmull = 0, pllsource = 0;

		/* Get SYSCLK source -------------------------------------------------------*/
		tmp = RCC->CFGR & RCC_CFGR_SWS;

		switch (tmp)
		{
		case 0x00:  /* HSI used as system clock */
			return HSI_VALUE;
		case 0x04:  /* HSE used as system clock */
			return HSE_VALUE;
		case 0x08:  /* PLL used as system clock */
		/* Get PLL clock source and multiplication factor ----------------------*/
			pllmull = RCC->CFGR & (CFGR_PLLMull_Mask | GD32_PLL_MASK);
			pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;

#ifndef STM32F10X_CL
			if ((pllmull & GD32_PLL_MASK) != 0)
				pllmull = ((pllmull & CFGR_PLLMull_Mask) >> 18) + 17;
			else
				pllmull = (pllmull >> 18) + 2;

			/* HSI oscillator clock divided by 2 selected as PLL clock entry */
			if (pllsource == 0x00) return (HSI_VALUE >> 1) * pllmull;

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
			tmp = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
			/* HSE oscillator clock selected as PREDIV1 clock entry */
			return (HSE_VALUE / tmp) * pllmull;
#else
			/* HSE selected as PLL clock entry */
			if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
				/* HSE oscillator clock divided by 2 */
				return (HSE_VALUE >> 1) * pllmull;
			else
				return HSE_VALUE * pllmull;
#endif
#else
			pllmull = pllmull >> 18;

			if (pllmull != 0x0D)
				pllmull += 2;
			else
				/* PLL multiplication factor = PLL input clock * 6.5 */
				pllmull = 13 / 2;

			/* HSI oscillator clock divided by 2 selected as PLL clock entry */
			if (pllsource == 0x00) return (HSI_VALUE >> 1) * pllmull;
			/* PREDIV1 selected as PLL clock entry */

			/* Get PREDIV1 clock source and division factor */
			prediv1source = RCC->CFGR2 & CFGR2_PREDIV1SRC;
			prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;

			/* HSE oscillator clock selected as PREDIV1 clock entry */
			if (prediv1source == 0) return (HSE_VALUE / prediv1factor) * pllmull;
			/* PLL2 clock selected as PREDIV1 clock entry */

			/* Get PREDIV2 division factor and PLL2 multiplication factor */
			prediv2factor = ((RCC->CFGR2 & CFGR2_PREDIV2) >> 4) + 1;
			pll2mull = ((RCC->CFGR2 & CFGR2_PLL2MUL) >> 8) + 2;
			return (((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * pllmull;
#endif /* STM32F10X_CL */
		default: /* HSI used as system clock */
			return HSI_VALUE;
		}
	}

	uint32_t RCC_GetHCLK(void)
	{
		uint32_t tmp = 0, presc = 0;

		/* Get HCLK prescaler */
		tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
		tmp = tmp >> 4;
		presc = APBAHBPrescTable[tmp];
		/* HCLK clock frequency */
		return RCC_GetSysClock() >> presc;
	}

	uint32_t RCC_GetPCLK(void)
	{
		uint32_t tmp = 0, presc = 0;

		/* Get PCLK prescaler */
		tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
		tmp = tmp >> 8;
		presc = APBAHBPrescTable[tmp];
		/* PCLK clock frequency */
		return RCC_GetHCLK() >> presc;
	}

	uint32_t RCC_GetPCLK2(void)
	{
		uint32_t tmp = 0, presc = 0;

		/* Get PCLK prescaler */
		tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
		tmp = tmp >> 11;
		presc = APBAHBPrescTable[tmp];
		/* PCLK2 clock frequency */
		return RCC_GetHCLK() >> presc;
	}

#ifdef __cplusplus
}
#endif
