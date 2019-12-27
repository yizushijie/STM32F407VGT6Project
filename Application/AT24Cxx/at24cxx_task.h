#ifndef AT24CXX_TASK_H_
#define AT24CXX_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "at24cxx_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===º¯Êý¶¨Òå
	UINT8_T AT24CXXTask_I2C_Init(AT24CXX_HandlerType *AT24CXXx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT8_T isHWI2C);
	UINT8_T AT24CXXTask_I2C_DeInit(AT24CXX_HandlerType *AT24CXXx);
	UINT8_T AT24CXXTask_I2C_WriteOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T val);
	UINT8_T AT24CXXTask_I2C_WritePageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXXTask_I2C_WriteData(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXXTask_I2C_ReadOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal);
	UINT8_T AT24CXXTask_I2C_ReadPageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXXTask_I2C_ReadData(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXXTask_I2C_EraseChip(AT24CXX_HandlerType *AT24CXXx);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* AT24CXX_TASK_H_ */