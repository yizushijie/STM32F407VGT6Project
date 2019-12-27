#include "decode_cfg.h"

//===当前计数器的值
VLTUINT32_T g_DecodeICPCount[DECODE_SITE_COUNT] = { 0 };
//===计算高电批的脉冲宽度
VLTUINT32_T g_DecodeHighPulseWidth[DECODE_SITE_COUNT] = { 0 };
//===计算低电批的脉冲宽度
VLTUINT32_T g_DecodeLowPulseWidth[DECODE_SITE_COUNT] = { 0 };
//===记录当前的通道捕捉模式  1--- 下降沿 2---上升沿
VLTUINT8_T g_DecodeICPMode[DECODE_SITE_COUNT] = { 0 };
//===解码步序
VLTUINT8_T g_DecodeNewStep[DECODE_SITE_COUNT] = { 0 };
VLTUINT8_T g_DecodeOldStep[DECODE_SITE_COUNT] = { 0 };
//===记录当前的通道捕捉事件  1--- 发生捕捉 2---未发生捕捉
VLTUINT8_T g_DecodeICPEvent[DECODE_SITE_COUNT] = { 0 };
//===激活的SITE数
UINT8_T    g_DecodeActivateSite[DECODE_SITE_COUNT] = { 0 };
//===记录当前上升沿计数值
VLTUINT32_T  g_DecodeRiseCount[DECODE_SITE_COUNT] = { 0 };
//===记录当前下降沿计数值
VLTUINT32_T  g_DecodeFallCount[DECODE_SITE_COUNT] = { 0 };
//===记录当前电平的宽度
VLTUINT32_T  g_DecodeTickCount[DECODE_SITE_COUNT] = { 0 };
//===记录当前电平的宽度
UINT32_T     g_DecodePulseTick[DECODE_SITE_COUNT] = { 0 };
//===解码位序
UINT16_T     g_DecodeBitIndex[DECODE_SITE_COUNT] = { 0 };
//===解码波形模式，支持连续方波，帧停顿方波，PT2262
UINT8_T		 g_DecodeWaveMode[DECODE_SITE_COUNT] = { 0 };
//===帧停顿之后,第几个脉冲之后开始解码
UINT16_T	 g_DecodePulseIndex[DECODE_SITE_COUNT] = { 0 };
//===帧合格计数器，必须连续的
UINT8_T		 g_DecodeFrameCount[DECODE_SITE_COUNT] = { 0 };
//===PT2262的解码结果
UINT32_T     g_DecodeByteNewVal[DECODE_SITE_COUNT] = { 0 };
UINT32_T     g_DecodeByteOldVal[DECODE_SITE_COUNT] = { 0 };
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Decode_DecodeLEDInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(DECA_LED_PORT, 1);
	GPIOTask_Clock(DECB_LED_PORT, 1);
	GPIOTask_Clock(DECC_LED_PORT, 1);
	GPIOTask_Clock(DECD_LED_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---中速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;			//---输出模式---推完输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
	#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
	#endif

	//---DECA_SN_BIT的初始化
	GPIO_InitStruct.Pin = DECA_LED_BIT;
	LL_GPIO_Init(DECA_LED_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECA_LED_PORT, DECA_LED_BIT);

	//---DECB_SN_BIT的初始化
	GPIO_InitStruct.Pin = DECB_LED_BIT;
	LL_GPIO_Init(DECB_LED_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECB_LED_PORT, DECB_LED_BIT);

	//---DECC_SN_BIT的初始化
	GPIO_InitStruct.Pin = DECC_LED_BIT;
	LL_GPIO_Init(DECC_LED_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECC_LED_PORT, DECC_LED_BIT);

	//---DECD_SN_BIT的初始化
	GPIO_InitStruct.Pin = DECD_LED_BIT;
	LL_GPIO_Init(DECD_LED_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECD_LED_PORT, DECD_LED_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Decode_DecodeRSTInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(DECA_RST_PORT, 1);
	GPIOTask_Clock(DECB_RST_PORT, 1);
	GPIOTask_Clock(DECC_RST_PORT, 1);
	GPIOTask_Clock(DECD_RST_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---中速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
	#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
	#endif

	//---DECA_RST_BIT的初始化
	GPIO_InitStruct.Pin = DECA_RST_BIT;
	LL_GPIO_Init(DECA_RST_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(DECA_RST_PORT, DECA_RST_BIT);

	//---DECB_RST_BIT的初始化
	GPIO_InitStruct.Pin = DECB_RST_BIT;
	LL_GPIO_Init(DECB_RST_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(DECB_RST_PORT, DECB_RST_BIT);

	//---DECC_RST_BIT的初始化
	GPIO_InitStruct.Pin = DECC_RST_BIT;
	LL_GPIO_Init(DECC_RST_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(DECC_RST_PORT, DECC_RST_BIT);

	//---DECD_RST_BIT的初始化
	GPIO_InitStruct.Pin = DECD_RST_BIT;
	LL_GPIO_Init(DECD_RST_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(DECD_RST_PORT, DECD_RST_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Decode_DecodeCHInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(DECA_CH_PORT, 1);
	GPIOTask_Clock(DECB_CH_PORT, 1);
	GPIOTask_Clock(DECC_CH_PORT, 1);
	GPIOTask_Clock(DECD_CH_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;			//---GPIO的速度---高速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;						//---下拉
	#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
	#endif

	//---DECA_CH_BIT的初始化
	GPIO_InitStruct.Pin = DECA_CH_BIT;
	LL_GPIO_Init(DECA_CH_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECA_CH_PORT, DECA_CH_BIT);

	//---DECB_CH_BIT的初始化
	GPIO_InitStruct.Pin = DECB_CH_BIT;
	LL_GPIO_Init(DECB_CH_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECB_CH_PORT, DECB_CH_BIT);

	//---DECC_CH_BIT的初始化
	GPIO_InitStruct.Pin = DECC_CH_BIT;
	LL_GPIO_Init(DECC_CH_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECC_CH_PORT, DECC_CH_BIT);

	//---DECD_CH_BIT的初始化
	GPIO_InitStruct.Pin = DECD_CH_BIT;
	LL_GPIO_Init(DECD_CH_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECD_CH_PORT, DECD_CH_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 解码成功标志位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Decode_DecodePassInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(DECA_PASS_CTR_PORT, 1);
	GPIOTask_Clock(DECB_PASS_CTR_PORT, 1);
	GPIOTask_Clock(DECC_PASS_CTR_PORT, 1);
	GPIOTask_Clock(DECD_PASS_CTR_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---中速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
	#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
	#endif

	//---DECA_CH_BIT的初始化
	GPIO_InitStruct.Pin = DECA_PASS_CTR_BIT;
	LL_GPIO_Init(DECA_PASS_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(DECA_PASS_CTR_PORT, DECA_PASS_CTR_BIT);

	//---DECB_CH_BIT的初始化
	GPIO_InitStruct.Pin = DECB_PASS_CTR_BIT;
	LL_GPIO_Init(DECB_PASS_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(DECB_PASS_CTR_PORT, DECB_PASS_CTR_BIT);

	//---DECC_CH_BIT的初始化
	GPIO_InitStruct.Pin = DECC_PASS_CTR_BIT;
	LL_GPIO_Init(DECC_PASS_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(DECC_PASS_CTR_PORT, DECC_PASS_CTR_BIT);

	//---DECD_CH_BIT的初始化
	GPIO_InitStruct.Pin = DECD_PASS_CTR_BIT;
	LL_GPIO_Init(DECD_PASS_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(DECD_PASS_CTR_PORT, DECD_PASS_CTR_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void Decode_DecodeWaveInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(DECA_WAVE_CTR_PORT, 1);
	GPIOTask_Clock(DECB_WAVE_CTR_PORT, 1);
	GPIOTask_Clock(DECC_WAVE_CTR_PORT, 1);
	GPIOTask_Clock(DECD_WAVE_CTR_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---中速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;			//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

	//---DECA_WAVE_BIT的初始化
	GPIO_InitStruct.Pin = DECA_WAVE_CTR_BIT;
	LL_GPIO_Init(DECA_WAVE_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECA_WAVE_CTR_PORT, DECA_WAVE_CTR_BIT);

	//---DECB_WAVE_BIT的初始化
	GPIO_InitStruct.Pin = DECB_WAVE_CTR_BIT;
	LL_GPIO_Init(DECB_WAVE_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECB_WAVE_CTR_PORT, DECB_WAVE_CTR_BIT);

	//---DECC_WAVE_BIT的初始化
	GPIO_InitStruct.Pin = DECC_WAVE_CTR_BIT;
	LL_GPIO_Init(DECC_WAVE_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECC_WAVE_CTR_PORT, DECC_WAVE_CTR_BIT);

	//---DECD_WAVE_BIT的初始化
	GPIO_InitStruct.Pin = DECD_WAVE_CTR_BIT;
	LL_GPIO_Init(DECD_WAVE_CTR_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(DECD_WAVE_CTR_PORT, DECD_WAVE_CTR_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：解码器使用的定时器初始化
//////////////////////////////////////////////////////////////////////////////
void Decode_DecodeTimerInit(void)
{
	//---使能定时器时钟
	TimerTask_Clock(DECODE_USE_TIM, 1);

	//---定时器的结构体
	LL_TIM_InitTypeDef TIM_InitStruct = { 0 };

	//---输入捕捉通道的初始化
	LL_TIM_IC_InitTypeDef TIM_ICInitStruct = { 0 };

	//---设置定时器为缺省设置
	LL_TIM_DeInit(DECODE_USE_TIM);

	//---自动重载值
	TIM_InitStruct.Autoreload = DECODE_TIM_RELOAD;

	//---预分频的值---计算计数器的时钟频率---1MHz
	TIM_InitStruct.Prescaler = DECODE_TIM_ARR;//71
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;

	//---重复计数的次数
	TIM_InitStruct.RepetitionCounter = 0;

	//---初始化定时器
	LL_TIM_Init(DECODE_USE_TIM, &TIM_InitStruct);

	//---不使能自动重载
	LL_TIM_DisableARRPreload(DECODE_USE_TIM);

	//--定时器时钟使用内部时钟
	LL_TIM_SetClockSource(DECODE_USE_TIM, LL_TIM_CLOCKSOURCE_INTERNAL);

	//---主模式选择复位
	LL_TIM_SetTriggerOutput(DECODE_USE_TIM, LL_TIM_TRGO_RESET);

	//---不使能主从模式
	LL_TIM_DisableMasterSlaveMode(DECODE_USE_TIM);

	//---直连模式
	TIM_ICInitStruct.ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;

	//---时钟不分频
	TIM_ICInitStruct.ICPrescaler = LL_TIM_ICPSC_DIV1;

	//---触发模式---上升沿触发
	TIM_ICInitStruct.ICPolarity = LL_TIM_IC_POLARITY_RISING;

	//---触发滤波---8
	TIM_ICInitStruct.ICFilter = LL_TIM_IC_FILTER_FDIV1; //LL_TIM_IC_FILTER_FDIV1_N8;

	//---配置输入捕捉通道1
	LL_TIM_IC_Init(DECODE_USE_TIM, LL_TIM_CHANNEL_CH1, &TIM_ICInitStruct);

	//---配置输入捕捉通道2
	LL_TIM_IC_Init(DECODE_USE_TIM, LL_TIM_CHANNEL_CH2, &TIM_ICInitStruct);

	//---配置输入捕捉通道3
	LL_TIM_IC_Init(DECODE_USE_TIM, LL_TIM_CHANNEL_CH3, &TIM_ICInitStruct);

	//---配置输入捕捉通道4
	LL_TIM_IC_Init(DECODE_USE_TIM, LL_TIM_CHANNEL_CH4, &TIM_ICInitStruct);

	//---定时器中断等级
	NVIC_SetPriority(DECODE_USE_TIM_IRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

	//---使能定时器中断
	//NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(DECODE_USE_TIM_IRQ);
	
	//---清零标志位
	LL_TIM_ClearFlag(DECODE_USE_TIM);

	//---使能输入捕捉通道1的中断
	LL_TIM_EnableIT_CC1(DECODE_USE_TIM);

	//---使能输入捕捉通道2的中断
	LL_TIM_EnableIT_CC2(DECODE_USE_TIM);

	//---使能输入捕捉通道3的中断
	LL_TIM_EnableIT_CC3(DECODE_USE_TIM);

	//---使能输入捕捉通道4的中断
	LL_TIM_EnableIT_CC4(DECODE_USE_TIM);

	//---使能更新中断
	//LL_TIM_EnableIT_UPDATE(DECODE_USE_TIM);

	//---使能定时器
	LL_TIM_EnableCounter(DECODE_USE_TIM);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：解码器的初始化
//////////////////////////////////////////////////////////////////////////////
void Decode_Init(void)
{
	Decode_DecodeLEDInit();
	Decode_DecodeRSTInit();
	Decode_DecodeCHInit();
	Decode_DecodePassInit();
	Decode_DecodeWaveInit();
	Decode_DecodeTimerInit();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：解码器的初始化
//////////////////////////////////////////////////////////////////////////////
void  Decode_ActivateSites(UINT8_T activateSites)
{
	UINT8_T i = 0;
	for (i = 0; i < DECODE_SITE_COUNT; i++)
	{
		if (( activateSites & 0x01 ) != 0)
		{
			g_DecodeActivateSite[i] = 0x01;
		}
		else
		{
			g_DecodeActivateSite[i] = 0x00;
		}

		//---右移一位
		activateSites >>= 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 设置所有的SITE为不激活状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void  Decode_ClearActivateSites(void)
{
	//---关闭解码指示灯
	DEC_LED_OFF;

	//---置位解码失败
	DEC_PASS_FAIL;
	memset(g_DecodeActivateSite, 0, DECODE_SITE_COUNT);
	//---关闭实时解码
	Decode_STOP();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 清除所有的状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void  Decode_ClearState(void)
{
	int i = 0;
	//---清除解码步序
	//memset(decodeStep, 0, DECODE_SITE_COUNT);
	//---清零操作
	for (i = 0; i < DECODE_SITE_COUNT; i++)
	{
		g_DecodeICPCount[i]		= 0;
		g_DecodeHighPulseWidth[i] = 0;
		g_DecodeLowPulseWidth[i] = 0;
		g_DecodeICPMode[i] = 0;
		g_DecodeICPEvent[i] = 0;
		g_DecodeNewStep[i] = 0;
		g_DecodeOldStep[i] = 0;
		g_DecodeRiseCount[i] = 0;
		g_DecodeFallCount[i] = 0;
		g_DecodeTickCount[i] = 0;
		g_DecodePulseTick[i] = 0;
		g_DecodeWaveMode[i] = 0;
		g_DecodePulseIndex[i] = 0;
		g_DecodeFrameCount[i] = 0;
		g_DecodeByteNewVal[i] = 0;
		g_DecodeByteOldVal[i] = 0;
	}
	//---清除当前计数值
	//memset(decodeNowCount, 0, DECODE_SITE_COUNT);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：启动解码器
//////////////////////////////////////////////////////////////////////////////
void Decode_START(void)
{
	//---关闭实时解码灯
	DEC_LED_OFF;
	//---置位解码失败
	DEC_PASS_FAIL;

	//---关闭定时器
	LL_TIM_DisableCounter(DECODE_USE_TIM);

	//---清除中断标志位
	LL_TIM_ClearFlag_UPDATE(DECODE_USE_TIM);
	LL_TIM_ClearFlag_CC1(DECODE_USE_TIM);
	LL_TIM_ClearFlag_CC2(DECODE_USE_TIM);
	LL_TIM_ClearFlag_CC3(DECODE_USE_TIM);
	LL_TIM_ClearFlag_CC4(DECODE_USE_TIM);

	//---清零标志位
	//LL_TIM_ClearFlag(DEC_USE_TIM);
	//---清零计数器
	LL_TIM_SetCounter(DECODE_USE_TIM, 0);

	//---使能定时器
	LL_TIM_EnableCounter(DECODE_USE_TIM);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：关闭解码器
//////////////////////////////////////////////////////////////////////////////
void Decode_STOP(void)
{
	//---不使能定时器
	LL_TIM_DisableCounter(DECODE_USE_TIM);
	DEC_LED_OFF;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 读取解码的RST信号
//////////////////////////////////////////////////////////////////////////////
UINT8_T Decode_GetRST(void)
{
	UINT8_T _return = 0;

	//---检测SOT
	if (DECA_RST_STATE == 0)
	{
		_return |= 0x01;
	}
	if (DECB_RST_STATE == 0)
	{
		_return |= 0x02;
	}
	if (DECC_RST_STATE == 0)
	{
		_return |= 0x04;
	}
	if (DECD_RST_STATE == 0)
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
//////说		明： 扫描解码RST信号
//////////////////////////////////////////////////////////////////////////////
UINT8_T Decode_ScanRST(void)
{
	//---读取RST的状态
	UINT8_T _return = Decode_GetRST();

	//---检查是否有解码信号
	if (_return != 0)
	{
		#ifndef USE_FT_CP_TEST

		//---延时等待，调试模式下用于软件消抖同时用于等待其他SITE的触发信号的到来
		//DelayTask_ms(10);
		#endif

		//---读取RST的状态
		_return = Decode_GetRST();
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 解码通过的结果
//////////////////////////////////////////////////////////////////////////////
void Decode_DecodePass(UINT8_T channel, UINT8_T isPass)
{
	switch (channel)
	{
		case 0:
			if (isPass)
			{
				DECA_PASS_OK;
			}
			else
			{
				DECA_PASS_FAIL;
			}
			break;
		case 1:
			if (isPass)
			{
				DECB_PASS_OK;
			}
			else
			{
				DECB_PASS_FAIL;
			}
			break;
		case 2:
			if (isPass)
			{
				DECC_PASS_OK;
			}
			else
			{
				DECC_PASS_FAIL;
			}
			break;
		case 3:
			if (isPass)
			{
				DECD_PASS_OK;
			}
			else
			{
				DECD_PASS_FAIL;
			}
			break;
		default:
			break;
	}
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 实时解码灯
//////////////////////////////////////////////////////////////////////////////
void Decode_DecodeLED(UINT8_T channel, UINT8_T isPass)
{
	switch (channel)
	{
		case 0:
			if (isPass)
			{
				DECA_LED_IS_ON;
			}
			else
			{
				DECA_LED_IS_OFF;
			}
			break;
		case 1:
			if (isPass)
			{
				DECB_LED_IS_ON;
			}
			else
			{
				DECB_LED_IS_OFF;
			}
			break;
		case 2:
			if (isPass)
			{
				DECC_LED_IS_ON;
			}
			else
			{
				DECC_LED_IS_OFF;
			}
			break;
		case 3:
			if (isPass)
			{
				DECD_LED_IS_ON;
			}
			else
			{
				DECD_LED_IS_OFF;
			}
			break;
		default:
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void Decode_CalcPulseWidth(UINT32_T index, UINT8_T site)
{
	//---触发极性的切换,下降沿触发
	if (LL_TIM_IC_GetPolarity(DECODE_USE_TIM, index) != LL_TIM_IC_POLARITY_RISING)
	{
		//---切换为上升沿
		LL_TIM_IC_SetPolarity(DECODE_USE_TIM, index, LL_TIM_IC_POLARITY_RISING);
		//---下降沿计数
		g_DecodeFallCount[site] = g_DecodeICPCount[site];
		//---判断查询过程中数据不一致，且溢出中断标志未发生
		if (g_DecodeFallCount[site]< g_DecodeRiseCount[site])
		{
			//---计算高电平的宽度
			g_DecodeHighPulseWidth[site] = DECODE_TIM_RELOAD  + g_DecodeFallCount[site] - g_DecodeRiseCount[site];

		}
		else 
		{
			//---计算高电平的宽度
			g_DecodeHighPulseWidth[site] = g_DecodeFallCount[site] - g_DecodeRiseCount[site];

		}
		//---捕捉到下降沿,计算高电平的脉冲宽度
		g_DecodeICPMode[site] = DECODE_LEVEL_HIGH;
	}
	else   //---上升沿触发
	{
		//---切换为下升沿
		LL_TIM_IC_SetPolarity(DECODE_USE_TIM, index, LL_TIM_IC_POLARITY_FALLING);
		//---上升沿计数
		g_DecodeRiseCount[site] = g_DecodeICPCount[site];
		//---判断查询过程中数据不一致，且溢出中断标志未发生
		if (g_DecodeRiseCount[site] < g_DecodeFallCount[site])
		{
			//---计算低电平的宽度
			g_DecodeLowPulseWidth[site] = DECODE_TIM_RELOAD  + g_DecodeRiseCount[site] - g_DecodeFallCount[site];
		}
		else
		{
			//---计算低电平的宽度
			g_DecodeLowPulseWidth[site] = g_DecodeRiseCount[site] - g_DecodeFallCount[site];
		}
		//---捕捉到上升沿,计算低电平的脉冲宽度
		g_DecodeICPMode[site] = DECODE_LEVEL_LOW;
	}
	//---获取当前电平的时间节点
	g_DecodeTickCount[site] = SysTickTask_GetTick();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 查询计算脉冲宽度
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T Decode_QueryCalcPulseWidth(UINT8_T site)
{
	UINT8_T _return = ERROR_1;
	UINT32_T icpChannel = 0;
	switch (site)
	{
		case 0:
			icpChannel = LL_TIM_CHANNEL_CH1;
			break;
		case 1:
			icpChannel = LL_TIM_CHANNEL_CH2;
			break;
		case 2:
			icpChannel = LL_TIM_CHANNEL_CH3;
			break;
		case 3:
			icpChannel = LL_TIM_CHANNEL_CH4;
			break;
		default:
			return _return;
	}
	if (g_DecodeICPEvent[site]!=0)
	{
		//---清除标志位
		g_DecodeICPEvent[site] = 0;
		//---计算脉冲宽度
		Decode_CalcPulseWidth(icpChannel, site);
		_return = OK_0;
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
void Decode_CalcPulseTick(UINT8_T site)
{
	//---获取当前时间节拍
	UINT32_T nowTime = SysTickTask_GetTick()+1;
	//---计算时间节拍
	if (nowTime<g_DecodeTickCount[site])
	{
		g_DecodePulseTick[site]= (0xFFFFFFFF - g_DecodeTickCount[site] + nowTime);
	}
	else
	{
		g_DecodePulseTick[site] = (nowTime - g_DecodeTickCount[site]);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：关闭解码器
//////////////////////////////////////////////////////////////////////////////
void Decode_IRQTask(void)
{
	//---输入捕捉1的捕捉中断
	if (LL_TIM_IsActiveFlag_CC1(DECODE_USE_TIM) != RESET)
	{
		//---保存输入捕捉的值
		g_DecodeICPCount[0] = LL_TIM_IC_GetCaptureCH1(DECODE_USE_TIM);
		//---记录发生捕捉
		g_DecodeICPEvent[0] += 1;
		////---计算脉冲宽度
		//Decode_CalcPulseWidth(LL_TIM_CHANNEL_CH1, 0);
		//---清除中断标志位
		LL_TIM_ClearFlag_CC1(DECODE_USE_TIM);
	}

	//---输入捕捉2的捕捉中断
	if (LL_TIM_IsActiveFlag_CC2(DECODE_USE_TIM) != RESET)
	{
		#if (DECODE_SITE_COUNT>1)
		//---保存输入捕捉的值
		g_DecodeICPCount[1] = LL_TIM_IC_GetCaptureCH2(DECODE_USE_TIM);
		//---记录发生捕捉
		g_DecodeICPEvent[1] += 1;
		#endif
		////---计算脉冲宽度
		//Decode_CalcPulseWidth(LL_TIM_CHANNEL_CH2, 1);
		//---清除中断标志位
		LL_TIM_ClearFlag_CC2(DECODE_USE_TIM);
	}

	//---输入捕捉3的捕捉中断
	if (LL_TIM_IsActiveFlag_CC3(DECODE_USE_TIM) != RESET)
	{
		#if (DECODE_SITE_COUNT>2)
		//---保存输入捕捉的值
		g_DecodeICPCount[2] = LL_TIM_IC_GetCaptureCH3(DECODE_USE_TIM);
		//---记录发生捕捉
		g_DecodeICPEvent[2] += 1;
		#endif
		////---计算脉冲宽度
		//Decode_CalcPulseWidth(LL_TIM_CHANNEL_CH3, 2);
		//---清除中断标志位
		LL_TIM_ClearFlag_CC3(DECODE_USE_TIM);
	}
	//---输入捕捉4的捕捉中断
	if (LL_TIM_IsActiveFlag_CC4(DECODE_USE_TIM) != RESET)
	{
		#if (DECODE_SITE_COUNT>3)
		//---保存输入捕捉的值
		g_DecodeICPCount[3] = LL_TIM_IC_GetCaptureCH4(DECODE_USE_TIM);
		//---记录发生捕捉
		g_DecodeICPEvent[3] += 1;
		#endif
		////---计算脉冲宽度
		//Decode_CalcPulseWidth(LL_TIM_CHANNEL_CH4, 3);
		//---清除中断标志位
		LL_TIM_ClearFlag_CC4(DECODE_USE_TIM);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 设置上升沿触发
//////////////////////////////////////////////////////////////////////////////
void Decode_SetPolarityRising(UINT8_T site)
{
	switch (site)
	{
		case 0:
			LL_TIM_IC_SetPolarity(DECODE_USE_TIM, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
			break;
		case 1:
			LL_TIM_IC_SetPolarity(DECODE_USE_TIM, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
			break;
		case 2:
			LL_TIM_IC_SetPolarity(DECODE_USE_TIM, LL_TIM_CHANNEL_CH3, LL_TIM_IC_POLARITY_RISING);
			break;
		case 3:
			LL_TIM_IC_SetPolarity(DECODE_USE_TIM, LL_TIM_CHANNEL_CH4, LL_TIM_IC_POLARITY_RISING);
			break;
		default:
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 设置下降沿触发
//////////////////////////////////////////////////////////////////////////////
void Decode_SetPolarityFalling(UINT8_T site)
{
	switch (site)
	{
		case 0:
			LL_TIM_IC_SetPolarity(DECODE_USE_TIM, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_FALLING);
			break;
		case 1:
			LL_TIM_IC_SetPolarity(DECODE_USE_TIM, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_FALLING);
			break;
		case 2:
			LL_TIM_IC_SetPolarity(DECODE_USE_TIM, LL_TIM_CHANNEL_CH3, LL_TIM_IC_POLARITY_FALLING);
			break;
		case 3:
			LL_TIM_IC_SetPolarity(DECODE_USE_TIM, LL_TIM_CHANNEL_CH4, LL_TIM_IC_POLARITY_FALLING);
			break;
		default:
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 将解码结果加入到解码值存储区
//////////////////////////////////////////////////////////////////////////////
void Decode_AddBit(UINT16_T channel, UINT16_T value)
{
	//---判断解码值
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 打印解码波形
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void  Decode_PrintfWave(UINT16_T channel, UINT16_T value)
{
	//---判断解码值
	if (value)
	{
		switch (channel)
		{
			case 0:
				DECA_WAVE_CTR_OUT_1;
				break;
			case 1:
				DECB_WAVE_CTR_OUT_1;
				break;
			case 2:
				DECC_WAVE_CTR_OUT_1;
				break;
			case 3:
				DECD_WAVE_CTR_OUT_1;
				break;
			default:
				DECA_WAVE_CTR_OUT_1;
				DECB_WAVE_CTR_OUT_1;
				DECC_WAVE_CTR_OUT_1;
				DECD_WAVE_CTR_OUT_1;
				break;
		}

	}
	else
	{
		switch (channel)
		{
			case 0:
				DECA_WAVE_CTR_OUT_0;
				break;
			case 1:
				DECB_WAVE_CTR_OUT_0;
				break;
			case 2:
				DECC_WAVE_CTR_OUT_0;
				break;
			case 3:
				DECD_WAVE_CTR_OUT_0;
				break;
			default:
				DECA_WAVE_CTR_OUT_0;
				DECB_WAVE_CTR_OUT_0;
				DECC_WAVE_CTR_OUT_0;
				DECD_WAVE_CTR_OUT_0;
				break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：查询式解码
//////输入参数:
//////输出参数:
//////说		明：使用查询式的方式实现解码，查询定时器的输入捕捉的脉冲宽度
//////////////////////////////////////////////////////////////////////////////
void Decode_Query(void)
{
	UINT8_T site = 0;
	UINT8_T isOpenLed = 0;
	//---对四路进行解码
	for (site = 0; site < DECODE_SITE_COUNT; site++)
	{
		if (g_DecodeActivateSite[site] == 1)
		{
			//---计算脉冲时间节拍
			Decode_CalcPulseTick(site);
			//---查询计算脉冲宽度,判断是否是有效事件
			Decode_QueryCalcPulseWidth(site);
			//---解码过程
			switch (g_DecodeNewStep[site])
			{
				case DECODE_STEP_0:				//---设置上升沿触发
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---解码步序1
					g_DecodeNewStep[site] = DECODE_STEP_1;
					//---初始化捕捉模式
					g_DecodeICPMode[site] = 0;
					//---上升沿计数
					g_DecodeRiseCount[site] = 0;
					//---下降沿计数
					g_DecodeFallCount[site] = 0;
					//---设置通道上升沿捕捉模式
					Decode_SetPolarityRising(site);
					break;
				case DECODE_STEP_1:				//---初始化
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---解码位序清0
					g_DecodeBitIndex[site] = 0;
					//---解码脉冲计数归零
					g_DecodePulseIndex[site] = 0;
					//---帧合格计数器
					g_DecodeFrameCount[site] = 0;
					//---解码结果
					g_DecodeByteNewVal[site] = 0;
					//---关闭解码实时灯
					Decode_DecodeLED(site, 0);
					//---置位解码指示灯
					Decode_DecodePass(site, 0);
					//---打印波形
					Decode_PrintfWave(site, 1);
					//---解码步序
					g_DecodeNewStep[site] = DECODE_STEP_2;
					//---更新计时器的的值
					g_DecodeTickCount[site] = SysTickTask_GetTick();
					break;
				//===================
				//===波形判断开始
				//===================
				case DECODE_STEP_2:				//---捕捉下降沿，验证高电平的脉冲宽度
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证低电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_HIGH)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;

						//---判断脉冲的宽度,判断是不是连续方波
						if (((g_DecodeHighPulseWidth[site] >= DECODE_PT2262_NARROW_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_PT2262_NARROW_WIDTH_MAX))
							||((g_DecodeHighPulseWidth[site] >= DECODE_SQUARE_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_SQUARE_WIDTH_MAX))
							||((g_DecodeHighPulseWidth[site] >= DECODE_FRAME_START_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_FRAME_START_WIDTH_MAX)))
						{
							//---可能是2262，也可能是方波
							g_DecodeWaveMode[site] = DECODE_WAVE_SQUARE;
							//---验证低电平
							g_DecodeNewStep[site] = DECODE_STEP_3;
						}
						else if ((g_DecodeHighPulseWidth[site] >= DECODE_PT2262_WIDE_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_PT2262_WIDE_WIDTH_MAX))
						{
							//---判断形是PT2262
							g_DecodeWaveMode[site] = DECODE_WAVE_PT2262;
							//---验证窄低电平
							g_DecodeNewStep[site] = DECODE_STEP_6;
						}
						else
						{
							//---高电平不符合连续方波的脉宽,解码步序1，复位操作
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 0);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_3:			//---捕捉上升沿，验证低电平的脉冲宽度
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证低电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_LOW)
					{
						//---判断脉冲的宽度,判断是不是连续方波
						if (((g_DecodeLowPulseWidth[site] >= DECODE_PT2262_NARROW_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_PT2262_NARROW_WIDTH_MAX))
							|| ((g_DecodeLowPulseWidth[site] >= DECODE_SQUARE_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_SQUARE_WIDTH_MAX))
							|| ((g_DecodeLowPulseWidth[site] >= DECODE_FRAME_START_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_FRAME_START_WIDTH_MAX)))
						{
							//---判断是方波
							g_DecodeWaveMode[site] = DECODE_WAVE_SQUARE;
							//---进入解方波流程
							g_DecodeNewStep[site] = DECODE_STEP_9;
							//---1Bit的合格数据
							g_DecodePulseIndex[site]++;
						}
						else if ((g_DecodeLowPulseWidth[site] >= DECODE_PT2262_WIDE_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_PT2262_WIDE_WIDTH_MAX))
						{
							//---判断PT2262
							g_DecodeWaveMode[site] = DECODE_WAVE_PT2262;
							//---验证高电平
							g_DecodeNewStep[site] = DECODE_STEP_4;
							//---解码合格一次
							g_DecodeBitIndex[site]++;
							//---收到0
							g_DecodeByteNewVal[site] += 0;
							//---左移一位
							g_DecodeByteNewVal[site] <<= 1;
						}
						else
						{
							//---高电平不符合连续方波的脉宽,解码步序1，复位操作
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 1);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				//===================
				//===波形判断结束
				//===================

				//===================
				//===PT2262解码开始
				//===================
				case DECODE_STEP_4:				//---捕捉下降沿，验证高电平的脉冲宽度		
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证高电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_HIGH)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断高电平
						if ((g_DecodeHighPulseWidth[site] >= DECODE_PT2262_NARROW_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_PT2262_NARROW_WIDTH_MAX))
						{
							//---验证宽低电平
							g_DecodeNewStep[site] = DECODE_STEP_5;
						}
						else if ((g_DecodeHighPulseWidth[site] >= DECODE_PT2262_WIDE_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_PT2262_WIDE_WIDTH_MAX))
						{
							//---验证高电平
							g_DecodeNewStep[site] = DECODE_STEP_6;
							//---显示的值是下一电平脉冲宽度的值
							Decode_PrintfWave(site, 0);
							//---解码合格一次
							g_DecodeBitIndex[site]++;
							//---收到1
							g_DecodeByteNewVal[site] += 1;
							//---左移一位
							g_DecodeByteNewVal[site] <<= 1;
							//---判断是否解码结束
							if (g_DecodeBitIndex[site] >= DECODE_PT2262_PASS_MAX_COUNT)
							{
								//---解码结束，验证窄低电平
								g_DecodeNewStep[site] = DECODE_STEP_6;
							}
						}
						else
						{
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 0);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_5:				//---宽低电平
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证低电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_LOW)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断低电平
						if ((g_DecodeLowPulseWidth[site] >= DECODE_PT2262_WIDE_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_PT2262_WIDE_WIDTH_MAX))
						{
							//---验证高电平
							g_DecodeNewStep[site] = DECODE_STEP_4;
							//---解码合格一次
							g_DecodeBitIndex[site]++;
							//---收到0
							g_DecodeByteNewVal[site] += 0;
							//---左移一位
							g_DecodeByteNewVal[site] <<= 1;
							//---判断是否解码结束
							if (g_DecodeBitIndex[site] >= DECODE_PT2262_PASS_MAX_COUNT)
							{
								//---解码结束，验证低电平
								g_DecodeNewStep[site] = DECODE_STEP_7;
							}
						}
						else 
						{
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 1);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_6:				//---窄低电平
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证低电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_LOW)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						if ((g_DecodeLowPulseWidth[site] >= DECODE_PT2262_NARROW_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_PT2262_NARROW_WIDTH_MAX))
						{
							//---验证高电平
							g_DecodeNewStep[site] = DECODE_STEP_4;
							//---判断是否解码结束
							if (g_DecodeBitIndex[site] >= DECODE_PT2262_PASS_MAX_COUNT)
							{
								//---解码结束，验证窄低电平
								g_DecodeNewStep[site] = DECODE_STEP_7;
							}
						}
						else
						{
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 1);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_7:					//PT2262，验证高电平窄脉冲，帧停顿前的
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证高电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_HIGH)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断高电平
						if ((g_DecodeHighPulseWidth[site] >= DECODE_PT2262_NARROW_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_PT2262_NARROW_WIDTH_MAX))
						{
							//---验证帧停顿的低电平
							g_DecodeNewStep[site] = DECODE_STEP_8;
						}
						else
						{
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 0);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_8:						//PT2262，验证低电平脉冲，帧停顿时间
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证低电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_LOW)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断低电平
						if ((g_DecodeLowPulseWidth[site] >= DECODE_PT2262_FRAME_PAUSE_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_PT2262_FRAME_PAUSE_MAX))
						{
							//---帧停顿验证成功，重新解码
							g_DecodeNewStep[site] = DECODE_STEP_2;
							//---清零解码合格BIT
							g_DecodeBitIndex[site] = 0;
							//---解码合格帧数
							g_DecodeFrameCount[site]++;
							//---判断解码合格帧数
							if (g_DecodeFrameCount[site] >= DECODE_PT2262_PASS_FRAME_MIN_COUNT)
							{
								//---判断两次解码的值是否相同
								if (g_DecodeByteOldVal[site] == g_DecodeByteNewVal[site])
								{
									isOpenLed = 1;
								}
								else
								{
									isOpenLed = 0;
								}
								//---解码成功，打开解码实时灯
								Decode_DecodeLED(site,  isOpenLed);
								//---解码成功，置位解码示灯
								Decode_DecodePass(site, isOpenLed);
								//---清零合格帧计数
								g_DecodeFrameCount[site] = 0;
								//---清零当前值
								g_DecodeByteNewVal[site] = 0;

							}
							else
							{
								//---保存第一次的值
								g_DecodeByteOldVal[site] = g_DecodeByteNewVal[site];
								//---清零当前值
								g_DecodeByteNewVal[site] = 0;
							}
						}
						else
						{
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 1);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				//===================
				//===PT2262解码结束
				//===================

				//===================
				//===加帧方波解码开始
				//===================
				case DECODE_STEP_9:					//---捕捉下降沿，验证高电平的脉冲宽度
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证高电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_HIGH)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断是不是开始计数的合格电平
						if (g_DecodePulseIndex[site]< DECODE_FRAME_PAUSE_COUNT_INDEX)
						{
							if ((g_DecodeHighPulseWidth[site] >= DECODE_FRAME_START_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_FRAME_START_WIDTH_MAX))
							{
								//---验证加帧方波的低电平
								g_DecodeNewStep[site] = DECODE_STEP_10;
							}
							else
							{
								//---高电平不符合条件
								g_DecodeNewStep[site] = DECODE_STEP_1;
							}
						}
						else
						{
							//---判断高电平
							if ((g_DecodeHighPulseWidth[site] >= DECODE_FRAME_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_FRAME_WIDTH_MAX))
							{
								//---验证加帧方波的低电平
								g_DecodeNewStep[site] = DECODE_STEP_10;
							}
							else
							{
								//---高电平不符合条件
								g_DecodeNewStep[site] = DECODE_STEP_1;
							}
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 0);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_10:				//---捕捉上升沿，验证低电平的脉冲宽度
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证低电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_LOW)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断是不是开始计数的合格电平
						if (g_DecodePulseIndex[site] < DECODE_FRAME_PAUSE_COUNT_INDEX)
						{
							if ((g_DecodeLowPulseWidth[site] >= DECODE_FRAME_START_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_FRAME_START_WIDTH_MAX))
							{
								//---验证加帧方波的低电平
								g_DecodeNewStep[site] = DECODE_STEP_9;
								//---个数增加
								g_DecodePulseIndex[site]++;
								g_DecodeBitIndex[site]=g_DecodePulseIndex[site];
							}
							else
							{
								//---低电平不符合条件
								g_DecodeNewStep[site] = DECODE_STEP_1;
							}
						}
						else
						{
							//---判断低电平
							if ((g_DecodeLowPulseWidth[site] >= DECODE_FRAME_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_FRAME_WIDTH_MAX))
							{
								//---验证加帧方波的高电平
								g_DecodeNewStep[site] = DECODE_STEP_9;
								//---解码器解出来的BIT数
								g_DecodeBitIndex[site]++;
								//---判断是否合格
								if (g_DecodeBitIndex[site]>= DECODE_FREAME_PASS_MAX_COUNT)
								{
									//---验证加帧方波的帧停顿
									g_DecodeNewStep[site] = DECODE_STEP_11;
								}
							}
							else
							{
								//---高电平不符合条件
								g_DecodeNewStep[site] = DECODE_STEP_1;
							}
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 1);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_11:						//---查找帧停顿前导脉冲，高电平
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证高电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_HIGH)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断高电平
						if ((g_DecodeHighPulseWidth[site] >= DECODE_FRAME_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_FRAME_WIDTH_MAX))
						{
							//---验证帧停顿前的低电平
							g_DecodeNewStep[site] = DECODE_STEP_12;
						}
						else
						{
							//---高电平不符合条件
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 0);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_12:				//---验证帧停顿时间,低电平
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证低电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_LOW)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断低电平
						if ((g_DecodeLowPulseWidth[site] >= DECODE_FRAME_PAUSE_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_FRAME_PAUSE_MAX))
						{
							//---帧停顿验证成功，重新解码
							g_DecodeNewStep[site] = DECODE_STEP_2;
							//---解码合格帧数
							g_DecodeFrameCount[site]++;
							//---清零解码合格BIT
							g_DecodeBitIndex[site] = 0;
							g_DecodePulseIndex[site]=0;
							//---判断解码连续合格的帧数
							if (g_DecodeFrameCount[site] >= DECODE_FRAME_PASS_FRAME_MIN_COUNT)
							{
								//---解码成功，打开解码实时灯
								Decode_DecodeLED(site, 1);
								//---解码成功，置位解码示灯
								Decode_DecodePass(site, 1);
								//---清零合格帧计数
								g_DecodeFrameCount[site] = 0;
							}
						}
						else if((g_DecodeLowPulseWidth[site] >= DECODE_SQUARE_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_SQUARE_WIDTH_MAX))
						{
							g_DecodeBitIndex[site]++;
							//---连续方波解码处理程序
							g_DecodeWaveMode[site] = DECODE_WAVE_SQUARE;
							//---解码连续方波
							g_DecodeNewStep[site] = DECODE_STEP_13;
						}
						else
						{
							//---低电平不符合要求，重新解码
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 1);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				//===================
				//===加帧方波解码结束
				//===================

				//===================
				//===连续方波解码开始
				//===================
				case DECODE_STEP_13:		//---捕捉下降沿，验证高电平的脉冲宽度
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证高电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_HIGH)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断高电平
						if ((g_DecodeHighPulseWidth[site] >= DECODE_SQUARE_WIDTH_MIN) && (g_DecodeHighPulseWidth[site] <= DECODE_SQUARE_WIDTH_MAX))
						{
							//---连续方波的低电平
							g_DecodeNewStep[site] = DECODE_STEP_14;
						}
						else
						{
							//---高电平不符合条件
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
						//---显示的值是下一电平脉冲宽度的值，打印输出波形
						Decode_PrintfWave(site, 0);
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				case DECODE_STEP_14:		//---捕捉上升沿，验证低电平的脉冲宽度
					g_DecodeOldStep[site] = g_DecodeNewStep[site];
					//---验证低电平
					if ((g_DecodeICPMode[site] & DECODE_LEVEL_MASK) == DECODE_LEVEL_LOW)
					{
						//---复位沿抓取记录值
						g_DecodeICPMode[site] = DECODE_LEVEL_NONE;
						//---判断高电平
						if ((g_DecodeLowPulseWidth[site] >= DECODE_SQUARE_WIDTH_MIN) && (g_DecodeLowPulseWidth[site] <= DECODE_SQUARE_WIDTH_MAX))
						{
							//---解码合格数增加
							g_DecodeBitIndex[site]++;
							//---判断解码是否合格
							if (g_DecodeBitIndex[site]>= DECODE_SQUARE_PASS_MAX_COUNT)
							{
								//---连续方波验证成功，重新解码
								g_DecodeNewStep[site] = DECODE_STEP_2;
								//---解码成功，打开解码实时灯
								Decode_DecodeLED(site, 1);
								//---解码成功，置位解码示灯
								Decode_DecodePass(site, 1);
								//---清零解码合格BIT数
								g_DecodeBitIndex[site] = 0;
							}
							else
							{
								//---连续方波的高电平
								g_DecodeNewStep[site] = DECODE_STEP_13;
							}
							//---显示的值是下一电平脉冲宽度的值，打印输出波形
							Decode_PrintfWave(site, 1);
						}
						else
						{
							//---高电平不符合条件
							g_DecodeNewStep[site] = DECODE_STEP_1;
						}
					}
					else
					{
						//---同电平维持时间太长处理,最长同电平保持时间是20ms
						if (g_DecodePulseTick[site] > DECODE_SAME_LEVEL_MAX_WIDTH)
						{
							//---同电平保持时间,解码操作归零
							g_DecodeNewStep[site] = DECODE_STEP_1;
							//---更新时间标签，避免长时间的进入这个状态
							g_DecodeTickCount[site] = SysTickTask_GetTick();
						}
					}
					break;
				//===================
				//===连续方波解码结束
				//===================
				default:
					g_DecodeNewStep[site] = DECODE_STEP_0;
					break;
			}
		}
	}
}
