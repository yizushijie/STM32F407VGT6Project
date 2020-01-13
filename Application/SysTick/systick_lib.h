#ifndef SYSTICK_LIB_H_
#define SYSTICK_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "systick_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===函数定义
	UINT8_T	 SysTickLib_Init(void);
	UINT8_T	 SysTickLib_DeInit(void);
	UINT8_T	SysTickLib_CreateTickTask(void(*pFuncTick)(void));
	UINT8_T SysTickLib_DeleteTickTask(void(*pFuncTick)(void));
	void* SysTickLib_GetTickTask(UINT8_T index);
	UINT8_T  SysTickLib_IRQDecTick(UINT32_T waitDecTick);
	UINT8_T  SysTickLib_DecTick(UINT32_T waitDecTick);
	UINT32_T SysTickLib_GetTick(void);
	UINT8_T	 SysTickLib_IRQTask(void);
	UINT8_T	 SysTickLib_WaitTick(UINT32_T usTime);
	UINT8_T  SysTickLib_WaitusTick(UINT32_T usTime);
	UINT8_T  SysTickLib_1msIncTick(void);
	UINT32_T SysTickLib_GetTickOVF(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* SYSTICK_LIB_H_ */