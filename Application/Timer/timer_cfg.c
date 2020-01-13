#include "timer_cfg.h"

//===全局变量定义
CalcFreq_HandlerType  g_CalcFreq;
pCalcFreq_HandlerType pCalcFreq = &g_CalcFreq;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 外部计数模式的配置
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Timer_CalcFreqMode_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	//---使能定时器时钟
	Timer_Clock(TIM3, 1);
	//---使能端口时钟
	GPIO_Clock(GPIOC, 1);
	//---GPIO初始化---PC6映射为Tim3_CH1
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
#ifdef USE_MCU_STM32F1
	LL_GPIO_AF_RemapPartial_TIM3();
	//GPIO_InitStruct.Mode		= LL_GPIO_MODE_FLOATING;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
#else
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
#endif
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;														//---端口复用模式
#endif
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#ifdef CALC_FREQ_USE_lEVEL_SHIFT
	#ifdef STM32_USE_F407VGT6
		pCalcFreq->msgOE[0].msgPort=GPIOD;
		pCalcFreq->msgOE[0].msgBit= LL_GPIO_PIN_7;
	#elif defined(STM32_USE_F407VET6)
		pCalcFreq->msgOE[0].msgPort = GPIOC;
		pCalcFreq->msgOE[0].msgBit = LL_GPIO_PIN_8;
	#else	
		#error "暂时不支持的型号，请确认型号!"
	#endif
	//---使能端口时钟
	GPIO_Clock(pCalcFreq->msgOE[0].msgPort, 1);
	GPIO_InitStruct.Pin = pCalcFreq->msgOE[0].msgBit;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;														//---端口复用模式
#endif
	LL_GPIO_Init(pCalcFreq->msgOE[0].msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(pCalcFreq->msgOE[0].msgPort, pCalcFreq->msgOE[0].msgBit);
#endif
	//////////////////////////////////////////////////////////////////////////////
	//---GPIO初始化---PC7映射为Tim3_CH2
	GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
#ifdef USE_MCU_STM32F1
	LL_GPIO_AF_RemapPartial_TIM3();
	//GPIO_InitStruct.Mode		= LL_GPIO_MODE_FLOATING;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
#else
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
#endif
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;														//---端口复用模式
#endif
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#ifdef CALC_FREQ_USE_lEVEL_SHIFT
	#ifdef STM32_USE_F407VGT6
		pCalcFreq->msgOE[1].msgPort = GPIOD;
		pCalcFreq->msgOE[1].msgBit = LL_GPIO_PIN_7;
	#elif defined(STM32_USE_F407VET6)
		pCalcFreq->msgOE[1].msgPort = GPIOC;
		pCalcFreq->msgOE[1].msgBit = LL_GPIO_PIN_8;
	#else	
		#error "暂时不支持的型号，请确认型号!"
	#endif
	//---使能端口时钟
	GPIO_Clock(pCalcFreq->msgOE[1].msgPort, 1);
	GPIO_InitStruct.Pin = pCalcFreq->msgOE[1].msgBit;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;														//---端口复用模式
#endif
	LL_GPIO_Init(pCalcFreq->msgOE[1].msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(pCalcFreq->msgOE[1].msgPort, pCalcFreq->msgOE[1].msgBit);
#endif
	Timer_CalcFreqMode_DeInit();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Timer_CalcFreqMode_DeInit(void)
{
	LL_TIM_DeInit(TIM3);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Timer_CalcFreqMode_PreInit(UINT8_T ch)
{
	LL_TIM_InitTypeDef TIM_InitStruct = { 0 };
	//---计数器的时钟的预分频
	TIM_InitStruct.Prescaler = 0;
	//---向上计数
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	//---自动装载值，自动装载值为空时，计数器不工作
	TIM_InitStruct.Autoreload = 0xFFFFFF;
	//---定时器时钟分频数
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	//---初始化定时器
	LL_TIM_Init(TIM3, &TIM_InitStruct);
	//---是能自动装载
	LL_TIM_EnableARRPreload(TIM3);
	//---通道选择
	if (ch==0)
	{
		//---设置触发输入通道
		LL_TIM_SetTriggerInput(TIM3, LL_TIM_TS_TI1FP1);
		//---激活输入通道
		LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
		//---设置适合时钟源
		LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_EXT_MODE1);
		//---禁用捕获/比较通道
		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1);
		//---设置通道的滤波时间
		LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1_N2);
		//---通道2的上升沿触发
		LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
		pCalcFreq->msgChannel = 0;
	}
	else 
	{
		//---设置触发输入通道
		LL_TIM_SetTriggerInput(TIM3, LL_TIM_TS_TI2FP2);
		//---激活输入通道
		LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
		//---设置适合时钟源
		LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_EXT_MODE1);
		//---禁用捕获/比较通道
		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH2);
		//---设置通道的滤波时间
		LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1_N2);
		//---通道2的上升沿触发
		LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
		pCalcFreq->msgChannel = 1;
	}
	//---禁用触发中断（TIE）
	LL_TIM_DisableIT_TRIG(TIM3);
	//---禁用DMA触发中断（TIE）
	LL_TIM_DisableDMAReq_TRIG(TIM3);
	//---主模式选择复位
	LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
	//---不使能主从模式
	LL_TIM_DisableMasterSlaveMode(TIM3);
	//---使能计数器
	//LL_TIM_EnableCounter(TIM5);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Timer_CalcFreq_Init(void)
{
	VLTUINT32_T tempFreq = 0;
	//---计数次数
	static UINT8_T nCount = 0;
	//---定时器溢出次数
	static UINT8_T nOverCount = 0;
	//---开始计数
	if (pCalcFreq->msgStep[pCalcFreq->msgChannel] == 0)
	{
		pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel] = 0;
		pCalcFreq->msgFreqMHz[pCalcFreq->msgChannel] = 0;
		pCalcFreq->msgStep[pCalcFreq->msgChannel] = 1;
		//---清零触发器中断标记
		LL_TIM_ClearFlag_TRIG(TIM3);
		//---清零更新中断标记
		LL_TIM_ClearFlag_UPDATE(TIM3);
		//---清零计数器
		LL_TIM_SetCounter(TIM3, 0);
		//---使能计数器
		LL_TIM_EnableCounter(TIM3);
		nCount = 0;
	}
	//---读取初始值
	else if (pCalcFreq->msgStep[pCalcFreq->msgChannel] == 1)
	{
		//---触发中断发生
		if (LL_TIM_IsActiveFlag_TRIG(TIM3) != 0)
		{
			//---获取当前的计数结果
			pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel] = LL_TIM_GetCounter(TIM3);
			//---清理中断标志
			LL_TIM_ClearFlag_TRIG(TIM3);
			pCalcFreq->msgStep[pCalcFreq->msgChannel] = 2;
			//---必须清零这两个数据，使用了类似全局变量，需要手动清除
			nOverCount = 0;
			nCount = 0;
		}
	}
	else if (pCalcFreq->msgStep[pCalcFreq->msgChannel] == 2)
	{
		//---计数次数;多次计数能够提高获取频率的精准度
		nCount++;
		//---更新中断
		if (LL_TIM_IsActiveFlag_UPDATE(TIM3) != 0)
		{
			//---清零更新中断标记
			LL_TIM_ClearFlag_UPDATE(TIM3);
			nOverCount++;
		}
		//---触发中断发生
		if (LL_TIM_IsActiveFlag_TRIG(TIM3) != 0)
		{
			//---清理中断标志
			LL_TIM_ClearFlag_TRIG(TIM3);
		}
		//---读取10次后的值
		if (nCount > 9)
		{
			//---获取当前的计数结果
			tempFreq = LL_TIM_GetCounter(TIM3);
			//---不使能计数器
			LL_TIM_DisableCounter(TIM3);
			//---操作完成
			pCalcFreq->msgStep[pCalcFreq->msgChannel] = 3;
			//---计算频率
			pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel] = (0xFFFF * nOverCount + tempFreq - pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel]);
			//---必须清零这两个数据，使用了类似全局变量，需要手动清除
			nOverCount=0;
			nCount=0;
		}
	}
	else
	{
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 计算频率任务
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Timer_CalcFreq_Task(UINT8_T ch)
{
#ifdef CALC_FREQ_USE_lEVEL_SHIFT
	GPIO_OUT_0(pCalcFreq->msgOE[ch].msgPort, pCalcFreq->msgOE[ch].msgBit);
#endif
	//---初始化使用的定时器
	Timer_CalcFreqMode_PreInit(ch);	
	/*
	//---保存当前滴答定时器的任务
	pCalcFreq->msgFuncTask = pSysTick->msgFuncTick;
	
	//---重新注册当前滴答定时器的任务，获取输入信号的频率---1ms采集到上升沿的格式，比如采集到1个就是1KHz，暂不支持1KHz一下的信号
	SysTickTask_FuncTick(Timer_CalcFreq_Init);
	*/
	UINT8_T _return = SysTickTask_CreateTickTask(Timer_CalcFreq_Init);
	//---读取当前滴答定时时间
	UINT32_T nowTime = 0;
	UINT32_T oldTime = SysTickTask_GetTick();
	UINT32_T cnt = 0;
	if (_return!=0)
	{
		pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel] = 0;
		goto GoToExit;
	}
	//---等待计数器计数完成
	while (pCalcFreq->msgStep[pCalcFreq->msgChannel] != 3)
	{
		//---当前时间
		nowTime = SysTickTask_GetTick();
		//---判断滴答定时是否发生溢出操作
		if (nowTime < oldTime)
		{
			cnt = (0xFFFFFFFF - oldTime + nowTime);
		}
		else
		{
			cnt = nowTime - oldTime;
		}
		//---判断是否超时,超时100ms
		if (cnt > 100)
		{
			//---发送发生超时错误
			break;
		}
	}
	//---发生超时错误
	if (cnt > 100)
	{
		pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel] = 0;
	}
	else
	{
		pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel] *=0.1; //0.099983;
	}
	//---退出操作
GoToExit:
	//---注销任务函数
	_return = SysTickTask_DeleteTickTask(Timer_CalcFreq_Init);
	//---清零数据
	pCalcFreq->msgStep[pCalcFreq->msgChannel] = 0;
	//---换算之后的MHz
	pCalcFreq->msgFreqMHz[pCalcFreq->msgChannel] = (float)(pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel]) / 1000.0;
	//---销毁定时器的配置
	Timer_CalcFreqMode_DeInit();
#ifdef CALC_FREQ_USE_lEVEL_SHIFT
	GPIO_OUT_1(pCalcFreq->msgOE[ch].msgPort, pCalcFreq->msgOE[0].msgBit);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
float Timer_GetFreqKHz(void)
{
	return pCalcFreq->msgFreqKHz[pCalcFreq->msgChannel];
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
float Timer_GetFreqMHz(void)
{
	return pCalcFreq->msgFreqMHz[pCalcFreq->msgChannel];
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Timer_Init(void)
{
	Timer_CalcFreqMode_Init();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Timer_Clock(TIM_TypeDef *TIMx, UINT8_T isEnable)
{
#ifdef TIM1
	if (TIMx == TIM1)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM1);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM2
	else if (TIMx == TIM2)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM2);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM3
	else if (TIMx == TIM3)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM3);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM4
	else if (TIMx == TIM4)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM4);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM4);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM4);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM5
	else if (TIMx == TIM5)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM5);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM5);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM5);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM6
	else if (TIMx == TIM6)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM6);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM6);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM6);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM7
	else if (TIMx == TIM7)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM7);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM7);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM7);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM8
	else if (TIMx == TIM8)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM8);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM8);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM8);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM9
	else if (TIMx == TIM9)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM9);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM9);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM9);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM9);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM10
	else if (TIMx == TIM10)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM10);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM10);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM10);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM10);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM11
	else if (TIMx == TIM11)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM11);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM11);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM11);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM11);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM12
	else if (TIMx == TIM12)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM12);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM12);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM12);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM12);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM13
	else if (TIMx == TIM13)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM13);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM13);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM13);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM13);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif

#ifdef TIM14
	else if (TIMx == TIM14)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM14);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM14);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM14);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM15
	else if (TIMx == TIM15)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM15);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM15);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM15);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM15);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM16
	else if (TIMx == TIM16)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM16);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM16);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM16);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM16);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
#ifdef TIM17
	else if (TIMx == TIM17)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM17);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM17);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM17);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM17);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
	else
	{
		return ERROR_2;
	}
}