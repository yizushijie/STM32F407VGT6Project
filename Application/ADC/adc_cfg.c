#include "adc_cfg.h"

//===全局变量的定义
ADCASK_HandlerType	g_ABChannelADC = { 0 };
pADCASK_HandlerType	pABChannelADC = &g_ABChannelADC;
ADCASK_HandlerType	g_CDChannelADC = { 0 };
pADCASK_HandlerType	pCDChannelADC = &g_CDChannelADC;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_GPIO_Init(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(GPIOA, PERIPHERAL_CLOCK_ENABLE);
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	//---GPIO的初始化----
	//---HV12V ---PA6---ADC6
	//---DUTVCC---PA7---ADC7
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6| LL_GPIO_PIN_7;// | LL_GPIO_PIN_2 | LL_GPIO_PIN_3;			//---对应的GPIO的引脚
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;														//---配置状态为模拟输入引脚
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;														//---设置端口下拉使能
	//---SCL的初始化
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_Clock(ADC_TypeDef *ADCx, UINT8_T isEnable)
{
	if (ADCx == NULL)
	{
		return ERROR_1;
	}
	if (ADCx == ADC1)
	{
		if (isEnable)
		{
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
		}
		else
		{
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_ADC1);
		}
	}
#ifdef ADC2
	if (ADCx == ADC2)
	{
		if (isEnable)
		{
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC2);
		}
		else
		{
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_ADC2);
		}
	}
#endif

#ifdef ADC3
	if (ADCx == ADC3)
	{
		if (isEnable)
		{
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC3);
		}
		else
		{
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_ADC3);
		}
	}
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
UINT8_T ADC_ADC1_Init(void)
{
	LL_ADC_InitTypeDef ADC_InitStruct = { 0 };
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = { 0 };
	LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = { 0 };
	//---将ADC1全部寄存器重新设置为缺省值
	LL_ADC_DeInit(ADC1);
	//---使能ADC的时钟
	ADC_Clock(ADC1, 1);
#ifndef USE_MCU_STM32F1
	//---12位模式
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
#endif
	//---数据右对齐
	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	//---多通道ADC采集需要开启扫描模式，单通道ADC采集不需要开启扫描模式
	ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);
	//---软件触发模式
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	//---扫描通道个数,2通道扫描
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS;
	//---规则通道组上禁用间断模式
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	//---连续转换模式
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
	//---使能DMA请求并使能DMA
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
#ifdef USE_MCU_STM32F1
	//---72M/6===12M(ADC的时钟不能超过14MHz)
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);
#else
	//---时钟4分频
	ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV8;//LL_ADC_CLOCK_SYNC_PCLK_DIV4;
#endif
	//---独立模式
	ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
	//---采样间隔
#ifndef USE_MCU_STM32F1	
	ADC_CommonInitStruct.MultiTwoSamplingDelay = LL_ADC_MULTI_TWOSMP_DELAY_5CYCLES;
#endif
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
	//---第一次转换ADC的通道6
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_6);
	//---配置采样周期
#ifdef USE_MCU_STM32F1
	//---15个采样周期
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_10, LL_ADC_SAMPLINGTIME_7CYCLES_5);
#else
	//---15个采样周期
	//LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_10, LL_ADC_SAMPLINGTIME_15CYCLES);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_6, LL_ADC_SAMPLINGTIME_480CYCLES);
#endif
	//---第二次转换ADC通道4
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_7);
#ifdef USE_MCU_STM32F1
	//---15个采样周期
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_11, LL_ADC_SAMPLINGTIME_7CYCLES_5);
#else
	//---15个采样周期
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_480CYCLES);
#endif
	//---使能装换
	LL_ADC_Enable(ADC1);
	//---F1的ADC需要校准
#ifdef USE_MCU_STM32F1
	//---启动校准
	ADC_StartCalibration(ADC1);
#endif
	//LL_ADC_REG_StartConversionSWStart(ADC1);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： ADC校准
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_StartCalibration(ADC_TypeDef *ADCx)
{
#ifdef USE_MCU_STM32F1
	//---复位校准
	if (ADCx != NULL)
	{
		//---延时等待一下
		//DelayTask_nop(LL_ADC_DELAY_ENABLE_CALIB_ADC_CYCLES * 32);
		//---初始化校准寄存器
		SET_BIT(ADCx->CR2, ADC_CR2_RSTCAL);
		//---等待校准完成
		while ((READ_BIT(ADCx->CR2, ADC_CR2_RSTCAL) == (ADC_CR2_RSTCAL)));
		//---启动校准
		LL_ADC_StartCalibration(ADCx);
		//---等待校准完成
		while (LL_ADC_IsCalibrationOnGoing(ADCx) != 0);
		return OK_0;
	}
	return ERROR_1;
#else
	return OK_0;
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADC1_DMA_Init(void)
{
	//---将DMA全部寄存器重新设置为缺省值
#ifdef USE_MCU_STM32F1
	//---F1对应是的DMA1的通道1对应ADC1
	LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_1);
	//---DMA时钟总线配置
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
#else
	LL_DMA_DeInit(DMA2, LL_DMA_STREAM_0);
	//---DMA时钟总线配置
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
#endif
	//---DMA初始化结构体
	LL_DMA_InitTypeDef DMA_InitTypeDef = { 0 };
#ifndef USE_MCU_STM32F1
	//---DMA通道
	DMA_InitTypeDef.Channel = LL_DMA_CHANNEL_0;
#endif
	//---数据大小
	DMA_InitTypeDef.NbData = ADC_CHANNEL_MAX_SIZE;
	//---方向从外设到存储器
	DMA_InitTypeDef.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
#ifndef USE_MCU_STM32F1
	//---不用FIFO用直连模式
	DMA_InitTypeDef.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
	//---半字两字节
	DMA_InitTypeDef.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_2;
#endif
	//---存储器地址
	DMA_InitTypeDef.MemoryOrM2MDstAddress = (UINT32_T)&pABChannelADC->msgADCConvVal;
	//---半字两字节
	DMA_InitTypeDef.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
	//---存储器地址增加
	DMA_InitTypeDef.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	//---DMA正常模式
	DMA_InitTypeDef.Mode = LL_DMA_MODE_NORMAL; //LL_DMA_MODE_CIRCULAR;
	//---DMA外设地址
	DMA_InitTypeDef.PeriphOrM2MSrcAddress = (UINT32_T)&ADC1->DR;
#ifndef USE_MCU_STM32F1
	//---无FIFO
	DMA_InitTypeDef.PeriphBurst = LL_DMA_PBURST_SINGLE;
#endif
	//---半字两字节
	DMA_InitTypeDef.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	//---外设基地址不变
	DMA_InitTypeDef.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	//---传输通道优先级为高
	DMA_InitTypeDef.Priority = LL_DMA_PRIORITY_HIGH;
	//---DMA初始化
#ifdef USE_MCU_STM32F1
	//---初始化DMA
	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &DMA_InitTypeDef);
	//---使能DMA
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
#else
	//---初始化DMA
	LL_DMA_Init(DMA2, LL_DMA_STREAM_0, &DMA_InitTypeDef);
	//---使能DMA
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
#endif
	//---中断配置
#ifdef USE_MCU_STM32F1
	//---DMA中断配置
	NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	//---使能中断
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
#else
	//---DMA中断配置
	NVIC_SetPriority(DMA2_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	//---使能中断
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
#endif
	//---DMA传输结束中断
#ifdef USE_MCU_STM32F1
	//---使能DMA传输结束中断
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
#else
	//---使能DMA传输结束中断
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0);
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：重新启动ADC的DMA模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADC1_DMA_Restart(void)
{
	//---配置DMA的数据
#ifdef USE_MCU_STM32F1
	//---设置DMA读取数据的大小
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ADC_CHANNEL_MAX_SIZE);
	//---使能DMA
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
#else
	//---设置DMA读取数据的大小
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, ADC_CHANNEL_MAX_SIZE);
	//---使能DMA
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
#endif
	//---使能装换
	LL_ADC_Enable(ADC1);
	//---F1的ADC需要校准
#ifdef USE_MCU_STM32F1
	//---启动校准
	ADC_StartCalibration(ADC1);
#endif
	ADC_SoftStart(ADC1, NULL, NULL);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADC1_DMA_Stop(void)
{
#ifdef USE_MCU_STM32F1
	//---使能DMA
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_ClearFlag_GI1(DMA1);
	//LL_DMA_ClearFlag_HT1(DMA1);
	//LL_DMA_ClearFlag_TC1(DMA1);
	LL_DMA_ClearFlag(DMA1, LL_DMA_CHANNEL_1);
#else
	//---不使能DMA
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);
	//LL_DMA_ClearFlag_HT0(DMA2);
	//LL_DMA_ClearFlag_TC0(DMA2);
	LL_DMA_ClearFlag(DMA2, LL_DMA_STREAM_0);
#endif
	//---使能装换
	LL_ADC_Disable(ADC1);
#ifndef USE_MCU_STM32F1
	//---清除溢出中断标志位
	LL_ADC_ClearFlag_OVR(ADC1);
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
UINT8_T ADC_ADC3_Init(void)
{
	LL_ADC_InitTypeDef ADC_InitStruct = { 0 };
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = { 0 };
	LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = { 0 };
	//---将ADC1全部寄存器重新设置为缺省值
	LL_ADC_DeInit(ADC3);
	//---使能ADC的时钟
	ADC_Clock(ADC3, 1);
#ifndef USE_MCU_STM32F1
	//---12位模式
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
#endif
	//---数据右对齐
	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	//---多通道ADC采集需要开启扫描模式，单通道ADC采集不需要开启扫描模式
	ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
	LL_ADC_Init(ADC3, &ADC_InitStruct);
	//---软件触发模式
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	//---扫描通道个数
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS;
	//---规则通道组上禁用间断模式
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	//---连续转换模式
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
	//---使能DMA请求并使能DMA
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	LL_ADC_REG_Init(ADC3, &ADC_REG_InitStruct);

#ifdef USE_MCU_STM32F1
	//---72M/6===12M(ADC的时钟不能超过14MHz)
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);
#else
	//---时钟4分频
	ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
#endif
	//---独立模式
	ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
#ifndef USE_MCU_STM32F1
	//---采样间隔
	ADC_CommonInitStruct.MultiTwoSamplingDelay = LL_ADC_MULTI_TWOSMP_DELAY_20CYCLES;
#endif
	//LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC3), &ADC_CommonInitStruct);
	LL_ADC_CommonInit(ADC3_COMMON, &ADC_CommonInitStruct);
	//---第一次转换ADC的通道10
	LL_ADC_REG_SetSequencerRanks(ADC3, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_12);
#ifdef USE_MCU_STM32F1
	//---15个采样周期
	LL_ADC_SetChannelSamplingTime(ADC3, LL_ADC_CHANNEL_12, LL_ADC_SAMPLINGTIME_28CYCLES_5);
#else
	//---15个采样周期
	LL_ADC_SetChannelSamplingTime(ADC3, LL_ADC_CHANNEL_12, LL_ADC_SAMPLINGTIME_15CYCLES);
#endif
	//---第二次转换ADC通道11
	LL_ADC_REG_SetSequencerRanks(ADC3, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_13);
#ifdef USE_MCU_STM32F1
	//---15个采样周期
	LL_ADC_SetChannelSamplingTime(ADC3, LL_ADC_CHANNEL_13, LL_ADC_SAMPLINGTIME_28CYCLES_5);
#else
	//---15个采样周期
	LL_ADC_SetChannelSamplingTime(ADC3, LL_ADC_CHANNEL_13, LL_ADC_SAMPLINGTIME_15CYCLES);
#endif
	//---使能装换
	LL_ADC_Enable(ADC3);
	//---F1的ADC需要校准
#ifdef USE_MCU_STM32F1
	//---启动校准
	ADC_StartCalibration(ADC3);
#endif
	//LL_ADC_REG_StartConversionSWStart(ADC3);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADC3_DMA_Init(void)
{
	//---将DMA全部寄存器重新设置为缺省值
#ifdef USE_MCU_STM32F1
	//---F1对应是的DMA1的通道1对应ADC1
	LL_DMA_DeInit(DMA2, LL_DMA_CHANNEL_5);
	//---DMA时钟总线配置
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
#else
	LL_DMA_DeInit(DMA2, LL_DMA_STREAM_1);
	//---DMA时钟总线配置
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
#endif
	//---DMA初始化结构体
	LL_DMA_InitTypeDef DMA_InitTypeDef = { 0 };
#ifndef USE_MCU_STM32F1
	//---DMA通道
	DMA_InitTypeDef.Channel = LL_DMA_CHANNEL_2;
#endif
	//---数据大小
	DMA_InitTypeDef.NbData = ADC_CHANNEL_MAX_SIZE;
	//---方向从外设到存储器
	DMA_InitTypeDef.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
#ifndef USE_MCU_STM32F1
	//---不用FIFO用直连模式
	DMA_InitTypeDef.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
	//---半字两字节
	DMA_InitTypeDef.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_2;
#endif
	//---存储器地址
	DMA_InitTypeDef.MemoryOrM2MDstAddress = (UINT32_T)&pCDChannelADC->msgADCConvVal;;// (UINT32_T)ADC_ConvertedValue1;
	//---半字两字节
	DMA_InitTypeDef.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
	//---存储器地址增加
	DMA_InitTypeDef.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	//---DMA正常模式
	DMA_InitTypeDef.Mode = LL_DMA_MODE_NORMAL; //LL_DMA_MODE_CIRCULAR;
	//---DMA外设地址
	DMA_InitTypeDef.PeriphOrM2MSrcAddress = (UINT32_T)&ADC3->DR;
#ifndef USE_MCU_STM32F1
	//---无FIFO
	DMA_InitTypeDef.PeriphBurst = LL_DMA_PBURST_SINGLE;
#endif
	//---半字两字节
	DMA_InitTypeDef.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	//---外设基地址不变
	DMA_InitTypeDef.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	//---传输通道优先级为高
	DMA_InitTypeDef.Priority = LL_DMA_PRIORITY_HIGH;
#ifdef USE_MCU_STM32F1
	//---初始化DMA
	LL_DMA_Init(DMA2, LL_DMA_CHANNEL_5, &DMA_InitTypeDef);
	//---使能DMA
	LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_5);
#else
	//---初始化DMA
	LL_DMA_Init(DMA2, LL_DMA_STREAM_1, &DMA_InitTypeDef);
	//---使能DMA
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
#endif
#ifdef USE_MCU_STM32F1
	//---DMA中断配置
	NVIC_SetPriority(DMA2_Channel4_5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	//---使能中断
	NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
#else
	//---DMA中断配置
	NVIC_SetPriority(DMA2_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	//---使能中断
	NVIC_EnableIRQ(DMA2_Stream1_IRQn);
#endif
#ifdef USE_MCU_STM32F1
	//---使能DMA传输结束中断
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_CHANNEL_5);
#else
	//---使能DMA传输结束中断
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_1);
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：重新启动ADC的DMA模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADC3_DMA_Restart(void)
{
#ifdef USE_MCU_STM32F1
	//---设置DMA读取数据的大小
	LL_DMA_SetDataLength(DMA2, LL_DMA_CHANNEL_5, ADC_CHANNEL_MAX_SIZE);
	//---使能DMA
	LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_5);
#else
	//---设置DMA读取数据的大小
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_1, ADC_CHANNEL_MAX_SIZE);
	//---使能DMA
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
#endif
	//---使能装换
	LL_ADC_Enable(ADC3);
	//---F1的ADC需要校准
#ifdef USE_MCU_STM32F1
	//---启动校准
	ADC_StartCalibration(ADC3);
#endif
	ADC_SoftStart(NULL, NULL, ADC3);
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADC3_DMA_Stop(void)
{
#ifdef USE_MCU_STM32F1
	//---使能DMA
	LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_5);
	LL_DMA_ClearFlag_GI5(DMA2);
	LL_DMA_ClearFlag_HT5(DMA2);
	LL_DMA_ClearFlag_TC5(DMA2);
#else
	//---不使能DMA
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
	LL_DMA_ClearFlag_HT1(DMA2);
	LL_DMA_ClearFlag_TC1(DMA2);
#endif
	//---使能装换
	LL_ADC_Disable(ADC3);
#ifndef USE_MCU_STM32F1
	//---清除溢出中断标志位
	LL_ADC_ClearFlag_OVR(ADC3);
#endif
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 软件触发ADC的装换
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_SoftStart(ADC_TypeDef *ADCx1, ADC_TypeDef *ADCx2, ADC_TypeDef *ADCx3)
{
	if (ADCx1 != NULL)
	{
		LL_ADC_REG_StartConversionSWStart(ADCx1);
	}
	if (ADCx2 != NULL)
	{
		LL_ADC_REG_StartConversionSWStart(ADCx2);
	}
	if (ADCx3 != NULL)
	{
		LL_ADC_REG_StartConversionSWStart(ADCx3);
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
UINT8_T ADC_ADC1Task_Init(void)
{
	ADC_ADC1_DMA_Init();
	ADC_ADC1_Init();
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADC3Task_Init(void)
{
	ADC_ADC3_DMA_Init();
	ADC_ADC3_Init();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADCTask_Init(void)
{
	ADC_ADC1Task_Init();
	//ADC_ADC3Task_Init();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADC_Init(void)
{
	ADC_GPIO_Init();
	return ADC_ADCTask_Init();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADC_ADCTask_START(ADC_TypeDef *ADCx)
{
	if (ADCx == ADC1)
	{
		pABChannelADC->msgFlag = 0;
		//---清零ADC缓存区
		memset(pABChannelADC->msgADCConvVal, 0, ADC_CHANNEL_MAX_SIZE);
		//---启动ADC装换
		return ADC_ADC1_DMA_Restart();
	}
#ifdef ADC2
	if (ADCx == ADC2)
	{
		return OK_0;
	}
#endif
#ifdef ADC3
	if (ADCx == ADC3)
	{
		pCDChannelADC->msgFlag = 0;
		//---清零ADC缓存区
		memset(pCDChannelADC->msgADCConvVal, 0, ADC_CHANNEL_MAX_SIZE);
		//---启动ADC装换
		return ADC_ADC3_DMA_Restart();
	}
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
UINT8_T ADC_ADCTask_STOP(ADC_TypeDef *ADCx)
{
	if (ADCx == ADC1)
	{
		pABChannelADC->msgFlag += 1;
		return ADC_ADC1_DMA_Stop();
	}
#ifdef ADC2
	if (ADCx == ADC2)
	{
		return OK_0;
	}
#endif
#ifdef ADC3
	if (ADCx == ADC3)
	{
		pCDChannelADC->msgFlag += 1;
		return ADC_ADC3_DMA_Stop();
	}
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
UINT8_T ADC_HandleChannelVal(ADCASK_HandlerType *ADCASKx)
{
	UINT8_T i = 0;
	UINT16_T adcAChannel[ADC_CHANNEL_SIZE] = { 0 };
	UINT16_T adcBChannel[ADC_CHANNEL_SIZE] = { 0 };
	UINT16_T *pADCVal = ADCASKx->msgADCConvVal;
	//---各个通道数据的获取
	for (i = 0; i < ADC_CHANNEL_SIZE; i++)
	{
		//---取第一个通道的值
		adcAChannel[i] = *pADCVal;
		pADCVal++;
		//---取第二个通道的值
		adcBChannel[i] = *pADCVal;
		pADCVal++;
	}
	//---A通道数据降序排列
	DescSortWord(adcAChannel, ADC_CHANNEL_SIZE);
	//---B通道数据降序排列
	DescSortWord(adcBChannel, ADC_CHANNEL_SIZE);
	//---计算A通道采样值
	ADCASKx->msgAChannelVal = CalcAvgWordFromIndex(adcAChannel, (ADC_CHANNEL_SIZE - 2), 2);
	//---计算B通道采样值
	ADCASKx->msgBChannelVal = CalcAvgWordFromIndex(adcBChannel, (ADC_CHANNEL_SIZE - 2), 2);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：获取通道的电压值
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT16_T ADC_GetChipPower(void)
{
	UINT32_T tempPower=pABChannelADC->msgAChannelVal;
	//---将结果装换为电压值
	tempPower-=6;
	tempPower*=3260;
	tempPower<<=1;
	return (UINT16_T)(tempPower/4096);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：获取通道的电压值
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT16_T ADC_GetHVPower(void)
{
	UINT32_T tempPower = pABChannelADC->msgBChannelVal;
	//---将结果装换为电压值
	tempPower -= 6;
	tempPower *= 3260;
	tempPower=(tempPower*9)/50;
	return (UINT16_T)(tempPower / 4096);
}