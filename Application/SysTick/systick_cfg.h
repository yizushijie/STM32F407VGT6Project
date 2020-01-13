#ifndef SYSTICK_CFG_H_
#define SYSTICK_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===定义滴答定时器支持的任务调度函数的个数
	#define SYSTICK_FUNC_TASK_MAX_NUM							4
	//===定义结构体
	typedef struct _SysTick_HandlerType			SysTick_HandlerType;
	//===定义指针结构体
	typedef struct _SysTick_HandlerType			*pSysTick_HandlerType;
	//===结构体定义
	struct _SysTick_HandlerType
	{
		VLTUINT8_T				msgTickTaskFlag[SYSTICK_FUNC_TASK_MAX_NUM];																											//---滴答任务标签，0---可用，1---不可用
		VLTUINT8_T				msgTickTaskValid[SYSTICK_FUNC_TASK_MAX_NUM];																										//---滴答任务的有效性，0---当前周期可用，1---当前周期不可用
		VLTUINT8_T				msgTickTaskCount;																																	//---滴答任务的总数
		VLTUINT32_T				msgIncTick;																																			//---递加计数器
		VLTUINT32_T				msgDecTick;																																			//---递减计数器
		VLTUINT32_T				msgIncTickOVF;																																		//---递加计数器溢出
		void(*msgTickTask[SYSTICK_FUNC_TASK_MAX_NUM])(void);																														//---函数指针数组
	};

	//===外部调用接口
	extern SysTick_HandlerType		g_SysTick;
	extern pSysTick_HandlerType		pSysTick;

	//===函数定义
	UINT8_T  SysTick_msInit(void);
	UINT8_T  SysTick_DeInit(void);
	UINT8_T	SysTick_CreateTickTask(void(*pFuncTick)(void));
	UINT8_T SysTick_DeleteTickTask(void(*pFuncTick)(void));
	void	SysTick_PollTickTask(void);
	void* SysTick_GetTickTask(UINT8_T index);
	UINT8_T  SysTick_IRQDecTick(UINT32_T waitDecTick);
	UINT8_T  SysTick_DecTick(UINT32_T waitDecTick);
	UINT32_T SysTick_GetTick(void);
	UINT8_T  SysTick_IRQTask(void);
	UINT8_T  SysTick_WaitTick(UINT32_T usTime);
	UINT8_T  SysTick_WaitusTick(UINT32_T usTime);
	UINT8_T  SysTick_WaitSysTick(UINT32_T sysTickTime);
	UINT8_T  SysTick_1msIncTick(void);
	UINT32_T SysTick_GetTickOVF(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* SYSTICK_CFG_H_ */