#ifndef SI5351A_TASK_H_
#define SI5351A_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include"si5351a_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	
	//===º¯Êý¶¨Òå
	UINT8_T SI5351ATask_I2C_Init(SI5351A_HandlerType* SI5351Ax, void(*pFuncDelayus)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHWI2C);
	UINT8_T SI5351ATask_I2C_DeInit(SI5351A_HandlerType* SI5351Ax);
	UINT8_T SI5351ATask_SetFreqHz(SI5351A_HandlerType* SI5351Ax, UINT8_T clkChannel, UINT64_T freq);
	UINT8_T SI5351ATask_SetFreqKHz(SI5351A_HandlerType* SI5351Ax, UINT8_T clkChannel, float freqKHz);
	UINT8_T SI5351ATask_SetFreqMHz(SI5351A_HandlerType* SI5351Ax, UINT8_T clkChannel, float freqMHz);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*SI5351A_TASK_H_ */