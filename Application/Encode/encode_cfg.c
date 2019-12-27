#include"encode_cfg.h"

//===存放编码数据的字节数组
UINT8_T	g_GenCodeByteCycleCode[GEN_CODE_BYTE_OF_CYCLE_COUNT];
//===存储 #1 2262编码的位数组指针
UINT8_T *pGenCodeByteCycleCode;
//===字节序计数器	
VLTUINT16_T g_GenCodeByteIndex = 0;

//===开启编码的通道数据1
VLTUINT8_T g_GenCodeChannelMask1 = 0x0F;

//===开启编码的通道数据1
VLTUINT8_T g_GenCodeChannelMask2 = 0x00;

//===通道1供给 433.92MHz，干扰板提供433.92镜像
EnCode_HandlerType	g_GenCodeCH1[GEN_CODE_FRAME_COUNT] = {
															{   16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															},
															{   16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															},
															{	16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															}
														};
//===通道2数据结构，供给315MHz
EnCode_HandlerType	g_GenCodeCH2[GEN_CODE_FRAME_COUNT] = {
															{   16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															},
															{   16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															},
															{	16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															}
														};
//===通道3数据结构，供给230.4MHz
EnCode_HandlerType	g_GenCodeCH3[GEN_CODE_FRAME_COUNT] = {
															{   16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															},
															{   16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															},
															{	16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															}
														};
//===通道4数据结构，433.92MHz---大信号源
EnCode_HandlerType	g_GenCodeCH4[GEN_CODE_FRAME_COUNT] = {
															{   16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															},
															{   16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															},
															{	16,
																1,
																{2,2,2,2,2,2,2,2,2,2,2,2}
															}
														};

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void EnCode_GPIO_Init(void)
{
	//---端口的时钟使能
	GPIOTask_Clock(ENCODE_CH1_PORT, 1);
	GPIOTask_Clock(ENCODE_CH2_PORT, 1);
	GPIOTask_Clock(ENCODE_CH3_PORT, 1);
	GPIOTask_Clock(ENCODE_CH4_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;					//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;			//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;						//---下拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

	//---初始化编码器的通道1
	GPIO_InitStruct.Pin = ENCODE_CH1_BIT;
	LL_GPIO_Init(ENCODE_CH1_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(ENCODE_CH1_PORT,ENCODE_CH1_BIT);

	//---初始化编码器的通道2
	GPIO_InitStruct.Pin = ENCODE_CH2_BIT;
	LL_GPIO_Init(ENCODE_CH2_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(ENCODE_CH2_PORT, ENCODE_CH2_BIT);

	//---初始化编码器的通道3
	GPIO_InitStruct.Pin = ENCODE_CH3_BIT;
	LL_GPIO_Init(ENCODE_CH3_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(ENCODE_CH3_PORT, ENCODE_CH3_BIT);

	//---初始化编码器的通道4
	GPIO_InitStruct.Pin = ENCODE_CH4_BIT;
	LL_GPIO_Init(ENCODE_CH4_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(ENCODE_CH4_PORT, ENCODE_CH4_BIT);
}


///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void EnCode_Timer_Init(void)
{
	//---使能定时器时钟
	TimerTask_Clock(ENCODE_USE_TIMER, 1);

	//---定时器的结构体
	LL_TIM_InitTypeDef TIM_InitStruct = { 0 };

	//---设置定时器为缺省设置
	LL_TIM_DeInit(ENCODE_USE_TIMER);

	//---自动重载值
	TIM_InitStruct.Autoreload = ENCODE_TIMER_RELOAD;

	//---预分频的值---计算计数器的时钟频率---1MHz
	TIM_InitStruct.Prescaler = ENCODE_TIMER_ARR;//71
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;

	//---重复计数的次数
	TIM_InitStruct.RepetitionCounter = 0;

	//---初始化定时器
	LL_TIM_Init(ENCODE_USE_TIMER, &TIM_InitStruct);

	//---不使能自动重载
	LL_TIM_DisableARRPreload(ENCODE_USE_TIMER);

	//--定时器时钟使用内部时钟
	LL_TIM_SetClockSource(ENCODE_USE_TIMER, LL_TIM_CLOCKSOURCE_INTERNAL);

	//---主模式选择复位
	LL_TIM_SetTriggerOutput(ENCODE_USE_TIMER, LL_TIM_TRGO_RESET);

	//---不使能主从模式
	LL_TIM_DisableMasterSlaveMode(ENCODE_USE_TIMER);
	
	
	//---定时器中断等级
	NVIC_SetPriority(ENCODE_USE_TIMER_IRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

	//---使能定时器中断
	NVIC_EnableIRQ(ENCODE_USE_TIMER_IRQ);

	//---使能更新中断
	LL_TIM_EnableIT_UPDATE(ENCODE_USE_TIMER);

	//---使能定时器
	LL_TIM_EnableCounter(ENCODE_USE_TIMER);


}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： 
//////功	   能： 
//////输入参数: 
//////输出参数: 
//////说	   明： 产生1KHz的方波信号的位填充
//////////////////////////////////////////////////////////////////////////////
void EnCode_SquareWave_1KHzBit( UINT8_T *pbyteCycleCode, UINT8_T codeIndex, UINT8_T mask )
{
	*( pbyteCycleCode + codeIndex * 8 + 0 ) &= ( ~mask );
	*( pbyteCycleCode + codeIndex * 8 + 1 ) |= mask;
	*( pbyteCycleCode + codeIndex * 8 + 2 ) &= ( ~mask );
	*( pbyteCycleCode + codeIndex * 8 + 3 ) |= mask;
	*( pbyteCycleCode + codeIndex * 8 + 4 ) &= ( ~mask );
	*( pbyteCycleCode + codeIndex * 8 + 5 ) |= mask;
	*( pbyteCycleCode + codeIndex * 8 + 6 ) &= ( ~mask );
	*( pbyteCycleCode + codeIndex * 8 + 7 ) |= mask;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： 
//////功	   能： 
//////输入参数: 
//////输出参数: 
//////说	   明： 产生500Hz的方波信号的位填充
//////////////////////////////////////////////////////////////////////////////
void EnCode_SquareWave_500HzBit( UINT8_T *pbyteCycleCode, UINT8_T codeIndex, UINT8_T mask )
{
	*( pbyteCycleCode + codeIndex * 8 + 0 ) &= ( ~mask );
	*( pbyteCycleCode + codeIndex * 8 + 1 ) &= ( ~mask );
	*( pbyteCycleCode + codeIndex * 8 + 2 ) |= mask;
	*( pbyteCycleCode + codeIndex * 8 + 3 ) |= mask;
	*( pbyteCycleCode + codeIndex * 8 + 4 ) &= ( ~mask );
	*( pbyteCycleCode + codeIndex * 8 + 5 ) &= ( ~mask );
	*( pbyteCycleCode + codeIndex * 8 + 6 ) |= mask;
	*( pbyteCycleCode + codeIndex * 8 + 7 ) |= mask;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： 
//////功	   能： 
//////输入参数: 
//////输出参数: 
//////说	   明： 产生方波信号的初始化
//////////////////////////////////////////////////////////////////////////////
void EnCode_SquareWave_Init( UINT8_T mode, UINT8_T* pbyteCycleCode, UINT8_T codeLength, UINT8_T mask )
{
	volatile UINT8_T i = 0;
	//---填充方波帧Bit值
	for (i = 0;i < codeLength;i++)
	{
		if (mode == 1)
		{
			EnCode_SquareWave_1KHzBit( pbyteCycleCode, i, mask );
		}
		else
		{
			EnCode_SquareWave_500HzBit( pbyteCycleCode, i, mask );
		}
	}
	//---填充方波帧停止位
	for (i = codeLength * 8;i < GEN_CODE_FRAME_LENGTH * 8;i++)
	{
		*( pbyteCycleCode + codeLength * 8 + i ) &= ( ~mask );
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： 
//////功	   能： 
//////输入参数: 
//////输出参数: 
//////说	   明： 产生2262信号的位填充
//////////////////////////////////////////////////////////////////////////////
void EnCode_2262Wave_Bit( UINT8_T* pGetCode, UINT8_T* pPutBitCode, UINT8_T codeIndex, UINT8_T mask )
{
	switch (*( pGetCode + codeIndex ))
	{
		case 0:	// 0码
			*( pPutBitCode + codeIndex * 8 + 0 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 1 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 2 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 3 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 4 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 5 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 6 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 7 ) &= ( ~mask );
			break;
		case 1:	// 1码
			*( pPutBitCode + codeIndex * 8 + 0 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 1 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 2 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 3 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 4 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 5 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 6 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 7 ) &= ( ~mask );
			break;
		case 2:	// 悬空
			*( pPutBitCode + codeIndex * 8 + 0 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 1 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 2 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 3 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 4 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 5 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 6 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 7 ) &= ( ~mask );
			break;
		case 3:
			*( pPutBitCode + codeIndex * 8 + 0 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 1 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 2 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 3 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 4 ) |= mask;
			*( pPutBitCode + codeIndex * 8 + 5 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 6 ) &= ( ~mask );
			*( pPutBitCode + codeIndex * 8 + 7 ) &= ( ~mask );
			break;
		default:
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： 
//////功	   能： 
//////输入参数: 
//////输出参数: 
//////说	   明： 产生2262信号的初始化
//////////////////////////////////////////////////////////////////////////////
void EnCode_2262Wave_Init( UINT8_T* pGetCode, UINT8_T* pbyteCycleCode, UINT8_T codeLength, UINT8_T mask )
{
	volatile UINT8_T	i;
	//---填充2262码Bit值
	for (i = 0;i < codeLength;i++)
	{
		EnCode_2262Wave_Bit( pGetCode, pbyteCycleCode, i, mask );
	}
	//---填充2262帧位值
	*( pbyteCycleCode + codeLength * 8 ) |= mask;
	//---填充2262停止位
	for (i = codeLength * 8;i < GEN_CODE_FRAME_LENGTH * 8;i++)
	{
		*( pbyteCycleCode + codeLength * 8 + i ) &= ( ~mask );
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void Encode_Init( void )
{
	UINT8_T frameIndex=0;

	//---通道的初始化
	EnCode_GPIO_Init();

	//---定时器的初始化
	EnCode_Timer_Init();

	//---位数组的填充
	pGenCodeByteCycleCode = g_GenCodeByteCycleCode;
	for (frameIndex = 0;frameIndex < GEN_CODE_FRAME_COUNT;frameIndex++)
	{
		if (g_GenCodeCH1[frameIndex].codeMode == 0)
		{
			EnCode_2262Wave_Init( g_GenCodeCH1[frameIndex].codeVal, pGenCodeByteCycleCode, g_GenCodeCH1[frameIndex].codeLength, ENCODE_CH1_INDEX );
		}
		else
		{
			EnCode_SquareWave_Init( g_GenCodeCH1[frameIndex].codeMode, pGenCodeByteCycleCode, g_GenCodeCH1[frameIndex].codeLength, ENCODE_CH1_INDEX );
		}
		pGenCodeByteCycleCode += GEN_CODE_FRAME_LENGTH * 8;
	}
	//---位数组的填充
	pGenCodeByteCycleCode = g_GenCodeByteCycleCode;
	for (frameIndex = 0;frameIndex < GEN_CODE_FRAME_COUNT;frameIndex++)
	{
		if (g_GenCodeCH2[frameIndex].codeMode == 0)
		{
			EnCode_2262Wave_Init( g_GenCodeCH2[frameIndex].codeVal, pGenCodeByteCycleCode, g_GenCodeCH2[frameIndex].codeLength, ENCODE_CH2_INDEX );
		}
		else
		{
			EnCode_SquareWave_Init( g_GenCodeCH2[frameIndex].codeMode, pGenCodeByteCycleCode, g_GenCodeCH2[frameIndex].codeLength, ENCODE_CH2_INDEX );
		}
		pGenCodeByteCycleCode += GEN_CODE_FRAME_LENGTH * 8;
	}
	//---位数组的填充
	pGenCodeByteCycleCode = g_GenCodeByteCycleCode;
	for (frameIndex = 0;frameIndex < GEN_CODE_FRAME_COUNT;frameIndex++)
	{
		if (g_GenCodeCH3[frameIndex].codeMode == 0)
		{
			EnCode_2262Wave_Init( g_GenCodeCH3[frameIndex].codeVal, pGenCodeByteCycleCode, g_GenCodeCH3[frameIndex].codeLength, ENCODE_CH3_INDEX );
		}
		else
		{
			EnCode_SquareWave_Init( g_GenCodeCH3[frameIndex].codeMode, pGenCodeByteCycleCode, g_GenCodeCH3[frameIndex].codeLength, ENCODE_CH3_INDEX );
		}
		pGenCodeByteCycleCode += GEN_CODE_FRAME_LENGTH * 8;
	}
	//---位数组的填充
	pGenCodeByteCycleCode = g_GenCodeByteCycleCode;
	for (frameIndex = 0;frameIndex < GEN_CODE_FRAME_COUNT;frameIndex++)
	{
		if (g_GenCodeCH4[frameIndex].codeMode == 0)
		{
			EnCode_2262Wave_Init( g_GenCodeCH4[frameIndex].codeVal, pGenCodeByteCycleCode, g_GenCodeCH4[frameIndex].codeLength, ENCODE_CH4_INDEX );
		}
		else
		{
			EnCode_SquareWave_Init( g_GenCodeCH4[frameIndex].codeMode, pGenCodeByteCycleCode, g_GenCodeCH4[frameIndex].codeLength, ENCODE_CH4_INDEX );
		}
		pGenCodeByteCycleCode += GEN_CODE_FRAME_LENGTH * 8;
	}
	pGenCodeByteCycleCode = g_GenCodeByteCycleCode;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： 
//////功	   能： 
//////输入参数: 
//////输出参数: 
//////说	   明： 
//////////////////////////////////////////////////////////////////////////////
void EnCode_Task( void )
{
	//ENCODE_BUS_OUT( 0x0F/*g_GenCodeByteCycleCode[g_GenCodeByteIndex++]*/);
	ENCODE_BUS_OUT(((g_GenCodeByteCycleCode[g_GenCodeByteIndex++]&g_GenCodeChannelMask1)|g_GenCodeChannelMask2));
	//---溢出复位
	if (g_GenCodeByteIndex >= GEN_CODE_BYTE_OF_CYCLE_COUNT)
	{
		g_GenCodeByteIndex = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： 
//////功	   能： 
//////输入参数: 
//////输出参数: 
//////说	   明： 
//////////////////////////////////////////////////////////////////////////////
void EnCode_IRQTask( void )
{
	//---更新中断
	if (LL_TIM_IsActiveFlag_UPDATE(ENCODE_USE_TIMER) != RESET)
	{
		EnCode_Task();
		//---清除中断标志位
		LL_TIM_ClearFlag_UPDATE(ENCODE_USE_TIMER);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 设置指定的编码通道是否开启编码功能
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void EnCode_SetChannel(UINT8_T chIndex,UINT8_T isEnCode,UINT8_T isHigh)
{
	switch (chIndex)
	{
		case 1:
			//---使能编码
			if (isEnCode==1)
			{
				g_GenCodeChannelMask1 |= 0x01;
			}
			else
			{
				g_GenCodeChannelMask1 &= 0x0E;
			}

			//---持续输出高电平
			if (isHigh==1)
			{
				g_GenCodeChannelMask1 |= 0x01;
				g_GenCodeChannelMask2 |= 0x01;
			}
			else 
			{
				g_GenCodeChannelMask2 &= 0x0E;
			}
			break;
		case 2:
			//---使能编码
			if (isEnCode == 1)
			{
				g_GenCodeChannelMask1 |= 0x02;
			}
			else
			{
				g_GenCodeChannelMask1 &= 0x0D;
			}

			//---持续输出高电平
			if (isHigh == 1)
			{
				g_GenCodeChannelMask1 |= 0x02;
				g_GenCodeChannelMask2 |= 0x02;
			}
			else
			{
				g_GenCodeChannelMask2 &= 0x0D;
			}
			break;
		case 3:
			//---使能编码
			if (isEnCode == 1)
			{
				g_GenCodeChannelMask1 |= 0x04;
			}
			else
			{
				g_GenCodeChannelMask1 &= 0x0B;
			}

			//---持续输出高电平
			if (isHigh == 1)
			{
				g_GenCodeChannelMask1 |= 0x04;
				g_GenCodeChannelMask2 |= 0x04;
			}
			else
			{
				g_GenCodeChannelMask2 &= 0x0B;
			}
			break;
		case 4:
			//---使能编码
			if (isEnCode == 1)
			{
				g_GenCodeChannelMask1 |= 0x08;
			}
			else
			{
				g_GenCodeChannelMask1 &= 0x07;
			}

			//---持续输出高电平
			if (isHigh == 1)
			{
				g_GenCodeChannelMask1 |= 0x08;
				g_GenCodeChannelMask2 |= 0x08;
			}
			else
			{
				g_GenCodeChannelMask2 &= 0x07;
			}
			break;
		default:
			//---恢复默认值
			g_GenCodeChannelMask1 = 0x0F;
			g_GenCodeChannelMask2 = 0X00;
			break;
	}
}