#include "gpio_cfg.h"

#ifdef USE_MCU_STM32
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void GPIO_Clock(GPIO_TypeDef *GPIOx, UINT8_T isEnable)
{
	//---使能端口的时钟
#ifdef GPIOA
	if (GPIOx == GPIOA)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		#endif
		}
		return;
	}
#endif // GPIOA

#ifdef GPIOB
	if (GPIOx == GPIOB)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOB);
		#endif
		}
		return;
	}
#endif // GPIOB

#ifdef GPIOC
	if (GPIOx == GPIOC)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOC);
		#endif
		}
		return;
	}
#endif // GPIOC

#ifdef GPIOD
	if (GPIOx == GPIOD)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOD);
		#endif
		}
		return;
	}
#endif // GPIOD

#ifdef GPIOE
	if (GPIOx == GPIOE)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOE);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOE);
		#endif
		}
		return;
	}
#endif // GPIOE

#ifdef GPIOF
	if (GPIOx == GPIOF)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOF);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOF);
		#endif
		}
		return;
	}
#endif // GPIOF

#ifdef GPIOG
	if (GPIOx == GPIOG)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOG);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOG);
		#endif
		}
		return;
	}
#endif // GPIOG

#ifdef GPIOH
	if (GPIOx == GPIOH)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOH);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOH);
		#endif
		}
		return;
	}
#endif // GPIOH

#ifdef GPIOI
	if (GPIOx == GPIOI)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOI);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOI);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOI);
		#endif
		}
		return;
	}
#endif // GPIOI

#ifdef GPIOJ
	if (GPIOx == GPIOJ)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOJ);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOJ);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOJ);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOJ);
		#endif
		}
		return;
	}
#endif // GPIOJ

#ifdef GPIOK
	if (GPIOx == GPIOK)
	{
		if (isEnable)
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOK);
		#else
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOK);
		#endif
		}
		else
		{
		#ifndef USE_MCU_STM32F1
			LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOK);
		#else
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_GPIOK);
		#endif
		}
		return;
	}
#endif // GPIOK
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： void GPIO_Init(void)
//////功	   能： 设置所有的端口为模拟输入
//////输入参数: 无
//////输出参数: 无
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
void GPIO_Init(void)
{
	//---使能所有端口的时钟
	#ifdef GPIOA
		GPIO_Clock(GPIOA, 1);
	#endif
	
	#ifdef GPIOB
		GPIO_Clock(GPIOB, 1);
	#endif
	
	#ifdef GPIOC
		GPIO_Clock(GPIOC, 1);
	#endif
	
	#ifdef GPIOD
		GPIO_Clock(GPIOD, 1);
	#endif
	
	#ifdef GPIOE
		GPIO_Clock(GPIOE, 1);
	#endif
	
	#ifdef GPIOF
		GPIO_Clock(GPIOF, 1);
	#endif
	
	#ifdef GPIOG
		GPIO_Clock(GPIOG, 1);
	#endif
	
	#ifdef GPIOH
		GPIO_Clock(GPIOH, 1);
	#endif
	
	#ifdef GPIOI
		GPIO_Clock(GPIOI, 1);
	#endif
	
	//---复用时钟
	#ifndef USE_MCU_STM32F1
		//---使能复用时钟
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	#else
		//---使能端口复用时钟
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
	#endif
	
	//---是否使用SWD调试端口
	#ifdef USE_DEBUG_SWD
		#ifdef  USE_MCU_STM32F1
			//---屏蔽JTAG端口，使能SWD端口
			LL_GPIO_AF_Remap_SWJ_NOJTAG();
			//---释放PB4端口
			LL_GPIO_AF_Remap_SWJ_NONJTRST();
		#endif
	#endif
}

#endif 