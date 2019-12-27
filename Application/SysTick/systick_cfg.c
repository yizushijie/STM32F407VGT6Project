#include "systick_cfg.h"

//---变量定义
SysTick_HandlerType		g_SysTick = { 0 };
pSysTick_HandlerType	pSysTick = &g_SysTick;

#if defined (USE_HAL_DRIVER)
	//---外部调用的Hal库延时变量
	extern HAL_TickFreqTypeDef uwTickFreq;
#endif

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_Suspend(void)
{
	//---不使能系统滴答定时器的中断
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_Resume(void)
{
	//---使能系统滴答定时器的中断
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： SysTick使用的结构体的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_StructInit(SysTick_HandlerType *sysTickx)
{
	UINT8_T i = 0;
	sysTickx->msgIncTick = 0;
	sysTickx->msgDecTick = 0;
	sysTickx->msgIncTickOVF = 0;
	//sysTickx->msgHalIncTick = NULL;
	//sysTickx->msgFuncTick = NULL;

	for (i=0;i< SYSTICK_FUNC_TASK_MAX_NUM;i++)
	{
		sysTickx->msgTickTaskFlag[i] = 0;
		sysTickx->msgTickTask[i] = NULL;
	}

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 最好1ms的滴答定时器
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_msInit(void)
{
	//---初始化结构体
	SysTick_StructInit(pSysTick);

	//---挂起滴答定时器
	SysTick_Suspend();

	//---产生1ms的时间滴答器---该初始化函数没有注册中断
	LL_InitTick(SystemCoreClock, 1000);

	//---设置中断抢占分组
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//---配置滴答定时器中断---设置为最低优先级
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

	//---使能滴答定时的中断---默认是1ms
	SysTick_Resume();
#ifdef USE_HAL_DRIVER

	//---Hal库用于延时计算
	uwTickFreq = HAL_TICK_FREQ_DEFAULT;

	SysTick_CreateTickTask(HAL_IncTick);

	//---注册Hal库计数器
	//pSysTick->msgHalIncTick = HAL_IncTick;

#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_DeInit(void)
{
	SysTick_Suspend();
	SysTick_StructInit(pSysTick);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：创建滴答任务函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_CreateTickTask(void(*pFuncTick)(void))
{
	UINT8_T _return = ERROR_1;
	if (pFuncTick!=NULL)
	{
		for (_return = 0; _return < SYSTICK_FUNC_TASK_MAX_NUM; _return++)
		{
			//---检查事件是否已经注册过了,避免重复注册
			if ((pSysTick->msgTickTask[_return]!=NULL)&&(pSysTick->msgTickTask[_return] == pFuncTick))
			{
				pSysTick->msgTickTaskFlag[_return] = 1;
				//---事件已经注册过，退出循环
				break;
			}
			//---检查哪个事件为空
			if (pSysTick->msgTickTaskFlag[_return]==0)
			{
				pSysTick->msgTickTask[_return] = pFuncTick;
				pSysTick->msgTickTaskFlag[_return] = 1;
				//---滴答任务的增加
				if (pSysTick->msgTickTaskCount<0xFF)
				{
					pSysTick->msgTickTaskCount += 1;
				}
				//---找到空闲事件
				break;
			}
		}
		//---返回结果
		_return = (_return < SYSTICK_FUNC_TASK_MAX_NUM) ? OK_0 : ERROR_2;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：销毁任务函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_DeleteTickTask(void(*pFuncTick)(void))
{
	UINT8_T _return = ERROR_1;
	if (pFuncTick != NULL)
	{
		for (_return =0; _return < SYSTICK_FUNC_TASK_MAX_NUM; _return++)
		{
			//---检查要注销的事件是否存在
			if ((pSysTick->msgTickTask[_return] != NULL)&&(pSysTick->msgTickTask[_return] == pFuncTick))
			{
				pSysTick->msgTickTask[_return] = NULL;
				pSysTick->msgTickTaskFlag[_return] = 0;
				//---滴答任务的减少
				if (pSysTick->msgTickTaskCount>0)
				{
					pSysTick->msgTickTaskCount -= 1;
				}
				//---找到要注销的事件
				break;
			}
		}
		//---返回结果
		_return = (_return < SYSTICK_FUNC_TASK_MAX_NUM) ? OK_0 : ERROR_2;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：轮询任务函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void SysTick_PollTickTask(void)
{
	UINT8_T index = 0;
	for (index= 0; index< SYSTICK_FUNC_TASK_MAX_NUM; index++)
	{
		if (pSysTick->msgTickTaskFlag[index] !=0)
		{
			if (pSysTick->msgTickTask[index] != NULL)
			{
				pSysTick->msgTickTask[index]();
			}
		}
	}
}

/*
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_FuncTick(void(*pFuncTick)(void))
{
	if (pFuncTick != NULL)
	{
		//---注册任务函数
		pSysTick->msgFuncTick = pFuncTick;
	}
	return OK_0;
}
*/

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：等待延时到达
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_IRQDecTick(UINT32_T waitDecTick)
{
	//---判断当前的值
	UINT32_T load = SysTick->LOAD + 1;
	UINT32_T val = SysTick->VAL;
	load >>= 1;

	//---修正补偿延时
	if (val < load)
	{
		waitDecTick += 1;
	}

	//---计时函数
	pSysTick->msgDecTick = waitDecTick;

	//---等待时间到达
	while (pSysTick->msgDecTick != 0)
	{
		WDT_RESET();
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：等待延时到达
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_DecTick(UINT32_T waitDecTick)
{
	//---计算要滴答的时间
	UINT32_T ticks = SysTick->VAL;

	//---计算要延时的us时间
	UINT32_T usDelayTicks = (ticks / SYS_CLOCK_MHZ);

	//---如果时间在1us的误差内，放弃
	if (usDelayTicks > 1)
	{
		//---当前计数器归零值的计数
		SysTick_WaitusTick(usDelayTicks);

		//---ms中断计数减小
		waitDecTick -= 1;

		//---剩下的延时时间，强制装换，避免为负数
		ticks = (UINT32_T)(1000 - usDelayTicks - 20);
	}

	//---要延时的时间
	pSysTick->msgDecTick = waitDecTick;

	//---等待时间到达
	while (pSysTick->msgDecTick != 0)
	{
		WDT_RESET();
	}

	//---如果时间在1us的误差内，放弃
	if ((ticks > 1)&&(ticks<1000))
	{
		//---完成拆分1ms之后要完成的计数值
		SysTick_WaitusTick(ticks);
	}

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T SysTick_GetTick(void)
{
	return pSysTick->msgIncTick;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_IRQTask(void)
{
	//---递加计数
	pSysTick->msgIncTick++;

	//---判断是否溢出
	if (pSysTick->msgIncTick == 0)
	{
		pSysTick->msgIncTickOVF++;
	}

	/*
	//---任务函数
	if (pSysTick->msgFuncTick != NULL)
	{
		pSysTick->msgFuncTick();
	}

	//---hal延时函数
	if (pSysTick->msgHalIncTick != NULL)
	{
		pSysTick->msgHalIncTick();
	}
	*/
	//---轮询执行任务函数
	SysTick_PollTickTask();

	//---递减计数
	if (pSysTick->msgDecTick != 0)
	{
		pSysTick->msgDecTick--;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_WaitTick(UINT32_T usTime)
{
	UINT32_T load = SysTick->LOAD;
	UINT32_T ctrl = SysTick->CTRL;
	UINT32_T val = SysTick->VAL;

	//---关闭定时器，时钟选择为系统时钟，不进行8分频
	SysTick->CTRL = 0x00000004;

	//---计算装载值
	SysTick->LOAD = usTime * SYS_CLOCK_MHZ - SYS_CLOCK_ADD;//150;//usTimer*SYS_CLOCK_MHZ - 1;

	//---清零计数器
	SysTick->VAL = 0x00;

	//---使能计数器计数
	SysTick->CTRL |= 0x01;

	//---等待计数到0
	while (!(SysTick->CTRL & 0x00010000))
	{
		WDT_RESET();
	}

	//---关闭定时器，时钟选择为系统时钟，不进行8分频
	SysTick->CTRL = 0x00000004;

	//---恢复装载值
	SysTick->LOAD = load;

	//---计数器继续计数
	SysTick->VAL = val;

	//---恢复计数器配置
	SysTick->CTRL = ctrl;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：等待us延时的到达
//////输入参数:
//////输出参数:
//////说		明：本函数的最大输入值是1000，如果值大于1000，本函数会自动跳出
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_WaitusTick(UINT32_T usTime)
{
	//---计算计数值
	UINT32_T ticks = ((usTime * SYS_CLOCK_MHZ));
	//---计数值
	UINT32_T told = SysTick->VAL;
	UINT32_T tnow = 0;
	UINT32_T tcnt = 0;

	//---装载值
	UINT32_T tload = SysTick->LOAD;

	//---判断是不是超出界限
	if (usTime < 1001)
	{
		while (1)
		{
			tnow = SysTick->VAL;
			if (told != tnow)
			{
				//---计数器为递减计数器
				if (tnow < told)
				{
					tcnt += (told - tnow);
				}
				else
				{
					tcnt += (tload - tnow + told);
				}
				told = tnow;
				if (tcnt >= ticks)
				{
					break;
				}
			}
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_WaitSysTick(UINT32_T sysTickTime)
{
	//---计算计数值
	UINT32_T ticks = sysTickTime;

	//---计数值
	UINT32_T told = SysTick->VAL;
	UINT32_T tnow = 0;
	UINT32_T tcnt = 0;

	//---装载值
	UINT32_T tload = SysTick->LOAD;
	while (1)
	{
		tnow = SysTick->VAL;
		if (told != tnow)
		{
			//---计数器为递减计数器
			if (tnow < told)
			{
				tcnt += (told - tnow);
			}
			else
			{
				tcnt += (tload - tnow + told);
			}
			told = tnow;
			if (tcnt >= ticks)
			{
				break;
			}
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：判断系统滴答定时器是不是1ms的定时
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_1msIncTick(void)
{
	//---判断时钟源是否进行8分频
	if ((SysTick->CTRL&SysTick_CTRL_CLKSOURCE_Msk) != 0)
	{
		//===时钟源没有进行8分频
		if (SysTick->LOAD != (SYS_CLOCK_KHZ - 1))
		{
			return ERROR_1;
		}
	}
	else
	{
		//===时钟源进行8分频
		if (SysTick->LOAD != ((SYS_CLOCK_KHZ >> 3) - 1))
		{
			return ERROR_1;
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 获取溢出计数的大小
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T SysTick_GetTickOVF(void)
{
	return pSysTick->msgIncTickOVF;
}