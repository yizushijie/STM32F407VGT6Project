#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	#include "systick_task.h"

	//===定义使用的外部计数模式的通道数
	#define	CALC_FREQ_EXTERN_COUNT_MODE_CHANNEL_NUM			2
	//===定义是否使用电平转换芯片，带OE控制端的
	#define CALC_FREQ_lEVEL_SHIFT
	//===结构体定义
	typedef struct _CalcFreq_HandlerType					CalcFreq_HandlerType;
	//===指针结构体定义
	typedef struct _CalcFreq_HandlerType					*pCalcFreq_HandlerType;
	//===计算频率的数据结构体
	struct  _CalcFreq_HandlerType
	{
		VLTUINT8_T  msgChannel;																		//---操作通道
		VLTUINT8_T  msgStep[CALC_FREQ_EXTERN_COUNT_MODE_CHANNEL_NUM];								//---操作步序
		float		msgFreqMHz[CALC_FREQ_EXTERN_COUNT_MODE_CHANNEL_NUM];							//---MHz频率
		float		msgFreqKHz[CALC_FREQ_EXTERN_COUNT_MODE_CHANNEL_NUM];							//---KHz频率
		//void(*msgFuncTask)(void);																	//---操作任务
		#ifdef CALC_FREQ_lEVEL_SHIFT
		GPIO_HandlerType	msgOE[CALC_FREQ_EXTERN_COUNT_MODE_CHANNEL_NUM];							//---OE控制端口
		#endif
	};

	//===外部调用接口
	extern CalcFreq_HandlerType g_CalcFreq;
	extern pCalcFreq_HandlerType pCalcFreq;

	//===函数定义
	void Timer_CalcFreqMode_DeInit(void);
	void Timer_CalcFreq_Task(UINT8_T ch);
	float Timer_GetFreqKHz(void);
	float Timer_GetFreqMHz(void);
	void Timer_Init(void);
	UINT8_T Timer_Clock(TIM_TypeDef *TIMx, UINT8_T isEnable);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*TIMER_CFG_H_ */
