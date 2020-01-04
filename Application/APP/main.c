#include "main.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：系统时钟的配置
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void SystemClock_Config(void)
{
	//---设置FLASH的延时读取周期数
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
	//---等待FLASH延时设置完成---判断设置是否成功
	if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5)
	{
		Error_Handler();
	}
	//---PWR时钟
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	//---设置内部调节器的输出电压
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	#if 1
		//---使能系统外部无源晶振
		LL_RCC_HSE_Enable();
	#else
		//---使能系统外部有源晶振
		LL_RCC_HSE_EnableBypass();
	#endif
	//---等待外部系统晶振稳定
	while (LL_RCC_HSE_IsReady() != 1)
	{
	}
	//---使能内部低频晶振
	LL_RCC_LSI_Enable();
	//---等待内部低频晶振稳定
	while (LL_RCC_LSI_IsReady() != 1)
	{
	}
	//---使能备份域的访问
	LL_PWR_EnableBkUpAccess();
	//---设置系统的PLL,用于配置系统的主时钟
	#if (HSE_VALUE==4000000UL)
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_2, 168, LL_RCC_PLLP_DIV_2);
	#elif (HSE_VALUE==6000000UL)
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_3, 168, LL_RCC_PLLP_DIV_2);
	#elif (HSE_VALUE==8000000UL)
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
	#elif (HSE_VALUE==12000000UL)
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_6, 168, LL_RCC_PLLP_DIV_2);
	#elif (HSE_VALUE==16000000UL)
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 168, LL_RCC_PLLP_DIV_2);
	#elif (HSE_VALUE==20000000UL)
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_10, 168, LL_RCC_PLLP_DIV_2);
	#elif (HSE_VALUE==24000000UL)
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_12, 168, LL_RCC_PLLP_DIV_2);
	#else
		#error "外部晶振配置成错误，只支持8M和16M的配置!"
	#endif
	//---设置USB时钟
	//LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLQ_DIV_7);
	//---使能PLL
	LL_RCC_PLL_Enable();
	//---等待PLL稳定
	while (LL_RCC_PLL_IsReady() != 1)
	{
	}
	//---设置AHB总线时钟分频
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	//---设置APB1总线时钟分频
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
	//---设置APB2总线时钟分频
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
	//---设置系统时钟
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	//--- 等待系统时钟稳定，即PLL锁定倍频之后的频率
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{
	}
	//LL_Init1msTick( 168000000 );
	//---设置系统滴答定时器的时钟为系统高速时钿--使能滴答定时器的时钟
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
	//---设置系统时钟
	LL_SetSystemCoreClock(168000000);
	//---使能Flash的预读取功能
	LL_FLASH_EnablePrefetch();
	//---使能数据缓存功能
	LL_FLASH_EnableDataCache();
	//---使能指令高速缓存
	LL_FLASH_EnableInstCache();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void NVIC_Init(void)
{
	//---设置NVIC的分组模式
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	/* System interrupt init*/
	/* HardFault_IRQn interrupt configuration */
	#ifdef USE_MCU_STM32F1
		NVIC_SetPriority(HardFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	#endif
	/* MemoryManagement_IRQn interrupt configuration */
	NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* BusFault_IRQn interrupt configuration */
	NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* UsageFault_IRQn interrupt configuration */
	NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* SVCall_IRQn interrupt configuration */
	NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* DebugMonitor_IRQn interrupt configuration */
	NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* PendSV_IRQn interrupt configuration */
	NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Sys_Init(void)
{	
	//---系统时钟的初始化
	SystemClock_Config();	
	//---NVIC初始化
	NVIC_Init();
	//---自定义内存管理初始化
	MyMemInit();
	//---GPIO初始化
	GPIOTask_Init();    
	//---滴答定时器初始化
	SysTickTask_Init();
	//---随机数初始化,禁用中断方式
	RandomTask_Init(0);
	//---CRC校验初始化
	CRCTask_Init();
	//---硬件初始化
	HardWare_Init();
	//---初始化MCO的输出时钟
	MCO1_Init();
	//---定时器初始化
	TimerTask_Init();
	//---ISP的初始化
	ISPTask_Init(pIspDevice0,DelayTask_us,DelayTask_ms, SysTickTask_GetTick);
	//---WM8510初始化
	WM8510Task_I2C_Init(pWm8510Device0, DelayTask_us, SysTickTask_GetTick, 0);
	//---SI5351A初始化
	//SI5351ATask_I2C_Init(pSI5351ADevice0, DelayTask_us, SysTickTask_GetTick, 0);
	//---指示灯的初始化
	LEDTask_Init();	
	//---DAC的初始化
	DACTask_Init(DAC_CHANNEL_SELECT_ALL, DAC_CHANNEL_ENABLE_BUFFER);
	//---初始化LM317做的可调电源
	LM317Task_Init(0,3000);
	LM317_POWER_ON;
	//JTAG_Init(pJtagDevice0, DelayTask_us, DelayTask_ms, SysTickTask_GetTick);
	//JTAG_EnterProg(pJtagDevice0);
	//JTAG_ReadIDChip(pJtagDevice0,NULL);
	//JTAG_ReadChipID(pJtagDevice0, NULL);
	//JTAG_ExitProg(pJtagDevice0);
	HVPP_Init(pHvppDevice0, DelayTask_us, DelayTask_ms, SysTickTask_GetTick);
	UINT8_T tempID[3]={0x00,0x00,0x00};
	HVPP_EnterProg(pHvppDevice0,0);
	HVPP_ReadChipID(pHvppDevice0, tempID);
	UINT16_T i=0;
	for (i=0;i<256;i++)
	{
		HVPP_DATABUS_WRITE(pHvppDevice0, i);
	}
	HVPP_ExitProg(pHvppDevice0);
	//---ADC初始化
	ADCTask_ADC_Init();
	ADCTask_ADCTask_START(ADC1);
	//---串口的初始化
	USARTTask_Init(pUsart1, USART1_RX_MAX_SIZE , USART1_RX_BUFFER , USART_CRC_NONE , USART1_TX_MAX_SIZE , USART1_TX_BUFFER , USART_CRC_NONE , SysTickTask_GetTick );
	//---开启看门狗
	//IWDGTask_Init(pIWDG);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
int main(void)
{
	//---系统初始化函数
	Sys_Init();
	//---主循环
	while (1)
	{		
		//---任务管理函数
		Task_Manage();
		//---喂狗
		WDT_RESET();
	}
}