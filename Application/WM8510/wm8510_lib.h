#ifndef WM8510_LIB_H_
#define WM8510_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "wm8510_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===º¯Êý¶¨Òå
	UINT8_T WM8510Lib_I2C_Init(WM8510_HandlerType *WM8510x, void(*pFuncDelayus)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHWI2C);
	UINT8_T WM8510Lib_I2C_DeInit(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510Lib_I2C_SetFreqHz(WM8510_HandlerType *WM8510x, UINT32_T freq);
	UINT8_T WM8510Lib_I2C_SetFreqKHz(WM8510_HandlerType *WM8510x, float freq);
	UINT8_T WM8510Lib_I2C_SetFreqMHz(WM8510_HandlerType *WM8510x, float freq);
	void	WM8510Lib_I2C_Reset(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510Lib_I2C_SetFreqHzWithAllFreqReg(WM8510_HandlerType *WM8510x, UINT32_T freq);
	UINT8_T WM8510Lib_CalibrateFreqKHzOutPut(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510Lib_I2C_SetFreqHzWithAllFreqRegAndCalibrateFreqKHzOutPut(WM8510_HandlerType *WM8510x, UINT32_T freq);
	UINT8_T WM8510Lib_I2C_CalibrateClock(WM8510_HandlerType *WM8510x);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* WM8510_LIB_H_ */