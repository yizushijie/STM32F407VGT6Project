#include "key_cfg.h"

//===>>MultiButton START=========================================================
//===按键事件列表
KeyButton_HandlerType * pHeadHandle = NULL;

//===用于计数
UINT32_T(*keyButtonFuncTimeTick)(void) = NULL;

//===事件轮训的宽度
UINT8_T g_KeyButtonTimeSpan = 0;
UINT32_T g_KeyButtonTickTime = 0;

//===定义环形缓冲区时
UINT8_T g_keyButtonLoopBuffer[256] = { 0 };

//===定义环形缓冲区写索引
UINT32_T g_keyLoopWriteIndex = 0;

//===定义环形缓冲区读索引
UINT32_T g_KeyLoopReadIndex = 0;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_Init(KeyButton_HandlerType* handle, UINT8_T(*pPinReadLevel)(), UINT8_T pinActiveLevel)
{
	memset(handle, 0, sizeof(KeyButton_HandlerType));
	handle->msgEvent = (UINT8_T)NONE_PRESS;
	handle->msgFuncReadPinLevel = pPinReadLevel;
	handle->msgButtonLevel = handle->msgFuncReadPinLevel();
	handle->msgActiveLevel = pinActiveLevel;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_Attach(KeyButton_HandlerType *handle, KeyButtonPressEvent event, KeyButtonCallBack cb)
{
	handle->msgCB[event] = cb;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
KeyButtonPressEvent KeyButton_GetButtonEvent(KeyButton_HandlerType* handle)
{
	return (KeyButtonPressEvent)(handle->msgEvent);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数: 0---事件启动；1---事件已经存在
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T KeyButton_START(KeyButton_HandlerType *btn)
{
	KeyButton_HandlerType* target = pHeadHandle;
	while (target)
	{
		if (target == btn)
		{
			//---按键事件已经存在
			return 1;
		}
		target = target->msgNext;
	}
	btn->msgNext = pHeadHandle;
	pHeadHandle = btn;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_STOP(KeyButton_HandlerType* btn)
{
	KeyButton_HandlerType** curr;
	for (curr = &pHeadHandle; *curr; )
	{
		KeyButton_HandlerType* entry = *curr;
		if (entry == btn)
		{
			*curr = entry->msgNext;
		}
		else
		{
			curr = &entry->msgNext;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_CallBackHandlerEvent(KeyButton_HandlerType* handle, UINT8_T event)
{
	if (handle->msgCB[event] != NULL)
	{
		handle->msgCB[event]((KeyButton_HandlerType*)handle);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_HandlerEvent(KeyButton_HandlerType* handle)
{
	UINT32_T read_gpio_level = handle->msgFuncReadPinLevel();

	//ticks counter working..
	if ((handle->msgState) > 0)
	{
		handle->msgTicks++;
	}

	//---消抖处理
	if (read_gpio_level != handle->msgButtonLevel)
	{
		//---连续多次按键的状态和上衣状态不一致；代表按下稳定
		if (++(handle->msgDebounceCNT) >= DEBOUNCE_TICKS)
		{
			handle->msgButtonLevel = read_gpio_level;
			handle->msgDebounceCNT = 0;
		}
	}
	else
	{
		//---按键按下稳定，重新计数消抖
		handle->msgDebounceCNT = 0;
	}

	//---状态处理
	switch (handle->msgState)
	{
		case 0:
			if (handle->msgButtonLevel == handle->msgActiveLevel)
			{
				//---按键按下
				handle->msgEvent = (UINT8_T)PRESS_DOWN;

				//---回到事件处理函数
				KeyButton_CallBackHandlerEvent(handle, PRESS_DOWN);
				handle->msgTicks = 0;
				handle->msgRepeat = 1;
				handle->msgState = 1;
			}
			else
			{
				handle->msgEvent = (UINT8_T)NONE_PRESS;
			}
			break;

		case 1:
			if (handle->msgButtonLevel != handle->msgActiveLevel)
			{
				//---释放按键，按键弹起
				handle->msgEvent = (UINT8_T)PRESS_UP;

				//---回到事件处理函数
				KeyButton_CallBackHandlerEvent(handle, PRESS_UP);
				handle->msgTicks = 0;
				handle->msgState = 2;
			}
			else if (handle->msgTicks > LONG_TICKS)
			{
				//---按键长时间按下
				handle->msgEvent = (UINT8_T)LONG_RRESS_START;

				//---回到事件处理函数
				KeyButton_CallBackHandlerEvent(handle, LONG_RRESS_START);
				handle->msgState = 5;
			}
			break;

		case 2:
			if (handle->msgButtonLevel == handle->msgActiveLevel)
			{
				//---按键再次按下
				handle->msgEvent = (UINT8_T)PRESS_DOWN;

				//---回到事件处理函数
				KeyButton_CallBackHandlerEvent(handle, PRESS_DOWN);
				handle->msgRepeat++;
				if (handle->msgRepeat == 2)
				{
					//---回到事件处理函数
					KeyButton_CallBackHandlerEvent(handle, DOUBLE_CLICK);
				}

				//---按键多次按下
				//---回到事件处理函数
				KeyButton_CallBackHandlerEvent(handle, PRESS_REPEAT);
				handle->msgTicks = 0;
				handle->msgState = 3;
			}
			else if (handle->msgTicks > SHORT_TICKS)
			{
				if (handle->msgRepeat == 1)
				{
					//---单击
					handle->msgEvent = (UINT8_T)SINGLE_CLICK;

					//---回到事件处理函数
					KeyButton_CallBackHandlerEvent(handle, SINGLE_CLICK);
				}
				else if (handle->msgRepeat == 2)
				{
					handle->msgEvent = (UINT8_T)DOUBLE_CLICK;
				}
				handle->msgState = 0;
			}
			break;

		case 3:
			if (handle->msgButtonLevel != handle->msgActiveLevel)
			{
				//---释放按键
				handle->msgEvent = (UINT8_T)PRESS_UP;

				//---回到事件处理函数
				KeyButton_CallBackHandlerEvent(handle, PRESS_UP);
				if (handle->msgTicks < SHORT_TICKS)
				{
					//---按键短按
					handle->msgTicks = 0;
					handle->msgState = 2;
				}
				else
				{
					handle->msgState = 0;
				}
			}

			break;

		case 5:
			if (handle->msgButtonLevel == handle->msgActiveLevel)
			{
				//---按键长时间按下
				handle->msgEvent = (UINT8_T)LONG_PRESS_HOLD;

				//---回到事件处理函数
				KeyButton_CallBackHandlerEvent(handle, LONG_PRESS_HOLD);
			}
			else
			{
				//---按键释放
				handle->msgEvent = (UINT8_T)PRESS_UP;

				//---回到事件处理函数
				KeyButton_CallBackHandlerEvent(handle, PRESS_UP);

				//---状态清零
				handle->msgState = 0; //reset
			}
			break;
		default:

			//---状态清零
			handle->msgState = 0; //reset
			handle->msgEvent = (UINT8_T)NONE_PRESS;
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：定时器调用
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_TimeTicks()
{
	KeyButton_HandlerType* target;
	for (target = pHeadHandle; target; target = target->msgNext)
	{
		KeyButton_HandlerEvent(target);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：轮训调度的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_PollInit(UINT32_T(*pFuncTimerTick)(void), UINT8_T timeSpan)
{
	keyButtonFuncTimeTick = pFuncTimerTick;
	g_KeyButtonTimeSpan = timeSpan;
	if (keyButtonFuncTimeTick != NULL)
	{
		g_KeyButtonTickTime = keyButtonFuncTimeTick();
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：主函数轮训调用
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_PollTicks()
{
	UINT32_T nowTime = 0;
	UINT32_T cnt = 0;
	KeyButton_HandlerType* target;
	if (keyButtonFuncTimeTick != NULL)
	{
		//---获取当前时间节拍
		nowTime = keyButtonFuncTimeTick();

		//---判断滴答定时是否发生溢出操作
		if (nowTime < g_KeyButtonTickTime)
		{
			cnt = (0xFFFFFFFF - g_KeyButtonTickTime + nowTime);
		}
		else
		{
			cnt = nowTime - g_KeyButtonTickTime;
		}

		//---判断轮询时间是否到达
		if (cnt >= g_KeyButtonTimeSpan)
		{
			//---便利事件处理
			for (target = pHeadHandle; target; target = target->msgNext)
			{
				KeyButton_HandlerEvent(target);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：向环形缓冲区写一字节数据
//////输入参数:
//////输出参数:
//////说		明：0---成功；1---失败
//////////////////////////////////////////////////////////////////////////////
UINT8_T KeyButton_WriteKeyLoopBuffer(UINT8_T dat)
{
	UINT32_T next_wp;

	next_wp = (g_keyLoopWriteIndex + 1) & 255;//11 1111 1111

	if (next_wp == g_KeyLoopReadIndex)
	{
		return 1;
	}
	g_keyButtonLoopBuffer[g_keyLoopWriteIndex] = dat;
	g_keyLoopWriteIndex = next_wp;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：从环形缓冲区读一字节数据
//////输入参数:
//////输出参数:
//////说		明：0---成功；1---失败
//////////////////////////////////////////////////////////////////////////////
UINT8_T KeyButton_ReadKeyLoopBuffer(UINT8_T *dat)
{
	if (g_KeyLoopReadIndex == g_keyLoopWriteIndex)
	{
		return 1;
	}

	*dat = g_keyButtonLoopBuffer[g_KeyLoopReadIndex];
	g_KeyLoopReadIndex = (g_KeyLoopReadIndex + 1) & 255;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：清理缓存
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton_ClearKeyLoop(void)
{
	memset(g_keyButtonLoopBuffer, 0, sizeof(g_keyButtonLoopBuffer));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：清理缓存
//////输入参数:
//////输出参数:
//////说		明：0---无键值；1---有键值
//////////////////////////////////////////////////////////////////////////////
UINT8_T KeyButton_GetKeyVal(void) //获取键值
{
	UINT8_T key_val;

	if (KeyButton_ReadKeyLoopBuffer(&key_val) == 1) //读取队列成功
	{
		return key_val;
	}
	else
	{
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton1_PRESS_DOWN_Handler(void* btn)
{
	//---处理按键1按下的事件
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton1_PRESS_UP_Handler(void* btn)
{
	//---处理按键1弹起事件
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton2_PRESS_DOWN_Handler(void* btn)
{
	//---处理按键1按下的事件
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton2_PRESS_UP_Handler(void* btn)
{
	//---处理按键1弹起事件
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton3_PRESS_DOWN_Handler(void* btn)
{
	//---处理按键1按下的事件
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void KeyButton4_PRESS_UP_Handler(void* btn)
{
	//---处理按键1弹起事件
}

//===<<MultiButton END=========================================================

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_BINInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(BINA_CTR_PORT, 1);
	GPIOTask_Clock(BINB_CTR_PORT, 1);
	GPIOTask_Clock(BINC_CTR_PORT, 1);
	GPIOTask_Clock(BIND_CTR_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---中速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

//---BINA_CTR_BIT的初始化
	GPIO_InitStruct.Pin = BINA_CTR_BIT;
	LL_GPIO_Init(BINA_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(BINA_CTR_PORT, BINA_CTR_BIT);

	//---BINB_CTR_BIT的初始化
	GPIO_InitStruct.Pin = BINB_CTR_BIT;
	LL_GPIO_Init(BINB_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(BINB_CTR_PORT, BINB_CTR_BIT);

	//---BINC_CTR_BIT的初始化
	GPIO_InitStruct.Pin = BINC_CTR_BIT;
	LL_GPIO_Init(BINC_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(BINC_CTR_PORT, BINC_CTR_BIT);

	//---BIND_CTR_BIT的初始化
	GPIO_InitStruct.Pin = BIND_CTR_BIT;
	LL_GPIO_Init(BIND_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(BIND_CTR_PORT, BIND_CTR_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_SOTInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(SOTA_CTR_PORT, 1);
	GPIOTask_Clock(SOTB_CTR_PORT, 1);
	GPIOTask_Clock(SOTC_CTR_PORT, 1);
	GPIOTask_Clock(SOTD_CTR_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---中速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

//---SOTA_CTR_BIT的初始化
	GPIO_InitStruct.Pin = SOTA_CTR_BIT;
	LL_GPIO_Init(SOTA_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(SOTA_CTR_PORT, SOTA_CTR_BIT);

	//---SOTB_CTR_BIT的初始化
	GPIO_InitStruct.Pin = SOTB_CTR_BIT;
	LL_GPIO_Init(SOTB_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(SOTB_CTR_PORT, SOTB_CTR_BIT);

	//---SOTC_CTR_BIT的初始化
	GPIO_InitStruct.Pin = SOTC_CTR_BIT;
	LL_GPIO_Init(SOTC_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(SOTC_CTR_PORT, SOTC_CTR_BIT);

	//---SOTD_CTR_BIT的初始化
	GPIO_InitStruct.Pin = SOTD_CTR_BIT;
	LL_GPIO_Init(SOTD_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(SOTD_CTR_PORT, SOTD_CTR_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_EOTInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(EOTA_CTR_PORT, 1);
	GPIOTask_Clock(EOTB_CTR_PORT, 1);
	GPIOTask_Clock(EOTC_CTR_PORT, 1);
	GPIOTask_Clock(EOTD_CTR_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---中速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

//---EOTA_CTR_BIT的初始化
	GPIO_InitStruct.Pin = EOTA_CTR_BIT;
	LL_GPIO_Init(EOTA_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(EOTA_CTR_PORT, EOTA_CTR_BIT);

	//---EOTB_CTR_BIT的初始化
	GPIO_InitStruct.Pin = EOTB_CTR_BIT;
	LL_GPIO_Init(EOTB_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(EOTB_CTR_PORT, EOTB_CTR_BIT);

	//---EOTC_CTR_BIT的初始化
	GPIO_InitStruct.Pin = EOTC_CTR_BIT;
	LL_GPIO_Init(EOTC_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(EOTC_CTR_PORT, EOTC_CTR_BIT);

	//---EOTD_CTR_BIT的初始化
	GPIO_InitStruct.Pin = EOTD_CTR_BIT;
	LL_GPIO_Init(EOTD_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(EOTD_CTR_PORT, EOTD_CTR_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_Init(void)
{
	//Key_BINInit();
	//Key_SOTInit();
	//Key_EOTInit();
	Key_GenSelInit();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 获取SOT的状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Key_GetSOT(void)
{
	UINT8_T _return = 0;

	//---检测SOT
	if (SOTA_CTR_STATE == 0)
	{
		_return |= 0x01;
	}
	if (SOTB_CTR_STATE == 0)
	{
		_return |= 0x02;
	}
	if (SOTC_CTR_STATE == 0)
	{
		_return |= 0x04;
	}
	if (SOTD_CTR_STATE == 0)
	{
		_return |= 0x08;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Key_ScanSOT(void)
{
	//---读取SOT的状态
	UINT8_T _return = Key_GetSOT();
	if (_return != 0)
	{
	#ifndef USE_FT_CP_TEST

		//---延时等待，调试模式下用于软件消抖同时用于等待其他SITE的触发信号的到来
		//DelayTask_ms(10);
	#endif

	//---读取SOT的状态
		_return = Key_GetSOT();
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_EOTSTART(void)
{
	//---置位SOT
	EOTA_CTR_OUT_1;
	EOTB_CTR_OUT_1;
	EOTC_CTR_OUT_1;
	EOTD_CTR_OUT_1;

	//---置位BIN
	BINA_CTR_OUT_1;
	BINB_CTR_OUT_1;
	BINC_CTR_OUT_1;
	BIND_CTR_OUT_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_EOTSTOP(void)
{
	EOTA_CTR_OUT_0;
	EOTB_CTR_OUT_0;
	EOTC_CTR_OUT_0;
	EOTD_CTR_OUT_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_BINAPass(UINT8_T isPass)
{
	if (isPass == 0)
	{
		//---SITEA合格
		BINA_CTR_OUT_0;
	}
	else
	{
		//---SITEA不合格
		BINA_CTR_OUT_1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_BINBPass(UINT8_T isPass)
{
	if (isPass == 0)
	{
		//---SITEB合格
		BINB_CTR_OUT_0;
	}
	else
	{
		//---SITEB不合格
		BINB_CTR_OUT_1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_BINCPass(UINT8_T isPass)
{
	if (isPass == 0)
	{
		//---SITEC合格
		BINC_CTR_OUT_0;
	}
	else
	{
		//---SITEC不合格
		BINC_CTR_OUT_1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_BINDPass(UINT8_T isPass)
{
	if (isPass == 0)
	{
		//---SITED合格
		BIND_CTR_OUT_0;
	}
	else
	{
		//---SITED不合格
		BIND_CTR_OUT_1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Key_GenSelInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(GEN_SEL0_PORT, 1);
	GPIOTask_Clock(GEN_SEL1_PORT, 1);
	GPIOTask_Clock(GEN_SEL2_PORT, 1);
	//GPIOTask_Clock(GEN_SEL3_PORT, 1);
	//GPIOTask_Clock(GEN_SEL4_PORT, 1);
	//GPIOTask_Clock(GEN_SEL5_PORT, 1);
	//GPIOTask_Clock(GEN_SEL6_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode  =LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---中速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

	//---GEN_SEL0_BIT的初始化
	GPIO_InitStruct.Pin = GEN_SEL0_BIT;
	LL_GPIO_Init(GEN_SEL0_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(GEN_SEL0_PORT, GEN_SEL0_BIT);

	//---GEN_SEL1_BIT的初始化
	GPIO_InitStruct.Pin = GEN_SEL1_BIT;
	LL_GPIO_Init(GEN_SEL1_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(GEN_SEL1_PORT, GEN_SEL1_BIT);

	//---GEN_SEL2_BIT的初始化
	GPIO_InitStruct.Pin = GEN_SEL2_BIT;
	LL_GPIO_Init(GEN_SEL2_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(GEN_SEL2_PORT, GEN_SEL2_BIT);
}

//===数控信号源的按键扫描的新值
VLTUINT8_T g_KeyGenSelNowVal = 0xFF;
//===数控信号源的按键扫描的旧值
VLTUINT8_T g_KeyGenSelOldVal = 0xFF;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：SEL0---FT_RLY18。SEL2---FT_RLY22
//////////////////////////////////////////////////////////////////////////////
UINT8_T Key_GenSelScan(void)
{
	UINT8_T _return = 0x04;
	//---判断SEL0
	if (GEN_SEL2_STATE==0)
	{
		g_KeyGenSelNowVal &= 0xFE;
		_return&=0x02;
	}
	else
	{
		g_KeyGenSelNowVal |= 0x01;
		_return &= 0x03;
		_return |= 0x01;
	}
	////---判断SEL1
	//if (GEN_SEL1_STATE==0)
	//{
	//	g_KeyGenSelNowVal &= 0xFE;
	//}
	//else
	//{
	//	g_KeyGenSelNowVal |= 0x02;
	//}
	//---判断SEL2
	if (GEN_SEL0_STATE == 0)
	{
		g_KeyGenSelNowVal &= 0xFB;
		_return&=0x01;
	}
	else
	{
		g_KeyGenSelNowVal |= 0x04;
		_return &= 0x03;
		_return |= 0x02;
	}

	if (g_KeyGenSelNowVal!=g_KeyGenSelOldVal)
	{
		g_KeyGenSelOldVal = g_KeyGenSelNowVal;
	}
	else
	{
		_return = 0x04;
	}
	return _return ;
}