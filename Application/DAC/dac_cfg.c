#include "dac_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DAC_GPIO_Init(UINT8_T ch)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(GPIOA, 1);
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	//---GPIO的初始化----
	//---DAC1---PA4
	//---DAC2---PA5
	if (ch == 3)
	{
		GPIO_InitStruct.Pin = LL_GPIO_PIN_4 | LL_GPIO_PIN_5;				//---对应的GPIO的引脚
	}
	else if (ch == 2)
	{
		GPIO_InitStruct.Pin =LL_GPIO_PIN_5;									//---对应的GPIO的引脚
	}
	else
	{
		GPIO_InitStruct.Pin = LL_GPIO_PIN_4;								//---对应的GPIO的引脚
	}
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;								//---配置状态为模拟输入引脚
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;								//---设置端口下拉使能
	//---GPIO的初始化
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数：
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DAC_Clock(UINT8_T isEnableid)
{
	//---配置DAC时钟
	if (isEnableid)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);
	}
	else
	{
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_DAC1);
	}

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数：
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DAC_DAC1_Init(UINT8_T isEnableBuffer)
{
	LL_DAC_InitTypeDef DAC_InitStruct = { 0 };
	//---使用软件触发
	DAC_InitStruct.TriggerSource = LL_DAC_TRIG_SOFTWARE;
	//---不使用波形发生
	DAC_InitStruct.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
	//---使能DAC输出缓存器
	if (isEnableBuffer == 1)
	{
		DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_ENABLE;
	}
	else
	{
		DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_DISABLE;
	}
	//---初始化DAC
	LL_DAC_Init(DAC, LL_DAC_CHANNEL_1, &DAC_InitStruct);
	//---使能DAC
	LL_DAC_Enable(DAC, LL_DAC_CHANNEL_1);
	//---设置数据输出格式,右对齐，12位数据
	LL_DAC_ConvertData12RightAligned(DAC, LL_DAC_CHANNEL_1, 0);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数：
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DAC_DAC2_Init(UINT8_T isEnableBuffer)
{
	LL_DAC_InitTypeDef DAC_InitStruct = { 0 };
	//---使用软件触发
	DAC_InitStruct.TriggerSource = LL_DAC_TRIG_SOFTWARE;
	//---不使用波形发生
	DAC_InitStruct.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
	//---使能DAC输出缓存器
	if (isEnableBuffer == 1)
	{
		DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_ENABLE;
	}
	else
	{
		DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_DISABLE;
	}
	//---初始化DAC
	LL_DAC_Init(DAC, LL_DAC_CHANNEL_2, &DAC_InitStruct);
	//---使能DAC
	LL_DAC_Enable(DAC, LL_DAC_CHANNEL_2);
	//---设置数据输出格式,右对齐，12位数据
	LL_DAC_ConvertData12RightAligned(DAC, LL_DAC_CHANNEL_2, 0);

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DAC_Channel_Init(UINT8_T ch, UINT8_T isEnableBuffer)
{
	UINT32_T dacChannale = 0;
	LL_DAC_InitTypeDef DAC_InitStruct = { 0 };
	//---使用软件触发
	DAC_InitStruct.TriggerSource = LL_DAC_TRIG_SOFTWARE;
	//---不使用波形发生
	DAC_InitStruct.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
	//---使能DAC输出缓存器
	if (isEnableBuffer==1)
	{
		DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_ENABLE;
	}
	else
	{
		DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_DISABLE;
	}
	if (ch==2)
	{
		dacChannale = LL_DAC_CHANNEL_2;
	}
	else
	{
		dacChannale = LL_DAC_CHANNEL_1;
	}
	//---初始化DAC
	LL_DAC_Init(DAC, dacChannale, &DAC_InitStruct);
	//---使能DAC
	LL_DAC_Enable(DAC, dacChannale);
	//---设置数据输出格式,右对齐，12位数据
	LL_DAC_ConvertData12RightAligned(DAC, dacChannale, 0);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数：ch=1,初始化DAC通道1；ch=2，初始化DAC通道2；ch=3，初始化DAC通道1和2
////////////// isEnableBuffer=1,使能缓存器；isEnableBuffer=0，不使能缓存器
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DAC_Init(UINT8_T ch, UINT8_T isEnableBuffer)
{
	DAC_GPIO_Init(ch);
	DAC_Clock(1);
	if (ch==3)
	{
		DAC_Channel_Init(1, isEnableBuffer);
		DAC_Channel_Init(2, isEnableBuffer);
	}
	else
	{
		DAC_Channel_Init(ch, isEnableBuffer);
	}
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数: ch=1,初始化DAC通道1；ch=2，初始化DAC通道2；ch=3，初始化DAC通道1和2
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DAC_ChannelUV(UINT8_T ch,UINT32_T volUV)
{
	UINT32_T vol = 0;
	//---判断是否超出量程
	if (volUV > DAC_REF_POWER_UV)
	{
		volUV = DAC_REF_POWER_UV;
	}
	//---通过计算得到模拟量
	vol = (UINT32_T)(volUV / DAC_BIT_POWER_UV);
	//---判断是否超量程
	if (vol > 0xFFF)
	{
		vol = 0xFFF;
	}
	if(vol>1000)
	{
		vol-=4;
	}
	//---设置数据输出格式,右对齐，12位数据
	if (ch==3)
	{
		LL_DAC_ConvertData12RightAligned(DAC, LL_DAC_CHANNEL_1, vol);
		LL_DAC_ConvertData12RightAligned(DAC, LL_DAC_CHANNEL_2, vol);
	}
	else if(ch==2)
	{
		LL_DAC_ConvertData12RightAligned(DAC, LL_DAC_CHANNEL_1, vol);
	}
	else
	{
		LL_DAC_ConvertData12RightAligned(DAC, LL_DAC_CHANNEL_1, vol);
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
UINT8_T DAC_ChannelMV(UINT8_T ch, UINT32_T volMV)
{
	return DAC_ChannelUV(ch, (volMV * 1000));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DAC_ChannelV(UINT8_T ch, float volV)
{
	return DAC_ChannelUV(ch, (UINT32_T)(volV * 1000000));
}