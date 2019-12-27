#include "iwdg_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明： 在初始化看门狗的时候需要注意关闭全局中断，否则会导致看门狗的设置失败，但是寄存器操作无影响
//////////////////////////////////////////////////////////////////////////////
UINT8_T IWDG_Init(void)
{
	//---关闭中断
	CLI();
	//---使能看门狗
	LL_IWDG_Enable(IWDG);
	//---使能预分频寄存器和重装载寄存器可写（取消寄存器的写保护）
	LL_IWDG_EnableWriteAccess(IWDG);
	//--->>>独立看门狗时间开始
	#if(USE_IWDG_TIME==IWDG_500MS)	//---0.2ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_8);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 2500);
	#elif(USE_IWDG_TIME==IWDG_1000MS)	//---0.4ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_16);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 2500);
	#elif(USE_IWDG_TIME==IWDG_1500MS)	//---0.8ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_32);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 1875);
	#elif(USE_IWDG_TIME==IWDG_2000MS)	//---1.6ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_64);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 1250);
	#elif(USE_IWDG_TIME==IWDG_3000MS)	//---1.6ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_64);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 1875);
	#elif(USE_IWDG_TIME==IWDG_4000MS)	//---1.6ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_64);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 2500);
	#elif(USE_IWDG_TIME==IWDG_5000MS)	//---3.2ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_128);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 1563);
	#elif(USE_IWDG_TIME==IWDG_6000MS)//---3.2ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_128);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 1875);
	#elif(USE_IWDG_TIME==IWDG_7000MS)	//---3.2ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_128);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 2188);
	#elif(USE_IWDG_TIME==IWDG_8000MS)	//---3.2ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_128);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 2500);
	#elif(USE_IWDG_TIME==IWDG_9000MS)	//---6.4ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_256);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 1407);
	#elif(USE_IWDG_TIME==IWDG_10000MS)	//---6.4ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_256);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 1563);
	#else								//---1.6ms
		//---设置预分频器值
		LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_64);
		//---设置重装载寄存器值
		LL_IWDG_SetReloadCounter(IWDG, 625);
	#endif
	//---<<<独立看门狗时间结束
	//---等待准备完成
	while (LL_IWDG_IsReady(IWDG) != 1);
	//---复位看门狗
	LL_IWDG_ReloadCounter(IWDG);
	//---开启中断
	SEI();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T IWDG_DeInit(void)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能： 看门狗复位操作
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T IWDG_Reset(void)
{
	//---复位看门狗
	LL_IWDG_ReloadCounter(IWDG);
	return OK_0;
}