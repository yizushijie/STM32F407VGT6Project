#include "random_cfg.h"

//---定义全局变量
#ifdef RNG
volatile UINT32_T g_HASH_RNG_Val = 0;
#endif
///////////////////////////////////////////////////////////////////////////////
//////函	   数：void RandomCfg_Init(void)
//////功	   能：随机数的初始化
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
void Random_Init(UINT8_T isEnableIT)
{
#ifdef RNG
    LL_RNG_DeInit(RNG);    
	//---使能随机数总线时钟
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_RNG);
	//---是否使能中断函数
	if (isEnableIT!=0)
	{
		//---HASH_RNG_IRQn中断配置---中断等级配置15,0(最低优先级)
		NVIC_SetPriority(HASH_RNG_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
		//---使能中断
		NVIC_EnableIRQ(HASH_RNG_IRQn);
		//---使能随机数中断
		LL_RNG_EnableIT(RNG);
	}
	//---使能随机产生器
	LL_RNG_Enable(RNG);
	//---等待随机数就绪
	while (LL_RNG_IsActiveFlag_DRDY(RNG) != 1)
	{
		WDT_RESET();
	}
	g_HASH_RNG_Val = 0;
#endif
}
///////////////////////////////////////////////////////////////////////////////
//////函	   数：void RandomCfg_DeInit(void)
//////功	   能：随机数的注销
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
void Random_DeInit(void)
{
#ifdef RNG
	//---时钟总线不使能
	LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_RNG);
	//---注销随机数的初始化
	LL_RNG_DeInit(RNG);
	//---不使能随机数中断
	NVIC_DisableIRQ(HASH_RNG_IRQn);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T Random_GetVal(void)
{
#ifdef RNG
	//---判断是否使能随机数中断
	if (LL_RNG_IsEnabledIT(RNG))
	{
		return g_HASH_RNG_Val;
	}
	else
	{
		//---等待随机数就绪
		while (LL_RNG_IsActiveFlag_DRDY(RNG) != 1);
		//---读取产生的随机数
		return LL_RNG_ReadRandData32(RNG);
	}
#else
	return rand();
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：UINT32_T RandomCfg_GetRandomRang(UINT32_T minVal,UINT32_T maxVal)
//////功	   能：生成在指定最小值和指定最大值范围的随机数
//////输入参数:
//////输出参数: 当前的随机数
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T Random_GetRand(UINT32_T minVal, UINT32_T maxVal)
{
	//---获取当前的随机数
	UINT32_T randomVal = 0;
	//---返回值
	UINT32_T _return = 0;
	//---判断调用的随机数函数
#ifdef RNG
	randomVal = Random_GetVal();
#else
	//---判断选取的随机数种子
	if (minVal < maxVal)
	{
		randomVal = maxVal;
	}
	else
	{
		randomVal = minVal;
	}
	//---随机数的种子范围
	srand(randomVal);
	//---获取随机数
	randomVal = rand();
#endif
	//---计算返回的随机数
	if (minVal < maxVal)
	{
		_return = randomVal % (maxVal - minVal + 1) + minVal;
	}
	else
	{
		_return = randomVal % (minVal - maxVal + 1) + maxVal;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：随机数中断处理函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Random_IRQTask(void)
{
	#ifdef RNG
		//---发生种子错误
		if (LL_RNG_IsActiveFlag_SEIS(RNG))
		{
			//---清零种子错误标志
			LL_RNG_ClearFlag_SEIS(RNG);
			//---不会使能随机数
			LL_RNG_Disable(RNG);
			//---重新使能使能随机数
			LL_RNG_Enable(RNG);
		}
		//---发生时钟错误
		if (LL_RNG_IsActiveFlag_CEIS(RNG))
		{
			//---清零时钟错误标志
			LL_RNG_ClearFlag_CEIS(RNG);
		}
		//---数据就绪
		if (LL_RNG_IsActiveFlag_DRDY(RNG))
		{
			g_HASH_RNG_Val = LL_RNG_ReadRandData32(RNG);
		}
	#endif
}